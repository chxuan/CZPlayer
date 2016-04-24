#include "Utils.h"
#include <tchar.h>

#define _LITTLE_ENDIAN

//#if USE_DAUDIO == TRUE

#pragma warning(disable : 4996)
#pragma warning(disable : 4100)
#pragma warning(disable : 4800)

/* half a minute to wait before device list is re-read */
#define WAIT_BETWEEN_CACHE_REFRESH_MILLIS 30000

/* maximum number of supported devices, playback+capture */
#define MAX_DS_DEVICES 60

static DS_AudioDeviceCache g_audioDeviceCache[MAX_DS_DEVICES];
static signed int g_cacheCount = 0;
static unsigned long long g_lastCacheRefreshTime = 0;
static signed int g_mixerCount = 0;

bool DS_lockCache() {
    /* dummy implementation for now, Java does locking */
    return true;
}

void DS_unlockCache() {
    /* dummy implementation for now */
}

static GUID CLSID_DAUDIO_Zero = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

bool isEqualGUID(LPGUID lpGuid1, LPGUID lpGuid2)
{
    if (lpGuid1 == NULL || lpGuid2 == NULL)
    {
        if (lpGuid1 == lpGuid2)
        {
            return true;
        }

        if (lpGuid1 == NULL)
        {
            lpGuid1 = (LPGUID) (&CLSID_DAUDIO_Zero);
        }
        else
        {
            lpGuid2 = (LPGUID) (&CLSID_DAUDIO_Zero);
        }
    }

    return memcmp(lpGuid1, lpGuid2, sizeof(GUID)) == 0;
}

signed int findCacheItemByGUID(LPGUID lpGuid, bool isSource)
{
    int i;
    for (i = 0; i < g_cacheCount; i++)
    {
        if (isSource == g_audioDeviceCache[i].isSource
            && isEqualGUID(lpGuid, &(g_audioDeviceCache[i].guid)))
        {
                return i;
        }
    }

    return -1;
}

signed int findCacheItemByMixerIndex(signed int mixerIndex)
{
    int i;
    for (i = 0; i < g_cacheCount; i++)
    {
        if (g_audioDeviceCache[i].mixerIndex == mixerIndex)
        {
            return i;
        }
    }

    return -1;
}

bool CALLBACK DS_RefreshCacheEnum(LPGUID lpGuid, LPCSTR lpstrDescription,
                                  LPCSTR lpstrModule, DS_RefreshCacheStruct* rs)
{
    signed int cacheIndex = findCacheItemByGUID(lpGuid, rs->isSource);
    /*//TRACE3("Enumerating %d: %s (%s)\n", cacheIndex, lpstrDescription, lpstrModule);*/
    if (cacheIndex == -1)
    {
        /* add this device */
        if (g_cacheCount < MAX_DS_DEVICES-1)
        {
            g_audioDeviceCache[g_cacheCount].mixerIndex = rs->currMixerIndex;
            g_audioDeviceCache[g_cacheCount].isSource = rs->isSource;
            g_audioDeviceCache[g_cacheCount].dev = NULL;
            g_audioDeviceCache[g_cacheCount].refCount = 0;

            if (lpGuid == NULL)
            {
                memset(&(g_audioDeviceCache[g_cacheCount].guid), 0, sizeof(GUID));
            }
            else
            {
                memcpy(&(g_audioDeviceCache[g_cacheCount].guid), lpGuid, sizeof(GUID));
            }

            g_cacheCount++;
            rs->currMixerIndex++;
        }
        else
        {
            /* failure case: more than MAX_DS_DEVICES available... */
        }
    }
    else
    {
        /* device already exists in cache... update mixer number */
        g_audioDeviceCache[cacheIndex].mixerIndex = rs->currMixerIndex;
        rs->currMixerIndex++;
    }

    /* continue enumeration */
    return true;
}

///// implemented functions of DirectAudio.h
//该函数主要是枚举输入输出设备
signed int DAUDIO_GetDirectAudioDeviceCount()
{
    DS_RefreshCacheStruct rs;
    signed int oldCount;
    signed int cacheIndex;

    if (!DS_lockCache())
    {
        return 0;
    }

    if (g_lastCacheRefreshTime == 0 || (unsigned long long) timeGetTime() > (unsigned long long) (g_lastCacheRefreshTime + WAIT_BETWEEN_CACHE_REFRESH_MILLIS))
    {
            /* first, initialize any old cache items */
            for (cacheIndex = 0; cacheIndex < g_cacheCount; cacheIndex++)
            {
                g_audioDeviceCache[cacheIndex].mixerIndex = -1;
            }

            /* enumerate all devices and either add them to the device cache,
            * or refresh the mixer number
            */
            rs.currMixerIndex = 0;
            rs.isSource = true;
            DirectSoundEnumerate((LPDSENUMCALLBACK) DS_RefreshCacheEnum, &rs);//枚举系统输出声音的设备
            /* if we only got the Primary Sound Driver (GUID=NULL),
            * then there aren't any playback devices installed */
            if (rs.currMixerIndex == 1)
            {
                cacheIndex = findCacheItemByGUID(NULL, true);
                if (cacheIndex == 0)
                {
                    rs.currMixerIndex = 0;
                    g_audioDeviceCache[0].mixerIndex = -1;
                    //TRACE0("Removing stale Primary Sound Driver from list.\n");
                }
            }
            oldCount = rs.currMixerIndex;
            rs.isSource = false;
            DirectSoundCaptureEnumerate((LPDSENUMCALLBACK) DS_RefreshCacheEnum, &rs);//枚举系统输入声音的设备
            /* if we only got the Primary Sound Capture Driver (GUID=NULL),
            * then there aren't any capture devices installed */
            if ((rs.currMixerIndex - oldCount) == 1)
            {
                cacheIndex = findCacheItemByGUID(NULL, false);
                if (cacheIndex != -1)
                {
                    rs.currMixerIndex = oldCount;
                    g_audioDeviceCache[cacheIndex].mixerIndex = -1;
                    //TRACE0("Removing stale Primary Sound Capture Driver from list.\n");
                }
            }
            g_mixerCount = rs.currMixerIndex;

            g_lastCacheRefreshTime = (unsigned long long) timeGetTime();
    }

    DS_unlockCache();
    return g_mixerCount;
}

bool CALLBACK DS_GetDescEnum(LPGUID lpGuid, LPCTSTR lpstrDescription,
                             LPCTSTR lpstrModule, DirectAudioDeviceDescription* desc)
{

    signed int cacheIndex = findCacheItemByGUID(lpGuid, g_audioDeviceCache[desc->deviceID].isSource);
    if (cacheIndex == desc->deviceID) {
        StringCchCopy(desc->name, DAUDIO_STRING_LENGTH, lpstrDescription);
        //strncpy(desc->name, lpstrDescription, DAUDIO_STRING_LENGTH);
        //strncpy(desc->description, lpstrModule, DAUDIO_STRING_LENGTH);
        desc->maxSimulLines = -1;
        /* do not continue enumeration */
        return false;
    }
    return true;
}


signed int DAUDIO_GetDirectAudioDeviceDescription(signed int mixerIndex, DirectAudioDeviceDescription* desc) {

    if (!DS_lockCache()) {
        return false;
    }

    /* set the deviceID field to the cache index */
    desc->deviceID = findCacheItemByMixerIndex(mixerIndex);
    if (desc->deviceID < 0) {
        DS_unlockCache();
        return false;
    }
    desc->maxSimulLines = 0;
    if (g_audioDeviceCache[desc->deviceID].isSource) {
        DirectSoundEnumerate((LPDSENUMCALLBACK) DS_GetDescEnum, desc);
        StringCchCopy(desc->description, DAUDIO_STRING_LENGTH, L"DirectSound Playback");
        //strncpy(desc->description, "DirectSound Playback", DAUDIO_STRING_LENGTH);
    } else {
        DirectSoundCaptureEnumerate((LPDSENUMCALLBACK) DS_GetDescEnum, desc);
        StringCchCopy(desc->description, DAUDIO_STRING_LENGTH, L"DirectSound Capture");
        //strncpy(desc->description, "DirectSound Capture", DAUDIO_STRING_LENGTH);
    }

    /*desc->vendor;
    desc->version;*/

    DS_unlockCache();
    return (desc->maxSimulLines == -1)?true:false;
}

/* multi-channel info: http://www.microsoft.com/whdc/hwdev/tech/audio/multichaud.mspx */

//static UINT32 sampleRateArray[] = { 8000, 11025, 16000, 22050, 32000, 44100, 48000, 56000, 88000, 96000, 172000, 192000 };
static signed int sampleRateArray[] = { -1 };
static signed int channelsArray[] = { 1, 2};
static signed int bitsArray[] = { 8, 16};

#define SAMPLERATE_COUNT sizeof(sampleRateArray)/sizeof(signed int)
#define CHANNELS_COUNT sizeof(channelsArray)/sizeof(signed int)
#define BITS_COUNT sizeof(bitsArray)/sizeof(signed int)

void DAUDIO_GetFormats(signed int mixerIndex, signed int deviceID, int isSource, void* creator) {

    int rateIndex, channelIndex, bitIndex;

    /* no need to lock, since deviceID identifies the device sufficiently */

    /* sanity */
    if (deviceID >= g_cacheCount) {
        return;
    }
    if ((g_audioDeviceCache[deviceID].isSource && !isSource)
        || (!g_audioDeviceCache[deviceID].isSource && isSource)) {
            /* only support Playback or Capture */
            return;
    }

    for (rateIndex = 0; rateIndex < SAMPLERATE_COUNT; rateIndex++) {
        for (channelIndex = 0; channelIndex < CHANNELS_COUNT; channelIndex++) {
            for (bitIndex = 0; bitIndex < BITS_COUNT; bitIndex++) {
                DAUDIO_AddAudioFormat(creator, bitsArray[bitIndex],
                    ((bitsArray[bitIndex] + 7) / 8) * channelsArray[channelIndex],
                    channelsArray[channelIndex],
                    (float) sampleRateArray[rateIndex],
                    DAUDIO_PCM,
                    (bitsArray[bitIndex]==8)?false:true,  /* signed */
                    (bitsArray[bitIndex]==8)?false:
#ifndef _LITTLE_ENDIAN
                    true /* big endian */
#else
                    false /* little endian */
#endif
                    );
            }
        }
    }
}

LPSTR TranslateDSError(HRESULT hr) {
    switch(hr) {
        case DSERR_ALLOCATED:
            return "DSERR_ALLOCATED";

        case DSERR_CONTROLUNAVAIL:
            return "DSERR_CONTROLUNAVAIL";

        case DSERR_INVALIDPARAM:
            return "DSERR_INVALIDPARAM";

        case DSERR_INVALIDCALL:
            return "DSERR_INVALIDCALL";

        case DSERR_GENERIC:
            return "DSERR_GENERIC";

        case DSERR_PRIOLEVELNEEDED:
            return "DSERR_PRIOLEVELNEEDED";

        case DSERR_OUTOFMEMORY:
            return "DSERR_OUTOFMEMORY";

        case DSERR_BADFORMAT:
            return "DSERR_BADFORMAT";

        case DSERR_UNSUPPORTED:
            return "DSERR_UNSUPPORTED";

        case DSERR_NODRIVER:
            return "DSERR_NODRIVER";

        case DSERR_ALREADYINITIALIZED:
            return "DSERR_ALREADYINITIALIZED";

        case DSERR_NOAGGREGATION:
            return "DSERR_NOAGGREGATION";

        case DSERR_BUFFERLOST:
            return "DSERR_BUFFERLOST";

        case DSERR_OTHERAPPHASPRIO:
            return "DSERR_OTHERAPPHASPRIO";

        case DSERR_UNINITIALIZED:
            return "DSERR_UNINITIALIZED";

        default:
            return "Unknown HRESULT";
    }
}

/*
** data/routines for starting DS buffers by separate thread
** (joint into DS_StartBufferHelper class)
** see cr6372428: playback fails after exiting from thread that has started it
** due IDirectSoundBuffer8::Play() description:
** http://msdn.microsoft.com/archive/default.asp?url=/archive/en-us/directx9_c
**       /directx/htm/idirectsoundbuffer8play.asp
** (remark section): If the application is multithreaded, the thread that plays
** the buffer must continue to exist as long as the buffer is playing.
** Buffers created on WDM drivers stop playing when the thread is terminated.
** IDirectSoundCaptureBuffer8::Start() has the same remark:
** http://msdn.microsoft.com/archive/default.asp?url=/archive/en-us/directx9_c
**       /directx/htm/idirectsoundcapturebuffer8start.asp
*/
class DS_StartBufferHelper {
public:
    /* starts DirectSound buffer (playback or capture) */
    static HRESULT StartBuffer(DS_Info* info);
    /* checks for initialization success */
    static inline bool isInitialized() { return data.threadHandle != NULL; }
protected:
    DS_StartBufferHelper() {}  // no need to create an instance

    /* data class */
    class Data {
    public:
        Data();
        ~Data();
        // public data to access from parent class
        CRITICAL_SECTION crit_sect;
        volatile HANDLE threadHandle;
        volatile HANDLE startEvent;
        volatile HANDLE startedEvent;
        volatile DS_Info* line2Start;
        volatile HRESULT startResult;
    } static data;

    /* StartThread function */
    static DWORD WINAPI __stdcall ThreadProc(void *param);
};

/* StartBufferHelper class implementation
*/
DS_StartBufferHelper::Data DS_StartBufferHelper::data;

DS_StartBufferHelper::Data::Data() {
    threadHandle = NULL;
    ::InitializeCriticalSection(&crit_sect);
    startEvent = ::CreateEvent(NULL, false, false, NULL);
    startedEvent = ::CreateEvent(NULL, false, false, NULL);
    if (startEvent != NULL && startedEvent != NULL)
        threadHandle = ::CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
}

DS_StartBufferHelper::Data::~Data() {
    ::EnterCriticalSection(&crit_sect);
    if (threadHandle != NULL) {
        // terminate thread
        line2Start = NULL;
        ::SetEvent(startEvent);
        ::CloseHandle(threadHandle);
        threadHandle = NULL;
    }
    ::LeaveCriticalSection(&crit_sect);
    // won't delete startEvent/startedEvent/crit_sect
    // - Windows will do during process shutdown
}

DWORD WINAPI __stdcall DS_StartBufferHelper::ThreadProc(void *param)
{
    while (1)
    {
        // wait for something to do
        ::WaitForSingleObject(data.startEvent, INFINITE);
        if (data.line2Start == NULL)
        {
            // (data.line2Start == NULL) is a signal to terminate thread
            break;
        }
        if (data.line2Start->isSource)
        {
            data.startResult = data.line2Start->playBuffer->Play(0, 0, DSCBSTART_LOOPING);
        }
        else
        {
            data.startResult = data.line2Start->captureBuffer->Start(DSCBSTART_LOOPING);
        }
        ::SetEvent(data.startedEvent);
    }
    return 0;
}

HRESULT DS_StartBufferHelper::StartBuffer(DS_Info* info) {
    HRESULT hr;
    ::EnterCriticalSection(&data.crit_sect);
    if (!isInitialized()) {
        ::LeaveCriticalSection(&data.crit_sect);
        return E_FAIL;
    }
    data.line2Start = info;
    ::SetEvent(data.startEvent);
    ::WaitForSingleObject(data.startedEvent, INFINITE);
    hr = data.startResult;
    ::LeaveCriticalSection(&data.crit_sect);
    return hr;
}


/* helper routines for DS buffer positions */
/* returns distance from pos1 to pos2
*/
inline int DS_getDistance(DS_Info* info, int pos1, int pos2) {
    int distance = pos2 - pos1;
    while (distance < 0)
        distance += info->dsBufferSizeInBytes;
    return distance;
}

/* adds 2 positions
*/
inline int DS_addPos(DS_Info* info, int pos1, int pos2) {
    int result = pos1 + pos2;
    while (result >= info->dsBufferSizeInBytes)
        result -= info->dsBufferSizeInBytes;
    return result;
}

bool DS_addDeviceRef(signed int deviceID)
{
    HWND ownerWindow;
    HRESULT res = DS_OK;
    LPDIRECTSOUND8 devPlay;
    LPDIRECTSOUNDCAPTURE8 devCapture;
    LPGUID lpGuid = NULL;

    if (g_audioDeviceCache[deviceID].dev == NULL)
    {
        /* Create DirectSound */
        //TRACE1("Creating DirectSound object for device %d\n", deviceID);
        lpGuid = &(g_audioDeviceCache[deviceID].guid);
        if (isEqualGUID(lpGuid, NULL))
        {
            lpGuid = NULL;
        }
        if (g_audioDeviceCache[deviceID].isSource)
        {
#if 0
            //CoInitialize(NULL);
            res = DirectSoundCreate8(lpGuid, &devPlay, NULL);		//生成DirectSound接口对象
#else
            res = CoInitializeEx(NULL, 0);
            res = CoCreateInstance(CLSID_DirectSound8,
                NULL,
                CLSCTX_INPROC_SERVER,
                IID_IDirectSound8,
                (LPVOID*)&devPlay);
            devPlay->Initialize(NULL);
#endif

            g_audioDeviceCache[deviceID].dev = (void*) devPlay;
        }
        else
        {
            res = DirectSoundCaptureCreate8(lpGuid, &devCapture, NULL);
            g_audioDeviceCache[deviceID].dev = (void*) devCapture;
        }
        g_audioDeviceCache[deviceID].refCount = 0;
        if (FAILED(res))
        {
            ERROR1("DS_addDeviceRef: ERROR: Failed to create DirectSound: %s", TranslateDSError(res));
            g_audioDeviceCache[deviceID].dev = NULL;
            return false;
        }
        if (g_audioDeviceCache[deviceID].isSource)
        {
            ownerWindow = GetForegroundWindow();
            if (ownerWindow == NULL)
            {
                ownerWindow = GetDesktopWindow();
            }
            //TRACE0("DS_addDeviceRef: Setting cooperative level\n");
            res = devPlay->SetCooperativeLevel(ownerWindow, DSSCL_PRIORITY);	//设置应用程序对声音设备的合作级别
            if (FAILED(res))
            {
                ERROR1("DS_addDeviceRef: ERROR: Failed to set cooperative level: %s", TranslateDSError(res));
                return false;
            }
        }
    }
    g_audioDeviceCache[deviceID].refCount++;
    return true;
}

#define DEV_PLAY(devID)    ((LPDIRECTSOUND8) g_audioDeviceCache[devID].dev)
#define DEV_CAPTURE(devID) ((LPDIRECTSOUNDCAPTURE8) g_audioDeviceCache[devID].dev)

void DS_removeDeviceRef(signed int deviceID) {

    if (g_audioDeviceCache[deviceID].refCount) {
        g_audioDeviceCache[deviceID].refCount--;
    }
    if (g_audioDeviceCache[deviceID].refCount == 0) {
        if (g_audioDeviceCache[deviceID].dev != NULL) {
            if (g_audioDeviceCache[deviceID].isSource) {
                DEV_PLAY(deviceID)->Release();
            } else {
                DEV_CAPTURE(deviceID)->Release();
            }
            g_audioDeviceCache[deviceID].dev = NULL;
        }
    }
}

void createWaveFormat(WAVEFORMATEXTENSIBLE* format, int sampleRate,
                      int channels, int bits, int significantBits)
{
    GUID subtypePCM = {STATIC_KSDATAFORMAT_SUBTYPE_PCM};
    format->Format.nSamplesPerSec = (DWORD)sampleRate;
    format->Format.nChannels = (WORD) channels;
    /* do not support useless padding, like 24-bit samples stored in 32-bit containers */
    format->Format.wBitsPerSample = (WORD) ((bits + 7) & 0xFFF8);

    if (channels <= 2 && bits <= 16)
    {
        format->Format.wFormatTag = WAVE_FORMAT_PCM;
        format->Format.cbSize = 0;
    }
    else
    {
        format->Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
        format->Format.cbSize = 22;
        format->Samples.wValidBitsPerSample = bits;
        /* no way to specify speaker locations */
        format->dwChannelMask = 0xFFFFFFFF;
        format->SubFormat = subtypePCM;
    }
    format->Format.nBlockAlign = (WORD)((format->Format.wBitsPerSample * format->Format.nChannels) / 8);
    format->Format.nAvgBytesPerSec = format->Format.nSamplesPerSec * format->Format.nBlockAlign;
}

/* fill buffer with silence
*/
void DS_clearBuffer(DS_Info* info, bool fromWritePos)
{
    UBYTE* pb1=NULL, *pb2=NULL;
    DWORD  cb1=0, cb2=0;
    DWORD flags = 0;
    int start, count;
    //TRACE1("> DS_clearBuffer for device %d\n", info->deviceID);
    if (info->isSource)
    {
        if (fromWritePos)
        {
            DWORD playCursor, writeCursor;
            int end;
            if (FAILED(info->playBuffer->GetCurrentPosition(&playCursor, &writeCursor)))
            {
                ERROR0("  DS_clearBuffer: ERROR: Failed to get current position.");
                //TRACE0("< DS_clearbuffer\n");
                return;
            }
            //TRACE2("  DS_clearBuffer: DS playPos=%d  myWritePos=%d", (int) playCursor, (int) info->writePos);
            if (info->writePos >= 0)
            {
                start = info->writePos + info->silencedBytes;
            }
            else
            {
                start = writeCursor + info->silencedBytes;
                //flags |= DSBLOCK_FROMWRITECURSOR;
            }
            while (start >= info->dsBufferSizeInBytes)
            {
                start -= info->dsBufferSizeInBytes;
            }

            // fix for bug 6251460 (REGRESSION: short sounds do not play)
            // for unknown reason with hardware DS buffer playCursor sometimes
            // jumps back for little interval (mostly 2-8 bytes) (writeCursor moves forward as usual)
            // The issue happens right after start playing and for short sounds only (less then DS buffer,
            // when whole sound written into the buffer and remaining space filled by silence)
            // the case doesn't produce any audible aftifacts so just catch it to prevent filling
            // whole buffer by silence.
            if (((int)playCursor <= start && start < (int)writeCursor)
                || (writeCursor < playCursor    // buffer bound is between playCursor & writeCursor
                && (start < (int)writeCursor || (int)playCursor <= start)))
            {
                return;
            }

            count = info->dsBufferSizeInBytes - info->silencedBytes;
            // why / 4?
            //if (count > info->dsBufferSizeInBytes / 4) {
            //    count = info->dsBufferSizeInBytes / 4;
            //}
            end = start + count;
            if ((int) playCursor < start)
            {
                playCursor += (DWORD) info->dsBufferSizeInBytes;
            }
            if (start <= (int) playCursor && end > (int) playCursor)
            {
                /* at maximum, silence until play cursor */
                count = (int) playCursor - start;
#ifdef USE_TRACE
                if ((int) playCursor >= info->dsBufferSizeInBytes) playCursor -= (DWORD) info->dsBufferSizeInBytes;
                //TRACE3("\n  DS_clearBuffer: Start Writing from %d, "
                    //"would overwrite playCursor=%d, so reduce count to %d\n",
                    //start, playCursor, count);
#endif
            }
            //TRACE2("  clearing buffer from %d, count=%d. ", (int)start, (int) count);
            if (count <= 0)
            {
                //TRACE0("\n");
                //TRACE1("< DS_clearBuffer: no need to clear, silencedBytes=%d\n", info->silencedBytes);
                return;
            }
        }
        else
        {
            start = 0;
            count = info->dsBufferSizeInBytes;
            flags |= DSBLOCK_ENTIREBUFFER;
        }
        if (FAILED(info->playBuffer->Lock(start,
            count,
            (LPVOID*) &pb1, &cb1,
            (LPVOID*) &pb2, &cb2, flags)))
        {
                ERROR0("\n  DS_clearBuffer: ERROR: Failed to lock sound buffer.\n");
                //TRACE0("< DS_clearbuffer\n");
                return;
        }
    }
    else
    {
        if (FAILED(info->captureBuffer->Lock(0,
            info->dsBufferSizeInBytes,
            (LPVOID*) &pb1, &cb1,
            (LPVOID*) &pb2, &cb2, DSCBLOCK_ENTIREBUFFER)))
        {
                ERROR0("  DS_clearBuffer: ERROR: Failed to lock sound buffer.\n");
                //TRACE0("< DS_clearbuffer\n");
                return;
        }
    }
    if (pb1!=NULL)
    {
        memset(pb1, (info->bitsPerSample == 8)?128:0, cb1);
    }
    if (pb2!=NULL)
    {
        memset(pb2, (info->bitsPerSample == 8)?128:0, cb2);
    }
    if (info->isSource)
    {
        info->playBuffer->Unlock( pb1, cb1, pb2, cb2 );
        if (!fromWritePos)
        {
            /* doesn't matter where to start writing next time */
            info->writePos = -1;
            info->silencedBytes = info->dsBufferSizeInBytes;
        }
        else
        {
            info->silencedBytes += (cb1+cb2);
            if (info->silencedBytes > info->dsBufferSizeInBytes)
            {
                ERROR1("  DS_clearbuffer: ERROR: silencedBytes=%d exceeds buffer size!\n",
                    info->silencedBytes);
                info->silencedBytes = info->dsBufferSizeInBytes;
            }
        }
        //TRACE2("  silencedBytes=%d, my writePos=%d\n", (int)info->silencedBytes, (int)info->writePos);
    }
    else
    {
        info->captureBuffer->Unlock( pb1, cb1, pb2, cb2 );
    }
    //TRACE0("< DS_clearbuffer\n");
}

/* returns pointer to buffer */
void* DS_createSoundBuffer(DS_Info* info,
                           float sampleRate, /* 44100 */
                           int sampleSizeInBits, /* 16 */
                           int channels /* 2 */,
                           int bufferSizeInBytes/* 88200 0.5 sec */)
{
    DSBUFFERDESC dsbdesc;		//创建DSBUFFERDESC结构体
    DSCBUFFERDESC dscbdesc;
    HRESULT res;
    WAVEFORMATEXTENSIBLE format;
    void* buffer;

    //TRACE1("Creating secondary buffer for device %d\n", info->deviceID);
    //设置主声音缓冲区的波形音频格式
    createWaveFormat(&format, (int) sampleRate, channels, info->frameSize / channels * 8, sampleSizeInBits);

    /* 2 second secondary buffer */
    info->dsBufferSizeInBytes = 2 * ((int) sampleRate) * info->frameSize;

    if (bufferSizeInBytes > info->dsBufferSizeInBytes / 2)
    {
        bufferSizeInBytes = info->dsBufferSizeInBytes / 2;
    }

    bufferSizeInBytes = (bufferSizeInBytes / info->frameSize) * info->frameSize;
    info->bufferSizeInBytes = bufferSizeInBytes;

    if (info->isSource)
    {
        memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));								//清零
        dsbdesc.dwSize = sizeof(DSBUFFERDESC);									//结构大小
        //缓冲区性能标志常量
        dsbdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2	//获取当前位置2
            | DSBCAPS_GLOBALFOCUS						//有全局焦点的缓冲区
            | DSBCAPS_CTRLPAN							//声音可以从左声道移动到右声道
            | DSBCAPS_CTRLVOLUME						//可以改变音量大小
            | DSBCAPS_CTRLPOSITIONNOTIFY				//可以在缓冲区设置通知的位置
            | DSBCAPS_CTRLFX;							//设置增强音效特技
        dsbdesc.dwBufferBytes = info->dsBufferSizeInBytes;
        dsbdesc.lpwfxFormat = (WAVEFORMATEX*) &format;							//设置缓冲区格式
        res = DEV_PLAY(info->deviceID)->CreateSoundBuffer(&dsbdesc, (LPDIRECTSOUNDBUFFER*) &buffer, NULL);	//创建辅助缓冲区
    }
    else
    {
        memset(&dscbdesc, 0, sizeof(DSCBUFFERDESC));
        dscbdesc.dwSize = sizeof(DSCBUFFERDESC);
        dscbdesc.dwFlags = 0;
        dscbdesc.dwBufferBytes = info->dsBufferSizeInBytes;
        dscbdesc.lpwfxFormat = (WAVEFORMATEX*) &format;
        res = DEV_CAPTURE(info->deviceID)->CreateCaptureBuffer(&dscbdesc, (LPDIRECTSOUNDCAPTUREBUFFER*) &buffer, NULL);
    }

    if (FAILED(res))
    {
        ERROR1("DS_createSoundBuffer: ERROR: Failed to create sound buffer: %s", TranslateDSError(res));
        return NULL;
    }
    return buffer;
}

void DS_destroySoundBuffer(DS_Info* info)
{
    if (info->playBuffer != NULL)
    {
        info->playBuffer->Release();
        info->playBuffer = NULL;
    }
    if (info->captureBuffer != NULL)
    {
        info->captureBuffer->Release();
        info->captureBuffer = NULL;
    }
}

//该函数主要是创建DirectSound接口对象、设置协作级别以及创建辅助缓冲区
void* DAUDIO_Open(signed int mixerIndex, signed int deviceID, int isSource,
                  int encoding, float sampleRate /*44100*/, int sampleSizeInBits/*16*/,
                  int frameSize/*4*/, int channels/*2*/,
                  int isSigned, int isBigEndian, int bufferSizeInBytes)
{
    DS_Info* info;
    void* buffer;

    //TRACE0("> DAUDIO_Open\n");

    /* some sanity checks */
    if (deviceID >= g_cacheCount)
    {
        ERROR1("DAUDIO_Open: ERROR: cannot open the device with deviceID=%d!\n", deviceID);
        return NULL;
    }
    if ((g_audioDeviceCache[deviceID].isSource && !isSource) || (!g_audioDeviceCache[deviceID].isSource && isSource))
    {
        /* only support Playback or Capture */
        ERROR0("DAUDIO_Open: ERROR: Cache is corrupt: cannot open the device in specified isSource mode!\n");
        return NULL;
    }
    if (encoding != DAUDIO_PCM)
    {
        ERROR1("DAUDIO_Open: ERROR: cannot open the device with encoding=%d!\n", encoding);
        return NULL;
    }
    if (sampleSizeInBits > 8 &&
#ifdef _LITTLE_ENDIAN
        isBigEndian
#else
        !isBigEndian
#endif
        )
    {
            ERROR1("DAUDIO_Open: ERROR: wrong endianness: isBigEndian==%d!\n", isBigEndian);
            return NULL;
    }
    if (sampleSizeInBits == 8 && isSigned)
    {
        ERROR0("DAUDIO_Open: ERROR: wrong signed'ness: with 8 bits, data must be unsigned!\n");
        return NULL;
    }
    if (!DS_StartBufferHelper::isInitialized())		//初始化线程句柄
    {
        ERROR0("DAUDIO_Open: ERROR: StartBufferHelper initialization was failed!\n");
        return NULL;
    }

    info = (DS_Info*) malloc(sizeof(DS_Info));
    if (!info)
    {
        ERROR0("DAUDIO_Open: ERROR: Out of memory\n");
        return NULL;
    }
    memset(info, 0, sizeof(DS_Info));

    info->deviceID = deviceID;
    info->isSource = isSource;
    info->bitsPerSample = sampleSizeInBits;
    info->frameSize = frameSize;
    info->framePos = 0;
    info->started = false;
    info->underrun = false;

    if (!DS_addDeviceRef(deviceID))	//该函数主要是用来创建DirectSound对象接口以及设置协作级别
    {
        DS_removeDeviceRef(deviceID);
        free(info);
        return NULL;
    }

    //该函数主要是设置缓冲区格式以及创建辅助缓冲区
    buffer = DS_createSoundBuffer(info, sampleRate, sampleSizeInBits, channels, bufferSizeInBytes);
    if (!buffer)
    {
        DS_removeDeviceRef(deviceID);
        free(info);
        return NULL;
    }

    if (info->isSource)
    {
        info->playBuffer = (LPDIRECTSOUNDBUFFER8) buffer;
    }
    else
    {
        info->captureBuffer = (LPDIRECTSOUNDCAPTUREBUFFER8) buffer;
    }
    DS_clearBuffer(info, false /* entire buffer */);

    /* use writepos of device */
    if (info->isSource)
    {
        info->writePos = -1;
    }
    else
    {
        info->writePos = 0;
    }

    //TRACE0("< DAUDIO_Open: Opened device successfully.\n");
    return (void*) info;
}

int DAUDIO_Start(void* id, int isSource)
{
    DS_Info* info = (DS_Info*) id;
    HRESULT res = DS_OK;
    DWORD status;

    //TRACE0("> DAUDIO_Start\n");

    if (info->isSource)
    {
        res = info->playBuffer->GetStatus(&status);
        if (res == DS_OK)
        {
            if (status & DSBSTATUS_LOOPING)
            {
                ERROR0("DAUDIO_Start: ERROR: Already started!");
                return true;
            }

            /* only start buffer if already something written to it */
            if (info->writePos >= 0)
            {
                res = DS_StartBufferHelper::StartBuffer(info);
                if (res == DSERR_BUFFERLOST)
                {
                    res = info->playBuffer->Restore();
                    if (res == DS_OK)
                    {
                        DS_clearBuffer(info, false /* entire buffer */);
                        /* write() will trigger actual device start */
                    }
                }
                else
                {
                    /* make sure that we will have silence after
                    the currently valid audio data */
                    DS_clearBuffer(info, true /* from write position */);
                }
            }
        }
    }
    else
    {
        if (info->captureBuffer->GetStatus(&status) == DS_OK)
        {
            if (status & DSCBSTATUS_LOOPING)
            {
                ERROR0("DAUDIO_Start: ERROR: Already started!");
                return true;
            }
        }
        res = DS_StartBufferHelper::StartBuffer(info);
    }
    if (FAILED(res))
    {
        ERROR1("DAUDIO_Start: ERROR: Failed to start: %s", TranslateDSError(res));
        return false;
    }
    info->started = true;
    return true;
}

int DAUDIO_Stop(void* id, int isSource) {
    DS_Info* info = (DS_Info*) id;

    //TRACE0("> DAUDIO_Stop\n");

    info->started = false;
    if (info->isSource)  {
        info->playBuffer->Stop();
    } else {
        info->captureBuffer->Stop();
    }

    //TRACE0("< DAUDIO_Stop\n");
    return true;
}


void DAUDIO_Close(void* id, int isSource) {
    DS_Info* info = (DS_Info*) id;

    //TRACE0("DAUDIO_Close\n");

    if (info != NULL) {
        DS_destroySoundBuffer(info);
        DS_removeDeviceRef(info->deviceID);
        free(info);
    }
}

/* Check buffer for underrun
* This method is only meaningful for Output devices (write devices).
*/
void DS_CheckUnderrun(DS_Info* info, DWORD playCursor, DWORD writeCursor) {
    ////TRACE5("DS_CheckUnderrun: playCursor=%d, writeCursor=%d, "
    //	"info->writePos=%d  silencedBytes=%d  dsBufferSizeInBytes=%d\n",
    //	(int) playCursor, (int) writeCursor, (int) info->writePos,
    //	(int) info->silencedBytes, (int) info->dsBufferSizeInBytes);
    if (info->underrun || info->writePos < 0) return;
    int writeAhead = DS_getDistance(info, writeCursor, info->writePos);
    if (writeAhead > info->bufferSizeInBytes) {
        // this may occur after Stop(), when writeCursor decreases (real valid data size > bufferSizeInBytes)
        // But the case can occur only when we have more then info->bufferSizeInBytes valid bytes
        // (and less then (info->dsBufferSizeInBytes - info->bufferSizeInBytes) silenced bytes)
        // If we already have a lot of silencedBytes after valid data (written by
        // DAUDIO_StillDraining() or DAUDIO_Service()) then it's underrun
        if (info->silencedBytes >= info->dsBufferSizeInBytes - info->bufferSizeInBytes) {
            // underrun!
            ERROR0("DS_CheckUnderrun: ERROR: underrun detected!\n");
            info->underrun = true;
        }
    }
}

/* For source (playback) line:
*   (a) if (fromPlayCursor == false), returns number of bytes available
*     for writing: bufferSize - (info->writePos - writeCursor);
*   (b) if (fromPlayCursor == true), playCursor is used instead writeCursor
*     and returned value can be used for play position calculation (see also
*     note about bufferSize)
* For destination (capture) line:
*   (c) if (fromPlayCursor == false), returns number of bytes available
*     for reading from the buffer: readCursor - info->writePos;
*   (d) if (fromPlayCursor == true), captureCursor is used instead readCursor
*     and returned value can be used for capture position calculation (see
*     note about bufferSize)
* bufferSize parameter are filled by "actual" buffer size:
*   if (fromPlayCursor == false), bufferSize = info->bufferSizeInBytes
*   otherwise it increase by number of bytes currently processed by DirectSound
*     (writeCursor - playCursor) or (captureCursor - readCursor)
*/
int DS_GetAvailable(DS_Info* info, DWORD* playCursor, DWORD* writeCursor,
                    int* bufferSize, bool fromPlayCursor) {
    int available;
    int newReadPos;

    //TRACE2("DS_GetAvailable: fromPlayCursor=%d,  deviceID=%d\n", fromPlayCursor, info->deviceID);
    if (!info->playBuffer && !info->captureBuffer) {
        ERROR0("DS_GetAvailable: ERROR: buffer not yet created");
        return 0;
    }

    if (info->isSource)  {
        if (FAILED(info->playBuffer->GetCurrentPosition(playCursor, writeCursor))) {
            ERROR0("DS_GetAvailable: ERROR: Failed to get current position.\n");
            return 0;
        }
        int processing = DS_getDistance(info, (int)*playCursor, (int)*writeCursor);
        // workaround: sometimes DirectSound report writeCursor is less (for several bytes) then playCursor
        if (processing > info->dsBufferSizeInBytes / 2) {
            *writeCursor = *playCursor;
            processing = 0;
        }
        //TRACE3("   playCursor=%d, writeCursor=%d, info->writePos=%d\n",
            //*playCursor, *writeCursor, info->writePos);
        *bufferSize = info->bufferSizeInBytes;
        if (fromPlayCursor) {
            *bufferSize += processing;
        }
        DS_CheckUnderrun(info, *playCursor, *writeCursor);
        if (info->writePos == -1 || (info->underrun && !fromPlayCursor)) {
            /* always full buffer if at beginning */
            available = *bufferSize;
        } else {
            int currWriteAhead = DS_getDistance(info, fromPlayCursor ? (int)*playCursor : (int)*writeCursor, info->writePos);
            if (currWriteAhead > *bufferSize) {
                if (info->underrun) {
                    // playCursor surpassed writePos - no valid data, whole buffer available
                    available = *bufferSize;
                } else {
                    // the case may occur after stop(), when writeCursor jumps back to playCursor
                    // so "actual" buffer size has grown
                    *bufferSize = currWriteAhead;
                    available = 0;
                }
            } else {
                available = *bufferSize - currWriteAhead;
            }
        }
    } else {
        if (FAILED(info->captureBuffer->GetCurrentPosition(playCursor, writeCursor))) {
            ERROR0("DS_GetAvailable: ERROR: Failed to get current position.\n");
            return 0;
        }
        *bufferSize = info->bufferSizeInBytes;
        if (fromPlayCursor) {
            *bufferSize += DS_getDistance(info, (int)*playCursor, (int)*writeCursor);
        }
        //TRACE4("   captureCursor=%d, readCursor=%d, info->readPos=%d  refBufferSize=%d\n",
            //*playCursor, *writeCursor, info->writePos, *bufferSize);
        if (info->writePos == -1) {
            /* always empty buffer if at beginning */
            info->writePos = (int) (*writeCursor);
        }
        if (fromPlayCursor) {
            available = ((int) (*playCursor) - info->writePos);
        } else {
            available = ((int) (*writeCursor) - info->writePos);
        }
        if (available < 0) {
            available += info->dsBufferSizeInBytes;
        }
        if (!fromPlayCursor && available > info->bufferSizeInBytes) {
            /* overflow */
            ERROR2("DS_GetAvailable: ERROR: overflow detected: "
                "DirectSoundBufferSize=%d, bufferSize=%d, ",
                info->dsBufferSizeInBytes, info->bufferSizeInBytes);
            ERROR3("captureCursor=%d, readCursor=%d, info->readPos=%d\n",
                *playCursor, *writeCursor, info->writePos);
            /* advance read position, to allow exactly one buffer worth of data */
            newReadPos = (int) (*writeCursor) - info->bufferSizeInBytes;
            if (newReadPos < 0) {
                newReadPos += info->dsBufferSizeInBytes;
            }
            info->writePos = newReadPos;
            available = info->bufferSizeInBytes;
        }
    }
    available = (available / info->frameSize) * info->frameSize;

    //TRACE1("DS_available: Returning %d available bytes\n", (int) available);
    return available;
}

// returns -1 on error, otherwise bytes written
int DAUDIO_Write(void* id, char* data, int byteSize)
{
    DS_Info* info = (DS_Info*) id;
    int available;
    int thisWritePos;
    DWORD playCursor, writeCursor;
    HRESULT res;
    void* buffer1, *buffer2;
    DWORD buffer1len, buffer2len;
    bool needRestart = false;
    int bufferLostTrials = 2;
    int bufferSize;

    //TRACE1("> DAUDIO_Write %d bytes\n", byteSize);

    while (--bufferLostTrials > 0)
    {
        available = DS_GetAvailable(info, &playCursor, &writeCursor, &bufferSize, false /* fromPlayCursor */);
        if (byteSize > available) byteSize = available;
        if (byteSize == 0) break;
        thisWritePos = info->writePos;
        if (thisWritePos == -1 || info->underrun)
        {
            // play from current write cursor after flush, etc.
            needRestart = true;
            thisWritePos = writeCursor;
            info->underrun = false;
        }

        //TRACE2("DAUDIO_Write: writing from %d, count=%d\n", (int) thisWritePos, (int) byteSize);
        res = info->playBuffer->Lock(thisWritePos, byteSize,
            (LPVOID *) &buffer1, &buffer1len,
            (LPVOID *) &buffer2, &buffer2len,
            0);
        if (res != DS_OK)
        {
            /* some DS failure */
            if (res == DSERR_BUFFERLOST)
            {
                ERROR0("DAUDIO_write: ERROR: Restoring lost Buffer.");
                if (info->playBuffer->Restore() == DS_OK)
                {
                    DS_clearBuffer(info, false /* entire buffer */);
                    info->writePos = -1;
                    /* try again */
                    continue;
                }
            }
            /* can't recover from error */
            byteSize = 0;
            break;
        }
        /* buffer could be locked successfully */
        /* first fill first buffer */
        if (buffer1)
        {
            memcpy(buffer1, data, buffer1len);
            data = (char*) (((UINT_PTR) data) + buffer1len);
        }
        else
        {
            buffer1len = 0;
        }
        if (buffer2)
        {
            memcpy(buffer2, data, buffer2len);
        }
        else
        {
            buffer2len = 0;
        }
        byteSize = buffer1len + buffer2len;

        /* update next write pos */
        thisWritePos += byteSize;
        while (thisWritePos >= info->dsBufferSizeInBytes)
        {
            thisWritePos -= info->dsBufferSizeInBytes;
        }
        /* commit data to directsound */
        info->playBuffer->Unlock(buffer1, buffer1len, buffer2, buffer2len);

        info->writePos = thisWritePos;

        /* update position
        * must be AFTER updating writePos,
        * so that getSvailable doesn't return too little,
        * so that getFramePos doesn't jump
        */
        info->framePos += (byteSize / info->frameSize);

        /* decrease silenced bytes */
        if (info->silencedBytes > byteSize)
        {
            info->silencedBytes -= byteSize;
        }
        else
        {
            info->silencedBytes = 0;
        }
        break;
    } /* while */

    /* start the device, if necessary */
    if (info->started && needRestart && (info->writePos >= 0))
    {
        DS_StartBufferHelper::StartBuffer(info);
    }

    //TRACE1("< DAUDIO_Write: returning %d bytes.\n", byteSize);
    return byteSize;
}

// returns -1 on error
int DAUDIO_Read(void* id, char* data, int byteSize) {
    DS_Info* info = (DS_Info*) id;
    int available;
    int thisReadPos;
    DWORD captureCursor, readCursor;
    HRESULT res;
    void* buffer1, *buffer2;
    DWORD buffer1len, buffer2len;
    int bufferSize;

    //TRACE1("> DAUDIO_Read %d bytes\n", byteSize);

    available = DS_GetAvailable(info, &captureCursor, &readCursor, &bufferSize, false /* fromCaptureCursor? */);
    if (byteSize > available) byteSize = available;
    if (byteSize > 0) {
        thisReadPos = info->writePos;
        if (thisReadPos == -1) {
            /* from beginning */
            thisReadPos = 0;
        }
        res = info->captureBuffer->Lock(thisReadPos, byteSize,
            (LPVOID *) &buffer1, &buffer1len,
            (LPVOID *) &buffer2, &buffer2len,
            0);
        if (res != DS_OK) {
            /* can't recover from error */
            byteSize = 0;
        } else {
            /* buffer could be locked successfully */
            /* first fill first buffer */
            if (buffer1) {
                memcpy(data, buffer1, buffer1len);
                data = (char*) (((UINT_PTR) data) + buffer1len);
            } else buffer1len = 0;
            if (buffer2) {
                memcpy(data, buffer2, buffer2len);
            } else buffer2len = 0;
            byteSize = buffer1len + buffer2len;

            /* update next read pos */
            thisReadPos = DS_addPos(info, thisReadPos, byteSize);
            /* commit data to directsound */
            info->captureBuffer->Unlock(buffer1, buffer1len, buffer2, buffer2len);

            /* update position
            * must be BEFORE updating readPos,
            * so that getAvailable doesn't return too much,
            * so that getFramePos doesn't jump
            */
            info->framePos += (byteSize / info->frameSize);

            info->writePos = thisReadPos;
        }
    }

    //TRACE1("< DAUDIO_Read: returning %d bytes.\n", byteSize);
    return byteSize;
}


int DAUDIO_GetBufferSize(void* id, int isSource) {
    DS_Info* info = (DS_Info*) id;
    return info->bufferSizeInBytes;
}

int DAUDIO_StillDraining(void* id, int isSource) {
    DS_Info* info = (DS_Info*) id;
    bool draining = false;
    int available, bufferSize;
    DWORD playCursor, writeCursor;

    DS_clearBuffer(info, true /* from write position */);
    available = DS_GetAvailable(info, &playCursor, &writeCursor, &bufferSize, true /* fromPlayCursor */);
    draining = (available < bufferSize);

    //TRACE3("DAUDIO_StillDraining: available=%d  silencedBytes=%d  Still draining: %s\n",
        //available, info->silencedBytes, draining?"true":"false");
    return draining;
}


int DAUDIO_Flush(void* id, int isSource) {
    DS_Info* info = (DS_Info*) id;

    //TRACE0("DAUDIO_Flush\n");

    if (info->isSource)  {
        info->playBuffer->Stop();
        DS_clearBuffer(info, false /* entire buffer */);
    } else {
        DWORD captureCursor, readCursor;
        /* set the read pointer to the current read position */
        if (FAILED(info->captureBuffer->GetCurrentPosition(&captureCursor, &readCursor))) {
            ERROR0("DAUDIO_Flush: ERROR: Failed to get current position.");
            return false;
        }
        DS_clearBuffer(info, false /* entire buffer */);
        /* SHOULD set to *captureCursor*,
        * but that would be detected as overflow
        * in a subsequent GetAvailable() call.
        */
        info->writePos = (int) readCursor;
    }
    return true;
}

int DAUDIO_GetAvailable(void* id, int isSource) {
    DS_Info* info = (DS_Info*) id;
    DWORD playCursor, writeCursor;
    int ret, bufferSize;

    ret = DS_GetAvailable(info, &playCursor, &writeCursor, &bufferSize, /*fromPlayCursor?*/ false);

    //TRACE1("DAUDIO_GetAvailable returns %d bytes\n", ret);
    return ret;
}

INT64 estimatePositionFromAvail(DS_Info* info, INT64 javaBytePos, int bufferSize, int availInBytes) {
    // estimate the current position with the buffer size and
    // the available bytes to read or write in the buffer.
    // not an elegant solution - bytePos will stop on xruns,
    // and in race conditions it may jump backwards
    // Advantage is that it is indeed based on the samples that go through
    // the system (rather than time-based methods)
    if (info->isSource) {
        // javaBytePos is the position that is reached when the current
        // buffer is played completely
        return (INT64) (javaBytePos - bufferSize + availInBytes);
    } else {
        // javaBytePos is the position that was when the current buffer was empty
        return (INT64) (javaBytePos + availInBytes);
    }
}

INT64 DAUDIO_GetBytePosition(void* id, int isSource, INT64 javaBytePos) {
    DS_Info* info = (DS_Info*) id;
    int available, bufferSize;
    DWORD playCursor, writeCursor;
    INT64 result = javaBytePos;

    available = DS_GetAvailable(info, &playCursor, &writeCursor, &bufferSize, /*fromPlayCursor?*/ true);
    result = estimatePositionFromAvail(info, javaBytePos, bufferSize, available);
    return result;
}


void DAUDIO_SetBytePosition(void* id, int isSource, INT64 javaBytePos) {
    /* save to ignore, since GetBytePosition
    * takes the javaBytePos param into account
    */
}

int DAUDIO_RequiresServicing(void* id, int isSource) {
    // need servicing on for SourceDataLines
    return isSource?true:false;
}

void DAUDIO_Service(void* id, int isSource) {
    DS_Info* info = (DS_Info*) id;
    if (isSource) {
        if (info->silencedBytes < info->dsBufferSizeInBytes) {
            // clear buffer
            //TRACE0("DAUDIO_Service\n");
            DS_clearBuffer(info, true /* from write position */);
        }
        if (info->writePos >= 0
            && info->started
            && !info->underrun
            && info->silencedBytes >= info->dsBufferSizeInBytes) {
                // if we're currently playing, and the entire buffer is silenced...
                // then we are underrunning!
                info->underrun = true;
                ERROR0("DAUDIO_Service: ERROR: DirectSound: underrun detected!\n");
        }
    }
}

void DAUDIO_AddAudioFormat(void* creatorV, int significantBits, int frameSizeInBytes,
                           int channels, float sampleRate,
                           int encoding, int isSigned,
                           int bigEndian)
{
   if (frameSizeInBytes <= 0) {
       if (channels > 0) {
           frameSizeInBytes = ((significantBits + 7) / 8) * channels;
       } else {
           frameSizeInBytes = -1;
       }
   }

   //TRACE4("AddAudioFormat with sigBits=%d bits, frameSize=%d bytes, channels=%d, sampleRate=%d ",
       //significantBits, frameSizeInBytes, channels, (int) sampleRate);
   //TRACE3("enc=%d, signed=%d, bigEndian=%d\n\n", encoding, isSigned, bigEndian);
}

//#endif
