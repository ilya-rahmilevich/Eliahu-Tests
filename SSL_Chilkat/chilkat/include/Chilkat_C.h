
#ifndef _CHILKAT_C
#define _CHILKAT_C

#ifndef BOOL_IS_TYPEDEF
#ifndef BOOL
#define BOOL int
#endif
#endif
	
#ifndef TRUE
#define TRUE 1
#endif
	
#ifndef FALSE
#define FALSE 0
#endif	
	
#if !defined(WIN32) && !defined(WINCE)
#include "SystemTime.h"              
#include "FileTime.h"                
#endif                  

#include "int64.h"
	
// Use typedefs so we can explicitly see the kind of object pointed
// to by "void *"
	
typedef void *HCkByteData;
typedef void *HCkString;
typedef void *HCkCert;
typedef void *HCkEmail;
typedef void *HCkEmailBundle;
typedef void *HCkMailMan;
typedef void *HCkPrivateKey;
typedef void *HCkPublicKey;
typedef void *HCkCSP;
typedef void *HCkMime;
typedef void *HCkKeyContainer;
typedef void *HCkCertStore;
typedef void *HCkCreateCS;
typedef void *HCkCsp;
typedef void *HCkBounce;
typedef void *HCkCharset;
typedef void *HCkCrypt2;
typedef void *HCkFtp2;
typedef void *HCkHtmlToXml;
typedef void *HCkHtmlToText;
typedef void *HCkHttp;
typedef void *HCkHttpRequest;
typedef void *HCkHttpResponse;
typedef void *HCkImap;
typedef void *HCkMailboxes;
typedef void *HCkMessageSet;
typedef void *HCkMht;
typedef void *HCkRar;
typedef void *HCkRarEntry;
typedef void *HCkRsa;
typedef void *HCkSocket;
typedef void *HCkSpider;
typedef void *HCkUpload;
typedef void *HCkCgi;
typedef void *HCkSettings;
typedef void *HCkStringArray;
typedef void *HCkXml;
typedef void *HCkAtom;
typedef void *HCkRss;
typedef void *HCkZip;
typedef void *HCkZipEntry;
typedef void *HCkZipCrc;
typedef void *HCkCompression;
typedef void *HCkGzip;
typedef void *HCkUnixCompress;
typedef void *HCkSsh;
typedef void *HCkSFtp;
typedef void *HCkSFtpDir;
typedef void *HCkSFtpFile;
typedef void *HCkSshKey;
typedef void *HCkTar;
typedef void *HCkBz2;
typedef void *HCkDh;
typedef void *HCkDsa;
typedef void *HCkXmp;
typedef void *HCkCache;
typedef void *HCkDkim;
typedef void *HCkFileAccess;
typedef void *HCkSocksProxy;
typedef void *HCkDateTime;
typedef void *HCkCsv;
typedef void *HCkSshTunnel;
typedef void *HCkOmaDrm;
typedef void *HCkNtlm;
typedef void *HCkDirTree;
typedef void *HCkDtObj;


typedef void *HCkByteDataW;
typedef void *HCkStringW;
typedef void *HCkCertW;
typedef void *HCkEmailW;
typedef void *HCkEmailBundleW;
typedef void *HCkMailManW;
typedef void *HCkPrivateKeyW;
typedef void *HCkPublicKeyW;
typedef void *HCkCSPW;
typedef void *HCkMimeW;
typedef void *HCkKeyContainerW;
typedef void *HCkCertStoreW;
typedef void *HCkCreateCSW;
typedef void *HCkCspW;
typedef void *HCkBounceW;
typedef void *HCkCharsetW;
typedef void *HCkCrypt2W;
typedef void *HCkFtp2W;
typedef void *HCkHtmlToXmlW;
typedef void *HCkHtmlToTextW;
typedef void *HCkHttpW;
typedef void *HCkHttpRequestW;
typedef void *HCkHttpResponseW;
typedef void *HCkImapW;
typedef void *HCkMailboxesW;
typedef void *HCkMessageSetW;
typedef void *HCkMhtW;
typedef void *HCkRarW;
typedef void *HCkRarEntryW;
typedef void *HCkRsaW;
typedef void *HCkSocketW;
typedef void *HCkSpiderW;
typedef void *HCkUploadW;
typedef void *HCkCgiW;
typedef void *HCkSettingsW;
typedef void *HCkStringArrayW;
typedef void *HCkXmlW;
typedef void *HCkAtomW;
typedef void *HCkRssW;
typedef void *HCkZipW;
typedef void *HCkZipEntryW;
typedef void *HCkZipCrcW;
typedef void *HCkCompressionW;
typedef void *HCkGzipW;
typedef void *HCkUnixCompressW;
typedef void *HCkSshW;
typedef void *HCkSFtpW;
typedef void *HCkSFtpDirW;
typedef void *HCkSFtpFileW;
typedef void *HCkSshKeyW;
typedef void *HCkTarW;
typedef void *HCkBz2W;
typedef void *HCkDhW;
typedef void *HCkDsaW;
typedef void *HCkXmpW;
typedef void *HCkCacheW;
typedef void *HCkDkimW;
typedef void *HCkFileAccessW;
typedef void *HCkSocksProxyW;
typedef void *HCkDateTimeW;
typedef void *HCkCsvW;
typedef void *HCkSshTunnelW;
typedef void *HCkOmaDrmW;
typedef void *HCkNtlmW;
typedef void *HCkDirTreeW;
typedef void *HCkDtObjW;

#endif
