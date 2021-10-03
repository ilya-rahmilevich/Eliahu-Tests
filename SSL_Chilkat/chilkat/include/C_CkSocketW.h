#ifndef _CkSocket_W_H
#define _CkSocket_W_H
#include "Chilkat_C.h"

HCkSocketW CkSocketW_Create(void);
void CkSocketW_Dispose(HCkSocketW handle);
BOOL CkSocketW_getAsyncAcceptFinished(HCkSocketW cHandle);
void CkSocketW_getAsyncAcceptLog(HCkSocketW cHandle, HCkStringW retval);
BOOL CkSocketW_getAsyncAcceptSuccess(HCkSocketW cHandle);
BOOL CkSocketW_getAsyncConnectFinished(HCkSocketW cHandle);
void CkSocketW_getAsyncConnectLog(HCkSocketW cHandle, HCkStringW retval);
BOOL CkSocketW_getAsyncConnectSuccess(HCkSocketW cHandle);
BOOL CkSocketW_getAsyncDnsFinished(HCkSocketW cHandle);
void CkSocketW_getAsyncDnsLog(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_getAsyncDnsResult(HCkSocketW cHandle, HCkStringW retval);
BOOL CkSocketW_getAsyncDnsSuccess(HCkSocketW cHandle);
BOOL CkSocketW_getAsyncReceiveFinished(HCkSocketW cHandle);
void CkSocketW_getAsyncReceiveLog(HCkSocketW cHandle, HCkStringW retval);
BOOL CkSocketW_getAsyncReceiveSuccess(HCkSocketW cHandle);
void CkSocketW_getAsyncReceivedBytes(HCkSocketW cHandle, HCkByteDataW retval);
void CkSocketW_getAsyncReceivedString(HCkSocketW cHandle, HCkStringW retval);
BOOL CkSocketW_getAsyncSendFinished(HCkSocketW cHandle);
void CkSocketW_getAsyncSendLog(HCkSocketW cHandle, HCkStringW retval);
BOOL CkSocketW_getAsyncSendSuccess(HCkSocketW cHandle);
BOOL CkSocketW_getBigEndian(HCkSocketW cHandle);
void CkSocketW_putBigEndian(HCkSocketW cHandle, BOOL newVal);
void CkSocketW_getClientIpAddress(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_putClientIpAddress(HCkSocketW cHandle, const wchar_t *newVal);
int CkSocketW_getClientPort(HCkSocketW cHandle);
void CkSocketW_putClientPort(HCkSocketW cHandle, int newVal);
long CkSocketW_getConnectFailReason(HCkSocketW cHandle);
long CkSocketW_getDebugConnectDelayMs(HCkSocketW cHandle);
void CkSocketW_putDebugConnectDelayMs(HCkSocketW cHandle, long newVal);
long CkSocketW_getDebugDnsDelayMs(HCkSocketW cHandle);
void CkSocketW_putDebugDnsDelayMs(HCkSocketW cHandle, long newVal);
void CkSocketW_getDebugLogFilePath(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_putDebugLogFilePath(HCkSocketW cHandle, const wchar_t *newVal);
int CkSocketW_getElapsedSeconds(HCkSocketW cHandle);
long CkSocketW_getHeartbeatMs(HCkSocketW cHandle);
void CkSocketW_putHeartbeatMs(HCkSocketW cHandle, long newVal);
void CkSocketW_getHttpProxyAuthMethod(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_putHttpProxyAuthMethod(HCkSocketW cHandle, const wchar_t *newVal);
void CkSocketW_getHttpProxyDomain(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_putHttpProxyDomain(HCkSocketW cHandle, const wchar_t *newVal);
void CkSocketW_getHttpProxyHostname(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_putHttpProxyHostname(HCkSocketW cHandle, const wchar_t *newVal);
void CkSocketW_getHttpProxyPassword(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_putHttpProxyPassword(HCkSocketW cHandle, const wchar_t *newVal);
int CkSocketW_getHttpProxyPort(HCkSocketW cHandle);
void CkSocketW_putHttpProxyPort(HCkSocketW cHandle, int newVal);
void CkSocketW_getHttpProxyUsername(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_putHttpProxyUsername(HCkSocketW cHandle, const wchar_t *newVal);
BOOL CkSocketW_getIsConnected(HCkSocketW cHandle);
BOOL CkSocketW_getKeepSessionLog(HCkSocketW cHandle);
void CkSocketW_putKeepSessionLog(HCkSocketW cHandle, BOOL newVal);
void CkSocketW_getLastErrorHtml(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_getLastErrorText(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_getLastErrorXml(HCkSocketW cHandle, HCkStringW retval);
BOOL CkSocketW_getLastMethodFailed(HCkSocketW cHandle);
void CkSocketW_getLocalIpAddress(HCkSocketW cHandle, HCkStringW retval);
int CkSocketW_getLocalPort(HCkSocketW cHandle);
long CkSocketW_getMaxReadIdleMs(HCkSocketW cHandle);
void CkSocketW_putMaxReadIdleMs(HCkSocketW cHandle, long newVal);
long CkSocketW_getMaxSendIdleMs(HCkSocketW cHandle);
void CkSocketW_putMaxSendIdleMs(HCkSocketW cHandle, long newVal);
void CkSocketW_getMyIpAddress(HCkSocketW cHandle, HCkStringW retval);
int CkSocketW_getNumReceivedClientCerts(HCkSocketW cHandle);
int CkSocketW_getNumSocketsInSet(HCkSocketW cHandle);
int CkSocketW_getNumSslAcceptableClientCAs(HCkSocketW cHandle);
long CkSocketW_getObjectId(HCkSocketW cHandle);
long CkSocketW_getReceivePacketSize(HCkSocketW cHandle);
void CkSocketW_putReceivePacketSize(HCkSocketW cHandle, long newVal);
int CkSocketW_getReceivedCount(HCkSocketW cHandle);
void CkSocketW_putReceivedCount(HCkSocketW cHandle, int newVal);
void CkSocketW_getRemoteIpAddress(HCkSocketW cHandle, HCkStringW retval);
long CkSocketW_getRemotePort(HCkSocketW cHandle);
int CkSocketW_getSelectorIndex(HCkSocketW cHandle);
void CkSocketW_putSelectorIndex(HCkSocketW cHandle, int newVal);
int CkSocketW_getSelectorReadIndex(HCkSocketW cHandle);
void CkSocketW_putSelectorReadIndex(HCkSocketW cHandle, int newVal);
int CkSocketW_getSelectorWriteIndex(HCkSocketW cHandle);
void CkSocketW_putSelectorWriteIndex(HCkSocketW cHandle, int newVal);
long CkSocketW_getSendPacketSize(HCkSocketW cHandle);
void CkSocketW_putSendPacketSize(HCkSocketW cHandle, long newVal);
void CkSocketW_getSessionLog(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_getSessionLogEncoding(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_putSessionLogEncoding(HCkSocketW cHandle, const wchar_t *newVal);
int CkSocketW_getSoRcvBuf(HCkSocketW cHandle);
void CkSocketW_putSoRcvBuf(HCkSocketW cHandle, int newVal);
BOOL CkSocketW_getSoReuseAddr(HCkSocketW cHandle);
void CkSocketW_putSoReuseAddr(HCkSocketW cHandle, BOOL newVal);
int CkSocketW_getSoSndBuf(HCkSocketW cHandle);
void CkSocketW_putSoSndBuf(HCkSocketW cHandle, int newVal);
void CkSocketW_getSocksHostname(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_putSocksHostname(HCkSocketW cHandle, const wchar_t *newVal);
void CkSocketW_getSocksPassword(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_putSocksPassword(HCkSocketW cHandle, const wchar_t *newVal);
int CkSocketW_getSocksPort(HCkSocketW cHandle);
void CkSocketW_putSocksPort(HCkSocketW cHandle, int newVal);
void CkSocketW_getSocksUsername(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_putSocksUsername(HCkSocketW cHandle, const wchar_t *newVal);
int CkSocketW_getSocksVersion(HCkSocketW cHandle);
void CkSocketW_putSocksVersion(HCkSocketW cHandle, int newVal);
BOOL CkSocketW_getSsl(HCkSocketW cHandle);
void CkSocketW_putSsl(HCkSocketW cHandle, BOOL newVal);
void CkSocketW_getSslProtocol(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_putSslProtocol(HCkSocketW cHandle, const wchar_t *newVal);
void CkSocketW_getStringCharset(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_putStringCharset(HCkSocketW cHandle, const wchar_t *newVal);
BOOL CkSocketW_getTcpNoDelay(HCkSocketW cHandle);
void CkSocketW_putTcpNoDelay(HCkSocketW cHandle, BOOL newVal);
void CkSocketW_getUserData(HCkSocketW cHandle, HCkStringW retval);
void CkSocketW_putUserData(HCkSocketW cHandle, const wchar_t *newVal);
BOOL CkSocketW_getVerboseLogging(HCkSocketW cHandle);
void CkSocketW_putVerboseLogging(HCkSocketW cHandle, BOOL newVal);
void CkSocketW_getVersion(HCkSocketW cHandle, HCkStringW retval);
HCkSocketW CkSocketW_AcceptNextConnection(HCkSocketW cHandle, int maxWaitMs);
BOOL CkSocketW_AddSslAcceptableClientCaDn(HCkSocketW cHandle, const wchar_t *certAuthDN);
void CkSocketW_AsyncAcceptAbort(HCkSocketW cHandle);
HCkSocketW CkSocketW_AsyncAcceptSocket(HCkSocketW cHandle);
BOOL CkSocketW_AsyncAcceptStart(HCkSocketW cHandle, int maxWaitMs);
void CkSocketW_AsyncConnectAbort(HCkSocketW cHandle);
BOOL CkSocketW_AsyncConnectStart(HCkSocketW cHandle, const wchar_t *hostname, int port, BOOL ssl, int maxWaitMs);
void CkSocketW_AsyncDnsAbort(HCkSocketW cHandle);
BOOL CkSocketW_AsyncDnsStart(HCkSocketW cHandle, const wchar_t *hostname, int maxWaitMs);
void CkSocketW_AsyncReceiveAbort(HCkSocketW cHandle);
BOOL CkSocketW_AsyncReceiveBytes(HCkSocketW cHandle);
BOOL CkSocketW_AsyncReceiveBytesN(HCkSocketW cHandle, unsigned long numBytes);
BOOL CkSocketW_AsyncReceiveString(HCkSocketW cHandle);
BOOL CkSocketW_AsyncReceiveToCRLF(HCkSocketW cHandle);
BOOL CkSocketW_AsyncReceiveUntilMatch(HCkSocketW cHandle, const wchar_t *matchStr);
void CkSocketW_AsyncSendAbort(HCkSocketW cHandle);
BOOL CkSocketW_AsyncSendByteData(HCkSocketW cHandle, HCkByteDataW data);
BOOL CkSocketW_AsyncSendBytes(HCkSocketW cHandle, const unsigned char *byteData, unsigned long numBytes);
BOOL CkSocketW_AsyncSendString(HCkSocketW cHandle, const wchar_t *str);
BOOL CkSocketW_BindAndListen(HCkSocketW cHandle, int port, int backlog);
BOOL CkSocketW_BuildHttpGetRequest(HCkSocketW cHandle, const wchar_t *url, HCkStringW outStr);
int CkSocketW_CheckWriteable(HCkSocketW cHandle, int maxWaitMs);
void CkSocketW_ClearSessionLog(HCkSocketW cHandle);
void CkSocketW_Close(HCkSocketW cHandle, int maxWaitMs);
BOOL CkSocketW_Connect(HCkSocketW cHandle, const wchar_t *hostname, int port, BOOL ssl, int maxWaitMs);
BOOL CkSocketW_ConvertFromSsl(HCkSocketW cHandle);
BOOL CkSocketW_ConvertToSsl(HCkSocketW cHandle);
BOOL CkSocketW_DnsLookup(HCkSocketW cHandle, const wchar_t *hostname, int maxWaitMs, HCkStringW outStr);
HCkCertW CkSocketW_GetMyCert(HCkSocketW cHandle);
HCkCertW CkSocketW_GetReceivedClientCert(HCkSocketW cHandle, int index);
BOOL CkSocketW_GetSslAcceptableClientCaDn(HCkSocketW cHandle, int index, HCkStringW outStr);
HCkCertW CkSocketW_GetSslServerCert(HCkSocketW cHandle);
BOOL CkSocketW_InitSslServer(HCkSocketW cHandle, HCkCertW cert);
BOOL CkSocketW_IsUnlocked(HCkSocketW cHandle);
BOOL CkSocketW_PollDataAvailable(HCkSocketW cHandle);
BOOL CkSocketW_ReceiveBytes(HCkSocketW cHandle, HCkByteDataW outData);
BOOL CkSocketW_ReceiveBytesN(HCkSocketW cHandle, unsigned long numBytes, HCkByteDataW outData);
BOOL CkSocketW_ReceiveBytesToFile(HCkSocketW cHandle, const wchar_t *appendFilename);
int CkSocketW_ReceiveCount(HCkSocketW cHandle);
BOOL CkSocketW_ReceiveString(HCkSocketW cHandle, HCkStringW outStr);
BOOL CkSocketW_ReceiveStringMaxN(HCkSocketW cHandle, int maxBytes, HCkStringW outStr);
BOOL CkSocketW_ReceiveStringUntilByte(HCkSocketW cHandle, int byteValue, HCkStringW outStr);
BOOL CkSocketW_ReceiveToCRLF(HCkSocketW cHandle, HCkStringW outStr);
BOOL CkSocketW_ReceiveUntilByte(HCkSocketW cHandle, int byteValue, HCkByteDataW outBytes);
BOOL CkSocketW_ReceiveUntilMatch(HCkSocketW cHandle, const wchar_t *matchStr, HCkStringW outStr);
BOOL CkSocketW_SaveLastError(HCkSocketW cHandle, const wchar_t *filename);
int CkSocketW_SelectForReading(HCkSocketW cHandle, int timeoutMs);
int CkSocketW_SelectForWriting(HCkSocketW cHandle, int timeoutMs);
BOOL CkSocketW_SendByteData(HCkSocketW cHandle, HCkByteDataW data);
BOOL CkSocketW_SendBytes(HCkSocketW cHandle, const unsigned char *byteData, unsigned long numBytes);
BOOL CkSocketW_SendCount(HCkSocketW cHandle, int byteCount);
BOOL CkSocketW_SendString(HCkSocketW cHandle, const wchar_t *str);
BOOL CkSocketW_SetSslClientCert(HCkSocketW cHandle, HCkCertW cert);
BOOL CkSocketW_SetSslClientCertPem(HCkSocketW cHandle, const wchar_t *pemDataOrFilename, const wchar_t *pemPassword);
BOOL CkSocketW_SetSslClientCertPfx(HCkSocketW cHandle, const wchar_t *pfxFilename, const wchar_t *pfxPassword, const wchar_t *certSubjectCN);
void CkSocketW_SleepMs(HCkSocketW cHandle, int millisec);
void CkSocketW_StartTiming(HCkSocketW cHandle);
BOOL CkSocketW_TakeSocket(HCkSocketW cHandle, HCkSocketW sock);
BOOL CkSocketW_UnlockComponent(HCkSocketW cHandle, const wchar_t *code);
const wchar_t *CkSocketW__asyncAcceptLog(HCkSocketW cHandle);
const wchar_t *CkSocketW__asyncConnectLog(HCkSocketW cHandle);
const wchar_t *CkSocketW__asyncDnsLog(HCkSocketW cHandle);
const wchar_t *CkSocketW__asyncDnsResult(HCkSocketW cHandle);
const wchar_t *CkSocketW__asyncReceiveLog(HCkSocketW cHandle);
const wchar_t *CkSocketW__asyncReceivedString(HCkSocketW cHandle);
const wchar_t *CkSocketW__asyncSendLog(HCkSocketW cHandle);
const wchar_t *CkSocketW__buildHttpGetRequest(HCkSocketW cHandle, const wchar_t *url);
const wchar_t *CkSocketW__clientIpAddress(HCkSocketW cHandle);
const wchar_t *CkSocketW__debugLogFilePath(HCkSocketW cHandle);
const wchar_t *CkSocketW__dnsLookup(HCkSocketW cHandle, const wchar_t *hostname, int maxWaitMs);
const wchar_t *CkSocketW__getSslAcceptableClientCaDn(HCkSocketW cHandle, int index);
const wchar_t *CkSocketW__httpProxyAuthMethod(HCkSocketW cHandle);
const wchar_t *CkSocketW__httpProxyDomain(HCkSocketW cHandle);
const wchar_t *CkSocketW__httpProxyHostname(HCkSocketW cHandle);
const wchar_t *CkSocketW__httpProxyPassword(HCkSocketW cHandle);
const wchar_t *CkSocketW__httpProxyUsername(HCkSocketW cHandle);
const wchar_t *CkSocketW__lastErrorHtml(HCkSocketW cHandle);
const wchar_t *CkSocketW__lastErrorText(HCkSocketW cHandle);
const wchar_t *CkSocketW__lastErrorXml(HCkSocketW cHandle);
const wchar_t *CkSocketW__localIpAddress(HCkSocketW cHandle);
const wchar_t *CkSocketW__myIpAddress(HCkSocketW cHandle);
const wchar_t *CkSocketW__receiveString(HCkSocketW cHandle);
const wchar_t *CkSocketW__receiveStringMaxN(HCkSocketW cHandle, int maxBytes);
const wchar_t *CkSocketW__receiveStringUntilByte(HCkSocketW cHandle, int byteValue);
const wchar_t *CkSocketW__receiveToCRLF(HCkSocketW cHandle);
const wchar_t *CkSocketW__receiveUntilMatch(HCkSocketW cHandle, const wchar_t *matchStr);
const wchar_t *CkSocketW__remoteIpAddress(HCkSocketW cHandle);
const wchar_t *CkSocketW__sessionLog(HCkSocketW cHandle);
const wchar_t *CkSocketW__sessionLogEncoding(HCkSocketW cHandle);
const wchar_t *CkSocketW__socksHostname(HCkSocketW cHandle);
const wchar_t *CkSocketW__socksPassword(HCkSocketW cHandle);
const wchar_t *CkSocketW__socksUsername(HCkSocketW cHandle);
const wchar_t *CkSocketW__sslProtocol(HCkSocketW cHandle);
const wchar_t *CkSocketW__stringCharset(HCkSocketW cHandle);
const wchar_t *CkSocketW__userData(HCkSocketW cHandle);
const wchar_t *CkSocketW__version(HCkSocketW cHandle);
#endif
