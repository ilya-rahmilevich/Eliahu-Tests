#include "StdAfx.h"
#include "MainFrm.h"


LRESULT CMainFrame::OnEditPaste(WORD wNotifyCode, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
#ifdef AAA
	if (m_viewPicture.Paste())
	{
		if (!m_viewPicture.m_pix.IsNull())
		{
			m_viewPicture.SetSize(10);
		}
	}
#endif
	return 0;
}


LRESULT CMainFrame::OnEditCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWnd, BOOL& /*bHandled*/)
{
//	m_view.Copy(hWnd);

	return 0;
}

LRESULT CMainFrame::OnViewClear(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_viewPicture.Clear();
	m_viewText.Clear();

	return 0;
}


LRESULT CMainFrame::OnSerializationStore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	const TCHAR szFile[] = _T("c:\\Temp\\JetroClipboard.dat");

	StoreObject(szFile, TRUE);

	return 0;
}


LRESULT CMainFrame::OnSerializationLoad(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	const TCHAR szFile[] = _T("c:\\Temp\\JetroClipboard.dat");

	LoadObject(szFile, TRUE);

	return 0;
}
