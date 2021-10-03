// props.h : interface for the properties classes
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __PROPS_H__
#define __PROPS_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// not defined in the original VC6 headers
#ifndef BI_JPEG
#define BI_JPEG       4L
#endif
#ifndef BI_PNG
#define BI_PNG        5L
#endif

// decode a WORD value into its component bytes
#define UNMAKEWORD(w) (SHORT)(HIBYTE(w) | (LOBYTE(w) << 8))

class CFileName : public CWindowImpl<CFileName>
{
public:
	DECLARE_WND_CLASS_EX(NULL, 0, COLOR_3DFACE)

	LPCTSTR m_lpstrFilePath;

	enum { m_nToolTipID = 1313 };
	CToolTipCtrl m_tooltip;

	CFileName() : m_lpstrFilePath(NULL)
	{ }

	void Init(HWND hWnd, LPCTSTR lpstrName)
	{
		ATLASSERT(::IsWindow(hWnd));
		SubclassWindow(hWnd);

		// Set tooltip
		m_tooltip.Create(m_hWnd);
		ATLASSERT(m_tooltip.IsWindow());
		RECT rect;
		GetClientRect(&rect);
		CToolInfo ti(0, m_hWnd, m_nToolTipID, &rect, NULL);
		m_tooltip.AddTool(&ti);

		// set text
		m_lpstrFilePath = lpstrName;
		if(m_lpstrFilePath == NULL)
			return;

		CClientDC dc(m_hWnd);	// will not really paint
		HFONT hFontOld = dc.SelectFont(AtlGetStockFont(DEFAULT_GUI_FONT));

		RECT rcText = rect;
		dc.DrawText(m_lpstrFilePath, -1, &rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_CALCRECT);
		BOOL bTooLong = (rcText.right > rect.right);
		if(bTooLong)
			m_tooltip.UpdateTipText(m_lpstrFilePath, m_hWnd, m_nToolTipID);
		m_tooltip.Activate(bTooLong);

		dc.SelectFont(hFontOld);

		Invalidate();
		UpdateWindow();
	}

	BEGIN_MSG_MAP(CFileName)
		MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	LRESULT OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if(m_tooltip.IsWindow())
		{
			MSG msg = { m_hWnd, uMsg, wParam, lParam };
			m_tooltip.RelayEvent(&msg);
		}
		bHandled = FALSE;
		return 1;
	}

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CPaintDC dc(m_hWnd);
		if(m_lpstrFilePath != NULL)
		{
			RECT rect;
			GetClientRect(&rect);

			dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			dc.SetBkMode(TRANSPARENT);
			HFONT hFontOld = dc.SelectFont(AtlGetStockFont(DEFAULT_GUI_FONT));

			dc.DrawText(m_lpstrFilePath, -1, &rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_PATH_ELLIPSIS);

			dc.SelectFont(hFontOld);
		}
		return 0;
	}
};

class CPageOne : public CPropertyPageImpl<CPageOne>
{
public:
	enum { IDD = IDD_PROP_PAGE1 };

	LPCTSTR m_lpstrFilePath;
	CFileName m_filename;

	CPageOne() : m_lpstrFilePath(NULL)
	{ }

	BEGIN_MSG_MAP(CPageOne)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageOne>)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if(m_lpstrFilePath != NULL)	// get and set file properties
		{
			m_filename.Init(GetDlgItem(IDC_FILELOCATION), m_lpstrFilePath);

			WIN32_FIND_DATA fd;
			HANDLE hFind = ::FindFirstFile(m_lpstrFilePath, &fd);
			if(hFind != INVALID_HANDLE_VALUE)
			{
				int nSizeK = (int)(fd.nFileSizeLow / 1024);	// assume not bigger than 2GB
				if(nSizeK == 0 && fd.nFileSizeLow != 0)
					nSizeK = 1;
				TCHAR szBuff[100];
				wsprintf(szBuff, _T("%i KB  (%i bytes)"), nSizeK, fd.nFileSizeLow);
				SetDlgItemText(IDC_FILESIZE, szBuff);
				SYSTEMTIME st;
				::FileTimeToSystemTime(&fd.ftCreationTime, &st);
				::GetDateFormat(LOCALE_USER_DEFAULT, 0, &st, _T("dddd, MMMM dd',' yyyy',' "), szBuff, sizeof(szBuff) / sizeof(szBuff[0]));
				TCHAR szBuff1[50];
				::GetTimeFormat(LOCALE_USER_DEFAULT, 0, &st, _T("hh':'mm':'ss tt"), szBuff1, sizeof(szBuff1) / sizeof(szBuff1[0]));
				lstrcat(szBuff, szBuff1);
				SetDlgItemText(IDC_FILEDATE, szBuff);

				szBuff[0] = 0;
				if((fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0)
					lstrcat(szBuff, _T("Archive, "));
				if((fd.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0)
					lstrcat(szBuff, _T("Read-only, "));
				if((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0)
					lstrcat(szBuff, _T("Hidden, "));
				if((fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0)
					lstrcat(szBuff, _T("System"));
				int nLen = lstrlen(szBuff);
				if(nLen >= 2 && szBuff[nLen - 2] == _T(','))
					szBuff[nLen - 2] = 0;
				SetDlgItemText(IDC_FILEATTRIB, szBuff);

				::FindClose(hFind);
			}
		}
		else
		{
			SetDlgItemText(IDC_FILELOCATION, _T("(Clipboard)"));
			SetDlgItemText(IDC_FILESIZE, _T("N/A"));
			SetDlgItemText(IDC_FILEDATE, _T("N/A"));
			SetDlgItemText(IDC_FILEATTRIB, _T("N/A"));
		}

		return TRUE;
	}
};

class CPageTwo : public CPropertyPageImpl<CPageTwo>
{
public:
	enum { IDD = IDD_PROP_PAGE2 };

	LPPIXINFO m_pPixInfo;

	BEGIN_MSG_MAP(CPageTwo)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageTwo>)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		EnableWindow(false);

		// Special - remove unused buttons, move Close button, center wizard
		CPropertySheetWindow sheet = GetPropertySheet();
		sheet.CancelToClose();
		RECT rect;
		CButton btnCancel = sheet.GetDlgItem(IDCANCEL);
		btnCancel.GetWindowRect(&rect);
		sheet.ScreenToClient(&rect);
		btnCancel.ShowWindow(SW_HIDE);
		CButton btnClose = sheet.GetDlgItem(IDOK);
		btnClose.SetWindowPos(NULL, &rect, SWP_NOZORDER | SWP_NOSIZE);
		sheet.CenterWindow(GetPropertySheet().GetParent());

		sheet.ModifyStyleEx(WS_EX_CONTEXTHELP, 0);

		// get and display picture properties
		SetDlgItemText(IDC_TYPE, m_pPixInfo->piDescription);
		LPTSTR lpstrCompression = _T("Uncompressed");;
		SetDlgItemInt(IDC_WIDTH, m_pPixInfo->piSize.cx);
		SetDlgItemInt(IDC_HEIGHT, m_pPixInfo->piSize.cy);
		CheckDlgButton(IDC_TRANS, m_pPixInfo->piAttributes & PICTURE_TRANSPARENT ? BST_CHECKED : BST_UNCHECKED);
		CheckDlgButton(IDC_SCALE, m_pPixInfo->piAttributes & PICTURE_SCALABLE ? BST_CHECKED : BST_UNCHECKED);

		switch(m_pPixInfo->piType)
		{
		case PICTYPE_BITMAP:
			{
				LPBITMAPFILEHEADER lpBfh = (LPBITMAPFILEHEADER)&m_pPixInfo->piHeader;

				if (lpBfh->bfType == 0x4D42)
				{
					LPBITMAPINFOHEADER lpBih = (LPBITMAPINFOHEADER)&m_pPixInfo->piHeader[sizeof(BITMAPFILEHEADER)];

					SetDlgItemInt(IDC_HORRES, ::MulDiv(lpBih->biXPelsPerMeter, 254, 10000));
					SetDlgItemInt(IDC_VERTRES, ::MulDiv(lpBih->biYPelsPerMeter, 254, 10000));
					SetDlgItemInt(IDC_BITDEPTH, lpBih->biBitCount);

					switch(lpBih->biCompression)
					{
					case BI_RLE4:
					case BI_RLE8:
						lpstrCompression = _T("RLE");
						break;
					case BI_BITFIELDS:
						lpstrCompression = _T("Uncompressed with bitfields");
						break;
					case BI_JPEG:
					case BI_PNG:
						lpstrCompression = _T("Unknown");
						break;
					}
				}
				else
				{
					LPJPEGFILEHEADER lpJfh = (LPJPEGFILEHEADER)&m_pPixInfo->piHeader;
					if ((lpJfh->jfID == 0xE0FFD8FF))
					{
						if (lstrcmp(LPCTSTR(&lpJfh->jfSignature1), _T("JFIF")) == 0)
						{
							SetDlgItemText(IDC_TYPE, _T("JPEG - JFIF Compliant (JPG)"));
						}

						SetDlgItemInt(IDC_HORRES, UNMAKEWORD(lpJfh->jfXdpu));
						SetDlgItemInt(IDC_VERTRES, UNMAKEWORD(lpJfh->jfYdpu));

						if (lpJfh->jfUnits == 1)
							SetDlgItemText(IDC_UNITS, _T("dpi"));
						else if (lpJfh->jfUnits == 2)
							SetDlgItemText(IDC_UNITS, _T("dpcm"));

						int i;
						for (i=sizeof(JPEGFILEHEADER); i<sizeof(m_pPixInfo->piHeader); i++)
						{
							if (m_pPixInfo->piHeader[i] == 0xC0 && m_pPixInfo->piHeader[i-1] == 0xFF)
							{
								LPJPEGINFOHEADER lpJih = (LPJPEGINFOHEADER)&m_pPixInfo->piHeader[i+1];
								SetDlgItemInt(IDC_BITDEPTH, lpJih->jiPrecision);
								break;
							}
						}
					}
				}

				break;
			}
		case PICTYPE_ENHMETAFILE:
			{
				LPENHMETAFILEHEADER lpEfh = (LPENHMETAFILEHEADER)&m_pPixInfo->piHeader;
				if (lpEfh->dSignature == ENHMETA_SIGNATURE)
				{
					SetDlgItemInt(IDC_COLORDEPTH, lpEfh->nPalEntries);
					SetDlgItemInt(IDC_HORRES, lpEfh->szlDevice.cx); // ?
					SetDlgItemInt(IDC_VERTRES, lpEfh->szlDevice.cy); // ?
				}

				break;
			}
		case PICTYPE_ICON:
			{
				// works for both cursor and icon (type is different)
				LPICONFILEHEADER lpIfh = (LPICONFILEHEADER)&m_pPixInfo->piHeader;

				SetDlgItemInt(IDC_NUMIMAGES, lpIfh->ifCount);

				if (lpIfh->ifType == 2)
				{
					SetDlgItemText(IDC_TYPE, _T("Windows Cursor (CUR)"));

					LPCURSORINFOHEADER lpCih = (LPCURSORINFOHEADER)&m_pPixInfo->piHeader[sizeof(ICONFILEHEADER)];
					LPBITMAPINFOHEADER lpBih = (LPBITMAPINFOHEADER)&m_pPixInfo->piHeader[lpCih->ciOffset];
					SetDlgItemInt(IDC_BITDEPTH, lpBih->biBitCount);
				}
				else
				{
					LPICONINFOHEADER lpIih = (LPICONINFOHEADER)&m_pPixInfo->piHeader[sizeof(ICONFILEHEADER)];

					SetDlgItemInt(IDC_BITDEPTH, lpIih->iiBitCount);
					if (lpIih->iiColor == 0)
						SetDlgItemInt(IDC_COLORDEPTH, 256);
					else
						SetDlgItemInt(IDC_COLORDEPTH, lpIih->iiColor);
				}

				break;
			}
		case PICTYPE_METAFILE:
			{
				LPMETAFILEHEADER lpMfh = (LPMETAFILEHEADER)&m_pPixInfo->piHeader;
				if (lpMfh->mfKey == 0x9AC6CDD7)
				{
					SetDlgItemInt(IDC_HORRES, lpMfh->mfInch);
					SetDlgItemInt(IDC_VERTRES, lpMfh->mfInch);
					SetDlgItemText(IDC_UNITS, _T("twips"));
				}
				break;
			}
		}

		SetDlgItemText(IDC_COMPRESSION, lpstrCompression);

		return TRUE;
	}
};

class CPageThree : public CPropertyPageImpl<CPageThree>
{
public:
	enum { IDD = IDD_PROP_PAGE3 };

	BEGIN_MSG_MAP(CPageThree)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageThree>)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// get and set screen properties
		CClientDC dc(NULL);
		SetDlgItemInt(IDC_WIDTH, dc.GetDeviceCaps(HORZRES));
		SetDlgItemInt(IDC_HEIGHT, dc.GetDeviceCaps(VERTRES));
		SetDlgItemInt(IDC_HORRES, dc.GetDeviceCaps(LOGPIXELSX));
		SetDlgItemInt(IDC_VERTRES, dc.GetDeviceCaps(LOGPIXELSY));
		SetDlgItemInt(IDC_BITDEPTH, dc.GetDeviceCaps(BITSPIXEL));

		return TRUE;
	}
};

class CPixProperties : public CPropertySheetImpl<CPixProperties>
{
public:
	CPageOne m_page1;
	CPageTwo m_page2;
	CPageThree m_page3;


	CPixProperties()
	{
		m_psh.dwFlags |= PSH_NOAPPLYNOW;

		AddPage(m_page1);
		AddPage(m_page2);
		AddPage(m_page3);
		SetActivePage(1);
		SetTitle(_T("Picture Properties"));
	}

	void SetFileInfo(LPCTSTR lpstrFilePath, LPPIXINFO pPixInfo)
	{
		m_page1.m_lpstrFilePath = lpstrFilePath;
		m_page2.m_pPixInfo = pPixInfo;
	}

	BEGIN_MSG_MAP(CPixProperties)
		CHAIN_MSG_MAP(CPropertySheetImpl<CPixProperties>)
	END_MSG_MAP()
};

#endif // __PROPS_H__
