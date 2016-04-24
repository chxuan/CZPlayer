#ifndef INCLUDE_IO
#define INCLUDE_IO

#include "AudioStream.h"
#include "CriticalSection.h"
#include "Stream.h"
/* libmad support */
#include "mad.h"
/* libogg support */
#include "ogg.h"
/* libvorbis support */
#include "codec.h"
#include "vorbisfile.h"

#include <stdio.h>
#include <iostream>
using namespace std;

/************************************************************************/
/* CInput                                                               */
/************************************************************************/
class CInput
{
public:
    CInput(void);
    ~CInput(void);

public:
    virtual void Reset();										//重置
    virtual void Pause();										//暂停
    virtual void Resume();										//恢复，重新开始
    virtual DWORD GetTotalTime();								//获得总时间
    virtual LARGE_INTEGER GetTotalSamples();					//获得总的采样率

    virtual WORD GetBitsPerSample() = 0;						//获得每秒的采样率
    virtual WORD GetChannels() = 0;								//获得通道数
    virtual DWORD GetSampleRate() = 0;							//获得采样率

    LARGE_INTEGER GetSize();									//得到文件大小
    LARGE_INTEGER GetPosition();								//获得位置
    void GetData(void** buffer, DWORD* size);					//得到数据
    DWORD CopyData(void* buffer, DWORD size);					//拷贝数据
    DWORD FillBuffer(void* buffer, DWORD size, bool* eof);		//填充缓冲区
    void Init();												//初始化
    void Flush();												//Flush缓冲区
    void Lock();												//加锁
    void Unlock();												//解锁

protected:
    LARGE_INTEGER m_Position;									//位置
    LARGE_INTEGER m_Size;										//大小
    WORD m_SampleSize;											//采样率大小
    bool m_Busy;												//是否忙
    DWORD m_BufferStart;										//缓冲区开始
    DWORD m_BufferEnd;											//缓冲区结束
    CCriticalSection* m_DataCS;									//临界区
    bool m_EndOfStream;											//文件流是否结束

protected:
    virtual void InitInternal() = 0;								//初始化内部数据
    virtual void FlushInternal() = 0;								//Flush内部数据
    virtual void GetDataInternal(void** buffer, DWORD* bytes) = 0;	//获取内部数据
};

/************************************************************************/
/* CStreamedInput                                                       */
/************************************************************************/
class CStreamedInput : public CInput
{
public:
    CStreamedInput(void);
    ~CStreamedInput(void);

public:
    void GetData(void** buf, DWORD* Bytes);							//获得数据
    bool Seek(LARGE_INTEGER* SampleNum);							//定位
    CStream* GetStream() { return m_pStream; }						//获得文件流

protected:
    CStream* m_pStream;												//文件流指针
    bool m_StreamAssigned;											//文件流是否分配
    bool m_Seekable;												//是否可定位
    LARGE_INTEGER m_StartSample;									//采样开始位置
    LARGE_INTEGER m_EndSample;										//采样结束位置
    bool m_Loop;													//是否循环
    LARGE_INTEGER m_TotalSamples;									//总的采样率

protected:
    virtual void SetStream(CStream* pStream);						//得到流
    virtual bool SeekInternal(LARGE_INTEGER* SampleNum) = 0;		//定位内部数据
};

/************************************************************************/
/* CFileInput                                                              */
/************************************************************************/
class CFileInput : public CStreamedInput
{
public:
    CFileInput(void);
    ~CFileInput(void);

public:
    virtual void OpenFile() = 0;									//打开文件
    virtual void CloseFile() = 0;									//关闭文件

    void SetFileName(TCHAR* pszFileName);							//设置文件名字
    size_t GetFileNameLen();										//获得文件名长度
    WORD GetBitsPerSample();										//获得每秒采样率
    DWORD GetSampleRate();											//获得采样率
    WORD GetChannels();												//获得通道数
    int GetOpened();												//文件是否被打开

    bool GetValid();												//文件是否是可得到的
    LARGE_INTEGER GetTotalSamples();								//获得总的采样率
    DWORD GetTotalTime();											//获得总时间

    bool SetStartTime(DWORD Minutes, DWORD Seconds);				//设置开始时间
    bool SetEndTime(DWORD Minutes, DWORD Seconds);					//设置结束时间
    void Reset();													//重置
    void Jump(int offset);											//跳到offset位置

protected:
    CCriticalSection* m_OpenCS;										//临界区
    TCHAR m_FileName[MAX_PATH];										//文件名
    size_t m_FileNameLen;											//文件名长度
    int m_Opened;													//文件是否打开
    bool m_Valid;													//文件流是否可获得
    WORD m_BitsPerSample;											//每秒采样率
    DWORD m_SampleRate;												//采样率
    WORD m_Channels;												//通道数
    DWORD m_Time;													//时间

protected:
    void SetStream(CStream* pStream);								//设置流
    void FlushInternal();											//Flush内部数据
    void InitInternal();											//初始化内部数据
};

/************************************************************************/
/* CWmaInput                                                                */
/************************************************************************/
typedef enum _ChannelsNumber
{
    cnMaxAvailable = 0,
    cnMonoOrStereo,
    cn5dot1,
    cn7dot1
} ChannelsNumber;

const int wmfDefault = -2;

class CWmaInput : public CFileInput
{
private:
    WMA_SYNC_READER* m_Reader;
    DWORD m_Duration;
    bool m_HighPrecision;
    ChannelsNumber m_OutputChannels;
    int m_Format;

    SHORT CNToShortInt();

protected:
    void GetDataInternal(void** buffer, DWORD* bytes);
    bool SeekInternal(LARGE_INTEGER* SampleNum);

public:
    CWmaInput(void);
    ~CWmaInput(void);

    void OpenFile();
    void CloseFile();

    void SetHighPrecision(bool HighPrecision);
    void SetOutputChannels(ChannelsNumber OutputChannels);
    bool GetHasAudio();
    bool GetProtected();
    DWORD GetBitrate();
    bool GetIsVBR();
    DWORD GetFormatsCount();
    void GetFormatSpec(int index, WMAFormatSpec* pFormatSpec);
    void SetFormat(int iFormat);
    int GetFormat();
};

/************************************************************************/
/* CWaveInput                                                           */
/************************************************************************/
#define BUF_SIZE 16000

#define WaveHeaderOffs 44
#define DataSizeOffs 40

//wav文件结构标志
#define LookingForRIFF 0
#define LookingForWave 1
#define LookingForFMT 2
#define LookingForFACT 3
#define LookingForDATA 4

#define WAVE_FORMAT_MP3 85

//wav文件的类型
typedef enum _WaveType
{
    wtUnsupported = 1,
    wtPCM = 2,
    wtDVIADPCM = 3,
    wtMSADPCM = 4,
    wtACM = 5,
    wtIEEEFloat = 6,
    wtExtPCM = 7,
    wtExtIEEEFloat = 8
} WaveType;

////////////////////////////开始定义wav文件结构体////////////////////////////////
typedef struct _WaveHeader
{
    char RIFF[4]; /* 'RIFF' offset : 0000 */
    DWORD FileSize; /* FileSize - 8 offset : 0004 */
    char RIFFType[4]; /* 'WAVE'  offset : 0008 */
    char FmtChunkId[4]; /* 'fmt '   offset : 0012 */
    DWORD FmtChunkSize; /* 16      offset : 0016 */
    WORD FormatTag; /* One of WAVE_FORMAT_XXX constants    offset : 0020 */
    WORD Channels; /* 1 - mono = 2 - stereo             offset : 0022 */
    DWORD SampleRate; /* offset : 0024 */
    DWORD BytesPerSecond; /* offset : 0028 */
    WORD BlockAlign; /* offset : 0032 */
    WORD BitsPerSample; /* 8, 16 or 32 Bits/sample offset : 0034 */
    char DataChunkId[4]; /* 'data' offset : 0036*/
    DWORD DataSize; /* Data size in bytes offset : 0040 */
} WaveHeader, *LPWaveHeader;

typedef struct _WaveHeaderEx
{
    char RIFF[4];
    DWORD FileSize;
    char RIFFType[4];
    char FmtChunkId[4];
    DWORD FmtChunkSize;
    WAVEFORMATEXTENSIBLE Format;
    char DataChunkId[4];
    DWORD DataSize;
} WaveHeaderEx, *LPWaveHeaderEx;

typedef struct _DVIADPCMHeader
{
    char RIFF[4];
    DWORD FileSize;
    char RIFFType[4];
    char FmtChunkId[4];
    DWORD FmtChunkSize;
    WORD FormatTag; //WAVE_FORMAT_DVI_ADPCM
    WORD Channels;
    DWORD SampleRate;
    DWORD BytesPerSecond;
    WORD BlockAlign;
    WORD BitsPerSample;
    WORD cbSize;
    WORD SamplesPerBlock;
    char FactChunkId[4];
    DWORD FactChunkSize;
    DWORD DataLength;
    char DataChunkId[4];
    DWORD DataSize;
} DVIADPCMHeader, *LPDVIADPCMHeader;

typedef struct _DVI_ADPCM_INFO
{
    WORD BlockLength;
    WORD SamplesPerBlock;
    DWORD DataSize;
} DVI_ADPCM_INFO, *LPDVI_ADPCM_INFO;

typedef struct _DVI_ADPCM_STATE_STEREO
{
    SHORT valprev_l; // Previous output value
    BYTE index_l; // Index into stepsize table
    SHORT valprev_r; // Previous output value
    BYTE index_r; // Index into stepsize table
} DVI_ADPCM_STATE_STEREO, *LPDVI_ADPCM_STATE_STEREO;

typedef struct _DVI_ADPCM_ENCODE_STATE_STEREO
{
    SHORT PredSamp_l;
    BYTE Index_l;
    SHORT PredSamp_r;
    BYTE Index_r;
} DVI_ADPCM_ENCODE_STATE_STEREO, *LPDVI_ADPCM_ENCODE_STATE_STEREO;

typedef struct _MS_ADPCM_COEF_SET
{
    SHORT Coef1;
    SHORT Coef2;
} MS_ADPCM_COEF_SET, *LPMS_ADPCM_COEF_SET;

typedef struct _MS_ADPCM_INFO
{
    WORD BlockLength;
    WORD SamplesPerBlock;
    DWORD DataSize;
    WORD NumCoeff;
    MS_ADPCM_COEF_SET CoefSets[32];
} MS_ADPCM_INFO, *LPMS_ADPCM_INFO;

typedef struct _MSADPCMBlockHeaderMono
{
    BYTE predictor;
    SHORT Delta;
    SHORT Samp1;
    SHORT Samp2;
} MSADPCMBlockHeaderMono, *LPMSADPCMBlockHeaderMono;

typedef struct _MSADPCMBlockHeaderStereo
{
    BYTE predictor[2];
    SHORT Delta[2];
    SHORT Samp1[2];
    SHORT Samp2[2];
} MSADPCMBlockHeaderStereo, *LPMSADPCMBlockHeaderStereo;

typedef struct _DVIADPCMBlockHeader
{
    SHORT Samp0;
    BYTE StepTableIndex;
    BYTE Reserved;
} DVIADPCMBlockHeader, *LPDVIADPCMBlockHeader;

////////////////////////////结束定义wav文件结构体////////////////////////////////

//读取wav文件的类
class CWaveInput : public CFileInput
{
public:
    CWaveInput(void);
    ~CWaveInput(void);

public:
    void OpenFile();																//打开文件
    void CloseFile();																//关闭文件

protected:
    void GetDataInternal(void** buffer, DWORD* bytes);								//获得内部数据
    bool SeekInternal(LARGE_INTEGER* SampleNum);									//定位内部数据

private:
    BYTE m_Buf[BUF_SIZE];															//缓冲区
    WaveType m_WavType;																//wav格式类型

    //wav格式类型
    DVI_ADPCM_INFO m_DVI_ADPCM_INFO;
    DVI_ADPCM_STATE_STEREO m_DVI_ADPCM_STATE;
    MS_ADPCM_INFO m_MS_ADPCM_INFO;
    MSADPCMBlockHeaderStereo m_MS_ADPCM_STATE;

    WORD m_HeaderSize;																//wav头大小
    CMemoryStream* m_MS;
    CStream* m_OldStream;
    bool m_OldStreamAssigned;
    bool m_ShortIEEEFloat;

private:
    bool ReadDVIADPCMBlock(void* pData);											//读取DVIADPCM块
    bool ReadMSADPCMBlock(void* pData);												//读取MSADPCM快
    WaveType GetWavType();															//获取wav格式类型
    void ReadRIFFHeader();															//读取RIFF头
    void DecodeDVIADPCMMono(BYTE* pInData, SHORT* pOutData, DWORD *len);			//对DVIADPCMMono类型进行解码
    void DecodeDVIADPCMStereo(BYTE* pInData, SHORT* pOutData, DWORD* len);			//对DVIADPCMStereo类型进行解码
    void DecodeMSADPCMMono(BYTE* pInData, SHORT* pOutData, DWORD* len);				//对MSADPCMMono类型进行解码
    void DecodeMSADPCMStereo(BYTE* pInData, SHORT* pOutData, DWORD* len);			//对MSADPCMStereo进行解码
};

bool Compare4(CHAR* S1, CHAR* S2);
void ConvertIEEEFloatTo32(int* InOutBuf, int InSize);
void ConvertShortIEEEFloatTo32(int* InOutBuf, int InSize);

/************************************************************************/
/* CMp3Input                                                               */
/************************************************************************/
# define SAMPLE_DEPTH 16
# define scale(x, y) dither((x), (y))

struct xing
{
    long flags;
    unsigned long frames;
    unsigned long bytes;
    unsigned char toc[100];
    long scale;
};

enum
{
    XING_FRAMES = 0x00000001L,
    XING_BYTES  = 0x00000002L,
    XING_TOC    = 0x00000004L,
    XING_SCALE  = 0x00000008L
};

# define XING_MAGIC (('X' << 24) | ('i' << 16) | ('n' << 8) | 'g')

struct dither
{
    mad_fixed_t error[3];
    mad_fixed_t random;
};

typedef struct _ID3Tag
{
    char m_cTAG[3];  // Must equal "TAG"
    char m_cSong[30];
    char m_cArtist[30];
    char m_cAlbum[30];
    char m_cYear[4];
    char m_cComment[30];
    unsigned char m_cGenre;
} ID3Tag;

typedef struct _ID3v2Tag
{
    char m_cTAG[3];  // Must equal ID3
    unsigned char m_cVersion[2]; // Major / Minor
    unsigned char m_cFlags;
    unsigned char m_cSize_Encoded[4];
} ID3v2Tag;

typedef struct _ID3v2Frame
{
    char m_cFrameID[4];
    unsigned char m_cSize_Encoded[4];
    unsigned short m_cFlags;
} ID3v2Frame;

int parse_xing(struct xing *xing, struct mad_bitptr ptr, unsigned int bitlen);
int scan_header(CStream* pStream, struct mad_header *header, struct xing *xing);							//读取mp3头信息
void pack_pcm(unsigned char **pcm, unsigned int nsamples, mad_fixed_t const *ch1, mad_fixed_t const *ch2);

class CMp3Input : public CFileInput
{
public:
    CMp3Input(void);
    ~CMp3Input(void);

public:
    void OpenFile();
    void CloseFile();

private:
    unsigned long m_size;
    unsigned long m_streamsize;
    unsigned int m_samplecount;
    mad_timer_t m_timer;
    mad_timer_t m_length;
    unsigned long m_rate;
    unsigned long m_frames;

    struct mad_stream m_stream;
    struct mad_frame m_frame;
    struct mad_synth m_synth;
    struct xing m_xing;
    bool m_vbr;				//cbr:常量位速率，vbr:可变位速率

    unsigned char m_buffer[40000];
    unsigned int m_buflen;
    unsigned char m_pcmbuffer[BUF_SIZE];

protected:
    void GetDataInternal(void** buffer, DWORD* bytes);
    bool SeekInternal(LARGE_INTEGER* SampleNum);
};

/************************************************************************/
/* CVorbisInput                                                            */
/************************************************************************/
class CVorbisInput : public CFileInput
{
private:
    OggVorbis_File m_OggVorbisFile;
    int m_CurrentSection;

    unsigned char m_buffer[40000];
    unsigned int m_buflen;
    unsigned char m_pcmbuffer[BUF_SIZE];
protected:
    void GetDataInternal(void** buffer, DWORD* bytes);
    bool SeekInternal(LARGE_INTEGER* SampleNum);
public:
    CVorbisInput(void);
    ~CVorbisInput(void);

    void OpenFile();
    void CloseFile();

};

size_t vorbisRead(void *buffer, size_t size, size_t amount, void *file);
int vorbisSeek(void *file, ogg_int64_t newpos, int set);
int vorbisClose(void *file);
long vorbisTell(void *file);

#endif

