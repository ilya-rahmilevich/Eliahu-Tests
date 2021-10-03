#pragma once

#include "Serialization\JDsClipboardStringObject.h"
#include "resource.h"

class CTesterTextView : public CWindowImpl<CTesterTextView>
{
	CJDsClipboardStringObject _clipObject;

	CSerializedDownContainer _down_container;
	CSerializedUpContainer	 _upContainer;
public:
	DECLARE_WND_CLASS(NULL)
	CString m_sText;
    CFont m_FontText;

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
		COMMAND_ID_HANDLER(ID_TEXT_SERIALIZATION_STORE_MEM, OnSerializationStore)
		COMMAND_ID_HANDLER(ID_TEXT_SERIALIZATION_LOAD_MEM, OnSerializationLoad)
		COMMAND_ID_HANDLER(ID_TEXT_SERIALIZATION_STORE_FILE, OnSerializationStoreToFile)
		COMMAND_ID_HANDLER(ID_TEXT_SERIALIZATION_LOAD_FILE, OnSerializationLoadFromFile)
	END_MSG_MAP()

	void SetText(CString &sT) 
	{
		_clipObject.SetText(sT);
		m_sText = sT;
	};

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

	void ShowClipObject()
	{
		m_sText = _clipObject.GetText();

		Invalidate();
		UpdateWindow();
	}

	void Clear()
	{
		SetText(CString(_T("")));
		Invalidate();
		UpdateWindow();
	}

	BOOL IsDataEmpty() 
	{
		return m_sText.IsEmpty();
	}

	BOOL IsSerializedDataEmpty()
	{
		return ( _upContainer.GetLength() == 0 );
	}

	BOOL SaveToFile(const CString &sFile);
	BOOL LoadFromFile(const CString &sFile);

	LRESULT OnSerializationStore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSerializationLoad(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSerializationStoreToFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSerializationLoadFromFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
