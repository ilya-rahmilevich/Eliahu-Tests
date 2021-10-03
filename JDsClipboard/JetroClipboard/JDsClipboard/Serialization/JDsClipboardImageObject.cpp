#include "StdAfx.h"
#include "JDsClipboardImageObject.h"

// max size to copy 10 mb 
#define MAX_JDS_CLIPBOARD_BUFFER_SIZE 100000000

CJDsClipboardImageObject::CJDsClipboardImageObject(void)
{
	m_pBuffer   = NULL;
	m_pPix		= NULL;
}

CJDsClipboardImageObject::CJDsClipboardImageObject(UINT type)
		:CJDsClipboardObject(type)
{
	m_pBuffer = NULL;
	m_pPix		= NULL;
}

CJDsClipboardImageObject::~CJDsClipboardImageObject(void)
{
}

void CJDsClipboardImageObject::Serialize(CXArchive& ar, BOOL fSaveMemCopy)
{
	CJDsClipboardObject::Serialize(ar, fSaveMemCopy);

	if (ar.IsStoring())
	{
		BOOL bResult = FALSE;
		DWORD dwBytesWritten = 0;

		// copy the picture out to the memory buffer and then to the file
		DWORD dwBytesToWrite = SaveToMemory(fSaveMemCopy);
		if (dwBytesToWrite > 0)
		{
			try
			{
				ar.Write(m_pBuffer, dwBytesToWrite);
				::CoTaskMemFree(m_pBuffer);
				//memcpy(m_info.piHeader, m_pBuffer, sizeof(m_info.piHeader));
			}
			catch(...)
			{
			}
		}
	}
	else
	{
			DWORD dwBytesRead = 0;
			IStream* pStream = NULL;

		    // allocate buffer memory based on file size
			long iSize = ar.GetFile()->GetLength();
			m_pBuffer = ::CoTaskMemAlloc(iSize);
			if (m_pBuffer != NULL)
			{
				// read file and store its data in buffer
				dwBytesRead = ar.Read(m_pBuffer, iSize);

				BOOL bResult;
				// load IPicture from buffer using IStream
		        if ( dwBytesRead > 0)
				{
					::CreateStreamOnHGlobal(m_pBuffer, FALSE, &pStream);
					bResult = LoadFromIStream(pStream);
					if (bResult) memcpy(m_info.piHeader, m_pBuffer, sizeof(m_info.piHeader));
				}
			}

			if (pStream != NULL) pStream->Release();
			::CoTaskMemFree(m_pBuffer);
	}
}


////////////////////
// create methods //
////////////////////
BOOL CJDsClipboardImageObject::CreateEmpty(LPPICTDESC lpPictDesc, BOOL bOwn)
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
	BOOL bResult = SUCCEEDED(::OleCreatePictureIndirect(lpPictDesc, IID_IPicture, bOwn, (LPVOID*)&m_pPix));

	// clear the pix information structure
	memset(m_info.piHeader, 0, sizeof(m_info.piHeader));

	return bResult;
}

// from bitmap resource
BOOL CJDsClipboardImageObject::CreateFromBitmap(UINT idResource)
{
	CBitmap bmp;
	bmp.LoadBitmap(idResource);
	return CreateFromBitmap((HBITMAP)bmp.Detach(), NULL, TRUE);
}

// from CBitmap
BOOL CJDsClipboardImageObject::CreateFromBitmap(CBitmap* pBitmap, CPalette* pPal, BOOL bTransferOwnership)
{
	HBITMAP hbm = (HBITMAP)(pBitmap->m_hBitmap);
	HPALETTE hpal = (HPALETTE)(pPal->m_hPalette);
	if (bTransferOwnership)
	{
		if (pBitmap != NULL) pBitmap->Detach();
		if (pPal != NULL) pPal->Detach();
	}
	return CreateFromBitmap(hbm, hpal, bTransferOwnership);
}

// from bitmap handle
BOOL CJDsClipboardImageObject::CreateFromBitmap(HBITMAP hbm, HPALETTE hpal, BOOL bTransferOwnership)
{
	PICTDESC pdesc;
	pdesc.cbSizeofstruct = sizeof(pdesc);
	pdesc.picType = PICTYPE_BITMAP;
	pdesc.bmp.hbitmap = hbm;
	pdesc.bmp.hpal = hpal;
	return CreateEmpty(&pdesc, bTransferOwnership);
}

BOOL CJDsClipboardImageObject::CreateFromEnhMetafile(HENHMETAFILE hemf, BOOL bTransferOwnership)
{
	PICTDESC pdesc;
	pdesc.cbSizeofstruct = sizeof(pdesc);
	pdesc.picType = PICTYPE_ENHMETAFILE;
	pdesc.emf.hemf = hemf;
	return CreateEmpty(&pdesc, bTransferOwnership);
}

// works with either icon or cursor
BOOL CJDsClipboardImageObject::CreateFromIcon(HICON hicon, BOOL bTransferOwnership)
{
	PICTDESC pdesc;
	pdesc.cbSizeofstruct = sizeof(pdesc);
	pdesc.picType = PICTYPE_ICON;
	pdesc.icon.hicon = hicon;
	return CreateEmpty(&pdesc, bTransferOwnership);
}

BOOL CJDsClipboardImageObject::CreateFromMetafile(HMETAFILE hmf, int xExt, int yExt, BOOL bTransferOwnership)
{
	PICTDESC pdesc;
	pdesc.cbSizeofstruct = sizeof(pdesc);
	pdesc.picType = PICTYPE_METAFILE;
	pdesc.wmf.hmeta = hmf;
	pdesc.wmf.xExt = xExt;
	pdesc.wmf.yExt = yExt;
	return CreateEmpty(&pdesc, bTransferOwnership);
}