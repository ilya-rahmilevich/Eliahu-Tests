// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "JDsClipboard.h"
#include "JDsClipPictureHelper.h"
#include "viewText.h"
#include "view.h"
#include "resource.h"
using namespace JDsClipboard;

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

	HINSTANCE m_instJdsRdpClip;
	static HWND g_hWndJDsRdpClient; 
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)


	CMainFrame()
	{
		m_instJdsRdpClip = NULL;
		m_ShowMode = enShowImage;
	}

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
		COMMAND_ID_HANDLER(ID_EDIT_CUT, OnEditCut)
		COMMAND_ID_HANDLER(ID_EDIT_PASTE, OnEditPaste)
		COMMAND_ID_HANDLER(ID_EDIT_COPY, OnEditCopy)
		COMMAND_ID_HANDLER(ID_VIEW_CLEAR, OnViewClear)
		//COMMAND_ID_HANDLER(ID_VIEW_PROPERTIES, OnViewProperties)
		COMMAND_ID_HANDLER(ID_TOOLS_RUNJDSRDPCLIP, OnToolsRunjdsrdpclip)
		COMMAND_ID_HANDLER(ID_TOOLS_CLOSEJDSRDPCLIP, OnToolsClosejdsrdpclip)
	END_MSG_MAP()

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_FILE_SAVE_AS,	UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_IMAGE_SERIALIZATION_STORE_MEM,	UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_IMAGE_SERIALIZATION_LOAD_MEM,		UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_IMAGE_SERIALIZATION_STORE_TO_FILE,UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_TEXT_SERIALIZATION_STORE_MEM,		UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_TEXT_SERIALIZATION_LOAD_MEM,		UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_TEXT_SERIALIZATION_STORE_FILE,	UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	virtual BOOL OnIdle()
	{
		UIEnable(ID_FILE_SAVE_AS,						!m_viewPicture.IsDataEmpty());

		UIEnable(ID_IMAGE_SERIALIZATION_STORE_MEM,		!m_viewPicture.IsDataEmpty());
		UIEnable(ID_IMAGE_SERIALIZATION_LOAD_MEM,		!m_viewPicture.IsSerializedDataEmpty());
		UIEnable(ID_IMAGE_SERIALIZATION_STORE_TO_FILE,	!m_viewPicture.IsDataEmpty());
		
		UIEnable(ID_TEXT_SERIALIZATION_STORE_MEM,		!m_viewText.IsDataEmpty());
		UIEnable(ID_TEXT_SERIALIZATION_LOAD_MEM,		!m_viewText.IsSerializedDataEmpty());
		UIEnable(ID_TEXT_SERIALIZATION_STORE_FILE,		!m_viewText.IsDataEmpty());
		
		return FALSE;
	}

	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	//	if ( wParam == ID_VIEW_PROPERTIES )
		::PostMessage(m_viewPicture,uMsg,wParam,lParam);

		::PostMessage(m_viewText,uMsg,wParam,lParam);

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

		if (  m_viewPicture.PreTranslateMessage(pMsg) )
			return TRUE;

		if (  m_viewText.PreTranslateMessage(pMsg) )
			return TRUE;

		return FALSE;
	}

	
	LRESULT OnFileSaveas(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		LPCTSTR strFilter =  _T("Jetro image files (*.bmp)\0*.bmp\0|All Files (*.*)\0*.*\0");
		CFileDialog dlg(FALSE, _T("bmp"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);

		if ( IDOK == dlg.DoModal(*this) )
		{
			try
			{
				m_viewPicture.SaveToFile(dlg.m_ofn.lpstrFile, TRUE/*true for saving from Clipboard*/);
			}
			catch(CJDsClipboardException ex)
			{
				ATLTRACE(_T("#JDsClipboard# exeption %s\n"), ex.GetErrorDesc().GetString());
			}
			catch(...)
			{
				ATLTRACE(_T("#JDsClipboard# unknown exeption\n"));	
			}

		}

		return 0;
	}

	LRESULT OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static LPCTSTR g_lpcstrFileFilters = _T("All Pix (*.bmp;*.cur;*.emf;*.gif;*.ico;*.jpg;*.wmf)\0*.bmp;*.cur;*.emf;*.gif;*.ico;*.jpg;*.wmf\0Bitmap Files (*.bmp)\0*.bmp\0Cursor Files (*.cur)\0*.cur\0Enhanced Metafiles (*.emf)\0*.emf\0Graphics Interchange Files (*.gif)\0*.gif\0Icon Files (*.ico)\0*.ico\0JPEG Files (*.jpg)\0*.jpg\0Windows Metafiles (*.wmf)\0*.wmf\0All Files (*.*)\0*.*\0");

		CFileDialog dlg(TRUE, _T("bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			g_lpcstrFileFilters, m_hWnd);

		if(dlg.DoModal() == IDOK)
		{

			if (m_viewPicture.OpenTheFile(dlg.m_szFileName))
			{
				//
			}
		}

		return 0;
	}

	void ManageClipboardText(CString &sText)
	{
		CString sMess;
		sMess.Format(_T("<%s> put to Clipboard"), sText.GetString());

		m_viewText.SetText(sText);
		m_viewText.Invalidate();
		m_viewText.UpdateWindow();
	}

	void PasteImage(UINT uFormat)
	{
		m_viewPicture.PasteImage(uFormat);
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
		Clear();

		CString sForm;
		GetClipboardFormatName(uType, sForm);

		CString sText;
		sText.Format(_T("<%s> copied to Clipboard"), sForm.GetString());

		m_pane.SetTitle(sText);
	}

	static BOOL CALLBACK EnumWindowProc(HWND hWnd, LPARAM lParam);

	LRESULT OnEditPaste(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditCut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewClear(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnToolsRunjdsrdpclip(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnToolsClosejdsrdpclip(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
