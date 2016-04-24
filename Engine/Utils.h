#ifndef UTILS_H
#define UTILS_H

#include <tchar.h>
#include <windows.h>
#include <dsound.h>
#include <strsafe.h>

#define USE_DAUDIO TRUE
#define USE_ERROR
#define USE_TRACE

#define DAUDIO_PCM  0
#define DAUDIO_ULAW 1
#define DAUDIO_ALAW 2

typedef unsigned char   UBYTE;
typedef char            SBYTE;

#define DAUDIO_STRING_LENGTH 200

#if 0
typedef struct tag_DirectAudioDeviceDescription {
    // optional deviceID (complementary to deviceIndex)
    signed int deviceID;
    signed int maxSimulLines;
    TCHAR name[DAUDIO_STRING_LENGTH+1];
    TCHAR vendor[DAUDIO_STRING_LENGTH+1];
    TCHAR description[DAUDIO_STRING_LENGTH+1];
    TCHAR version[DAUDIO_STRING_LENGTH+1];
} DirectAudioDeviceDescription;
#else
typedef struct tag_DirectAudioDeviceDescription {
    // optional deviceID (complementary to deviceIndex)
    signed int deviceID;
    signed int maxSimulLines;
    WCHAR name[DAUDIO_STRING_LENGTH+1];
    WCHAR vendor[DAUDIO_STRING_LENGTH+1];
    WCHAR description[DAUDIO_STRING_LENGTH+1];
    WCHAR version[DAUDIO_STRING_LENGTH+1];
} DirectAudioDeviceDescription;
#endif

typedef struct {
    signed int mixerIndex;
    bool isSource;
    /* either LPDIRECTSOUND8 or LPDIRECTSOUNDCAPTURE8 */
    void* dev;
    /* how many instances use the dev */
    signed int refCount;
    GUID guid;
} DS_AudioDeviceCache;

typedef struct {
    signed int currMixerIndex;
    bool isSource;
} DS_RefreshCacheStruct;

typedef struct {
    int deviceID;
    /* for convenience */
    bool isSource;
    /* the secondary buffer (Playback) */
    LPDIRECTSOUNDBUFFER8 playBuffer;
    /* the secondary buffer (Capture) */
    LPDIRECTSOUNDCAPTUREBUFFER8 captureBuffer;

    /* size of the directsound buffer, usually 2 seconds */
    int dsBufferSizeInBytes;

    /* size of the read/write-ahead, as specified by Java */
    int bufferSizeInBytes;
    int bitsPerSample;
    int frameSize; // storage size in Bytes

    unsigned long long framePos;
    /* where to write into the buffer.
    * -1 if at current position (Playback)
    * For Capture, this is the read position
    */
    int writePos;

    /* if start() had been called */
    bool started;

    /* how many bytes there is silence from current write position */
    int silencedBytes;

    bool underrun;

} DS_Info;

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_
typedef struct {
    WAVEFORMATEX    Format;
    union {
        WORD wValidBitsPerSample;       /* bits of precision  */
        WORD wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
        WORD wReserved;                 /* If neither applies, set to zero. */
    } Samples;
    DWORD           dwChannelMask;      /* which channels are */
    /* present in stream  */
    GUID            SubFormat;
} WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;
#endif // !_WAVEFORMATEXTENSIBLE_

#if !defined(WAVE_FORMAT_EXTENSIBLE)
#define  WAVE_FORMAT_EXTENSIBLE                 0xFFFE
#endif // !defined(WAVE_FORMAT_EXTENSIBLE)

#if !defined(DEFINE_WAVEFORMATEX_GUID)
#define DEFINE_WAVEFORMATEX_GUID(x) (USHORT)(x), 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71
#endif
#ifndef STATIC_KSDATAFORMAT_SUBTYPE_PCM
#define STATIC_KSDATAFORMAT_SUBTYPE_PCM\
    DEFINE_WAVEFORMATEX_GUID(WAVE_FORMAT_PCM)
#endif

#ifdef USE_ERROR
#define ERROR0(string)                        fprintf(stdout, (string)); fflush(stdout);
#define ERROR1(string, p1)                    fprintf(stdout, (string), (p1)); fflush(stdout);
#define ERROR2(string, p1, p2)                fprintf(stdout, (string), (p1), (p2)); fflush(stdout);
#define ERROR3(string, p1, p2, p3)            fprintf(stdout, (string), (p1), (p2), (p3)); fflush(stdout);
#define ERROR4(string, p1, p2, p3, p4)        fprintf(stdout, (string), (p1), (p2), (p3), (p4)); fflush(stdout);
#else
#define ERROR0(string)
#define ERROR1(string, p1)
#define ERROR2(string, p1, p2)
#define ERROR3(string, p1, p2, p3)
#define ERROR4(string, p1, p2, p3, p4)
#endif


// TRACE PRINTS
#ifdef USE_TRACE
#define TRACE0(string)                        fprintf(stdout, (string)); fflush(stdout);
#define TRACE1(string, p1)                    fprintf(stdout, (string), (p1)); fflush(stdout);
#define TRACE2(string, p1, p2)                fprintf(stdout, (string), (p1), (p2)); fflush(stdout);
#define TRACE3(string, p1, p2, p3)            fprintf(stdout, (string), (p1), (p2), (p3)); fflush(stdout);
#define TRACE4(string, p1, p2, p3, p4)        fprintf(stdout, (string), (p1), (p2), (p3), (p4)); fflush(stdout);
#define TRACE5(string, p1, p2, p3, p4, p5)    fprintf(stdout, (string), (p1), (p2), (p3), (p4), (p5)); fflush(stdout);
#else
#define TRACE0(string)
#define TRACE1(string, p1)
#define TRACE2(string, p1, p2)
#define TRACE3(string, p1, p2, p3)
#define TRACE4(string, p1, p2, p3, p4)
#define TRACE5(string, p1, p2, p3, p4, p5)
#endif


// VERBOSE TRACE PRINTS
#ifdef USE_VERBOSE_TRACE
#define VTRACE0(string)                 fprintf(stdout, (string));
#define VTRACE1(string, p1)             fprintf(stdout, (string), (p1));
#define VTRACE2(string, p1, p2)         printf(stdout, (string), (p1), (p2));
#define VTRACE3(string, p1, p2, p3)     fprintf(stdout, (string), (p1), (p2), (p3));
#define VTRACE4(string, p1, p2, p3, p4) fprintf(stdout, (string), (p1), (p2), (p3), (p4));
#else
#define VTRACE0(string)
#define VTRACE1(string, p1)
#define VTRACE2(string, p1, p2)
#define VTRACE3(string, p1, p2, p3)
#define VTRACE4(string, p1, p2, p3, p4)
#endif

//#if (USE_DAUDIO == TRUE)
// callback from GetFormats, implemented in DirectAudioDevice.c
void DAUDIO_AddAudioFormat(void* creator, int significantBits, int frameSizeInBytes,
                           int channels, float sampleRate,
                           int encoding, int isSigned,
                           int bigEndian);


// the following methods need to be implemented by the platform dependent code

/* returns the number of mixer devices */
signed int DAUDIO_GetDirectAudioDeviceCount();

/* returns true on success, false otherwise */
signed int DAUDIO_GetDirectAudioDeviceDescription(signed int mixerIndex,
                                             DirectAudioDeviceDescription* description);

// SourceDataLine and TargetDataLine

void DAUDIO_GetFormats(signed int mixerIndex, signed int deviceID, int isSource, void* creator);

void* DAUDIO_Open(signed int mixerIndex, signed int deviceID, int isSource,
                  int encoding, float sampleRate, int sampleSizeInBits,
                  int frameSize, int channels,
                  int isSigned, int isBigEndian, int bufferSizeInBytes);
int DAUDIO_Start(void* id, int isSource);
int DAUDIO_Stop(void* id, int isSource);
void DAUDIO_Close(void* id, int isSource);
int DAUDIO_Write(void* id, char* data, int byteSize); // returns -1 on error
int DAUDIO_Read(void* id, char* data, int byteSize);  // returns -1 on error

int DAUDIO_GetBufferSize(void* id, int isSource);
int DAUDIO_StillDraining(void* id, int isSource);
int DAUDIO_Flush(void* id, int isSource);
/* in bytes */
int DAUDIO_GetAvailable(void* id, int isSource);
INT64 DAUDIO_GetBytePosition(void* id, int isSource, INT64 javaBytePos);
void DAUDIO_SetBytePosition(void* id, int isSource, INT64 javaBytePos);

int DAUDIO_RequiresServicing(void* id, int isSource);
void DAUDIO_Service(void* id, int isSource);

void DAUDIO_AddAudioFormat(void* creatorV, int significantBits, int frameSizeInBytes,
                           int channels, float sampleRate,
                           int encoding, int isSigned,
                           int bigEndian);

//#endif

#endif
