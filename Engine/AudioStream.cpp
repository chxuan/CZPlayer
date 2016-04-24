#include "AudioStream.h"

#pragma warning(disable : 4100)

CAudioStream::CAudioStream()
{
    m_cRef = 0;
}

CAudioStream::~CAudioStream(void)
{
}

HRESULT STDMETHODCALLTYPE CAudioStream::QueryInterface(const IID &riid, void **ppvObject)
{
    if(IID_IStream == riid || IID_IUnknown == riid)
    {
        *ppvObject = static_cast<IStream*>(this);
        AddRef();
        return S_OK;
    }

    *ppvObject = NULL;
    return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE CAudioStream::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

ULONG STDMETHODCALLTYPE CAudioStream::Release()
{
    if(InterlockedDecrement(&m_cRef) == 0)
    {
        delete this;
        return S_OK;
    }

    return m_cRef;
}

HRESULT STDMETHODCALLTYPE CAudioStream::Read(void *pv, ULONG cb, ULONG *pcbRead)
{
    *pcbRead = m_Stream->Read(pv, cb);
    if(*pcbRead != 0) return S_OK;
    else return -1;
}

HRESULT STDMETHODCALLTYPE CAudioStream::Write(const void *pv, ULONG cb, ULONG *pcbWritten)
{
    *pcbWritten = m_Stream->Write((void*)pv, cb);
    if(*pcbWritten != 0) return S_OK;
    else return -1;
}

HRESULT STDMETHODCALLTYPE CAudioStream::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition)
{
    m_Stream->Seek(dlibMove.LowPart, (WORD)dwOrigin);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CAudioStream::SetSize(ULARGE_INTEGER libNewSize)
{
    m_Stream->Seek(libNewSize.LowPart, 0);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CAudioStream::CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CAudioStream::Commit(DWORD grfCommitFlags)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CAudioStream::Revert(void)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CAudioStream::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CAudioStream::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CAudioStream::Stat(STATSTG *pstatstg, DWORD grfStatFlag)
{
    BY_HANDLE_FILE_INFORMATION fi;
    memset(&fi, 0, sizeof(fi));
    GetFileInformationByHandle(((CFileStream*)m_Stream)->GetHandle(), &fi);

    pstatstg->cbSize.u.LowPart = fi.nFileSizeLow;
    pstatstg->cbSize.u.HighPart = fi.nFileSizeHigh;
    pstatstg->mtime = fi.ftLastWriteTime;
    pstatstg->ctime = fi.ftCreationTime;
    pstatstg->atime = fi.ftLastAccessTime;
    pstatstg->grfLocksSupported = 0;
    memcpy(&pstatstg->clsid, &IID_IStream, sizeof(CLSID));
    pstatstg->reserved = 0;

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CAudioStream::Clone(IStream **ppstm)
{
    return S_OK;
}
