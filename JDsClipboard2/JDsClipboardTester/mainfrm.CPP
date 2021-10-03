#include "StdAfx.h"
#include "MainFrm.h"
#include /**/ <shellapi.h>

HWND CMainFrame::g_hWndJDsRdpClient = NULL;

LRESULT CMainFrame::OnEditPaste(WORD wNotifyCode, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_viewPicture.Paste();

	return 0;
}


LRESULT CMainFrame::OnEditCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWnd, BOOL& /*bHandled*/)
{
	m_viewPicture.Copy(hWnd);

	return 0;
}

LRESULT CMainFrame::OnEditCut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWnd, BOOL& /*bHandled*/)
{
	m_viewPicture.Cut(hWnd);

	return 0;
}

LRESULT CMainFrame::OnViewClear(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_viewPicture.Clear();
	m_viewText.Clear();

	return 0;
}




LRESULT CMainFrame::OnToolsRunjdsrdpclip(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SHELLEXECUTEINFO execInfo;
	::ZeroMemory(&execInfo, sizeof(execInfo));
	execInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    execInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

    execInfo.lpFile = _T("C:\\COCKPIT43\\Jetro COCKPIT Client\\Source Control\\JDsCockpit\\BinDebug\\JDsRdpClip.exe");
    execInfo.lpParameters = _T("");
	execInfo.lpDirectory = _T("");
    execInfo.nShow = SW_HIDE;

    if(!::ShellExecuteEx(&execInfo))
		return 1;

	m_instJdsRdpClip = (HINSTANCE)execInfo.hProcess;

	return 0;
}


LRESULT CMainFrame::OnToolsClosejdsrdpclip(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here

	DWORD dwProcessID =  GetProcessId(m_instJdsRdpClip);

	g_hWndJDsRdpClient = NULL;
	::EnumWindows((WNDENUMPROC)CMainFrame::EnumWindowProc, (LPARAM)dwProcessID);

	SendMessage(g_hWndJDsRdpClient, WM_CLOSE,0,0); 

	return 0;
}

BOOL CALLBACK CMainFrame::EnumWindowProc(HWND hWnd, LPARAM lParam)
{
	DWORD dwProcessID = 0;
	::GetWindowThreadProcessId(hWnd, &dwProcessID);

	if (dwProcessID != (DWORD)lParam)
		return TRUE;

	if (::GetParent(hWnd) == NULL)
	{
		// 
		g_hWndJDsRdpClient = hWnd;
		return FALSE;
	}

	return TRUE;
}

