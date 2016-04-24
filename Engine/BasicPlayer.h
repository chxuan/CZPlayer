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
* \file   BasicPlayer.h
* \author chengxuan   787280310@qq.com
* \date   2015-02-06
* \brief  播放引擎头文件。
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

#ifndef BASICPLAYER_H
#define BASICPLAYER_H

/*! \def BASICPLAYER_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define BASICPLAYER_VERSION   0x030000

#include "AudioStream.h"
#include "FastFourierTransform.h"
#include "Utils.h"
#include "Thread.h"
#include "signal_slot.h"
#include <vector>
using namespace std;

// pre-defined class
class CCriticalSection;
class CInput;
class CFileInput;
class CWmaInput;
class CMp3Input;
class CWaveInput;
class CVorbisInput;
class CBasicPlayer;
class CSpectrumAnalyser;

#pragma warning(disable : 4244)

/************************************************************************/
/* CSystem                                                              */
/************************************************************************/
typedef __int64				jlong;
typedef unsigned int		juint;
typedef unsigned __int64	julong;
typedef long				jint;
typedef signed char			jbyte;

#define CONST64(x)				(x ## LL)
#define NANOS_PER_SEC			CONST64(1000000000)
#define NANOS_PER_MILLISEC		1000000

jlong as_long(LARGE_INTEGER x);
void set_high(jlong* value, jint high);
void set_low(jlong* value, jint low);

class CSystem
{
private:
    static jlong frequency;
    static int ready;

    static void init()
    {
        LARGE_INTEGER liFrequency = { 0 };
        QueryPerformanceFrequency(&liFrequency);
        frequency = as_long(liFrequency);
        ready = 1;
    }
public:
    static jlong nanoTime()
    {
        if (ready != 1)
            init();

        LARGE_INTEGER liCounter = { 0 };
        QueryPerformanceCounter(&liCounter);
        double current = as_long(liCounter);
        double freq = frequency;
        return (jlong)((current / freq) * NANOS_PER_SEC);
    }
};

/************************************************************************/
/* CPlayThread                                                          */
/************************************************************************/

/*! 播放线程
*/
class CPlayThread : public CThread
{
public:
    CPlayThread(CBasicPlayer* pPlayer);
    ~CPlayThread(void);

protected:
    void Execute();					//执行

c_signals:
    Signal sigFinished;			//播放完成信号

private:
    CBasicPlayer* m_Player;
    CCriticalSection* m_CriticalSection;
};

/************************************************************************/
/* CSpectrumAnalyserThread                                              */
/************************************************************************/

/*! 频谱分析线程
*/
class CSpectrumAnalyserThread : public CThread
{
public:
    CSpectrumAnalyserThread(CSpectrumAnalyser* pSpectrumAnalyser);
    ~CSpectrumAnalyserThread(void);

private:
    CBasicPlayer* m_Player;
    CSpectrumAnalyser* m_SpectrumAnalyser;
    CCriticalSection* m_CriticalSection;
    bool m_process;
    jlong m_lfp;
    int m_frameSize;

private:
    int calculateSamplePosition();
    void processSamples(int pPosition);

protected:
    void Execute();
};

/************************************************************************/
/* CSpectrumAnalyser：频谱分析仪                                        */
/************************************************************************/

/*! 频谱分析仪
*/
class CSpectrumAnalyser
{
    friend class CSpectrumAnalyserThread;

public:
    CSpectrumAnalyser(CBasicPlayer* pPlayer);
    ~CSpectrumAnalyser(void);

public:
    void Start();
    void Stop();
    void Process(float pFrameRateRatioHint);

    jbyte* GetAudioDataBuffer() { return m_AudioDataBuffer; }

    int GetPosition() { return m_position; }
    void SetPosition(int pPosition) { m_position = pPosition; }

    DWORD GetAudioDataBufferLength() { return m_AudioDataBufferLength; }
    void SetAudioDataBufferLength(DWORD pAudioDataBufferLength) { m_AudioDataBufferLength = pAudioDataBufferLength; }

c_signals:
    Signal1<vector<float>> sigSpectrumChanged;

private:
    CBasicPlayer* m_Player;
    CSpectrumAnalyserThread* m_SpectrumAnalyserThread;	//频谱分析线程
    CFastFourierTransform* m_FFT;						//快速傅里叶变换
    vector<float> m_vecFrequency;						//频率集合

    /* digital signal process */
    DWORD m_AudioDataBufferLength;
    jbyte* m_AudioDataBuffer;
    int m_SampleSize;
    LONG m_FpsAsNS;
    LONG m_DesiredFpsAsNS;
    float* m_Left;
    float* m_Right;
    int m_position;
    int m_offset;
    int m_sampleType;
    int m_channelMode;

    /* spectrum analyser */
    int m_width;
    int m_height;
    int* m_peaks;
    int* m_peaksDelay;
    float* m_oldFFT;
    int m_saFFTSampleSize;
    int m_saBands;
    float m_saMultiplier;
    float m_saDecay;
    int m_barOffset;
    int m_peakDelay;
    int m_winwidth, m_winheight;
};

/************************************************************************/
/* CBasicPlayer                                                         */
/************************************************************************/

/*! 播放引擎类
*/
class CBasicPlayer
{
    //friend class declare
    friend class CPlayThread;
    friend class CSpectrumAnalyser;

public:
    /*! 构造函数
    *  \param fileName 文件路径
    */
    CBasicPlayer(TCHAR* fileName);

    /*! 析构函数.*/
    ~CBasicPlayer(void);

public:
    /*! 获得Input文件流
    */
    CFileInput* GetInput();

    /*! 是否可以获取播放源
    */
    bool isVaild();

    /*! 开始播放
    */
    void Start();

    /*! 停止播放
    */
    void Stop();

    /*! 暂停
    */
    void Pause();

    /*! 恢复播放
    */
    void Play();

    /*! 获取帧位置
    */
    jlong GetLongFramePosition();

c_signals:
    Signal1<vector<float>> sigSpectrumChanged;	/*!< 频谱改变*/
    Signal sigFinished;							/*!< 播放完成*/

private c_slots:
    /*! 频谱改变
     * \param vecFrequency 频谱数据
     */
    void handleSpectrumChanged(vector<float> vecFrequency);

    /*! 播放完成
    */
    void handleFinished();

private:
    CPlayThread* m_PlayThread;				/*!< 播放线程*/
    CSpectrumAnalyser* m_SpectrumAnalyser;	/*!< 频谱分析仪*/
    CCriticalSection* m_CriticalSection;	/*!< 临界区*/

    CFileInput* m_Input;			/*!< 文件IO*/
    CWmaInput* m_WmaInput;			/*!< wma文件IO*/
    CMp3Input* m_Mp3Input;			/*!< mp3文件IO*/
    CWaveInput* m_WavInput;			/*!< wave文件IO*/
    CVorbisInput* m_VorbisInput;	/*!< vorbis文件IO*/

    DS_Info* m_info;				/*!< DirectSound信息*/
    volatile long m_bytePosition;	/*!< byte位置*/

    DWORD m_BufferSize;				/*!< 缓冲区大小*/
    float m_SampleRate;				/*!< 采样率*/
    WORD m_FrameSize;				/*!< 帧数*/
    WORD m_BitPerSample;			/*!< 每秒采样率*/
    WORD m_Channels;				/*!< 通道数*/

    /*! 音乐格式*/
    enum MusicFormat
    {
        mp3 = 1,
        wav = 2,
        wma = 3,
        ogg = 4
    };

    MusicFormat m_musicFormat;	/*!< 音乐格式*/
};

#endif
