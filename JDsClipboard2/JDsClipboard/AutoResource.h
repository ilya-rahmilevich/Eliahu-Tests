#pragma once
#include <wincrypt.h>	// Cryptographic API Prototypes and Definitions

struct JDs_handle_file
{
    void operator()(void* handle)
    {
        if(handle != nullptr)
            ::CloseHandle(handle);
    }
};
typedef std::unique_ptr<void, JDs_handle_file> UniqueHandle_File;


struct JDs_handle_Crypt
{
    void operator()(void* handle)
    {
        if(handle != nullptr)
            ::CryptReleaseContext((HCRYPTPROV)handle, 0);
    }
};
typedef std::unique_ptr<void, JDs_handle_Crypt> UniqueHandle_Crypt;

struct JDs_handle_Hash
{
    void operator()(void* handle)
    {
        if(handle != nullptr)
            ::CryptDestroyHash((HCRYPTHASH)handle);
    }
};
typedef std::unique_ptr<void, JDs_handle_Hash> UniqueHandle_Hash;



class AutoClipboard
{
public:

	~AutoClipboard()
	{
		::CloseClipboard();
	}
};
