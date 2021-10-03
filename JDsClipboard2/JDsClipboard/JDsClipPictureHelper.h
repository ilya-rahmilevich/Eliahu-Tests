#ifndef __CWTLIPICTURE_H__
#define __CWTLIPICTURE_H__

/*
	CJDsClipPictureHelper 
	- helper class for managing clipboard image formats
*/

#pragma once

#include "AutoResource.h"

#include <pshpack2.h> // does #pragma pack(push,2) removes automatic structure alignment
typedef struct _PIXINFO {
	short piType;
	LPCTSTR piDescription;
	SIZE piSize;
	DWORD piAttributes;
	BYTE piHeader[256];
} PIXINFO;
#include <poppack.h> // does #pragma pack(pop) - restores automatic structure alignment

class CJDsClipPictureHelper
{
public:
// Attributes
    CComPtr<IPicture> _pPicture;
	PIXINFO	 _info;
	CComHeapPtr<BYTE> _pBuffer;
	long	 _iBufferLength;
public:
/////////////////////////////////////
// constructor/destructor/operator //
/////////////////////////////////////
	~CJDsClipPictureHelper();
	CJDsClipPictureHelper();
	CJDsClipPictureHelper& operator=(IPicture* pPix);


////////////////////
// create methods //
////////////////////
	void Create(UINT nValidFormat);
	void CreateEmpty(LPPICTDESC lpPictDesc = NULL, BOOL bOwn = FALSE);
	void CreateFromBitmap(UINT idResource);
	void CreateFromBitmapBuffer(LPVOID pBuffer, int iLength);
	void CreateFromBitmap(HBITMAP hbm, HPALETTE hpal, BOOL bTransferOwnership);
	void CreateFromBitmap(CBitmap* pBitmap, CPalette* pPal, BOOL bTransferOwnership);
	void CreateFromEnhMetafile(HENHMETAFILE hemf, BOOL bTransferOwnership);
	void CreateFromMetafile(HMETAFILE hmf, int xExt, int yExt, BOOL bTransferOwnership);
///////////////////////////
// load IPicture methods //
///////////////////////////
	void LoadFromISequentialStream(ISequentialStream* pISeqStream, ULONG& ulLength);
	void LoadFromPath(const CString &sFilePath);
	void LoadFromMemory();
	void LoadFromMemoryBuffer(LPVOID pBuffer);
	void LoadFromIStream(IStream* pStream);
	void LoadFromFile(const CString &sFilePath);
	void LoadFromDispatch(const CString &sFilePath);

	void SaveToDispatch(const CString &sFilePath);
	void SaveToFile(const CString &sFilePath, BOOL fSaveMemCopy = FALSE);
	DWORD SaveToMemory(BOOL fSaveMemCopy = FALSE);

	// painting
	void		Render(CDCHandle dc, const CRect& rcRender);

	///////////
	// Tests //
	///////////
	inline bool IsNull() const { return (_pPicture == NULL); }
	inline bool IsBitmap() const { return GetType() == PICTYPE_BITMAP; }
	inline bool IsMetafile() const { return GetType() == PICTYPE_METAFILE; }
	inline bool IsIcon() const { return GetType() == PICTYPE_ICON; }
	inline bool IsEnhMetafile() const { return GetType() == PICTYPE_ENHMETAFILE; }


	///////////////////////////
	// Miscellaneous helpers //
	///////////////////////////
	inline void Clear()
	{
		_pPicture.Release();
	}

	OLE_HANDLE GetHandle() const;
	LPCTSTR GetDescription() const;
	void SetPictureDispatch(LPPICTUREDISP pDisp);
	LPPICTUREDISP GetPictureDispatch() const;
	void		SetPixInfo(const CString &sFilePath);
	short		GetType() const;
	void		GetSizeInPixels(SIZE& size) const;
	void		GetSizeInHiMetric(SIZE& hmSize) const;
	void		GetPixInformation(PIXINFO &pixInfo);
	void		GetHandle(OLE_HANDLE& handle) const;
	DWORD		GetAttributes() const;
	LPVOID GetBuffer() { return _pBuffer; }

	void  Copy(HWND hWnd);

	void ClearPixInfo();
};


#endif // __CWTLIPICTURE_H__
