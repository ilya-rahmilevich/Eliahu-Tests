#include "stdafx.h"
#include "JDsClipPictureHelper.h"
#include "JDsClipboardException.h"
#include "AutoResource.h"

/*
	microsoft doc. - "Twip" is a unit of measurement equal to 1/20th of a printers point. 
	There are 1440 twips to and inch
*/
#define HIMETRIC_PER_INCH 2540
/*
LPMETAFILEPICT pMF = (LPMETAFILEPICT)::GetClipboardData(CF_METAFILEPICT);
The Picture Object returns HIMETRIC values, which are
metric (1/100 mm) and represent physical size. Since a
centimeter is 2.54 inches, an inch is 2540 HIMETRIC units.
*/
#define TWIPS_PER_INCH	  1440

CJDsClipPictureHelper::CJDsClipPictureHelper()
{
}

CJDsClipPictureHelper::~CJDsClipPictureHelper()
{
	Clear();
}

CJDsClipPictureHelper& CJDsClipPictureHelper::operator=(IPicture* pPix)
{
	_pPicture = pPix;
	return *this;
}

// useful for OLE DB image blobs
void CJDsClipPictureHelper::LoadFromISequentialStream(ISequentialStream* pISeqStream, ULONG& ulLength)
{
	if (pISeqStream == NULL || ulLength == 0)
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperNullParam);

	BOOL bResult = FALSE;
	ULONG ulBytesRead = 0;

	// copy bytes to buffer from input sequential stream
	_pBuffer.Reallocate(ulLength);
	if ( !SUCCEEDED(pISeqStream->Read(_pBuffer.m_pData, ulLength, &ulBytesRead)) )
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperReadFromSeqStreamFailed);

	// create an IStream on buffer. Needed for IPicture
	// since ISequentialStream will not work directly
	LoadFromMemory();
}

void CJDsClipPictureHelper::CreateEmpty(LPPICTDESC lpPictDesc, BOOL bOwn)
{
	Clear();

	// build the PICTDESC structure if not passed in
	if (lpPictDesc == NULL)
	{
		PICTDESC pdesc;
		pdesc.cbSizeofstruct = sizeof(pdesc);
		pdesc.picType = PICTYPE_NONE;
		lpPictDesc = &pdesc;
	}

	// create the IPicture object
	BOOL bResult = SUCCEEDED(::OleCreatePictureIndirect(lpPictDesc, IID_IPicture, bOwn, (LPVOID*)&_pPicture));
	if ( bResult == FALSE )
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperCreatingPictureIndirectFailed);

	// clear the pix information structure
	ClearPixInfo();
}

// simple method of loading from a file or URL but
// rumored to have resource leaks
void CJDsClipPictureHelper::LoadFromPath(const CString &sFilePath)
{
	if ( sFilePath.IsEmpty() ) 
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperEmptyPath);

	Clear();
	if ( !SUCCEEDED(::OleLoadPicturePath(CComBSTR(sFilePath), NULL, 0, 0, IID_IPicture, (LPVOID *)&_pPicture)) )
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperLoadPictureFromPathFailed);

	SetPixInfo(sFilePath);
}

///////////////////////////
// save IPicture methods //
///////////////////////////
// save to disk. OleSavePictureFile may have problems; Use with caution!
void CJDsClipPictureHelper::SaveToDispatch(const CString &sFilePath)
{
	if (sFilePath.IsEmpty() || _pPicture == NULL) 
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperEmptyPath);


	if ( !SUCCEEDED(::OleSavePictureFile((LPDISPATCH)GetPictureDispatch(), CComBSTR(sFilePath))) )
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperSavePictureFileFailed);

	SetPixInfo(sFilePath);
}

// save to diskfile using traditional file handle
void CJDsClipPictureHelper::SaveToFile(const CString &sFilePath, BOOL fSaveMemCopy)
{
	if (sFilePath.IsEmpty() ) 
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperEmptyPath);


	BOOL bResult = FALSE;
	DWORD dwBytesWritten = 0;

	// copy the picture out to the memory buffer and then to the file
	DWORD dwBytesToWrite = SaveToMemory(fSaveMemCopy);
	if (dwBytesToWrite > 0)
	{
		UniqueHandle_File hFile(::CreateFile(sFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL));

		if( hFile.get() != INVALID_HANDLE_VALUE)
		{
			if ( ::WriteFile(hFile.get(), _pBuffer, dwBytesToWrite, &dwBytesWritten, FALSE) == FALSE )
			{
				throw CJDsClipboardException (CJDsClipboardException::enPicHelperWritingFileFailed);
			}
		}

	}
	SetPixInfo(sFilePath);
}


void CJDsClipPictureHelper::LoadFromMemory()
{
	if (_pBuffer == NULL) 
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperEmptyBuffer);

	BOOL bResult = FALSE;
	CComPtr<IStream> pIStream;

	if ( _pPicture != NULL )
		Clear();

	::CreateStreamOnHGlobal(_pBuffer, FALSE, &pIStream);
	if ( !SUCCEEDED(::OleLoadPicture(pIStream, 0, FALSE, IID_IPicture, (LPVOID*)&_pPicture)) )
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperLoadingFailed);

	memcpy(_info.piHeader, _pBuffer, sizeof(_info.piHeader));
}

void CJDsClipPictureHelper::LoadFromMemoryBuffer(LPVOID pBuffer)
{

	if (pBuffer == NULL)
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperEmptyBuffer);

	CComPtr<IStream> pIStream;

	if ( _pPicture )
		Clear();

	if ( !SUCCEEDED(::CreateStreamOnHGlobal(pBuffer, FALSE, &pIStream) ) )
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperOleStreamCreationFailed);

	if ( !SUCCEEDED(::OleLoadPicture(pIStream, 0, FALSE, IID_IPicture, (LPVOID*)&_pPicture)) )
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperLoadingFailed);

	memcpy(_info.piHeader, pBuffer, sizeof(_info.piHeader));
}


// fSaveMemCopy - whether or not to save a copy of the picture in memory.
// Set true for images loaded from clipboard or resource, false for file
DWORD CJDsClipPictureHelper::SaveToMemory(BOOL fSaveMemCopy)
{
	if (_pPicture == NULL) 
		return 0;

	//ATLASSERT( _pPicture != NULL );

	CComPtr<IStream> pIStream;

	LARGE_INTEGER liOffset = { 0, 0 };
	long lBytesWritten = 0;
	ULONG ulBytesRead = 0;

	// create the IStream and place the picture bytes in it
	::CreateStreamOnHGlobal(NULL, FALSE, &pIStream);
	if (_pPicture->SaveAsFile(pIStream, fSaveMemCopy, &lBytesWritten) == S_OK)
	{
		// reset stream seek position to 0
		if (pIStream->Seek(liOffset, STREAM_SEEK_SET, NULL) == S_OK)
		{
			// initialize the buffer and copy the stream to it
			_pBuffer.Reallocate(lBytesWritten);
			pIStream->Read(_pBuffer.m_pData, lBytesWritten, &ulBytesRead);
		}

		if (_pBuffer != NULL) 
			memcpy(_info.piHeader, _pBuffer, sizeof(_info.piHeader));
	}

	return (DWORD)lBytesWritten;
}


// load from disk using traditional file handle
void CJDsClipPictureHelper::LoadFromFile(const CString &sFilePath)
{
	if (sFilePath.IsEmpty()) 
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperEmptyPath);

	Clear();
	BOOL bResult = FALSE;
		
	UniqueHandle_File hFile(::CreateFile(sFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL));
	if (hFile.get() != INVALID_HANDLE_VALUE)
	{
	    DWORD dwBytesRead = 0;
		CComPtr<IStream> pStream;

		// get file size
		DWORD dwFileSize = ::GetFileSize(hFile.get(), NULL);

		// allocate buffer memory based on file size
		_pBuffer.Reallocate(dwFileSize);
		if (_pBuffer != NULL)
		{
			// read file and store its data in buffer
			BOOL bRead = ::ReadFile(hFile.get(), _pBuffer.m_pData, dwFileSize, &dwBytesRead, NULL);

			// load IPicture from buffer using IStream
		    if (bRead && dwBytesRead > 0)
			{
				if ( ::CreateStreamOnHGlobal(_pBuffer, FALSE, &pStream) != S_OK )
				{
					throw CJDsClipboardException (CJDsClipboardException::enPicHelperNullParam);
				}

				LoadFromIStream(pStream);
				memcpy(_info.piHeader, _pBuffer, sizeof(_info.piHeader));
			}
		}
	}
	SetPixInfo(sFilePath);
}

void CJDsClipPictureHelper::LoadFromIStream(IStream* pStream)
{
	ATLASSERT( pStream != NULL);

	if (pStream == NULL)
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperNullParameter);

	Clear();
	if ( !SUCCEEDED(::OleLoadPicture(pStream, 0, FALSE, IID_IPicture, (LPVOID*)&_pPicture)) )
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperLoadingFailed);
}
	
// load from disk using dispatch
void CJDsClipPictureHelper::LoadFromDispatch(const CString &sFilePath)
{
	if (sFilePath.IsEmpty() ) 
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperEmptyPath);

	Clear();
	BOOL bResult = FALSE;
	LPDISPATCH lpDisp = NULL;
	VARIANT varFileName;
	varFileName.vt = VT_BSTR;
	varFileName.bstrVal = CComBSTR(sFilePath);
	if ( !SUCCEEDED(::OleLoadPictureFile(varFileName, &lpDisp)) )
		throw CJDsClipboardException (CJDsClipboardException::enPicHelperLoadPictureFromDispatchFailed);

	SetPictureDispatch((LPPICTUREDISP)lpDisp);

	SetPixInfo(sFilePath);
}


// from bitmap resource
void CJDsClipPictureHelper::CreateFromBitmap(UINT idResource)
{
	CBitmap bmp;
	bmp.LoadBitmap(idResource);

	// thows exception
	CreateFromBitmap((HBITMAP)bmp.Detach(), NULL, TRUE);
}

// from CBitmap
void CJDsClipPictureHelper::CreateFromBitmap(CBitmap* pBitmap, CPalette* pPal, BOOL bTransferOwnership)
{
	HBITMAP hbm = (HBITMAP)(pBitmap->m_hBitmap);
	HPALETTE hpal = (HPALETTE)(pPal->m_hPalette);
	if (bTransferOwnership)
	{
		if (pBitmap != NULL) pBitmap->Detach();
		if (pPal != NULL) pPal->Detach();
	}

	CreateFromBitmap(hbm, hpal, bTransferOwnership);
}

void CJDsClipPictureHelper::CreateFromBitmapBuffer(LPVOID pBuffer, int iLength)
{
	_pBuffer.Reallocate(iLength);

	if ( _pBuffer )
		memcpy(_pBuffer, pBuffer, iLength);

	// frees the memory buffer also
	LoadFromMemory();
}

// from bitmap handle
void CJDsClipPictureHelper::CreateFromBitmap(HBITMAP hbm, HPALETTE hpal, BOOL bTransferOwnership)
{
	PICTDESC pdesc;
	pdesc.cbSizeofstruct = sizeof(pdesc);
	pdesc.picType = PICTYPE_BITMAP;
	pdesc.bmp.hbitmap = hbm;
	pdesc.bmp.hpal = hpal;
	// throws exception
	CreateEmpty(&pdesc, bTransferOwnership);
}

void CJDsClipPictureHelper::CreateFromEnhMetafile(HENHMETAFILE hemf, BOOL bTransferOwnership)
{
	PICTDESC pdesc;
	pdesc.cbSizeofstruct = sizeof(pdesc);
	pdesc.picType = PICTYPE_ENHMETAFILE;
	pdesc.emf.hemf = hemf;

	// throws exception
	CreateEmpty(&pdesc, bTransferOwnership);
}

void CJDsClipPictureHelper::CreateFromMetafile(HMETAFILE hmf, int xExt, int yExt, BOOL bTransferOwnership)
{
	PICTDESC pdesc;
	pdesc.cbSizeofstruct = sizeof(pdesc);
	pdesc.picType = PICTYPE_METAFILE;
	pdesc.wmf.hmeta = hmf;
	pdesc.wmf.xExt = xExt;
	pdesc.wmf.yExt = yExt;

	// throws exception
	CreateEmpty(&pdesc, bTransferOwnership);
}


////////////////////////////////
// IPicture drawing function //
////////////////////////////////
void CJDsClipPictureHelper::Render(CDCHandle dc, const CRect& rcRender)
{
	if (_pPicture != NULL)
	{
		long hmWidth;
		long hmHeight;

		_pPicture->get_Width(&hmWidth);
		_pPicture->get_Height(&hmHeight);

		_pPicture->Render(dc, rcRender.left, rcRender.top,
			rcRender.Width(), rcRender.Height(), 0, hmHeight-1,
			hmWidth, -hmHeight, NULL);
	}
}

///////////////////////////////
// IPicture property getters //
///////////////////////////////
DWORD CJDsClipPictureHelper::GetAttributes() const
{
	DWORD dwAttributes = 0;
	if (_pPicture != NULL) _pPicture->get_Attributes(&dwAttributes);
		return dwAttributes;
}

LPCTSTR CJDsClipPictureHelper::GetDescription() const
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

OLE_HANDLE CJDsClipPictureHelper::GetHandle() const
{
	OLE_HANDLE handle = NULL;
	GetHandle(handle);
	return handle;
}

void CJDsClipPictureHelper::GetHandle(OLE_HANDLE& handle) const
{
	if (_pPicture != NULL) _pPicture->get_Handle(&handle);
}

void CJDsClipPictureHelper::GetPixInformation(PIXINFO &pixINfo)
{
	if ( _pPicture == NULL ) 
		return;

	_pPicture->get_Type(&_info.piType);
	_info.piDescription = GetDescription();
	GetSizeInPixels(_info.piSize); 
	_pPicture->get_Attributes(&_info.piAttributes);
}

void CJDsClipPictureHelper::GetSizeInHiMetric(SIZE& hmSize) const
{
	if (_pPicture == NULL) 
		return;

	_pPicture->get_Width(&hmSize.cx);
	_pPicture->get_Height(&hmSize.cy);
}

void CJDsClipPictureHelper::GetSizeInPixels(SIZE& size) const
{
	if (_pPicture == NULL) 
		return;

	SIZE hmSize = { 0, 0 };
	GetSizeInHiMetric(hmSize);
	AtlHiMetricToPixel(&hmSize, &size);
}

short CJDsClipPictureHelper::GetType() const
{
	short sPixType = (short)PICTYPE_UNINITIALIZED;

	if (_pPicture != NULL) 
		_pPicture->get_Type(&sPixType);

	return sPixType;
}

///////////////////////////
// CWTLIPicture property setters //
///////////////////////////

void CJDsClipPictureHelper::ClearPixInfo()
{
	memset(_info.piHeader, 0, sizeof(_info.piHeader));
}

void CJDsClipPictureHelper::SetPixInfo(const CString &sFilePath)
{
	memset(_info.piHeader, 0, sizeof(_info.piHeader));

	if ( !sFilePath.IsEmpty() )
	{
		UniqueHandle_File hFile(::CreateFile(sFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL));
		if (hFile.get() != INVALID_HANDLE_VALUE)
		{
			DWORD dwBytesRead = 0;
			::ReadFile(hFile.get(), _info.piHeader, sizeof(_info.piHeader), &dwBytesRead, NULL);
		}
	}
}

////////////////////////////////
// IPicture Dispatch property //
//                            //
// Use with Active X controls //
////////////////////////////////
LPPICTUREDISP CJDsClipPictureHelper::GetPictureDispatch() const
{
	LPPICTUREDISP pDisp = NULL;
	if (_pPicture == NULL) 
		return NULL;

	if (SUCCEEDED(_pPicture->QueryInterface(IID_IPictureDisp, (LPVOID*)&pDisp)))
		ATLASSERT(pDisp != NULL);

	return pDisp;
}

void CJDsClipPictureHelper::SetPictureDispatch(LPPICTUREDISP pDisp)
{
	LPPICTURE pPix = NULL;
	if (pDisp == NULL) 
		return;

	Clear();

	if (SUCCEEDED(pDisp->QueryInterface(IID_IPicture, (LPVOID*)&pPix)))
	{
		ATLASSERT(pPix != NULL);
		_pPicture = pPix;
	}
	else 
	{
		_pPicture = NULL;
	}
}

void CJDsClipPictureHelper::Copy(HWND hWnd)
{
	if ( ::OpenClipboard(NULL) )
	{
		// to call CloseClipboard when leaving the stack
		AutoClipboard hClipboard;

		HANDLE hData = NULL;

		::EmptyClipboard();

		// make a copy of the image and send it to the clipboard
		switch(GetType()) {
		case PICTYPE_BITMAP:
			{
				HBITMAP hCopy = (HBITMAP)::CopyImage((HBITMAP)GetHandle(), IMAGE_BITMAP, 0, 0, 0);
				if (hCopy != NULL) 
					hData = ::SetClipboardData(CF_BITMAP, hCopy);

				break;
			}
		case PICTYPE_METAFILE:
			{
				HMETAFILE hCopy = ::CopyMetaFile((HMETAFILE)GetHandle(), NULL);
				SIZE size = { 0, 0 };
				GetSizeInHiMetric(size);
				CComHeapPtr<METAFILEPICT> hMem;
				hMem.Allocate();
				//HGLOBAL hMem = GlobalAlloc(GMEM_SHARE | GMEM_MOVEABLE, sizeof(METAFILEPICT));
				LPMETAFILEPICT pMF = (LPMETAFILEPICT)GlobalLock(hMem);
				pMF->hMF = hCopy;
				pMF->mm = MM_ANISOTROPIC;
				pMF->xExt = size.cx;
				pMF->yExt = size.cy;
				if (hCopy != NULL) 
					hData = ::SetClipboardData(CF_METAFILEPICT, pMF);

				break;
			}
		case PICTYPE_ENHMETAFILE:
			{
				HENHMETAFILE hCopy = ::CopyEnhMetaFile((HENHMETAFILE)GetHandle(), NULL);
				if (hCopy != NULL) 
					hData = ::SetClipboardData(CF_ENHMETAFILE, hCopy);

				break;
			}
		}


		if (hData == NULL)
			throw CJDsClipboardException (CJDsClipboardException::enPicHelperCopyFailed);
	}

}	

void CJDsClipPictureHelper::Create(UINT nValidFormat)
{
	BOOL bResult = FALSE;

	// copy the image to the IPicture object
	switch(nValidFormat) {
	case CF_BITMAP:
		{
			HBITMAP	hClip = (HBITMAP)::GetClipboardData(CF_BITMAP);
			if (::GetObjectType(hClip) == OBJ_BITMAP)
			{
				HBITMAP hBmp = (HBITMAP)::CopyImage(hClip, IMAGE_BITMAP, 0, 0, 0);
				CreateFromBitmap(hBmp, NULL, TRUE);
			}
			break;
		}
	// currently - if CF_ENHMETAFILE is set by other client, we get CF_METAFILEPICT
	case CF_METAFILEPICT:
		{
			LPMETAFILEPICT pMF = (LPMETAFILEPICT)::GetClipboardData(CF_METAFILEPICT);
			HMETAFILE hClip = ::CopyMetaFile(pMF->hMF, NULL);
			// convert HIMETRIC to twips
			int cx = MulDiv(pMF->xExt, TWIPS_PER_INCH, HIMETRIC_PER_INCH);
			int cy = MulDiv(pMF->yExt, TWIPS_PER_INCH, HIMETRIC_PER_INCH);
			if (::GetObjectType(hClip) == OBJ_METAFILE)
				CreateFromMetafile(hClip, cx, cy, TRUE);
			break;
		}
	case CF_ENHMETAFILE:
		{
			HENHMETAFILE hClip = (HENHMETAFILE)::GetClipboardData(CF_ENHMETAFILE);
			if (::GetObjectType(hClip) == OBJ_ENHMETAFILE)
				CreateFromEnhMetafile(hClip, TRUE);
			break;
		}
		default:
			break;
	} // end switch
}