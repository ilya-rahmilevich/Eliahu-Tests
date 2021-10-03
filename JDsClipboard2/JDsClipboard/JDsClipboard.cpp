// This is the main DLL file.

#include "stdafx.h"

#include "JDsClipboard.h"


void CreateMD5HashCode(BYTE* pbContent, int iLength, CString &csDigest)
{
	HCRYPTPROV hCryptProv = NULL; 

	HCRYPTHASH hHash; 
	BYTE bHash[0x7f]; 
	DWORD dwHashLen= 16; // The MD5 algorithm always returns 16 bytes. 

	if(CryptAcquireContext(&hCryptProv, 
		NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET)) 
	{
		UniqueHandle_Crypt autoCrypt((void *)hCryptProv);

		if(CryptCreateHash(hCryptProv, 
			CALG_MD5,	// algorithm identifier definitions see: wincrypt.h
			0, 0, &hHash)) 
		{
			if(CryptHashData(hHash, pbContent, iLength, 0))
			{
				UniqueHandle_Hash autoHash((void *)hHash);

				if(CryptGetHashParam(hHash, HP_HASHVAL, bHash, &dwHashLen, 0)) 
				{
					// Make a string version of the numeric digest value
					csDigest.Empty();
					CString tmp;

					for (int i = 0; i<16; i++)
					{
						tmp.Format(_T("%02x"), bHash[i]);
						csDigest+=tmp;
					}
				}
				else
				{
					throw CJDsClipboardException (CJDsClipboardException::enHashErrorGet);
				}
			}
			else
			{
				throw CJDsClipboardException (CJDsClipboardException::enHashErrorData);
			}
		}
		else
		{
			throw CJDsClipboardException (CJDsClipboardException::enHashErrorCreate);
		}
	}
	else
	{
		throw CJDsClipboardException (CJDsClipboardException::enHashAcquireContext);
	}

}
