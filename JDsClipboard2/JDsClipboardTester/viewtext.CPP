#include "StdAfx.h"
#include "viewText.h"
#include "JDsClipboardException.h"
#include "AutoResource.h"

LRESULT CTesterTextView::OnSerializationStoreToFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
{
	try
	{
		int count = _clipObject.Serialize(&_down_container);
		_down_container.SerializeAllTo(&_upContainer);

		LPCTSTR lpcstrFileFilters = _T("Jetro clipboard text serializaton files (*.jdst)\0*.jdst\0");
		CFileDialog dlg(FALSE, _T("jdst"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, lpcstrFileFilters);

		if ( IDOK == dlg.DoModal(*this) )
		{
			int count = _clipObject.Serialize(&_down_container);
			_down_container.SerializeAllTo(&_upContainer);

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


LRESULT CTesterTextView::OnSerializationLoadFromFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
{
	LPCTSTR lpcstrFileFilters = _T("Jetro clipboard text serializaton files (*.jdst)\0*.jdst\0");
	CFileDialog dlg(TRUE, _T("jdst"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, lpcstrFileFilters);

	if(dlg.DoModal() == IDOK)
	{
		try
		{
			LoadFromFile(dlg.m_ofn.lpstrFile);

			_clipObject.UnSerialize(&_upContainer);

			m_sText = _clipObject.GetText();

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

	}	return 0;
}


LRESULT CTesterTextView::OnSerializationStore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
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

LRESULT CTesterTextView::OnSerializationLoad(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
{
	_clipObject.Reset();

	_clipObject.UnSerialize(&_upContainer);

	_upContainer.Clear();

	ShowClipObject();

	bHandled = TRUE;

	return 0;
}

BOOL CTesterTextView::SaveToFile(const CString &sFile)
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

BOOL CTesterTextView::LoadFromFile(const CString &sFilePath)
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

