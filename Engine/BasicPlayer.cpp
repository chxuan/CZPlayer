/***************************************************************************
*   Copyright (C) 2012-2015 Highway-9 Studio.                             *
*   787280310@qq.com									                   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   CUIT Highway-9 Studio, China.									       *
***************************************************************************/

/*!
* \file   BasicPlayer.cpp
* \author chengxuan   787280310@qq.com
* \date   2015-02-06
* \brief  播放引擎实现文件。
* \version 3.0.0
*
* \verbatim
* 历史
*          3.0.0   创建,
*          2015-02-06    by chengxuan
*
* \endverbatim
*
*/

#include "BasicPlayer.h"
#include "CriticalSection.h"
#include "IO.h"

#pragma warning(disable : 4800)
#pragma warning(disable : 4189)

#define DEBUG_OUTPUT_BUFFER_SIZE 256
TCHAR BUFFER[DEBUG_OUTPUT_BUFFER_SIZE];

/* constants used in direct sound */
#define DEFAULT_BUFFER_SIZE 88200
#define DEFAULT_SAMPLE_RATE 44100.0F
#define DEFAULT_FRAME_SIZE	4
#define DEFAULT_BITS_PER_SAMPLE	16
#define DEFAULT_CHANNELS	2
#define DEFAULT_SECONDS		2
#define DEFAULT_DS_BUFFER_SIZE	DEFAULT_SECONDS*DEFAULT_SAMPLE_RATE*(DEFAULT_BITS_PER_SAMPLE>>3)*DEFAULT_CHANNELS

/* constants used in digital signal process */
#define DEFAULT_WIDTH 300
#define DEFAULT_HEIGHT 180
#define DEFAULT_SAMPLE_SIZE 2048
#define DEFAULT_FPS 30
#define DEFAULT_SPECTRUM_ANALYSER_FFT_SAMPLE_SIZE 512
#define DEFAULT_SPECTRUM_ANALYSER_BAND_COUNT 30				//频谱bar个数
#define DEFAULT_SPECTRUM_ANALYSER_DECAY 0.05F
#define DEFAULT_SPECTRUM_ANALYSER_PEAK_DELAY 5 /* the value is more lower, fall faster */
#define DEFAULT_SPECTRUM_ANALYSER_PEAK_DELAY_FPS_RATIO 0.4F
#define DEFAULT_SPECTRUM_ANALYSER_PEAK_DELAY_FPS_RATIO_RANGE 0.1F
#define MIN_SPECTRUM_ANALYSER_DECAY	0.02F
#define MAX_SPECTRUM_ANALYSER_DECAY 0.08F
#define SAMPLE_TYPE_EIGHT_BIT 1
#define SAMPLE_TYPE_SIXTEEN_BIT 2
#define CHANNEL_MODE_MONO 1
#define CHANNEL_MODE_STEREO 2

/************************************************************************/
/* CSystem                                                              */
/************************************************************************/
inline void set_low(jlong* value, jint low)
{
    *value &= (jlong)0xffffffff << 32;
    *value |= (jlong)(julong)(juint)low;
}

inline void set_high(jlong* value, jint high)
{
    *value &= (jlong)(julong)(juint)0xffffffff;
    *value |= (jlong)high << 32;
}

jlong as_long(LARGE_INTEGER x)
{
    jlong result = 0; // initialization to avoid warning
    set_high(&result, x.HighPart);
    set_low(&result, x.LowPart);
    return result;
}

LARGE_INTEGER liFrequency = { 0 };
bool gSupportPerformanceFrequency = QueryPerformanceFrequency(&liFrequency);
jlong CSystem::frequency = as_long(liFrequency);
int CSystem::ready = 1;

/************************************************************************/
/* CPlayThread                                                          */
/************************************************************************/
CPlayThread::CPlayThread(CBasicPlayer* pPlayer)
    : CThread(true),
    m_Player(pPlayer),
    m_CriticalSection(NULL)
{
    if (m_CriticalSection == NULL)
    {
        m_CriticalSection = new CCriticalSection;
    }
    /*Else do nothing, and continue.*/
}

CPlayThread::~CPlayThread(void)
{
    if (m_CriticalSection != NULL)
    {
        delete m_CriticalSection;
        m_CriticalSection = NULL;
    }
    /*Else do nothing, and continue.*/
}

void CPlayThread::Execute()
{
    if (m_Player == NULL || m_Stop)
    {
        return;
    }
    /*Else do nothing, and continue.*/

    const DWORD buffersize = 16000;

    // wait time = 1/4 of buffer time
    DWORD waitTime = (DWORD)((m_Player->m_BufferSize * 1000.0F) / (m_Player->m_SampleRate * m_Player->m_FrameSize));
    waitTime = (DWORD)(waitTime / 4);

    if (waitTime < 10)
    {
        waitTime = 1;
    }
    /*Else do nothing, and continue.*/

    if (waitTime > 1000)
    {
        waitTime = 1000;
    }
    /*Else do nothing, and continue.*/

    CSpectrumAnalyser* pSpectrum = m_Player->m_SpectrumAnalyser;
    CFileInput* pInput = m_Player->m_Input;
    pInput->Init();

    if (DAUDIO_Start((void*)m_Player->m_info, true))
    {
        char buffer[buffersize];
        DWORD len = 0;
        DWORD offset = 0;
        DWORD written = 0;
        bool isEndOfInput = false;

        //开始播放
        while (!m_Stop)
        {
            memset(buffer, 0, buffersize);
            pInput->FillBuffer((void*)buffer, buffersize, &isEndOfInput);	//填充缓冲区

            len = buffersize;
            offset = 0;
            written = 0;

            /*
            * in this loop, the data may can not be written to device one time,
            * maybe more than one times. so, we need this loop to process it.
            */
            while (!m_Stop)
            {
                m_CriticalSection->Enter();
                int thisWritten = DAUDIO_Write((void*)m_Player->m_info, buffer + offset, len);	//将缓冲区的数据写入DirectSound进行播放
                if (thisWritten < 0)
                {
                    break;
                }
                m_Player->m_bytePosition += thisWritten;
                m_CriticalSection->Leave();

                len -= thisWritten;
                written += thisWritten;
                if (len > 0)
                {
                    offset += thisWritten;
                    m_CriticalSection->Enter();
                    Sleep(waitTime);
                    m_CriticalSection->Leave();
                }
                else
                {
                    break;
                }
            }

            //copy audio data to audio buffer
            //for audio data synchronize
            DWORD pLength = buffersize;
            jbyte* pAudioDataBuffer = pSpectrum->GetAudioDataBuffer();
            if (pAudioDataBuffer != NULL)
            {
                int wOverrun = 0;
                int iPosition = pSpectrum->GetPosition();
                DWORD dwAudioDataBufferLength = pSpectrum->GetAudioDataBufferLength();
                if (iPosition + pLength > (int)(dwAudioDataBufferLength - 1))
                {
                    wOverrun = (iPosition + pLength) - dwAudioDataBufferLength;
                    pLength = dwAudioDataBufferLength - iPosition;
                }

                memcpy(pAudioDataBuffer + iPosition, buffer, pLength);
                if (wOverrun > 0)
                {
                    memcpy(pAudioDataBuffer, buffer + pLength, wOverrun);
                    pSpectrum->SetPosition(wOverrun);
                }
                else
                {
                    pSpectrum->SetPosition(iPosition + pLength);
                }
            }

            if (isEndOfInput)
            {
                break;
            }
            /*Else do nothing, and continue.*/
        }

        m_Player->m_SpectrumAnalyser->Stop();
        DAUDIO_Stop((void*)m_Player->m_info, true);
        DAUDIO_Close((void*)m_Player->m_info, true);
        m_Player->m_bytePosition = 0;

        if (isEndOfInput)
        {
            c_emit sigFinished();	//播放完成信号
        }
        /*Else do nothing, and continue.*/
    }

    m_Player->m_info = NULL;
}

/************************************************************************/
/* CSpectrumAnalyserThread                                              */
/************************************************************************/
CSpectrumAnalyserThread::CSpectrumAnalyserThread(CSpectrumAnalyser* pSpectrumAnalyser) : CThread(true)
{
    m_Player = pSpectrumAnalyser->m_Player;
    m_SpectrumAnalyser = pSpectrumAnalyser;
    m_CriticalSection = new CCriticalSection;
    m_process = true;
    m_lfp = 0;
    m_frameSize = DEFAULT_FRAME_SIZE;
}

CSpectrumAnalyserThread::~CSpectrumAnalyserThread(void)
{
    if (m_CriticalSection != NULL)
    {
        delete m_CriticalSection;
        m_CriticalSection = NULL;
    }
    /*Else do nothing, and continue.*/
}

int CSpectrumAnalyserThread::calculateSamplePosition()
{
    jlong wFp = m_Player->GetLongFramePosition();
    jlong wNfp = m_lfp;
    m_lfp = wFp;
    int wSdp = (int)((jlong)(wNfp * m_frameSize) - (jlong)(m_SpectrumAnalyser->m_AudioDataBufferLength * m_SpectrumAnalyser->m_offset));
    return wSdp;
}

void CSpectrumAnalyserThread::processSamples(int nPosition)
{
    int c = nPosition;
    if (m_SpectrumAnalyser->m_channelMode == 1 && m_SpectrumAnalyser->m_sampleType == 1)
    {
        for (int a = 0; a < m_SpectrumAnalyser->m_SampleSize;)
        {
            if ((DWORD)c >= m_SpectrumAnalyser->m_AudioDataBufferLength)
            {
                m_SpectrumAnalyser->m_offset++;
                c -= m_SpectrumAnalyser->m_AudioDataBufferLength;
            }
            /*Else do nothing, and continue.*/

            m_SpectrumAnalyser->m_Left[a] = (float)m_SpectrumAnalyser->m_AudioDataBuffer[c] / 128.0F;
            m_SpectrumAnalyser->m_Right[a] = m_SpectrumAnalyser->m_Left[a];
            a++;
            c++;
        }
    }
    else if (m_SpectrumAnalyser->m_channelMode == 2 && m_SpectrumAnalyser->m_sampleType == 1)
    {
        for (int a = 0; a < m_SpectrumAnalyser->m_SampleSize;)
        {
            if ((DWORD)c >= m_SpectrumAnalyser->m_AudioDataBufferLength)
            {
                m_SpectrumAnalyser->m_offset++;
                c -= m_SpectrumAnalyser->m_AudioDataBufferLength;
            }
            /*Else do nothing, and continue.*/

            m_SpectrumAnalyser->m_Left[a] = (float)m_SpectrumAnalyser->m_AudioDataBuffer[c] / 128.0F;
            m_SpectrumAnalyser->m_Right[a] = (float)m_SpectrumAnalyser->m_AudioDataBuffer[c + 1] / 128.0F;
            a++;
            c += 2;
        }
    }
    else if (m_SpectrumAnalyser->m_channelMode == 1 && m_SpectrumAnalyser->m_sampleType == 2)
    {
        for (int a = 0; a < m_SpectrumAnalyser->m_SampleSize;)
        {
            if ((DWORD)c >= m_SpectrumAnalyser->m_AudioDataBufferLength)
            {
                m_SpectrumAnalyser->m_offset++;
                c -= m_SpectrumAnalyser->m_AudioDataBufferLength;
            }
            /*Else do nothing, and continue.*/

            m_SpectrumAnalyser->m_Left[a] = (float)((m_SpectrumAnalyser->m_AudioDataBuffer[c + 1] << 8) +
                m_SpectrumAnalyser->m_AudioDataBuffer[c]) / 32767.0F;
            m_SpectrumAnalyser->m_Right[a] = m_SpectrumAnalyser->m_Left[a];
            a++;
            c += 2;
        }
    }
    else if (m_SpectrumAnalyser->m_channelMode == 2 && m_SpectrumAnalyser->m_sampleType == 2)
    {
        for (int a = 0; a < m_SpectrumAnalyser->m_SampleSize;)
        {
            if ((DWORD)c >= m_SpectrumAnalyser->m_AudioDataBufferLength)
            {
                m_SpectrumAnalyser->m_offset++;
                c -= m_SpectrumAnalyser->m_AudioDataBufferLength;
            }
            /*Else do nothing, and continue.*/

            m_SpectrumAnalyser->m_Left[a] = (float)((m_SpectrumAnalyser->m_AudioDataBuffer[c + 1] << 8) +
                m_SpectrumAnalyser->m_AudioDataBuffer[c]) / 32767.0F;
            m_SpectrumAnalyser->m_Right[a] = (float)((m_SpectrumAnalyser->m_AudioDataBuffer[c + 3] << 8) +
                m_SpectrumAnalyser->m_AudioDataBuffer[c + 2]) / 32767.0F;
            a++;
            c += 4;
        }
    }
    /*Else do nothing, and continue.*/
}

void CSpectrumAnalyserThread::Execute()
{
    while (!m_Stop)
    {
        jlong wStn = CSystem::nanoTime();
        int wSdp = calculateSamplePosition();

        if (wSdp > 0)
        {
            processSamples(wSdp);
        }
        /*Else do nothing, and continue.*/

        for (int a = 0; a < 1; a++)
        {
            float wFrr = (float)m_SpectrumAnalyser->m_FpsAsNS / (float)m_SpectrumAnalyser->m_DesiredFpsAsNS;
            m_SpectrumAnalyser->Process(wFrr);
        }

        jlong wEdn = CSystem::nanoTime();
        long wDelay = m_SpectrumAnalyser->m_FpsAsNS - (long)(wEdn - wStn);

        if (wDelay > 0L)
        {
            DWORD ms = (DWORD)wDelay / 0xf4240L;
            DWORD ns = (DWORD)wDelay % 0xf4240L;
            if (ns >= 500000) ms++;
            Sleep(ms);

            if (m_SpectrumAnalyser->m_FpsAsNS > m_SpectrumAnalyser->m_DesiredFpsAsNS)
            {
                m_SpectrumAnalyser->m_FpsAsNS -= wDelay;
            }
            else
            {
                m_SpectrumAnalyser->m_FpsAsNS = m_SpectrumAnalyser->m_DesiredFpsAsNS;
            }
        }
        else
        {
            m_SpectrumAnalyser->m_FpsAsNS += -wDelay;
            Sleep(10);
        }
    }

    Sleep(50);
    memset(m_SpectrumAnalyser->m_peaks, 0, m_SpectrumAnalyser->m_saBands);
    memset(m_SpectrumAnalyser->m_peaksDelay, 0, m_SpectrumAnalyser->m_saBands);
    memset(m_SpectrumAnalyser->m_oldFFT, 0, m_SpectrumAnalyser->m_saFFTSampleSize);
}

/************************************************************************/
/* CSpectrumAnalyser                                                    */
/************************************************************************/
CSpectrumAnalyser::CSpectrumAnalyser(CBasicPlayer* pPlayer)
    : m_Player(pPlayer),
    m_SpectrumAnalyserThread(NULL)
{
    m_vecFrequency.clear();

    //创建一个频谱分析线程
    if (m_SpectrumAnalyserThread == NULL)
    {
        m_SpectrumAnalyserThread = new CSpectrumAnalyserThread(this);
    }
    /*Else do nothing, and continue.*/

    /* digital signal process */
    m_AudioDataBufferLength = DEFAULT_BUFFER_SIZE << 1;
    m_AudioDataBuffer = new jbyte[m_AudioDataBufferLength];
    m_SampleSize = DEFAULT_SAMPLE_SIZE;
    m_DesiredFpsAsNS = 0x3B9ACA00L / DEFAULT_FPS;
    m_FpsAsNS = m_DesiredFpsAsNS;
    m_Left = new float[DEFAULT_SAMPLE_SIZE];
    m_Right = new float[DEFAULT_SAMPLE_SIZE];
    m_position = 0;
    m_offset = 0;
    m_sampleType = SAMPLE_TYPE_SIXTEEN_BIT;
    m_channelMode = CHANNEL_MODE_STEREO;

    memset(m_AudioDataBuffer, 0, m_AudioDataBufferLength);
    memset(m_Left, 0, DEFAULT_SAMPLE_SIZE);
    memset(m_Right, 0, DEFAULT_SAMPLE_SIZE);

    /* spectrum analyser */
    m_width = DEFAULT_WIDTH;
    m_height = DEFAULT_HEIGHT;
    m_saFFTSampleSize = DEFAULT_SPECTRUM_ANALYSER_FFT_SAMPLE_SIZE;
    m_saBands = DEFAULT_SPECTRUM_ANALYSER_BAND_COUNT;				//频谱bar的条数
    m_saDecay = DEFAULT_SPECTRUM_ANALYSER_DECAY;
    m_FFT = new CFastFourierTransform(m_saFFTSampleSize);			//创建一个快速傅里叶变换对象
    m_peaks = new int[m_saBands];									//频谱bar数组集合
    m_peaksDelay = new int[m_saBands];								//频谱小点数组结合
    m_oldFFT = new float[m_saFFTSampleSize];
    m_saMultiplier = (float)((m_saFFTSampleSize / 2) / m_saBands);
    m_barOffset = 1;
    m_peakDelay = DEFAULT_SPECTRUM_ANALYSER_PEAK_DELAY;				//频谱小点停留的时间

    memset(m_peaks, 0, m_saBands);
    memset(m_peaksDelay, 0, m_saBands);
    memset(m_oldFFT, 0, m_saFFTSampleSize);
}

CSpectrumAnalyser::~CSpectrumAnalyser(void)
{
    if (m_SpectrumAnalyserThread != NULL)
    {
        delete m_SpectrumAnalyserThread;
        m_SpectrumAnalyserThread = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_AudioDataBuffer != NULL)
    {
        delete[] m_AudioDataBuffer;
        m_AudioDataBuffer = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_Left != NULL)
    {
        delete[] m_Left;
        m_Left = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_Right != NULL)
    {
        delete[] m_Right;
        m_Right = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_peaks != NULL)
    {
        delete[] m_peaks;
        m_peaks = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_peaksDelay != NULL)
    {
        delete[] m_peaksDelay;
        m_peaksDelay = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_oldFFT != NULL)
    {
        delete[] m_oldFFT;
        m_oldFFT = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_FFT != NULL)
    {
        delete m_FFT;
        m_FFT = NULL;
    }
    /*Else do nothing, and continue.*/
}

void CSpectrumAnalyser::Start()
{
    if (m_SpectrumAnalyserThread != NULL)
    {
        m_SpectrumAnalyserThread->Resume();
    }
    /*Else do nothing, and continue.*/
}

void CSpectrumAnalyser::Stop()
{
    if (m_SpectrumAnalyserThread != NULL)
    {
        m_SpectrumAnalyserThread->Stop();
    }
    /*Else do nothing, and continue.*/
}

void CSpectrumAnalyser::Process(float pFrameRateRatioHint)//pFrameRateRatioHint = 1
{
    for (int a = 0; a < m_SampleSize; a++)
    {
        m_Left[a] = (m_Left[a] + m_Right[a]) / 2.0f;
    }

    float c = 0;
    float pFrrh = pFrameRateRatioHint;
    float* wFFT = m_FFT->Calculate(m_Left, m_SampleSize);
    float wSadfrr = m_saDecay * pFrrh;
    float wBw = ((float)m_width / (float)m_saBands);

    m_vecFrequency.clear();
    for (int a = 0, bd = 0; bd < m_saBands; a += (int)m_saMultiplier, bd++)
    {
        float wFs = 0;
        for (int b = 0; b < (int)m_saMultiplier; b++)
        {
            wFs += wFFT[a + b];
        }

        wFs = (wFs * (float)log(bd + 2.0F));

        if (wFs > 0.005F && wFs < 0.009F)
        {
            wFs *= 9.0F * PI;
        }
        else if (wFs > 0.01F && wFs < 0.1F)
        {
            wFs *= 3.0F * PI;
        }
        else if (wFs > 0.1F && wFs < 0.5F)
        {
            wFs *= PI; //enlarge PI times, if do not, the bar display abnormally, why??
        }
        /*Else do nothing, and continue.*/

        if (wFs > 1.0F)
        {
            wFs = 0.9F;
        }
        /*Else do nothing, and continue.*/

        if (wFs >= (m_oldFFT[a] - wSadfrr))
        {
            m_oldFFT[a] = wFs;
        }
        else
        {
            m_oldFFT[a] -= wSadfrr;
            if (m_oldFFT[a] < 0)
            {
                m_oldFFT[a] = 0;
            }
            /*Else do nothing, and continue.*/
            wFs = m_oldFFT[a];
        }

        m_vecFrequency.push_back(wFs);

        c += wBw;
    }
    c_emit sigSpectrumChanged(m_vecFrequency);	//发射频谱改变信号
}

/************************************************************************/
/* CBasicPlayer                                                         */
/************************************************************************/
CBasicPlayer::CBasicPlayer(TCHAR *fileName)
    : m_PlayThread(NULL),
    m_SpectrumAnalyser(NULL),
    m_CriticalSection(NULL),
    m_Input(NULL),
    m_WmaInput(NULL),
    m_WavInput(NULL),
    m_Mp3Input(NULL),
    m_VorbisInput(NULL)
{
    if (fileName == NULL)
    {
        MessageBox(NULL, L"You should specify a file to play.", L"Error", MB_OK);
        return;
    }
    /*Else do nothing, and continue.*/

    //创建一个播放线程
    if (m_PlayThread == NULL)
    {
        m_PlayThread = new CPlayThread(this);
        c_connect(m_PlayThread, sigFinished, this, &CBasicPlayer::handleFinished);
    }
    /*Else do nothing, and continue.*/

    //创建一个频谱分析仪(创建频谱分析线程和创建快速傅里叶变换对象)
    if (m_SpectrumAnalyser == NULL)
    {
        m_SpectrumAnalyser = new CSpectrumAnalyser(this);
        c_connect(m_SpectrumAnalyser, sigSpectrumChanged, this, &CBasicPlayer::handleSpectrumChanged);
    }
    /*Else do nothing, and continue.*/

    //创建一个临界区
    if (m_CriticalSection == NULL)
    {
        m_CriticalSection = new CCriticalSection;
    }
    /*Else do nothing, and continue.*/

    WCHAR suffixName[MAX_PATH];
    //得到文件后缀名
    for (int i = lstrlen(fileName) - 1; i >= 0; --i)
    {
        if (fileName[i] == '.')
        {
            lstrcpy(suffixName, &fileName[i]);
            break;
        }
        /*Else do nothing, and continue.*/
    }

    //判断打开的音乐文件类型
    if (lstrcmp(suffixName, L".mp3") == 0 || lstrcmp(suffixName, L".MP3") == 0)
    {
        m_musicFormat = CBasicPlayer::mp3;
    }
    else if (lstrcmp(suffixName, L".wma") == 0 || lstrcmp(suffixName, L".WMA") == 0)
    {
        m_musicFormat = CBasicPlayer::wma;
    }
    else if (lstrcmp(suffixName, L".wav") == 0 || lstrcmp(suffixName, L".WAV") == 0)
    {
        m_musicFormat = CBasicPlayer::wav;
    }
    else if (lstrcmp(suffixName, L".ogg") == 0 || lstrcmp(suffixName, L".OGG") == 0)
    {
        m_musicFormat = CBasicPlayer::ogg;
    }
    /*Else do nothing, and continue.*/

    //根据不同的音乐格式建立相应的音乐文件读取类
    if (m_musicFormat == CBasicPlayer::wma)
    {
        if (m_WmaInput == NULL)
        {
            m_WmaInput = new CWmaInput;
        }
        /*Else do nothing, and continue.*/
        m_Input = (CFileInput*)m_WmaInput;
    }
    else if (m_musicFormat == CBasicPlayer::wav)
    {
        if (m_WavInput == NULL)
        {
            m_WavInput = new CWaveInput;
        }
        /*Else do nothing, and continue.*/
        m_Input = (CFileInput*)m_WavInput;
    }
    else if (m_musicFormat == CBasicPlayer::ogg)
    {
        if (m_VorbisInput == NULL)
        {
            m_VorbisInput = new CVorbisInput;
        }
        /*Else do nothing, and continue.*/
        m_Input = (CFileInput*)m_VorbisInput;
    }
    else if (m_musicFormat == CBasicPlayer::mp3)
    {
        if (m_Mp3Input == NULL)
        {
            m_Mp3Input = new CMp3Input;
        }
        /*Else do nothing, and continue.*/
        m_Input = (CFileInput*)m_Mp3Input;
    }

    m_Input->SetFileName(fileName);
    m_bytePosition = 0;
}

void CBasicPlayer::handleSpectrumChanged(vector<float> vecFrequency)
{
    c_emit sigSpectrumChanged(vecFrequency);
}

void CBasicPlayer::handleFinished()
{
    c_emit sigFinished();
}

CBasicPlayer::~CBasicPlayer(void)
{
    if (m_PlayThread != NULL)
    {
        delete m_PlayThread;
        m_PlayThread = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_SpectrumAnalyser != NULL)
    {
        delete m_SpectrumAnalyser;
        m_SpectrumAnalyser = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_CriticalSection != NULL)
    {
        delete m_CriticalSection;
        m_CriticalSection = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_Input != NULL)
    {
        m_Input->CloseFile();
    }
    /*Else do nothing, and continue.*/

    if (m_WmaInput != NULL)
    {
        delete m_WmaInput;
        m_WmaInput = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_WavInput != NULL)
    {
        delete m_WavInput;
        m_WavInput = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_Mp3Input != NULL)
    {
        delete m_Mp3Input;
        m_Mp3Input = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_VorbisInput != NULL)
    {
        delete m_VorbisInput;
        m_VorbisInput = NULL;
    }
    /*Else do nothing, and continue.*/
}

void CBasicPlayer::Start()
{
    m_BufferSize = DEFAULT_BUFFER_SIZE;
    m_SampleRate = m_Input->GetSampleRate();
    m_FrameSize = DEFAULT_FRAME_SIZE;
    m_BitPerSample = m_Input->GetBitsPerSample();
    m_Channels = m_Input->GetChannels();

    //初始化 direct sound 该函数主要是枚举输入输出设备
    signed int count = DAUDIO_GetDirectAudioDeviceCount();

    //该函数主要是创建DirectSound接口对象、设置协作级别以及创建辅助缓冲区
    m_info = (DS_Info*)DAUDIO_Open(0, 0, true, DAUDIO_PCM, m_SampleRate, m_BitPerSample, m_FrameSize, m_Channels, true, false, m_BufferSize);

    //启动频谱分析仪
    m_SpectrumAnalyser->Start();
    if (m_PlayThread != NULL && m_PlayThread->Suspended())	//如果该播放线程不为NULL并且处于暂停状态则恢复
    {
        m_PlayThread->Resume();								//恢复播放线程(调用Execute函数播放音乐)
    }
    /*Else do nothing, and continue.*/
}

void CBasicPlayer::Stop()
{
    if (m_PlayThread != NULL)
    {
        m_PlayThread->Stop();
        m_SpectrumAnalyser->Stop();
    }
    /*Else do nothing, and continue.*/
}

void CBasicPlayer::Pause()
{
    m_info->playBuffer->Stop();
}

void CBasicPlayer::Play()
{
    m_info->playBuffer->Play(0, 0, DSCBSTART_LOOPING);
}

jlong CBasicPlayer::GetLongFramePosition()
{
    jlong pos = DAUDIO_GetBytePosition((void*)m_info, true, m_bytePosition);
    if (pos < 0) pos = 0;
    return (jlong)(pos / DEFAULT_FRAME_SIZE);
}

CFileInput* CBasicPlayer::GetInput()
{
    return m_Input;
}

bool CBasicPlayer::isVaild()
{
    return m_Input->GetValid();
}
