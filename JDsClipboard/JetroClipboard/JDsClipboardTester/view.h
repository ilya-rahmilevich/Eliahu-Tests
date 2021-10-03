#pragma once

#include "JDsClipboard.h"
#include "JDsClipPictureHelper.h"
#include "JDsClipArchive\Archive.h"
#include "JDsClipArchive\Exception.h"
#include "JDsClipArchive\File.h"
//#include "props.h"
#include "resource.h"

using namespace JDsClipboard;

class CWTLIPictureView : 
	public CScrollWindowImpl<CWTLIPictureView>
{
    CFont m_FontText;
public:
	DECLARE_WND_CLASS_EX(NULL, 0, -1)
	CWTLIPicture m_pix;
	bool m_border;
	int m_scale;
	SIZE m_size;

	CWTLIPictureView()
	{
		m_border = true;
		m_scale = 100;
		m_size.cx = m_size.cy = 300;

		CLogFont logFont;
        TCHAR fontName[] = _T("Arial");
        wcscpy_s(logFont.lfFaceName,fontName);
        logFont.lfHeight = 60;
        logFont.lfItalic = TRUE;
        logFont.lfWeight = FW_BOLD;
        logFont.lfStrikeOut = TRUE;
        m_FontText.CreateFontIndirect(&logFont);

	}

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	BEGIN_MSG_MAP(CWTLIPictureView)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		CHAIN_MSG_MAP(CScrollWindowImpl<CWTLIPictureView>)
		COMMAND_ID_HANDLER(ID_VIEW_PROPERTIES, OnViewProperties)
	END_MSG_MAP()

	void PasteImage(IPicture *pPicture)
	{
		m_pix = pPicture;

		if (!m_pix.IsNull())
		{
			SetSize(100);
		}

		Invalidate();
		UpdateWindow();
	}


	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		CDCHandle dc = (HDC)wParam;

		RECT rect;
		GetClientRect(&rect);
		dc.FillRect(&rect, (HBRUSH)(COLOR_WINDOW + 1));


		if(!m_pix.IsNull())
		{
			// draw border around picture
			if(m_border)
			{
				dc.MoveTo(m_size.cx, 0);
				dc.LineTo(m_size.cx, m_size.cy);
				dc.LineTo(0, m_size.cy);
			}
		}

		return 1;
	}

	void DoPaint(CDCHandle dc)
	{
		if(!m_pix.IsNull())
		{
			// draw the picture
			RECT rc = { 0, 0, m_size.cx, m_size.cy };
			m_pix.Render(dc, rc);
		}
	}


	void SetSize(int nScale = 100)
	{
		if (!m_pix.IsNull())
			m_pix.GetSizeInPixels(m_size);
		else
			m_size.cx = m_size.cy = 0;

		// allowable scale 25% to 800%
		if (nScale >= 25 && nScale <= 800)
		{
			m_scale = nScale;

			// need at least 4 pixels to scale 25%
			if (m_size.cx >= 4 && m_size.cy >= 4)
			{
				m_size.cx = (m_size.cx * nScale) / 100;
				m_size.cy = (m_size.cy * nScale) / 100;
			}
		}

		SetScrollOffset(0, 0, FALSE);

		if (m_size.cx > 0 && m_size.cy > 0)
			SetScrollSize(m_size);
		else
			SetScrollSize(1, 1);

	}

	void Clear()
	{
		m_pix.Clear();
		Invalidate();
		UpdateWindow();
	}

	void ShowClipObject(CJDsClipboardObject &clipObject)
	{
		CJDsClipboardImageObject *pObj = (CJDsClipboardImageObject *)&clipObject;
		m_pix = pObj->GetImage();

		if (!m_pix.IsNull())
		{
			SetSize(100);
		}

		Invalidate();
		UpdateWindow();
	}

	LRESULT OnViewProperties(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		/*
		CPixProperties prop;
		prop.SetFileInfo(m_szFilePath, m_view.m_pix.GetPixInformation());
		prop.DoModal();
		*/
		return 0;
	}
};

