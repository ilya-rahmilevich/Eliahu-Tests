#pragma once
#include "Serialization\JDsClipboardStringObject.h"
#include "Serialization\JDsClipboardImageObject.h"

// Support for multiple clipboard image formats
class CJDsClipboardPicture
{
public:
	BOOL Copy(CJDsClipboardImageObject &imageobject, HWND hWnd)
	{
		BOOL bResult = false;

		if (::OpenClipboard(NULL))
		{
			HANDLE hData = NULL;

			::EmptyClipboard();

			// make a copy of the image and send it to the clipboard
			switch(imageobject.GetType()) {
			case PICTYPE_ICON:
				{
					HBITMAP hCopy = BitmapFromIconCursor((HICON)imageobject.GetHandle(), hWnd);
					if (hCopy != NULL) hData = ::SetClipboardData(CF_BITMAP, hCopy);
					break;
				}
			case PICTYPE_BITMAP:
				{
					HBITMAP hCopy = (HBITMAP)::CopyImage((HBITMAP)imageobject.GetHandle(), IMAGE_BITMAP, 0, 0, 0);
					if (hCopy != NULL) hData = ::SetClipboardData(CF_BITMAP, hCopy);
					break;
				}
			case PICTYPE_METAFILE:
				{
					HMETAFILE hCopy = ::CopyMetaFile((HMETAFILE)imageobject.GetHandle(), NULL);
					SIZE size = { 0, 0 };
					imageobject.GetSizeInHiMetric(size);
					HGLOBAL hMem = GlobalAlloc(GMEM_SHARE | GMEM_MOVEABLE, sizeof(METAFILEPICT));
					LPMETAFILEPICT pMF = (LPMETAFILEPICT)GlobalLock(hMem);
					pMF->hMF = hCopy;
					pMF->mm = MM_ANISOTROPIC;
					pMF->xExt = size.cx;
					pMF->yExt = size.cy;
					GlobalUnlock(hMem);
					if (hCopy != NULL) hData = ::SetClipboardData(CF_METAFILEPICT, pMF);
					break;
				}
			case PICTYPE_ENHMETAFILE:
				{
					HENHMETAFILE hCopy = ::CopyEnhMetaFile((HENHMETAFILE)imageobject.GetHandle(), NULL);
					if (hCopy != NULL) hData = ::SetClipboardData(CF_ENHMETAFILE, hCopy);
					break;
				}
			}

			::CloseClipboard();

			if (hData != NULL) bResult = TRUE;
		}

		return bResult;
	}

	inline BOOL IsPastable()
	{
		UINT clipFormats[] = { CF_METAFILEPICT, CF_ENHMETAFILE, CF_BITMAP, CF_DIB };
		if (::GetPriorityClipboardFormat(clipFormats, 4) == NULL)
			return FALSE;
		if (::GetPriorityClipboardFormat(clipFormats, 4) == -1)
			return FALSE;
		return TRUE;
	}

	IPicture *CreateImageObject(CJDsClipboardImageObject &imageobject, UINT nValidFormat = 0)
	{

		BOOL bResult = FALSE;
		int nNumFormats = ::CountClipboardFormats();

		if (nNumFormats > 1 && nValidFormat == 0)
		{
			// set to CF_BITMAP for either DIB or BITMAP
			if (nNumFormats == 2 && (::IsClipboardFormatAvailable(CF_BITMAP)
				&& ::IsClipboardFormatAvailable(CF_DIB)))
			{
				nValidFormat = CF_BITMAP;
			}
		}

		if (::OpenClipboard(NULL))
		{
			// copy the image to the IPicture object
			switch(nValidFormat) {
			case CF_BITMAP:
				{
					HBITMAP	hClip = (HBITMAP)::GetClipboardData(CF_BITMAP);
					::CloseClipboard();
					if (::GetObjectType(hClip) == OBJ_BITMAP)
					{
						HBITMAP hBmp = (HBITMAP)::CopyImage(hClip, IMAGE_BITMAP, 0, 0, 0);
						bResult = imageobject.CreateFromBitmap(hBmp, NULL, TRUE);
					}
					break;
				}
			case CF_METAFILEPICT:
				{
					LPMETAFILEPICT pMF = (LPMETAFILEPICT)::GetClipboardData(CF_METAFILEPICT);
					::CloseClipboard();
					HMETAFILE hClip = ::CopyMetaFile(pMF->hMF, NULL);
					// convert HIMETRIC to twips
					int cx = MulDiv(pMF->xExt, 1440, 2540);
					int cy = MulDiv(pMF->yExt, 1440, 2540);
					if (::GetObjectType(hClip) == OBJ_METAFILE)
						bResult = imageobject.CreateFromMetafile(hClip, cx, cy, TRUE);
					break;
				}
			case CF_ENHMETAFILE:
				{
					HENHMETAFILE hClip = (HENHMETAFILE)::GetClipboardData(CF_ENHMETAFILE);
					::CloseClipboard();
					if (::GetObjectType(hClip) == OBJ_ENHMETAFILE)
						bResult = imageobject.CreateFromEnhMetafile(hClip, TRUE);
					break;
				}
			} // end switch
		}

		if ( bResult == TRUE )
			return imageobject.GetImage();

		return FALSE;
	}

	HBITMAP BitmapFromIconCursor(HICON hIconCursor, HWND hWnd)
	{
		if (hIconCursor == NULL || hWnd == NULL) return NULL;

		ICONINFO iconInfo;
		::GetIconInfo(hIconCursor, &iconInfo);

		BITMAP bm;
		::GetObject(iconInfo.hbmMask, sizeof(bm), &bm);

		// monochrome icon/cursor: top half of mask is AND segment,
		// bottom half is XOR, so use width as cy value
		int cy = bm.bmHeight;
		if (cy == 2 * bm.bmWidth) cy = bm.bmWidth;

		CClientDC hdc(hWnd);
		CDC hdcMem = ::CreateCompatibleDC(hdc);

		// combine the icon's mask and color bitmaps into one
		HBITMAP oldBmp = hdcMem.SelectBitmap(iconInfo.hbmColor);
		hdc.MaskBlt(0, 0, bm.bmWidth, cy, hdcMem, 0, 0, iconInfo.hbmMask, 0, cy, MAKEROP4(SRCINVERT, SRCAND));

		// make a copy of the combined bitmap
		HBITMAP hBmpCopy;
		if (iconInfo.hbmColor != NULL)
			hBmpCopy = ::CreateCompatibleBitmap(hdc, bm.bmWidth, cy);
		else // monochrome bitmap
			hBmpCopy = ::CreateBitmap(bm.bmWidth, cy, bm.bmPlanes, bm.bmBitsPixel, NULL);
		hdcMem.SelectBitmap(hBmpCopy);
		hdcMem.BitBlt(0, 0, bm.bmWidth, cy, hdc, 0, 0, SRCCOPY);

		// clean up
		hdcMem.SelectBitmap(oldBmp);
		::DeleteObject(iconInfo.hbmMask);
		::DeleteObject(iconInfo.hbmColor);

		return hBmpCopy;
	}
};