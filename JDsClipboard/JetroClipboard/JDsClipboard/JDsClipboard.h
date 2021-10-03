// JDsClipboard.h

#pragma once

#include "Serialization\JDsClipboardStringObject.h"
#include "Serialization\JDsClipboardImageObject.h"
#include "JDsClipboardPicture.h"

#ifdef UNICODE
	#define CF_TCHAR CF_UNICODETEXT
#else
	#define CF_TCHAR CF_TEXT
#endif

#define NBPRIORITY   26                 // nbr format codes priority

namespace JDsClipboard {

	template<typename T> 
	class CJDSClipboardListener
	{
	protected:
			CJDsClipboardPicture m_clipPictureHelper;
			CJDsClipboardObject *_clipObject;
	public:
		HWND m_hWndNextCBViewer;
		bool m_bRegistered;
        // clipboard formats
		UINT        m_uFormat[NBPRIORITY];  // priority format table
		UINT        m_cbFormats[32];        // available formats
		int         m_cbFormatsCnt;         // available formats count
		DWORD       m_cbDataAvailable;      // available data formats flags
		UINT        m_oldFormat;            // last format request

		BEGIN_MSG_MAP(CJDSClipboardListener< T >)
			ALT_MSG_MAP(1)
			MESSAGE_HANDLER(WM_CREATE, OnCreate)       // for CWindowImpl based windows
			MESSAGE_HANDLER(WM_INITDIALOG, OnCreate)   // for CDialogImpl based windows
			MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
			// Notification handlers
			MESSAGE_HANDLER(WM_DRAWCLIPBOARD, OnDrawClipboard)
			MESSAGE_HANDLER(WM_CHANGECBCHAIN, OnChangeCbChain) // necessary indeed
		END_MSG_MAP()
        
		CJDSClipboardListener() : m_hWndNextCBViewer(0), m_bRegistered(false) 
		{
			   const static UINT auPriorityList[NBPRIORITY] = { 0,// reserved for the last used
							CF_TEXT,			CF_BITMAP,			CF_METAFILEPICT,	CF_SYLK,             
							CF_DIF,				CF_TIFF,			CF_OEMTEXT,			CF_DIB, 
							CF_PALETTE,			CF_PENDATA,			CF_RIFF,			CF_WAVE,
							CF_UNICODETEXT,		CF_ENHMETAFILE,		CF_HDROP,			CF_LOCALE,           
							CF_OWNERDISPLAY,	CF_DSPTEXT,			CF_DSPBITMAP,        
							CF_DSPMETAFILEPICT,	CF_DSPENHMETAFILE,	CF_PRIVATEFIRST,	CF_PRIVATELAST,      
							CF_GDIOBJFIRST,		CF_GDIOBJLAST};

				// CF_DIBV5 - Used in Windows 2000 only, this is also a handle to a BITMAPINFO structure where the subsequent bits represent the bitmap color information and the bitmap image.

			   for (int i = 0; i < NBPRIORITY; i++) 
				   m_uFormat[i] = auPriorityList[i];

			   _clipObject = NULL;
		}

		~CJDSClipboardListener()
		{
			if ( _clipObject != NULL )
				delete _clipObject;
		}

		LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			T* pT = static_cast<T*>(this);

			// Join the members of clipboard viewer chain
			m_hWndNextCBViewer = pT->SetClipboardViewer(); //can be nothing
			m_bRegistered = true;
			return 0;
		}
    
		LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			T* pT = static_cast<T*>(this);

			/*
			Before closing, a clipboard viewer window must remove itself from the clipboard viewer chain by calling the ChangeClipboardChain function.
			The system then sends a WM_CHANGECBCHAIN message to the first window in the chain.
			*/
			if (IsWindow(m_hWndNextCBViewer))
				pT->ChangeClipboardChain(m_hWndNextCBViewer);

			bHandled = FALSE;

			return 0;
		}
    
		UINT GetClipboardFormat()
		{
			  m_uFormat[0] = ::GetPriorityClipboardFormat(m_uFormat, NBPRIORITY);

			  return m_uFormat[0];
		}

		LRESULT OnDrawClipboard(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
		{
			// Viewer receives whenever the content of the clipboard is changed
			// wParam is a handle to the window that has put clipboard data
   
        
			// Registering this window as a CB wiewer window
			if (m_bRegistered == false) // calling ::SetClipboardViewer() in OnCreate handler
			{
				ATLTRACE("Registering... wParam: %#x\n", wParam);
				return 0; // do nothing but return gracefully
			}

			ATLTRACE("OnDrawClipboard(): Wnd %#x put CB data\n", wParam);

			m_uFormat[0] = ::GetPriorityClipboardFormat(m_uFormat, NBPRIORITY);
			if ( CreateClipboardObject() == FALSE )
				return 1;

			T* pT = static_cast<T*>(this);
			pT->ClipBoardEventReceived(m_uFormat[0]);

			ManageClipboardData();

			// Pass WM_DRAWCLIPBOARD to the next window in clipboard viewer chain
			RouteMessage(uMsg, wParam, lParam);
			return 0;
		}
    
		BOOL Paste()
		{
			m_uFormat[0] = ::GetPriorityClipboardFormat(m_uFormat, NBPRIORITY);
			if ( CreateClipboardObject() == FALSE )
				return 1;

			ManageClipboardData();

			return TRUE;
		}

		BOOL Copy(HWND hWnd)
		{
			if ( _clipObject == NULL )
				return FALSE;

			m_clipPictureHelper.Copy( *((CJDsClipboardImageObject *)_clipObject), hWnd);

			return TRUE;
		}




		LRESULT OnChangeCbChain(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
		{
			// Clipboard wiewer window is supposed to respond to WM_CHANGECBCHAIN.

			HWND hWndRemove = reinterpret_cast<HWND>(wParam);
			HWND hWndAfter = reinterpret_cast<HWND>(lParam);
        
			if (m_hWndNextCBViewer == hWndRemove)
			{
				m_hWndNextCBViewer = hWndAfter;
			}
        
			// pass the WM_CHANGECBCHAIN (0x030D)
			RouteMessage(WM_CHANGECBCHAIN, wParam, lParam);
			return 0;
		}

		void RouteMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			// Toss WM_CHANGECBCHAIN message to the next Clipboard wiewer in the viewer chain
			// to notify of the update of chain.
			if (m_hWndNextCBViewer)
				::SendMessage(m_hWndNextCBViewer, uMsg, wParam, lParam);
		}


		BOOL ManageClipboardData()
		{
			T* pT = static_cast<T*>(this);

			pT->Clear();

			switch( m_uFormat[0] )
			{
				case CF_TEXT:
				case CF_OEMTEXT:
				case CF_UNICODETEXT:
					ManageText();
				break;
				case CF_BITMAP:
				case CF_DIB:
				case CF_METAFILEPICT:
				case CF_ENHMETAFILE:
					ManageImage(m_uFormat[0]);
				break;
			case CF_HDROP :
				ManageDropFiles();
            break;
			}

			return true;
		}


		BOOL ManageText()
		{
			T* pT = static_cast<T*>(this);

			if ( pT->OpenClipboard() )
			{
				CString text;
				HANDLE hData = ::GetClipboardData( CF_TCHAR );
				text = (PTSTR)::GlobalLock( hData );
				::GlobalUnlock( hData );
				CloseClipboard();

				((CJDsClipboardStringObject *)_clipObject)->SetText(text);
				pT->ManageClipboardText(text);
			}

			return TRUE;
		}

		BOOL ManageImage(UINT uFormat)
		{
			if ( _clipObject == NULL )
				return FALSE;

			T* pT = static_cast<T*>(this);

			IPicture *pPicture = m_clipPictureHelper.CreateImageObject(*((CJDsClipboardImageObject *)_clipObject), uFormat);

			if ( pPicture )
				pT->PasteImage( pPicture );

			return TRUE;

		}


		BOOL CreateClipboardObject()
		{
			if ( _clipObject != NULL )
				delete _clipObject;

			switch( m_uFormat[0] )
			{
				case CF_TEXT:
				case CF_OEMTEXT:
				case CF_UNICODETEXT:
				// eli - 19.07.2012 - currently managed as text
				case CF_HDROP:
					_clipObject = new CJDsClipboardStringObject(m_uFormat[0]);
				break;
				case CF_BITMAP:
				case CF_DIB:
				case CF_METAFILEPICT:
				case CF_ENHMETAFILE:
					_clipObject = new CJDsClipboardImageObject(m_uFormat[0]);
				break;
			}

			return TRUE;
		}

		BOOL StoreObject(LPCTSTR szFilePath, BOOL fSaveMemCopy = FALSE)
		{
			if ( _clipObject == NULL )
				return FALSE;

			try
			{
				CXFile file1;
				file1.Open(szFilePath,								// file name
							GENERIC_WRITE | GENERIC_READ,			// access mode 
							FILE_SHARE_READ | FILE_SHARE_WRITE,		// share mode 
							NULL,									// no security 
							CREATE_ALWAYS,							// create a new file, overwrite if it exists
							FILE_ATTRIBUTE_NORMAL,					// file attributes
							NULL);									// no template file

				CXArchive ar1(&file1, CXArchive::store);

				_clipObject->Serialize(ar1, TRUE);

				ar1.Close();
			}
			catch (CXException& Ex)
			{
				AtlMessageBox(NULL, Ex.GetErrorDesc().c_str(), _T("JDsArchive"), MB_OK);
				/*
				Or you can do it this way:
				if (Ex.GetErrorNumber() == ERROR_FILE_NOT_FOUND)
					return -1;
				if (Ex.GetErrorNumber() == CXArchiveException::readOnly)
					return -2;
				*/
			}
			catch (...)
			{
				AtlMessageBox(NULL, _T("Unexpected error"), _T("JDsArchive"), MB_OK);
			}

		}

		BOOL LoadObject(LPCTSTR szFilePath, BOOL fSaveMemCopy = FALSE)
		{
			
			if ( PreloadObjectToGetType(szFilePath, fSaveMemCopy) == FALSE )
				return FALSE;

			if ( CreateClipboardObject() == FALSE )
				return FALSE;

			try
			{
				CXFile file1;
				file1.Open(szFilePath,								// file name
							GENERIC_WRITE | GENERIC_READ,			// access mode 
							FILE_SHARE_READ | FILE_SHARE_WRITE,		// share mode 
							NULL,									// no security 
							OPEN_EXISTING,							// create a new file, overwrite if it exists
							FILE_ATTRIBUTE_NORMAL,					// file attributes
							NULL);									// no template file

				CXArchive ar1(&file1, CXArchive::load);

				_clipObject->Serialize(ar1, TRUE);

				ar1.Close();

				T* pT = static_cast<T*>(this);

				pT->ShowClipObject();
			}
			catch (CXException& Ex)
			{
				AtlMessageBox(NULL, Ex.GetErrorDesc().c_str(), _T("JDsArchive"), MB_OK);
				/*
				Or you can do it this way:
				if (Ex.GetErrorNumber() == ERROR_FILE_NOT_FOUND)
					return -1;
				if (Ex.GetErrorNumber() == CXArchiveException::readOnly)
					return -2;
				*/
			}
			catch (...)
			{
				AtlMessageBox(NULL, _T("Unexpected error"), _T("JDsArchive"), MB_OK);
			}

		}

		BOOL PreloadObjectToGetType(LPCTSTR szFilePath, BOOL fSaveMemCopy = FALSE)
		{
			_clipObject = new CJDsClipboardObject();

			try
			{
				CXFile file1;
				file1.Open(szFilePath,								// file name
							GENERIC_WRITE | GENERIC_READ,			// access mode 
							FILE_SHARE_READ | FILE_SHARE_WRITE,		// share mode 
							NULL,									// no security 
							OPEN_EXISTING,							// create a new file, overwrite if it exists
							FILE_ATTRIBUTE_NORMAL,					// file attributes
							NULL);									// no template file

				CXArchive ar1(&file1, CXArchive::load);

				_clipObject->Serialize(ar1, TRUE);
				ar1.Close();

				m_uFormat[0] = _clipObject->GetType();

				delete _clipObject;
				_clipObject = NULL;

				return TRUE;
			}
			catch (CXException& Ex)
			{
				AtlMessageBox(NULL, Ex.GetErrorDesc().c_str(), _T("JDsArchive"), MB_OK);
				/*
				Or you can do it this way:
				if (Ex.GetErrorNumber() == ERROR_FILE_NOT_FOUND)
					return -1;
				if (Ex.GetErrorNumber() == CXArchiveException::readOnly)
					return -2;
				*/
			}
			catch (...)
			{
				AtlMessageBox(NULL, _T("Unexpected error"), _T("JDsArchive"), MB_OK);
			}

			return FALSE;
		}

		void GetClipboardFormatName(UINT format, CString &sName)
		{
		   TCHAR szFormatName[80];

		   if (format == 0) format = m_uFormat[0];

		   switch (format) 
		   {
			   case 0 :                sName =  _T("empty");			break;
			   case CF_TEXT :          sName = _T("CF_TEXT");			break;
			   case CF_BITMAP :        sName = _T("CF_BITMAP");			break;
			   case CF_METAFILEPICT :  sName = _T("CF_METAFILEPICT");	break;	
			   case CF_SYLK :          sName = _T("CF_SYLK");			break;
			   case CF_DIF :           sName = _T("CF_DIF");			break;
			   case CF_TIFF :          sName = _T("CF_TIFF");			break;
			   case CF_OEMTEXT :       sName = _T("CF_OEMTEXT");		break;
			   case CF_DIB :           sName = _T("CF_DIB");			break;
			   case CF_PALETTE :       sName = _T("CF_PALETTE");		break;
			   case CF_PENDATA :       sName = _T("CF_PENDATA");		break;
			   case CF_RIFF :          sName = _T("CF_RIFF");			break;
			   case CF_WAVE :          sName = _T("CF_WAVE");			break;
			   case CF_UNICODETEXT :   sName = _T("CF_UNICODETEXT");	break;
			   case CF_ENHMETAFILE :   sName = _T("CF_ENHMETAFILE");	break;
			   case CF_HDROP :         sName = _T("CF_HDROP");			break;
			   case CF_LOCALE :        sName = _T("CF_LOCALE");			break;
			   case CF_MAX :           sName = _T("CF_MAX");			break;
																		
			   case CF_OWNERDISPLAY :  sName = _T("CF_OWNERDISPLAY");	break;
			   case CF_DSPTEXT :       sName = _T("CF_DSPTEXT");		break;	
			   case CF_DSPBITMAP :     sName = _T("CF_DSPBITMAP");		break;
			   case CF_DSPMETAFILEPICT:sName = _T("CF_DSPMETAFILEPICT");break;
			   case CF_DSPENHMETAFILE :sName = _T("CF_DSPENHMETAFILE");	break;

			   default:
				  if (CF_PRIVATEFIRST == (format & 0xFFFFFF00)) 
					 sName.Format(_T("CF_PRIVATE_%02X"), format);
				  else if (CF_GDIOBJFIRST == (format & 0xFFFFFF00)) 
					 sName.Format(_T("CF_GDIOBJ_%02X"), format);
				  else if (::GetClipboardFormatName(format, szFormatName,
														  sizeof(szFormatName))) 
				  {
					 sName = szFormatName;
				  }
				  else
					  sName.Format(_T("unknown_%08x"), format);

				  break;
		   }
		}

		void ManageDropFiles()
		{
		   T* pT = static_cast<T*>(this);
		   if ( pT->OpenClipboard() == FALSE )
			   return;

		   HANDLE hGlobal = ::GetClipboardData( CF_HDROP );

		   DROPFILES* pDf = (DROPFILES*)GlobalLock(hGlobal);
		   CString buffer;
		   TCHAR c;

		   if (pDf->fWide) 
		   {
			  // The file contains Unicode characters
			  PWORD pW = PWORD(PBYTE(pDf) + pDf->pFiles);

			  buffer = _T("CF_HDROP Unicode characters :");
			  // Extract a file names
			  while (*pW) {
				 buffer += _T("\r\n ");
				 while (c = (TCHAR)*pW++) buffer += c;
			  }
		   } 
		   else 
		   {
			  // The file contains ANSI characters
			  PBYTE pB = PBYTE(PBYTE(pDf) + pDf->pFiles);

			  buffer = "CF_HDROP ANSI characters :";
			  // Extract a file names
			  while (*pB)
			  {
				 buffer += "\r\n ";
				 while (c = (TCHAR)*pB++) buffer += c;
			  }
		   }

			((CJDsClipboardStringObject *)_clipObject)->SetText(buffer);
			pT->ManageClipboardText(buffer);


		   ::GlobalUnlock(hGlobal);
			CloseClipboard();
		}

	};
}

