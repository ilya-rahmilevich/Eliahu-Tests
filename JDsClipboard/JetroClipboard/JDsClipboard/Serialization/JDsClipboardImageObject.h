#pragma once
#include "JDsClipboardObject.h"

typedef struct PIXINFO {
	short piType;
	LPCTSTR piDescription;
	SIZE piSize;
	DWORD piAttributes;
	BYTE piHeader[256];
} FAR *LPPIXINFO;

class CJDsClipboardImageObject : public CJDsClipboardObject
{
public:
	CJDsClipboardImageObject(void);
	CJDsClipboardImageObject(UINT type);
	virtual ~CJDsClipboardImageObject(void);

	void Serialize(CXArchive& ar, BOOL fSaveMemCopy);
	IPicture* GetImage() { return m_pPix; }

	BOOL CreateEmpty(LPPICTDESC lpPictDesc = NULL, BOOL bOwn = FALSE);
	BOOL CreateFromMetafile(HMETAFILE hmf, int xExt, int yExt, BOOL bTransferOwnership);
	BOOL CreateFromEnhMetafile(HENHMETAFILE hemf, BOOL bTransferOwnership);
	BOOL CreateFromIcon(HICON hicon, BOOL bTransferOwnership);
	BOOL CreateFromIcon(UINT idResource);
	BOOL CreateFromBitmap(HBITMAP hbm, HPALETTE hpal, BOOL bTransferOwnership);
	BOOL CreateFromBitmap(CBitmap* pBitmap, CPalette* pPal, BOOL bTransferOwnership);
	BOOL CreateFromBitmap(UINT idResource);
	inline void Clear()
	{
		SafeRelease((LPUNKNOWN*)&m_pPix);
	}

	DWORD SafeRelease(LPUNKNOWN* lplpUnknown)
	{
		ATLASSERT(lplpUnknown != NULL);
		if (*lplpUnknown != NULL)
		{
			DWORD dwRef = (*lplpUnknown)->Release();
			*lplpUnknown = NULL;
			return dwRef;
		}
		return 0;
	}


	LPPIXINFO GetPixInformation()
	{
		if (m_pPix == NULL) return NULL;
		m_pPix->get_Type(&m_info.piType);
		m_info.piDescription = GetDescription();
		GetSizeInPixels(m_info.piSize); 
		m_pPix->get_Attributes(&m_info.piAttributes);
		return &m_info;
	}

	LPCTSTR GetDescription() const
	{
		LPCTSTR strType = NULL;
		short sPixType = GetType();

		switch(sPixType) {
		case PICTYPE_UNINITIALIZED: strType = _T("Uninitialized"); break;
		case PICTYPE_NONE: strType = _T("None"); break;
		case PICTYPE_BITMAP: strType = _T("Windows 3.x Bitmap (BMP)"); break;
		case PICTYPE_METAFILE: strType = _T("Windows Metafile (WMF)"); break;
		case PICTYPE_ICON: strType = _T("Windows Icon (ICO)"); break;
		case PICTYPE_ENHMETAFILE: strType = _T("Enhanced Metafile (EMF)"); break;
		default: strType = _T("Unknown"); break; }

		return strType;
	}

	void GetSizeInPixels(SIZE& size) const
	{
		if (m_pPix == NULL) return;

		SIZE hmSize = { 0, 0 };
		GetSizeInHiMetric(hmSize);
		AtlHiMetricToPixel(&hmSize, &size);
	}

	short GetType() const
	{
		short sPixType = (short)PICTYPE_UNINITIALIZED;
		if (m_pPix != NULL) m_pPix->get_Type(&sPixType);
		return sPixType;
	}

	void GetSizeInHiMetric(SIZE& hmSize) const
	{
		if (m_pPix == NULL) return;

		m_pPix->get_Width(&hmSize.cx);
		m_pPix->get_Height(&hmSize.cy);
	}

// load from disk using traditional file handle
	BOOL LoadFromFile(LPCTSTR szFilePath)
	{
		if (szFilePath == NULL) return FALSE;
		Clear();
		BOOL bResult = FALSE;
		HANDLE hFile = ::CreateFile(szFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
	        DWORD dwBytesRead = 0;
			IStream* pStream = NULL;

			// get file size
			DWORD dwFileSize = ::GetFileSize(hFile, NULL);

		    // allocate buffer memory based on file size
			m_pBuffer = ::CoTaskMemAlloc(dwFileSize);
			if (m_pBuffer != NULL)
			{
				// read file and store its data in buffer
				BOOL bRead = ::ReadFile(hFile, m_pBuffer, dwFileSize, &dwBytesRead, NULL);

				// load IPicture from buffer using IStream
		        if (bRead && dwBytesRead > 0)
				{
					::CreateStreamOnHGlobal(m_pBuffer, FALSE, &pStream);
					bResult = LoadFromIStream(pStream);

					memset(m_info.piHeader, 0, sizeof(m_info.piHeader));
					if (bResult)
						memcpy(m_info.piHeader, m_pBuffer, sizeof(m_info.piHeader));
				}
			}

			::CloseHandle(hFile);
			if (pStream != NULL) pStream->Release();
			::CoTaskMemFree(m_pBuffer);
		}

		return bResult;
	}

	BOOL LoadFromIStream(IStream* pStream)
	{
		if (pStream == NULL) return FALSE;
		Clear();
		return SUCCEEDED(::OleLoadPicture(pStream, 0, FALSE, IID_IPicture, (LPVOID*)&m_pPix));
	}

	// fSaveMemCopy - whether or not to save a copy of the picture in memory.
	// Set true for images loaded from clipboard or resource, false for file
	DWORD SaveToMemory(BOOL fSaveMemCopy = FALSE)
	{
		if (m_pPix == NULL) return 0;

		IStream* pIStream = NULL;
		LARGE_INTEGER liOffset = { 0, 0 };
		long lBytesWritten = 0;
		ULONG ulBytesRead = 0;

		// create the IStream and place the picture bytes in it
		::CreateStreamOnHGlobal(NULL, FALSE, &pIStream);
		if (m_pPix->SaveAsFile(pIStream, fSaveMemCopy, &lBytesWritten) == S_OK)
		{
			// reset stream seek position to 0
			if (pIStream->Seek(liOffset, STREAM_SEEK_SET, NULL) == S_OK)
			{
				// initialize the buffer and copy the stream to it
				m_pBuffer = ::CoTaskMemAlloc(lBytesWritten);
				pIStream->Read(m_pBuffer, lBytesWritten, &ulBytesRead);
			}
			if (m_pBuffer != NULL) memcpy(m_info.piHeader, m_pBuffer, sizeof(m_info.piHeader));
		}
		if (pIStream != NULL) pIStream->Release();

		return (DWORD)lBytesWritten;
	}

	OLE_HANDLE GetHandle() const
	{
		OLE_HANDLE handle = NULL;
		GetHandle(handle);
		return handle;
	}

	void GetHandle(OLE_HANDLE& handle) const
	{
		if (m_pPix != NULL) m_pPix->get_Handle(&handle);
	}

private:
    IPicture* m_pPix;
	void* m_pBuffer;
	PIXINFO m_info;
};

