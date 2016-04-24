#pragma once

#include <stdlib.h>
#include <windows.h>

#ifndef INCLUDE_CSTREAM
#define INCLUDE_CSTREAM

#pragma warning(disable : 4006)
#pragma warning(disable : 4995)

/************************************************************************/
/* CStream                                                              */
/************************************************************************/
class CStream
{
public:
    CStream(void) {}
    ~CStream(void) {}

public:
    virtual DWORD Read(void* buf, DWORD size) = 0;			//读取流
    virtual DWORD Write(void* buf, DWORD size) = 0;			//写入流
    virtual DWORD Seek(int offset, WORD origin) = 0;		//定位流

    INT64 GetPosition(void);								//获得位置
    void SetPosition(INT64 position);						//设置位置

    virtual INT64 GetSize(void);							//获得流大小
    virtual void SetSize(const INT64 size);					//设置流大小
    virtual void SetSize(DWORD size);						//设置流大小
};

/************************************************************************/
/* CHandleStream：处理流                                                */
/************************************************************************/
class CHandleStream : public CStream
{
public:
    CHandleStream(void) : m_handle(0) {}
    ~CHandleStream(void) {}

public:
    DWORD Read(void* buf, DWORD size);
    DWORD Write(void* buf, DWORD size);
    DWORD Seek(int offset, WORD origin);
    HANDLE GetHandle() { return m_handle; }

protected:
    HANDLE m_handle;

protected:
    void SetSize(const INT64 size);
    void SetSize(int size);
};

/************************************************************************/
/* CCustomMemoryStream：自定义内存流                                    */
/************************************************************************/
class CCustomMemoryStream : public CStream
{
public:
    CCustomMemoryStream(void);
    ~CCustomMemoryStream(void);

public:
    DWORD Read(void* buf, DWORD size);
    DWORD Seek(int offset, WORD origin);
    void SaveToStream(CStream* pStream);
    void SaveToFile(TCHAR* pszFileName);

protected:
    void* m_Memory;
    DWORD m_Size;
    DWORD m_Position;

protected:
    void SetPointer(void* Ptr, DWORD Size);
};

/************************************************************************/
/* CMemoryStream：内存流                                                */
/************************************************************************/
class CMemoryStream : public CCustomMemoryStream
{
public:
    CMemoryStream(void);
    ~CMemoryStream(void);

public:
    void Clear();
    void LoadFromStream(CStream* pStream);
    void LoadFromFile(TCHAR* pszFileName);
    void SetSize(DWORD NewSize);
    DWORD Write(void* buf, DWORD size);
    void SetCapacity(DWORD NewCapacity);

private:
    DWORD m_Capacity;

protected:
    void* Realloc(DWORD NewCapacity);
};

/************************************************************************/
/* CFileStream：文件流                                                  */
/************************************************************************/
typedef enum _FILE_MODE
{
    FILE_OPEN = 0,
    FILE_CREATE = 1
} FILE_MODE;
class CFileStream : public CHandleStream
{
public:
    CFileStream(TCHAR* pszFileName, FILE_MODE mode, DWORD rights = 0);
    ~CFileStream(void);
};

/************************************************************************/
/* CWaveConverter：波转换器                                             */
/************************************************************************/
typedef struct _RiffHeader
{
    char ID[4];
    DWORD BytesFollowing;
} RiffHeader, *LPRiffHeader;

typedef struct _ACMWaveFormat
{
    union
    {
        WAVEFORMATEX format;
        BYTE RawData[128];
    };
} ACMWaveFormat, *LPACMWaveFormat;

class CWaveConverter : public CMemoryStream
{
private:
    DWORD m_MaxFmtSize;
public:
    CWaveConverter(void);
    ~CWaveConverter(void);

    ACMWaveFormat CurrentFormat;
    ACMWaveFormat NewFormat;
    int LoadStream(CStream* pStream);
    int Convert();
    int SaveWavToStream(CStream* pStream);
};

#endif
