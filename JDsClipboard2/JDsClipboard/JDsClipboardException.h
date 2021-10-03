#pragma once
// Exception.h: implementation of the exception classes.
//
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Class CXException - root class for all exception classes
//-----------------------------------------------------------------------------
class CXException
{
public:
	virtual long GetErrorNumber() = 0;
	virtual CString GetErrorDesc() = 0;

protected:
	long m_lErrorNo;		// error number
	CString m_strError;		// error description
};

class CJDsClipboardException : public CXException
{
public:
	CJDsClipboardException(int nCause = CJDsClipboardException::none, 
		CString strArchiveName = _T(""))
	{
		m_lErrorNo = nCause;
		m_strError = ErrNumberToDescription(nCause);
	}

	~CJDsClipboardException() {}

	virtual long GetErrorNumber()
	{
		return m_lErrorNo;
	}

	virtual CString GetErrorDesc()
	{
		return m_strError;
	}

	enum {
		none,
		enGeneric,
		// Seriazalation exceptions
		enSerAbnormalLength,
		enSerIncreaseCurrFailed,
		enSerEmptyBuffer,
		// Clipboard listener exceptions
		enOpenClipboardFailed,
		// picture helper execptions
		enPicHelperEmptyBuffer,
		enPicHelperLoadingFailed,
		enPicHelperNullParam,
		enPicHelperReadFromSeqStreamFailed,
		enPicHelperLoadPictureFromDispatchFailed,
		enPicHelperCreatingPictureIndirectFailed,
		enPicHelperOleStreamCreationFailed,
		enPicHelperEmptyPath,
		enPicHelperLoadPictureFromPathFailed,
		enPicHelperWritingFileFailed,
		enPicHelperSavePictureFileFailed,
		enPicHelperNullParameter,
		enSerFileWritingFailed,
		enSerFileReadingFailed,
		enPicHelperCopyFailed,
		// hash code creator exceptions
		enHashErrorGet,
		enHashErrorData,
		enHashErrorCreate,
		enHashAcquireContext
	};

private:
	// Maps error number to its textual description
	CString ErrNumberToDescription(int lErr)
	{
		CString strError;

		switch (lErr)
		{
		case none:
			strError =  _T("None");
			break;
		case enGeneric:
			strError =  _T("Generic serialization exception");
			break;
		case enSerAbnormalLength:
			strError =  _T("Length error while Unserializing");
			break;
		case enSerIncreaseCurrFailed:
			strError =  _T("Error while increasing current length shile Unserializing");
			break;
		case enSerEmptyBuffer:
			strError =  _T("Error while trying to Serialize empty buffer");
			break;
		case enOpenClipboardFailed:
			strError =  _T("Error while trying to open Clipboard");
			break;
		case enPicHelperEmptyBuffer:
			strError =  _T("Error while trying to manage empty buffer");
			break;
		case enPicHelperLoadingFailed:
			strError =  _T("Error to create IPicture interface by ::OleLoadPicture");
			break;
		case enPicHelperNullParam:
			strError =  _T("Null parameters in picture helper method");
			break;
		case enPicHelperReadFromSeqStreamFailed:
			strError =  _T("Error to read from Sequential Stream");
			break;
		case enPicHelperCreatingPictureIndirectFailed:
			strError =  _T("Error to create IPicture by calling OleCreatePictureIndirect");
			break;
		case enPicHelperOleStreamCreationFailed:
			strError =  _T("Error to create CreateStreamOnHGlobal for buffer");
			break;
		case enPicHelperEmptyPath:
			strError =  _T("Trying to use empty path");
			break;
		case enPicHelperLoadPictureFromDispatchFailed:
			strError =  _T("Loading Picture by OleLoadPictureFile failed");
			break;
		case enPicHelperLoadPictureFromPathFailed:
			strError =  _T("Loading Picture by OleLoadPicturePath failed");
			break;
		case enPicHelperWritingFileFailed:
			strError =  _T("Picture Helper Writing to file failed");
			break;
		case enPicHelperSavePictureFileFailed:
			strError =  _T("OleSavePictureFile failed");
			break;
		case enPicHelperNullParameter:
			strError =  _T("Picture Helper - Null pointer parameter");
			break;
		case enSerFileReadingFailed:
			strError =  _T("Reading from Clipboard serialization file failed");
			break;
		case enSerFileWritingFailed:
			strError =  _T("Writing Clipboard serialization file failed");
			break;
		case enPicHelperCopyFailed:
			strError =  _T("Copying Clipboard image data failed");
			break;
		case enHashErrorGet:
			strError =  _T("Error getting hash param");
			break;
		case enHashErrorData:
			strError =  _T("Error hashing data");
			break;
		case enHashErrorCreate:
			strError =  _T("Error creating hash");
			break;
		case enHashAcquireContext:
			strError =  _T("Error acquiring hash context");
			break;
		default:
			strError =  _T("Unknown archive exception");
			break;
		}

		return strError;
	}
};
