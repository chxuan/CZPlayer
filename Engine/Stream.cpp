#include "Stream.h"
#include <tchar.h>
#include <mmreg.h>
#include <msacm.h>
#include <strsafe.h>

#pragma warning(disable : 4100)

/************************************************************************/
/* CStream                                                               */
/************************************************************************/
INT64 CStream::GetSize()
{
    INT64 pos(0);
    DWORD ret(0);

    pos = Seek(0, FILE_CURRENT);
    ret = Seek(0, FILE_END);
    Seek((DWORD)pos, FILE_BEGIN);

    pos = ret;
    return pos;
}

void CStream::SetSize(const INT64 size)
{
    SetSize((DWORD)size);
}

void CStream::SetSize(DWORD size)
{
    //do nothing
}

void CStream::SetPosition(INT64 position)
{
    Seek((DWORD)position, FILE_BEGIN);
}

INT64 CStream::GetPosition()
{
    return (INT64)Seek(0, FILE_CURRENT);
}

/************************************************************************/
/* CHandleStream                                                        */
/************************************************************************/
void CHandleStream::SetSize(const INT64 size)
{
    SetSize(size);
}

void CHandleStream::SetSize(int size)
{
    Seek(size, FILE_BEGIN);
}

DWORD CHandleStream::Read(void* buf, DWORD size)
{
    DWORD ret(0);
    if(!ReadFile(m_handle, buf, size, &ret, NULL))
        return 0;

    return ret;
}

DWORD CHandleStream::Write(void *buf, DWORD size)
{
    DWORD ret;
    if(!WriteFile(m_handle, buf, size, &ret, NULL))
        return 0;

    return ret;
}

DWORD CHandleStream::Seek(int offset, WORD origin)
{
    return SetFilePointer(m_handle, offset, NULL, origin);
}

/************************************************************************/
/* CCustomMemoryStream                                                  */
/************************************************************************/
CCustomMemoryStream::CCustomMemoryStream(void)
{
}

CCustomMemoryStream::~CCustomMemoryStream(void)
{
}

void CCustomMemoryStream::SetPointer(void* Ptr, DWORD Size)
{
    m_Memory = Ptr;
    m_Size = Size;
}

DWORD CCustomMemoryStream::Read(void *buf, DWORD size)
{
    DWORD ret = 0;
    if(m_Position >=0 && size >=0 )
    {
        ret = m_Size - m_Position;
        if(ret > 0)
        {
            if(ret > size)
                ret = size;


            memcpy(buf, (void*)((DWORD_PTR)m_Memory+m_Position), ret);
            m_Position += ret;
        }
    }

    return ret;
}

DWORD CCustomMemoryStream::Seek(int offset, WORD origin)
{
    switch(origin)
    {
    case FILE_BEGIN:
        m_Position = offset;
        break;
    case FILE_CURRENT:
        m_Position += offset;
        break;
    case FILE_END:
        m_Position = m_Size + offset;
        break;
    default:
        break;
    }

    return m_Position;
}

void CCustomMemoryStream::SaveToStream(CStream* pStream)
{
    if(m_Size != 0 && pStream != NULL)
        pStream->Write(m_Memory, m_Size);
}

void CCustomMemoryStream::SaveToFile(TCHAR *pszFileName)
{
    CStream* pStream = new CFileStream(pszFileName, FILE_CREATE);
    SaveToStream(pStream);
    delete pStream;
    pStream = NULL;
}

/************************************************************************/
/* CMemoryStream                                                        */
/************************************************************************/
const DWORD MemoryDelta = 0x2000;
CMemoryStream::CMemoryStream(void) : CCustomMemoryStream()
{
    m_Memory = NULL;
}

CMemoryStream::~CMemoryStream(void)
{
    Clear();
}

void CMemoryStream::Clear()
{
    SetCapacity(0);
    m_Size = 0;
    m_Position = 0;
}

void CMemoryStream::LoadFromStream(CStream *pStream)
{
    pStream->SetPosition(0);
    INT64 count = pStream->GetSize();
    SetSize((DWORD)count);

    if(count != 0)
        pStream->Read(m_Memory, (DWORD)count);
}

void CMemoryStream::LoadFromFile(TCHAR *pszFileName)
{
    CStream* pStream = new CFileStream(pszFileName, FILE_OPEN);
    LoadFromStream(pStream);
    delete pStream;
    pStream = NULL;
}

void CMemoryStream::SetCapacity(DWORD NewCapacity)
{
    SetPointer(Realloc(NewCapacity), m_Size);
    m_Capacity = NewCapacity;
}

void CMemoryStream::SetSize(DWORD NewSize)
{
    DWORD OldPosition = m_Position;
    SetCapacity(NewSize);
    m_Size = NewSize;
    if(OldPosition > NewSize)
        Seek(0, FILE_END);
}

void* CMemoryStream::Realloc(DWORD NewCapacity)
{
    if(NewCapacity > 0 && NewCapacity != m_Size)
        NewCapacity = (NewCapacity + (MemoryDelta - 1)) & (~(MemoryDelta - 1));

    if(NewCapacity != m_Capacity)
    {
        if(NewCapacity == 0)
        {
            HGLOBAL hMem = GlobalHandle(m_Memory);
            if(hMem)
            {
                if(GlobalUnlock(hMem))
                    GlobalFree(hMem);

                m_Memory = NULL;
            }
        }
        else
        {
            if(m_Capacity == 0)
            {
                HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, NewCapacity);
                if(hMem)
                    m_Memory = GlobalLock(hMem);
                else
                    m_Memory = NULL;
            }
            else
            {
                HGLOBAL hMem = GlobalHandle(m_Memory);
                if(hMem)
                {
                    if(GlobalUnlock(hMem))
                    {
                        HGLOBAL hMem1 = GlobalReAlloc(hMem, GMEM_MOVEABLE, NewCapacity);
                        m_Memory = GlobalLock(hMem1);
                    }
                }
            }
        }
    }

    return m_Memory;
}

DWORD CMemoryStream::Write(void *buf, DWORD size)
{
    DWORD pos = 0;
    DWORD ret = 0;
    if(m_Position >= 0 && size >= 0)
    {
        pos = m_Position + size;
        if(pos > 0)
        {
            if(pos > m_Size)
            {
                if(pos > m_Capacity)
                    SetCapacity(pos);

                m_Size = pos;
            }

            memcpy((void*)((DWORD_PTR)m_Memory + m_Position), buf, size);
            m_Position = pos;
            ret = size;
        }
    }

    return ret;
}

/************************************************************************/
/* CFileStream                                                          */
/************************************************************************/
CFileStream::CFileStream(TCHAR* pszFileName, FILE_MODE mode, DWORD rights) : CHandleStream()
{
    if(mode == FILE_CREATE)
    {
        m_handle = CreateFile(pszFileName, GENERIC_READ | GENERIC_WRITE, 0 /*FILE_SHARE_READ | FILE_SHARE_WRITE*/,
            NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    else
    {
        m_handle = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    }

    if(m_handle == INVALID_HANDLE_VALUE)
    {
        TCHAR msg[2*MAX_PATH];
        StringCchPrintf(msg, 2*MAX_PATH, L"Can not open file '%s'.", pszFileName);
        MessageBox(GetActiveWindow(), msg, L"Error", MB_OK);
    }
}

CFileStream::~CFileStream(void)
{
    if(m_handle != INVALID_HANDLE_VALUE)
        CloseHandle(m_handle);

    m_handle = INVALID_HANDLE_VALUE;
}

/************************************************************************/
/* CWaveConverter                                                       */
/************************************************************************/
CWaveConverter::CWaveConverter(void) : CMemoryStream()
{
    m_MaxFmtSize = 0;
    acmMetrics(NULL, ACM_METRIC_MAX_SIZE_FORMAT, &m_MaxFmtSize);
    memset(&CurrentFormat.format, 0, m_MaxFmtSize);
    memset(&CurrentFormat, 0, m_MaxFmtSize);
}

CWaveConverter::~CWaveConverter(void)
{
}

int CWaveConverter::LoadStream(CStream *pStream)
{
    RiffHeader Header = {0};
    char ID[4];
    void* Mem = NULL;
    BYTE* Data = NULL;
    DWORD NumRead = 0;
    DWORD pos = 0;
    int ret = 0;

    NumRead = pStream->Read(&Header, sizeof(RiffHeader));
    pos = NumRead;

    NumRead = pStream->Read(ID, 4);
    pos += NumRead;

    if(strcmp(Header.ID, "RIFF") != 0 && strcmp(ID, "WAVE") != 0)
        return 0;

    while(pos < (DWORD)(pStream->GetSize() -1) )
    {
        pos -= 7;
        pStream->Seek(pos, FILE_BEGIN);

        NumRead = pStream->Read(&Header, sizeof(RiffHeader));
        pos += NumRead;

        if(strcmp(Header.ID, "fmt ") == 0)
        {
            Mem = (void*)malloc(Header.BytesFollowing);
            NumRead = pStream->Read(Mem, Header.BytesFollowing);
            pos += NumRead;

            if(Header.BytesFollowing < sizeof(WAVEFORMATEX))
                memcpy(&CurrentFormat.format, Mem, sizeof(WAVEFORMATEX));
            else
                memcpy(&CurrentFormat.format, Mem, Header.BytesFollowing);

            free(Mem);
            Mem = NULL;
        }
        else if(strcmp(Header.ID, "fact") == 0)
        {
            Data = (BYTE*)malloc(Header.BytesFollowing);
            NumRead = pStream->Read(Data, Header.BytesFollowing);
            pos += NumRead;
            free(Data);
            Data = NULL;
        }
        else if(strcmp(Header.ID, "data") == 0)
        {
            if(Header.BytesFollowing > 0)
            {
                Data = (BYTE*)malloc(Header.BytesFollowing);
                NumRead = pStream->Read(Data, Header.BytesFollowing);
                pos += NumRead;

                this->SetSize(Header.BytesFollowing);
                this->Seek(0, FILE_BEGIN);
                this->Write(Data, (DWORD)this->GetSize());

                free(Data);
                Data = NULL;
            }
        }
    }

    return ret;
}

int CWaveConverter::Convert()
{
    HACMSTREAM StreamHandle = NULL;
    DWORD OutputBufferSize = 0;
    ACMSTREAMHEADER StreamHeader = {0};
    void* OutPut = NULL;
    MMRESULT Res = 0;

    // Open the stream we're going to use to convert from the current to the
    // new format
    Res = acmStreamOpen(&StreamHandle, NULL, &CurrentFormat.format, &NewFormat.format,
        NULL, 0, 0, ACM_STREAMOPENF_NONREALTIME);
    if(Res != 0)
        return Res;

    // Calculate the size of the converted data
    Res = acmStreamSize(StreamHandle, (DWORD)GetSize(), &OutputBufferSize, ACM_STREAMSIZEF_SOURCE);
    if(Res != 0)
        return Res;

    // Allocate memory for the converted data
    OutPut = malloc(OutputBufferSize);
    memset(OutPut, 0, OutputBufferSize);
    this->Seek(0, FILE_BEGIN);

    // Initialize and prepare a header
    StreamHeader.cbStruct = sizeof(ACMSTREAMHEADER);
    StreamHeader.fdwStatus = 0;
    StreamHeader.dwUser = 0;
    StreamHeader.pbSrc = (BYTE*)m_Memory;
    StreamHeader.cbSrcLength = (DWORD)this->GetSize();
    StreamHeader.cbSrcLengthUsed = 0;
    StreamHeader.dwSrcUser = 0;
    StreamHeader.pbDst = (BYTE*)OutPut;
    StreamHeader.cbDstLength = OutputBufferSize;
    StreamHeader.cbDstLengthUsed = 0;
    StreamHeader.dwDstUser = 0;

    Res = acmStreamPrepareHeader(StreamHandle, &StreamHeader, 0);
    if(Res != 0)
    {
        free(OutPut);
        OutPut = NULL;

        return Res;
    }

    // Tell acm to convert the stream
    Res = acmStreamConvert(StreamHandle, &StreamHeader, ACM_STREAMCONVERTF_BLOCKALIGN);
    if(Res != 0)
    {
        free(OutPut);
        OutPut = NULL;

        return Res;
    }

    // Set the format equal to the newformat and copy the
    // data over to the streams memory
    memcpy(CurrentFormat.RawData, NewFormat.RawData, m_MaxFmtSize);
    this->SetSize(OutputBufferSize);
    this->Seek(0, FILE_BEGIN);
    this->Write(OutPut, OutputBufferSize);

    // Unprepare the header
    Res = acmStreamUnprepareHeader(StreamHandle,&StreamHeader, 0);
    if(Res != 0)
    {
        free(OutPut);
        OutPut = NULL;

        return Res;
    }

    // Close the stream
    Res = acmStreamClose(StreamHandle, 0);
    if(Res != 0)
    {
        free(OutPut);
        OutPut = NULL;

        return Res;
    }

    free(OutPut);
    OutPut = NULL;
    return Res;
}

int CWaveConverter::SaveWavToStream(CStream *pStream)
{
    DWORD CurrentPos = 0;
    RiffHeader H = {0};
    int ret = 0;

    try
    {
        CurrentPos = m_Position;
        strcpy_s(H.ID, 4, "RIFF");
        H.BytesFollowing = 0;
        pStream->Write(&H, sizeof(RiffHeader));

        char ID[4] = {'W', 'A', 'V', 'E'};
        pStream->Write(ID, sizeof(ID));

        strcpy_s(H.ID, 4, "fmt ");
        H.BytesFollowing = sizeof(WAVEFORMAT) + 2;
        pStream->Write(&H, sizeof(RiffHeader));
        pStream->Write(&CurrentFormat, sizeof(WAVEFORMAT) + 2);

        strcpy_s(H.ID, 4, "data");
        H.BytesFollowing = (DWORD)GetSize();
        pStream->Write(&H, sizeof(RiffHeader));
        Seek(0, FILE_BEGIN);
        this->SaveWavToStream(pStream);

        pStream->Seek(0, FILE_BEGIN);
        strcpy_s(H.ID, 4, "RIFF");
        H.BytesFollowing = (DWORD)pStream->GetSize() - sizeof(H) + 1;
        pStream->Write(&H, sizeof(RiffHeader));
        m_Position = CurrentPos;
    }
    catch(...)
    {
        ret = MCIERR_FILE_NOT_SAVED;
    }

    return ret;
}

