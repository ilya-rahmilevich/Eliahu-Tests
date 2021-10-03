// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "JDsClipboard.h"
#include "JDsClipPictureHelper.h"
#include "viewText.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atldlgs.h>
#include <atlscrl.h>
#include <atlsplit.h>
#include <atlctrlx.h>

#include "view.h"
#include "resource.h"
using namespace JDsClipboard;

class CFileDialogFilter  
{
private:
	CString m_strFilter;
public:
	CFileDialogFilter()
	{
	}

	/// nID The ID of a resource string containing the filter
	CFileDialogFilter(UINT nID)
	{
		SetFilter(nID);
	}
	
	/// lpsz The filter string
	CFileDialogFilter(LPCTSTR lpsz)
	{
		SetFilter(lpsz);
	}
	
	~CFileDialogFilter()
	{
	}

	inline LPCTSTR GetFilter() const { return m_strFilter; }
	inline operator LPCTSTR() const { return m_strFilter; }

	// Set the filter string to use
	// nID - The ID of a resource string containing the filter
	void SetFilter(UINT nID)
	{
		if (m_strFilter.LoadString(nID) && !m_strFilter.IsEmpty())
			ModifyString();
	}

	// Set the filter string to use
	// lpsz - The filter string
	void SetFilter(LPCTSTR lpsz)
	{		
		m_strFilter = lpsz;
		if (!m_strFilter.IsEmpty())
			ModifyString();
	}
private:
	// Replace '|' with '\0'
	void ModifyString(void)
	{
		// Get a pointer to the string buffer
		LPTSTR psz = m_strFilter.GetBuffer(0);
		// Replace '|' with '\0'
		while ((psz = _tcschr(psz, '|')) != NULL)
			*psz++ = '\0';
	}
};

class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>, 
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, public CIdleHandler,
	public CJDSClipboardListener<CMainFrame>
{
	enum ShowMode { enShowText, enShowImage} m_ShowMode;
public:
	CWTLIPictureView	 m_viewPicture;
	CTesterTextView		 m_viewText;

	CSplitterWindow m_splitter;
	CPaneContainer m_pane;

	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)


	CMainFrame()
	{
		m_ShowMode = enShowImage;
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_FILE_SAVE_AS, OnFileSaveas)
		COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
		CHAIN_MSG_MAP_ALT(CJDSClipboardListener<CMainFrame>, 1)
		COMMAND_ID_HANDLER(ID_EDIT_PASTE, OnEditPaste)
		COMMAND_ID_HANDLER(ID_EDIT_COPY, OnEditCopy)
		COMMAND_ID_HANDLER(ID_VIEW_CLEAR, OnViewClear)
		COMMAND_ID_HANDLER(ID_SERIALIZATION_SERIALIZE, OnSerializationStore)
		COMMAND_ID_HANDLER(ID_SERIALIZATION_SERIALIZE32776, OnSerializationLoad)
		//COMMAND_ID_HANDLER(ID_VIEW_PROPERTIES, OnViewProperties)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	//	if ( wParam == ID_VIEW_PROPERTIES )
			bHandled =::PostMessage(m_viewPicture,uMsg,wParam,lParam);

		bHandled = FALSE;

		return 0;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{

		m_hWndClient = m_splitter.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_pane.SetPaneContainerExtendedStyle(PANECNT_NOBORDER);
		m_pane.Create(m_splitter, _T("Clipboard Format<>"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_viewPicture.Create(m_pane, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
		m_pane.SetClient(m_viewPicture);

		// create view window
		m_viewText.Create(m_splitter, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
		m_splitter.SetSplitterPanes(m_pane, m_viewText);

		UpdateLayout();
		m_splitter.SetSplitterPosPct(25);


		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		bHandled = FALSE;

		return 0;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		bHandled = FALSE;
		return 1;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: add code to initialize document

		return 0;
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		return m_viewPicture.PreTranslateMessage(pMsg);
	}
	
	LRESULT OnFileSaveas(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{

		CFileDialogFilter strFilter( _T("Jetro image files (*.bmp)|All Files (*.*)|*.*||") );
		CFileDialog dlg(FALSE, _T("bmp"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);

		if ( IDOK == dlg.DoModal(*this) )
		{
			m_viewPicture.m_pix.SaveToFile(dlg.m_ofn.lpstrFile, TRUE/*true for saving from Clipboard*/);
		}

		return 0;
	}

	BOOL OpenTheFile(LPTSTR szFilePath)
	{

		if (szFilePath == NULL) 
			return FALSE;

		

		return FALSE;
	}

	LRESULT OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static LPCTSTR g_lpcstrFileFilters = _T("All Pix (*.bmp;*.cur;*.emf;*.gif;*.ico;*.jpg;*.wmf)\0*.bmp;*.cur;*.emf;*.gif;*.ico;*.jpg;*.wmf\0Bitmap Files (*.bmp)\0*.bmp\0Cursor Files (*.cur)\0*.cur\0Enhanced Metafiles (*.emf)\0*.emf\0Graphics Interchange Files (*.gif)\0*.gif\0Icon Files (*.ico)\0*.ico\0JPEG Files (*.jpg)\0*.jpg\0Windows Metafiles (*.wmf)\0*.wmf\0All Files (*.*)\0*.*\0");

		CFileDialog dlg(TRUE, _T("bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			g_lpcstrFileFilters, m_hWnd);

		if(dlg.DoModal() == IDOK)
		{
			if (OpenTheFile(dlg.m_szFileName))
			{
				//
			}
		}

		return 0;
	}

	void ShowClipObject()
	{
		if ( _clipObject == NULL )
			return;

		Clear();

		switch( m_uFormat[0] )
		{
			case CF_TEXT:
			case CF_OEMTEXT:
			case CF_UNICODETEXT:
				m_viewText.ShowClipObject(*_clipObject);
			break;
			case CF_BITMAP:
			case CF_DIB:
			case CF_METAFILEPICT:
			case CF_ENHMETAFILE:
				//ManageImage(m_uFormat[0]);
				m_viewPicture.ShowClipObject(*_clipObject);
			break;
			default:
				break;
		}
	}


	void ManageClipboardText(CString &sText)
	{
		CString sMess;
		sMess.Format(_T("<%s> put to Clipboard"), sText.GetString());

		//MessageBox(sMess, _T("JDsClipboardListener"));
		m_viewText.SetText(sText);
		m_viewText.Invalidate();
		m_viewText.UpdateWindow();
	}

	void PasteImage(IPicture *pPicture)
	{
		m_viewPicture.PasteImage(pPicture);
	}

	void Clear()
	{
		m_viewPicture.Clear();
		m_viewText.Clear();
	}

	LRESULT OnViewProperties(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		return 0;
	}

	void ClipBoardEventReceived(UINT uType)
	{
		CString sForm;
		GetClipboardFormatName(uType, sForm);

		CString sText;
		sText.Format(_T("<%s> copied to Clipboard"), sForm.GetString());

		m_pane.SetTitle(sText);
	}

	LRESULT OnEditPaste(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewClear(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSerializationStore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSerializationLoad(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
