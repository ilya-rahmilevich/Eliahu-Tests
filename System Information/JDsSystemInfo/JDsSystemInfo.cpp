// This is the main DLL file.

#include "stdafx.h"

#include "JDsSystemInfo.h"
#include <vector>
using namespace std;

using namespace JDsSystemInfo;

//
// Test Rollback -04-02-2014
//
bool CJDsSystemInfo::GetUserFreeDiskSpace(__int64 &bytes)
{
	__int64 freeSpace;
	__int64 totalSpace;
	__int64 totalFreeSpace;

	// first parameter is NULL, the function uses the root of the current disk.
	BOOL bRet = ::GetDiskFreeSpaceEx( NULL, (PULARGE_INTEGER)&freeSpace, (PULARGE_INTEGER)&totalSpace, (PULARGE_INTEGER)&totalFreeSpace );
	if ( !bRet )
		return FALSE;

	bytes = freeSpace;

	return TRUE;
}

bool CJDsSystemInfo::GetTotalFreeDiskSpace(__int64 &bytes)
{
	__int64 freeSpace;
	__int64 totalSpace;
	__int64 totalFreeSpace;

	// first parameter is NULL, the function uses the root of the current disk.
	BOOL bRet = ::GetDiskFreeSpaceEx( NULL, (PULARGE_INTEGER)&freeSpace, (PULARGE_INTEGER)&totalSpace, (PULARGE_INTEGER)&totalFreeSpace );
	if ( !bRet )
		return FALSE;

	bytes = totalFreeSpace;

	return TRUE;
}

/*
  Produces a readable string like "386.200.200.200" to detect between Gigabytes, 
  Megabytes Kilobytes and Bytes
*/
void CJDsSystemInfo::ConvertDiskSpaceToString( __int64 Number, CString &sNumber)
{
	vector<int> arrReminders;
	__int64 iCurrent = Number;
	__int64 iTemp;


	int iRem;
	for (int i = 0; iCurrent != 0; i++ ) 
	{
		iTemp = iCurrent/1000;
		iRem = iCurrent%1000;

		if ( iTemp != 0)
			iRem = iCurrent%1000;
		else
			iRem = iCurrent;

		iCurrent = iTemp;
			  
		arrReminders.push_back(iRem);
	}

	int iSize = arrReminders.size();
	CString sTemp;
	TCHAR buff[50];
	for (int i = iSize-1; i >= 0; i-- ) 
	{
		sTemp = _itow(arrReminders[i], buff, 10);
		// we need bytes between dots to be like 225.005.777 not 225.5.777
		while( sTemp.GetLength() < 3 )
			sTemp.Insert(0, _T('0'));

		sNumber += sTemp;


		// don't put dot at the end
		if ( i != 0 )
			sNumber += CString(_T("."));
	}
}



/* 
	to loop through all volumes, try to use FindNextVolume
	and other Windows volume methods
*/