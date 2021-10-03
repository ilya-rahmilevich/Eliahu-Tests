#pragma once

#ifndef __CWTLIPICTUREVIEW_H__
#define __CWTLIPICTUREVIEW_H__

#include "JDsClipboard.h"
#include "JDsClipPictureHelper.h"
#include "Serialization\JDsClipboardImageObject.h"
#include "resource.h"
#include "base64_string.h"

using namespace JDsClipboard;

char *base64_encode(const char *data, size_t input_length, size_t *output_length);
char *base64_decode(const char *data, size_t input_length, size_t *output_length);


class CWTLIPictureView : 
	public CScrollWindowImpl<CWTLIPictureView>
{
private:
	CJDsClipboardImageObject _clipObject;
	CSerializedDownContainer _down_container;
	CSerializedUpContainer	 _upContainer;
	CJDsClipPictureHelper	_pix;
	bool					m_border;
	int						m_scale;
	SIZE					m_size;
public:
	DECLARE_WND_CLASS_EX(NULL, 0, -1)

	CWTLIPictureView()
	{
		m_border = true;
		m_scale = 100;
		m_size.cx = m_size.cy = 300;
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
		COMMAND_ID_HANDLER(ID_IMAGE_SERIALIZATION_STORE_MEM, OnImageSerializationStoreMem)
		COMMAND_ID_HANDLER(ID_IMAGE_SERIALIZATION_LOAD_MEM, OnImageSerializationLoadMem)
		COMMAND_ID_HANDLER(ID_IMAGE_SERIALIZATION_STORE_TO_FILE, OnImageSerializationStoreFile)
		COMMAND_ID_HANDLER(ID_IMAGE_SERIALIZATION_LOAD_FROM_FILE, OnImageSerializationLoadFromFile)
	END_MSG_MAP()

	void PasteImage(UINT uFormat)
	{
		try
		{
			_pix.Clear();
			_pix.Create(uFormat);

			int iLength = _pix.SaveToMemory(TRUE);
			_clipObject.CopyBuffer(_pix.GetBuffer(), iLength);

			size_t length;
			const char *pBuffer = (const char *)_pix.GetBuffer();

			for ( int i = 0; i < 10; i++)
			{
				ATLTRACE(_T("#JDsbase64# encodeing decoding loop N#%d\n"), i);
				ATLTRACE(_T("#JDsbase64# %d kbytes buffer length before decoding\n"), iLength/1024);
				std::string encoded = CJDsBase64::base64_encode(reinterpret_cast<const unsigned char*>(pBuffer), iLength);
				ATLTRACE(_T("#JDsbase64# %d kbytes encoded\n"), encoded.length()/1024);
				std::string decoded = CJDsBase64::base64_decode(encoded);
				ATLTRACE(_T("#JDsbase64# %d kbytes decoded\n"), decoded.length()/1024);
			}

			ATLTRACE(_T("#JDsbase64# %d 1000 type encoding decoding done"));
			int i = 5;

		}
		catch(CJDsClipboardException ex)
		{
			ATLTRACE(_T("#JDsClipboard# exeption %s\n"), ex.GetErrorDesc().GetString());
		}
		catch(...)
		{
			ATLTRACE(_T("#JDsClipboard# unknown exeption\n"));	
		}

		if (!_pix.IsNull())
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


		if(!_pix.IsNull())
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
		if(!_pix.IsNull())
		{
			// draw the picture
			RECT rc = { 0, 0, m_size.cx, m_size.cy };
			_pix.Render(dc, rc);
		}
	}


	void SetSize(int nScale = 100)
	{
		if (!_pix.IsNull())
			_pix.GetSizeInPixels(m_size);
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
		_pix.Clear();
		_clipObject.Reset();

		Invalidate();
		UpdateWindow();
	}

	void ShowClipObject()
	{
		try
		{
			_pix.CreateFromBitmapBuffer(_clipObject.GetBuffer(), _clipObject.GetLength());
		}
		catch(CJDsClipboardException ex) 
		{
			ATLTRACE(_T("#JDsClipboard# exeption %s\n"), ex.GetErrorDesc().GetString());
		}

		if (!_pix.IsNull())
		{
			SetSize(100);
		}

		Invalidate();
		UpdateWindow();

	}

	BOOL Paste()
	{
		try
		{
		    if ( OpenClipboard() == FALSE )
			  return FALSE;

			// to call CloseClipboard when leaving the stack
			AutoClipboard hClipboard;

			if ( ::IsClipboardFormatAvailable(CF_BITMAP) )
			{
				PasteImage(CF_BITMAP);
			}

			return TRUE;
		}
		catch(CJDsClipboardException ex) 
		{
			ATLTRACE(_T("#JDsClipboard# exeption %s\n"), ex.GetErrorDesc().GetString());
		}
		catch(...)
		{
			ATLTRACE(_T("#JDsClipboard# Unknown execption when opening the image file\n"));
		}

		return FALSE;
	}

	BOOL OpenTheFile(LPTSTR szFilePath)
	{
		if (szFilePath == NULL) 
			return FALSE;

		try
		{
			_pix.LoadFromFile(szFilePath);
		}
		catch(CJDsClipboardException ex) 
		{
			ATLTRACE(_T("#JDsClipboard# exeption %s\n"), ex.GetErrorDesc().GetString());
		}
		catch(...)
		{
			ATLTRACE(_T("#JDsClipboard# Unknown execption when opening the image file\n"));
		}


		int iLength = _pix.SaveToMemory(TRUE);
		_clipObject.CopyBuffer(_pix.GetBuffer(), iLength);

		if (!_pix.IsNull())
		{
			SetSize(100);
		}

		Invalidate();
		UpdateWindow();

		return FALSE;
	}


	LRESULT OnViewProperties(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		return 0;
	}

	LRESULT OnImageSerializationStoreMem(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
	{
		try
		{
			int count = _clipObject.Serialize(&_down_container);
			_down_container.SerializeAllTo(&_upContainer);
		}
		catch(...)
		{
			AtlMessageBox(NULL, _T("Serialization error"));
		}


		bHandled = TRUE;

		return 0;
	}


	LRESULT OnImageSerializationLoadMem(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
	{
		try 
		{
			_clipObject.UnSerialize(&_upContainer);

			_pix.LoadFromMemoryBuffer( _clipObject.GetBuffer() );

			_upContainer.Clear();

			ShowClipObject();

			bHandled = TRUE;
		}
		catch(CJDsClipboardException& Ex)
		{
			AtlMessageBox(NULL, Ex.GetErrorDesc().GetString());
		}
		catch(...)
		{
			AtlMessageBox(NULL, _T("Unknown error"));
		}

		return 0;
	}

	void SaveToFile(LPCTSTR szFilePath, BOOL fSaveMemCopy)
	{
		_pix.SaveToFile(szFilePath, fSaveMemCopy);
	}

	LRESULT OnImageSerializationStoreFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
	{
		try
		{
			int count = _clipObject.Serialize(&_down_container);
			_down_container.SerializeAllTo(&_upContainer);

			//CFileDialogFilter strFilter( _T("Jetro clipboard image serializaton files (*.jdsi)|All Files (*.*)|*.*||") );
			LPCTSTR strFilter = _T("Jetro clipboard image serializaton files (*.jdsi)\0*.jdsi\0");

			CFileDialog dlg(FALSE, _T("jdsi"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);

			if ( IDOK == dlg.DoModal(*this) )
			{
				SaveToFile(dlg.m_ofn.lpstrFile);

				bHandled = true;
			}
		}
		catch(CJDsClipboardException ex)
		{
			ATLTRACE(_T("#JDsClipboard# exeption %s\n"), ex.GetErrorDesc().GetString());
		}
		catch(...)
		{
			ATLTRACE(_T("#JDsClipboard# unknown exeption\n"));	
		}

		return 0;
	}

	LRESULT OnImageSerializationLoadFromFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
	{
		LPCTSTR lpcstrFileFilters = _T("Jetro clipboard image serializaton files (*.jdsi)\0*.jdsi\0");
		CFileDialog dlg(TRUE, _T("jdsi"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, lpcstrFileFilters);

		if(dlg.DoModal() == IDOK)
		{
			try
			{
				LoadSerializedDataFromFile(dlg.m_ofn.lpstrFile);

				_clipObject.UnSerialize(&_upContainer);

				_pix.LoadFromMemoryBuffer( _clipObject.GetBuffer() );

				ShowClipObject();

				bHandled = TRUE;
			}
			catch(CJDsClipboardException& Ex)
			{
				AtlMessageBox(NULL, Ex.GetErrorDesc().GetString());
			}
			catch(...)
			{
				AtlMessageBox(NULL, _T("Unknown error"));
			}

		}
		return 0;
	}

	BOOL SaveToFile(const CString &sFile)
	{
		UniqueHandle_File hFile(::CreateFile(sFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL));

		DWORD dwBytesToWrite = _upContainer.GetLength();
		DWORD dwBytesWritten;

		if( hFile.get() != INVALID_HANDLE_VALUE)
		{
			if ( ::WriteFile(hFile.get(), _upContainer.Get(), dwBytesToWrite, &dwBytesWritten, FALSE) == FALSE )
			{
				return FALSE;
			}
		}

		return TRUE;
	}

	BOOL LoadSerializedDataFromFile(const CString &sFilePath)
	{
		UniqueHandle_File hFile(::CreateFile(sFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL));

		if( hFile.get() != INVALID_HANDLE_VALUE)
		{
			// get file size
			DWORD dwFileSize = ::GetFileSize(hFile.get(), NULL);

			if ( dwFileSize <= 0 )
				return FALSE;

			CComHeapPtr<BYTE> pBuffer;
			pBuffer.Allocate(dwFileSize);

			DWORD dwBytesRead;

			// read file and store its data in buffer
			if (!SUCCEEDED(::ReadFile(hFile.get(), pBuffer, dwFileSize, &dwBytesRead, NULL)))
				return FALSE;

			_upContainer.Set(pBuffer, dwFileSize);
		}

		return TRUE;
	}

	BOOL IsDataEmpty() 
	{
		return _pix.IsNull();
	}

	BOOL IsSerializedDataEmpty()
	{
		return ( _upContainer.GetLength() == 0 );
	}

	BOOL Copy(HWND hWnd)
	{
		try
		{
			_pix.Copy(hWnd);
		}
		catch(CJDsClipboardException& Ex)
		{
			AtlMessageBox(NULL, Ex.GetErrorDesc().GetString());
			return FALSE;
		}
		catch(...)
		{
			AtlMessageBox(NULL, _T("Unknown error"));
			return FALSE;
		}

		return TRUE;
	}

	BOOL Cut(HWND hWnd)
	{
		try
		{
			_pix.Copy(hWnd);
			Clear();
		}
		catch(CJDsClipboardException& Ex)
		{
			AtlMessageBox(NULL, Ex.GetErrorDesc().GetString());
			return FALSE;
		}
		catch(...)
		{
			AtlMessageBox(NULL, _T("Unknown error"));
			return FALSE;
		}

		return TRUE;
	}

};

#endif // __CWTLIPICTUREVIEW_H__