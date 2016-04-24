#include "AudioStream.h"
#include <tchar.h>
#include <windows.h>
#include <wmsdk.h>
#include <msacm.h>

#pragma warning(disable : 4006)
#pragma warning( disable : 4100 )

void WMA_Reader_Init(WMA_SYNC_READER* reader, CStream* pStream, bool bDescrete, SHORT iSpeakers)
{
	DWORD dwOutputCount = 0, dwFormatCount = 0;
	DWORD i = 0, j = 0, dwSize = 0;
	IWMOutputMediaProps* pWMOutputMediaProps = NULL;
	WM_MEDIA_TYPE* pOutputMediaType = NULL;
	WM_MEDIA_TYPE* pFormatMediaType = NULL;
	PWAVEFORMATEX pWaveFmtEx = NULL;
	WMT_ATTR_DATATYPE dataType;
	WORD wLen = 0, wAStream = 0;
	ULARGE_INTEGER uliNewPosition;
	BYTE bEnable = false;
	DWORD dwValue = 0;
	bool bCond = false;
	HRESULT hr = S_OK;

	CoInitialize(NULL);

	CCriticalSection cs;
	cs.Enter();

	reader->pAudioStream = NULL;
	reader->pNSSBuffer = NULL;
	reader->pStream = NULL;
	reader->pWMHeaderInfo = NULL;
	reader->pWMSyncReader = NULL;

	hr = WMCreateSyncReader(NULL, 0, &reader->pWMSyncReader);
	if(hr != S_OK)
	{
		cs.Leave();
		return;
	}

	reader->pAudioStream = new CAudioStream;
	reader->pAudioStream->SetStream(pStream);
	reader->pStream = static_cast<IStream*>(reader->pAudioStream);
	reader->pStream->AddRef();

	// open stream
	hr = reader->pWMSyncReader->OpenStream(reader->pStream);
	if(hr != S_OK)
	{
		LARGE_INTEGER liMove;
		liMove.LowPart = 30;
		reader->pAudioStream->Seek(liMove, 0, &uliNewPosition);

		hr = reader->pWMSyncReader->OpenStream(reader->pStream);
		if(hr != S_OK)
		{
			reader->pWMSyncReader->Release();
			reader->pWMSyncReader = NULL;

			reader->pStream->Release();
			reader->pAudioStream = NULL;
			reader->pStream = NULL;

			cs.Leave();
			return;
		}
	}

	//get output count
	hr = reader->pWMSyncReader->GetOutputCount(&dwOutputCount);
	if(hr != S_OK)
	{
		reader->pWMSyncReader->Release();
		reader->pWMSyncReader = NULL;

		reader->pStream->Release();
		reader->pAudioStream = NULL;
		reader->pStream = NULL;

		cs.Leave();
		return;
	}

	for(i = 0; i < dwOutputCount; i++)
	{
		hr = reader->pWMSyncReader->GetOutputProps(i, &pWMOutputMediaProps);
		if(hr != S_OK) continue;

		hr = pWMOutputMediaProps->GetMediaType(NULL, &dwSize);
		if(hr != S_OK) continue;

		pOutputMediaType = (WM_MEDIA_TYPE*)malloc(dwSize);
		if(pOutputMediaType == NULL)
		{
            //MessageBox(GetActiveWindow(), _TEXT("Out of memory!"), _TEXT("Error"), MB_OK);
			continue;
		}

		hr = pWMOutputMediaProps->GetMediaType(pOutputMediaType, &dwSize);
		if(hr != S_OK)
		{
			free(pOutputMediaType);
			pOutputMediaType = NULL;

			continue;
		}

		if(IsEqualGUID(pOutputMediaType->majortype, WMMEDIATYPE_Audio))
		{
			bEnable = (BYTE)bDescrete;
            reader->pWMSyncReader->SetOutputSetting(i, (LPCWSTR)"EnableDiscreteOutput",
				WMT_TYPE_BOOL, (const BYTE*)&bEnable, 4);

			if(iSpeakers == 0)
			{
				pWaveFmtEx = (PWAVEFORMATEX)pOutputMediaType->pbFormat;
				reader->dwSampleRate = pWaveFmtEx->nSamplesPerSec;
				reader->dwChannels = pWaveFmtEx->nChannels;

				hr = reader->pWMSyncReader->GetOutputFormatCount(i, &dwFormatCount);
				if(hr == S_OK)
				{
					for(j = 0; j < dwFormatCount; j++)
					{
						hr = reader->pWMSyncReader->GetOutputFormat(i, j, &pWMOutputMediaProps);
						if(hr != S_OK) continue;

						hr = pWMOutputMediaProps->GetMediaType(NULL, &dwSize);
						if(hr != S_OK) continue;

						pFormatMediaType = (WM_MEDIA_TYPE*)malloc(dwSize);
						if(pFormatMediaType == NULL)
						{
                            //MessageBox(GetActiveWindow(), _TEXT("Out of memory!"), _TEXT("Error"), MB_OK);
							continue;
						}

						hr = pWMOutputMediaProps->GetMediaType(pFormatMediaType, &dwSize);
						if(hr != S_OK)
						{
							free(pFormatMediaType);
							pFormatMediaType = NULL;

							continue;
						}

						if(IsEqualGUID(pFormatMediaType->formattype, WMFORMAT_WaveFormatEx))
						{
							pWaveFmtEx = (PWAVEFORMATEX)pFormatMediaType->pbFormat;
							bCond = (pWaveFmtEx->wBitsPerSample >= reader->dwBitsPerSample) &&
								(pWaveFmtEx->nSamplesPerSec == reader->dwSampleRate) &&
								(pWaveFmtEx->nChannels == reader->dwChannels);

							if(bCond)
							{
								reader->bHasAudio = true;
								reader->dwOutput = i;
								reader->dwBitsPerSample = pWaveFmtEx->wBitsPerSample;
								reader->pWMSyncReader->SetOutputProps(i, pWMOutputMediaProps);
							}
						}

						free(pFormatMediaType);
						pFormatMediaType = NULL;
					}
				}
			}
			else
			{
				if(iSpeakers > 2)
				{
					dwValue = iSpeakers;
                    reader->pWMSyncReader->SetOutputSetting(i, (LPCWSTR)"SpeakerConfig",
						WMT_TYPE_DWORD, (const BYTE*)&dwValue, 4);
				}

				reader->pWMSyncReader->GetOutputFormatCount(i, &dwFormatCount);
				for(j = 0; j< dwFormatCount; j++)
				{
					reader->pWMSyncReader->GetOutputFormat(i, j, &pWMOutputMediaProps);
					pWMOutputMediaProps->GetMediaType(NULL, &dwSize);

					pFormatMediaType = (WM_MEDIA_TYPE*)malloc(dwSize);
					if(pFormatMediaType == NULL)
					{
                        //MessageBox(GetActiveWindow(), _TEXT("Out of memory!"), _TEXT("Error"), MB_OK);
						continue;
					}

					pWMOutputMediaProps->GetMediaType(pFormatMediaType, &dwSize);
					if(IsEqualGUID(pFormatMediaType->formattype, WMFORMAT_WaveFormatEx))
					{
						pWaveFmtEx = (PWAVEFORMATEX)pFormatMediaType->pbFormat;
						if(iSpeakers > 0)
							bCond = (pWaveFmtEx->nChannels == iSpeakers);
						else
						{
							bCond = (pWaveFmtEx->wBitsPerSample >= reader->dwBitsPerSample) &&
								(pWaveFmtEx->nSamplesPerSec >= reader->dwSampleRate) &&
								(pWaveFmtEx->nChannels >= reader->dwChannels);
						}

						if(bCond)
						{
							reader->bHasAudio = true;
							reader->dwOutput = i;
							reader->dwChannels = pWaveFmtEx->nChannels;
							reader->dwSampleRate = pWaveFmtEx->nSamplesPerSec;
							reader->dwBitsPerSample = pWaveFmtEx->wBitsPerSample;
							reader->pWMSyncReader->SetOutputProps(i, pWMOutputMediaProps);
						}
					}
				}
			}
		}

		free(pOutputMediaType);
		pOutputMediaType = NULL;

		pWMOutputMediaProps->Release();
		pWMOutputMediaProps = NULL;
	}

	if(reader->bHasAudio)
	{
		reader->pWMSyncReader->GetStreamNumberForOutput(reader->dwOutput, &reader->wStream);
		if(reader->pWMSyncReader->SetReadStreamSamples(reader->wStream, false) == NS_E_PROTECTED_CONTENT)
		{
			reader->bProtected = true;
		}
		else reader->bProtected = false;

		wLen = 8;
		wAStream = 0;

		hr = reader->pWMSyncReader->QueryInterface(IID_IWMHeaderInfo, (void**)&reader->pWMHeaderInfo);
		if(hr == S_OK)
		{
            hr = reader->pWMHeaderInfo->GetAttributeByName(&wAStream, (LPCWSTR)"Duration", &dataType,
				(BYTE*)&reader->liDuration, &wLen);
			if(hr != S_OK) reader->liDuration.LowPart = 0;
		}
	}

	cs.Leave();
}

DWORD WMA_Reader_GetDuration(WMA_SYNC_READER* reader)
{
	if(reader == NULL) return 0;

	return (DWORD)(reader->liDuration.LowPart/0.1e6);
}

void WMA_Reader_GetTag(WMA_SYNC_READER* reader, WCHAR* pszTagName, WCHAR* pszTagValue, size_t vsize)
{
	WORD wStream = 0, wLen = vsize;
	WMT_ATTR_DATATYPE dataType;// = WMT_TYPE_STRING;
	HRESULT hr;

	hr = reader->pWMHeaderInfo->GetAttributeByName(&wStream, pszTagName, &dataType, (BYTE*)pszTagValue, &wLen);
	if(hr != S_OK) pszTagValue[0] = '\0';
}

WORD WMA_Reader_GetTagLength(WMA_SYNC_READER* reader, WCHAR* pszTagName)
{
	if(reader == NULL) return 0;

	WORD wStream = 0, wLength = 0;
	WMT_ATTR_DATATYPE dataType;
	HRESULT hr;

	hr = reader->pWMHeaderInfo->GetAttributeByName(&wStream, pszTagName, &dataType, NULL, &wLength);
	if(hr != S_OK) wLength = 0;

	return wLength;
}

void WMA_Reader_GetAuthor(WMA_SYNC_READER* reader, WCHAR* pszAuthor)
{
	if(reader == NULL) return;

    WORD wLen = WMA_Reader_GetTagLength(reader, (WCHAR*)"Author");
	if(wLen == 0) return;


}

void WMA_Reader_Uninit(WMA_SYNC_READER* reader)
{
	if(reader == NULL) return;

	CCriticalSection cs;
	cs.Enter();

	if(reader->pNSSBuffer != NULL)
		reader->pNSSBuffer->Release();

	if(reader->pStream != NULL)
		reader->pStream->Release();

	if(reader->pWMHeaderInfo != NULL)
		reader->pWMHeaderInfo->Release();

	if(reader->pWMSyncReader != NULL)
	{
		reader->pWMSyncReader->Close();
		reader->pWMSyncReader->Release();
	}

	reader->pAudioStream = NULL;
	reader->pNSSBuffer = NULL;
	reader->pStream = NULL;
	reader->pWMHeaderInfo = NULL;
	reader->pWMSyncReader = NULL;

	CoUninitialize();
	cs.Leave();
}

DWORD WMA_Reader_GetBitrate(WMA_SYNC_READER* reader)
{
	WORD len, stream;
	WMT_ATTR_DATATYPE dataType;

	len = 4;
	stream = 0;
	DWORD ret;

    if(reader->pWMHeaderInfo->GetAttributeByName(&stream, (LPCWSTR)"Bitrate", &dataType, (BYTE*)&ret, &len) != S_OK)
		return 0;

	return ret;
}

DWORD WMA_Reader_GetFormatCount(WMA_SYNC_READER* reader, bool Descrete)
{
	BYTE Enable = (BYTE)Descrete;
	DWORD dwCount = 0;
    reader->pWMSyncReader->SetOutputSetting(reader->dwOutput, (LPCWSTR)"EnableDiscreteOutput",
		WMT_TYPE_BOOL, &Enable, 4);
	reader->pWMSyncReader->GetOutputFormatCount(reader->dwOutput, &dwCount);

	return dwCount;
}

void WMA_Reader_GetFormat(WMA_SYNC_READER* reader, bool Descrete, int index, WMAFormatSpec* pFormatSpec)
{
	IWMOutputMediaProps* pOutputMediaProps;
	WM_MEDIA_TYPE* pFormatMediaType;
	PWAVEFORMATEX pFormatEx;
	BYTE Enable = (BYTE)Descrete;
	DWORD dwSize = 0;

    reader->pWMSyncReader->SetOutputSetting(reader->dwOutput, (LPCWSTR)"EnableDiscreteOutput",
		WMT_TYPE_BOOL, &Enable, 4);
	reader->pWMSyncReader->GetOutputFormat(reader->dwOutput, index, &pOutputMediaProps);

	pOutputMediaProps->GetMediaType(NULL, &dwSize);
	pFormatMediaType = (WM_MEDIA_TYPE*)malloc(dwSize);
	pOutputMediaProps->GetMediaType(pFormatMediaType, &dwSize);

	if(IsEqualGUID(pFormatMediaType->formattype, WMFORMAT_WaveFormatEx))
	{
		pFormatEx = (PWAVEFORMATEX)pFormatMediaType->pbFormat;
		pFormatSpec->BitsPerSample = pFormatEx->wBitsPerSample;
		pFormatSpec->Channels = pFormatEx->nChannels;
		pFormatSpec->SampleRate = pFormatEx->nSamplesPerSec;
	}
	else
	{
		pFormatSpec->BitsPerSample = 0;
		pFormatSpec->Channels = 0;
		pFormatSpec->SampleRate = 0;
	}

	free(pFormatMediaType);
	pOutputMediaProps->Release();
	pOutputMediaProps = NULL;
}

bool WMA_Reader_GetIsVBR(WMA_SYNC_READER* reader)
{
	WORD len, stream;
	WMT_ATTR_DATATYPE dataType;

	len = 4;
	stream = 0;
	BYTE ret;
    if(reader->pWMHeaderInfo->GetAttributeByName(&stream, (LPCWSTR)"", &dataType, &ret, &len) != S_OK)
		return false;

	return true;
}

void WMA_Reader_Free(WMA_SYNC_READER* reader)
{
	reader->pWMSyncReader->Close();
	if(reader->pNSSBuffer != NULL)
		reader->pNSSBuffer->Release();

	if(reader->pWMHeaderInfo != NULL)
		reader->pWMHeaderInfo->Release();

	if(reader->pStream != NULL)
		reader->pStream->Release();

	reader->pWMSyncReader->Release();
	reader->pWMSyncReader = NULL;

	reader->pWMHeaderInfo = NULL;
	reader->pNSSBuffer = NULL;
	reader->pStream = NULL;
	reader->pAudioStream = NULL;
}

void WMA_Reader_GetData(WMA_SYNC_READER* reader, void** buffer, DWORD* bytes)
{
	BYTE* pBuf;
	DWORD len;
	DWORD copylen;
	ULARGE_INTEGER time, duration;
	DWORD flags;
	DWORD Output;
	WORD w;
	HRESULT hr;

	while(true)
	{
		if(reader->pNSSBuffer != NULL)
		{
			reader->pNSSBuffer->GetBufferAndLength((BYTE**)&pBuf, &len);
			if(reader->dwOffset < len)
			{
				if(*bytes < (len - reader->dwOffset))
					copylen = *bytes;
				else
					copylen = len - reader->dwOffset;

				*buffer = pBuf + reader->dwOffset;
				*bytes = copylen;
				reader->dwOffset += copylen;
				return;
			}

			reader->pNSSBuffer->Release();
			reader->pNSSBuffer = NULL;
			reader->dwOffset = 0;
		}

		hr = reader->pWMSyncReader->GetNextSample(reader->wStream, &reader->pNSSBuffer, &time.QuadPart,
			&duration.QuadPart, &flags, &Output, &w);
		if(hr != S_OK)
		{
			*buffer = NULL;
			*bytes = 0;
			return;
		}
	}
}

void WMA_Reader_GetAudioPropties(WMA_SYNC_READER* reader, WORD* channels, WORD* bitspersamples, DWORD* sampledate)
{
	*channels = (WORD)reader->dwChannels;
	*bitspersamples = (WORD)reader->dwBitsPerSample;
	*sampledate = reader->dwSampleRate;
}

void WMA_Reader_Seek(WMA_SYNC_READER* reader, DWORD offset)
{
	reader->pWMSyncReader->SetRange((QWORD)(offset*0.1E6), 0);
}

void WMA_Reader_SetFormat(WMA_SYNC_READER* reader, bool Descrete, DWORD FormatIndex)
{
	IWMOutputMediaProps* pOutputMediaProps = NULL;
	WM_MEDIA_TYPE* pFormatMediaType = NULL;
	PWAVEFORMATEX pFormatEx = NULL;
	BYTE Enable = 0;
	DWORD size = 0;

	Enable = (BYTE)Descrete;

    reader->pWMSyncReader->SetOutputSetting(reader->dwOutput, (LPCWSTR)"EnableDiscreteOutput",
		WMT_TYPE_BOOL, &Enable, 4);
	reader->pWMSyncReader->GetOutputFormat(reader->dwOutput, FormatIndex, &pOutputMediaProps);

	pOutputMediaProps->GetMediaType(NULL, &size);
	pFormatMediaType = (WM_MEDIA_TYPE*)malloc(size);
	pOutputMediaProps->GetMediaType(pFormatMediaType, &size);

	if(IsEqualGUID(pFormatMediaType->formattype, WMFORMAT_WaveFormatEx))
	{
		pFormatEx = (PWAVEFORMATEX)pFormatMediaType->pbFormat;
		reader->dwChannels = pFormatEx->nChannels;
		reader->dwBitsPerSample = pFormatEx->wBitsPerSample;
		reader->dwSampleRate = pFormatEx->nSamplesPerSec;
	}

	reader->pWMSyncReader->SetOutputProps(reader->dwOutput, pOutputMediaProps);
	free(pFormatMediaType);

	pOutputMediaProps->Release();
	pOutputMediaProps = NULL;
}

