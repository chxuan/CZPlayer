#ifndef INCLUDE_AUDIOSTREAM
#define INCLUDE_AUDIOSTREAM

#include "CriticalSection.h"
#include "Stream.h"
#include <tchar.h>
#include <windows.h>
#include <wmsdk.h>
#include <msacm.h>

#pragma warning(disable : 4006)
#pragma warning(disable : 4995)

class CAudioStream;

typedef struct _WMA_SYNC_READER
{
    CAudioStream* pAudioStream;
    IWMSyncReader* pWMSyncReader;
    INSSBuffer* pNSSBuffer;
    IWMHeaderInfo* pWMHeaderInfo;
    DWORD dwOffset;
    IStream* pStream;
    WORD wStream;
    DWORD dwOutput;
    bool bHasAudio;
    bool bProtected;
    WORD dwChannels;
    DWORD dwSampleRate;
    WORD dwBitsPerSample;
    LARGE_INTEGER liDuration;
} WMA_SYNC_READER, *LPWMA_SYNC_READER;

typedef struct _WMAFormatSpec
{
    DWORD BitsPerSample;
    DWORD Channels;
    DWORD SampleRate;
} WMAFormatSpec, LPWMAFormatSpec;

class CAudioStream : public IStream
{
private:
    LONG m_cRef;
    CStream* m_Stream;
public:
    CAudioStream();
    ~CAudioStream(void);

    void SetStream(CStream* pStream) { m_Stream = pStream; }

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);

    HRESULT STDMETHODCALLTYPE Read(void *pv, ULONG cb, ULONG *pcbRead);
    HRESULT STDMETHODCALLTYPE Write(const void *pv, ULONG cb, ULONG *pcbWritten);

    HRESULT STDMETHODCALLTYPE Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition);
    HRESULT STDMETHODCALLTYPE SetSize(ULARGE_INTEGER libNewSize);
    HRESULT STDMETHODCALLTYPE CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten);
    HRESULT STDMETHODCALLTYPE Commit(DWORD grfCommitFlags);
    HRESULT STDMETHODCALLTYPE Revert(void);
    HRESULT STDMETHODCALLTYPE LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
    HRESULT STDMETHODCALLTYPE UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
    HRESULT STDMETHODCALLTYPE Stat(STATSTG *pstatstg, DWORD grfStatFlag);
    HRESULT STDMETHODCALLTYPE Clone(IStream **ppstm);
};

void WMA_Reader_Init(WMA_SYNC_READER* reader, CStream* pStream, bool bDescrete, SHORT iSpeakers);
DWORD WMA_Reader_GetDuration(WMA_SYNC_READER* reader);
void WMA_Reader_GetTag(WMA_SYNC_READER* reader, WCHAR* pszTagName, WCHAR* pszTagValue, size_t vsize);
WORD WMA_Reader_GetTagLength(WMA_SYNC_READER* reader, WCHAR* pszTagName);
void WMA_Reader_GetAuthor(WMA_SYNC_READER* reader, WCHAR* pszAuthor);
void WMA_Reader_Uninit(WMA_SYNC_READER* reader);
DWORD WMA_Reader_GetBitrate(WMA_SYNC_READER* reader);
DWORD WMA_Reader_GetFormatCount(WMA_SYNC_READER* reader, bool Descrete);
void WMA_Reader_GetFormat(WMA_SYNC_READER* reader, bool Descrete, int index, WMAFormatSpec* pFormatSpec);
bool WMA_Reader_GetIsVBR(WMA_SYNC_READER* reader);
void WMA_Reader_Free(WMA_SYNC_READER* reader);
void WMA_Reader_GetData(WMA_SYNC_READER* reader, void** buffer, DWORD* bytes);
void WMA_Reader_GetAudioPropties(WMA_SYNC_READER* reader, WORD* channels, WORD* bitspersamples, DWORD* sampledate);
void WMA_Reader_Seek(WMA_SYNC_READER* reader, DWORD offset);
void WMA_Reader_SetFormat(WMA_SYNC_READER* reader, bool Descrete, DWORD FormatIndex);

#endif
