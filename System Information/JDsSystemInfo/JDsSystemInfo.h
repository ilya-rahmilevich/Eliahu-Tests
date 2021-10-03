// JDsSystemInfo.h

#pragma once

//using namespace System;

namespace JDsSystemInfo {

	class CJDsSystemInfo
	{
	public:
		static bool GetTotalFreeDiskSpace(__int64 &bytes);
		static bool GetUserFreeDiskSpace(__int64 &bytes);
		static void ConvertDiskSpaceToString( __int64 Number, CString &sNumber);
	};

	class CJDsDefragInfo
	{
	public:
		static void GetDefragmentInfo(TCHAR *szBuff);
	};
}
