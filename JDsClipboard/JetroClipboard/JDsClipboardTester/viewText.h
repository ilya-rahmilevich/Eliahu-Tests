#pragma once

class CTesterTextView : public CWindowImpl<CTesterTextView>
{
public:
	DECLARE_WND_CLASS(NULL)
	CString m_sText;
    CFont m_FontText;

	void SetText(const CString &sT) { m_sText = sT; };

	CTesterTextView()
	{
		m_sText = _T("kuku");

		CLogFont logFont;
        TCHAR fontName[] = _T("Arial");
        wcscpy_s(logFont.lfFaceName,fontName);
        logFont.lfHeight = 20;
        logFont.lfItalic = TRUE;
        logFont.lfWeight = FW_BOLD;
        //logFont.lfStrikeOut = TRUE;
        m_FontText.CreateFontIndirect(&logFont);
	}

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	BEGIN_MSG_MAP(CTesterTextView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CPaintDC dc(m_hWnd);

       
        //Do painting work here...
        CRect rc;
        GetClientRect(&rc);
        dc.SaveDC();
        dc.SelectFont(m_FontText);
        dc.DrawText(m_sText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
        
        dc.RestoreDC(-1);

		return 0;
	}

	void ShowClipObject(CJDsClipboardObject &clipObject)
	{
		CJDsClipboardStringObject *pObj = (CJDsClipboardStringObject *)&clipObject;
		m_sText = pObj->GetText();

		Invalidate();
		UpdateWindow();
	}

	void Clear()
	{
		SetText(_T(""));
		Invalidate();
		UpdateWindow();
	}
};
