// ChilkatSSLClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include <CkImap.h>
#include <CkSocket.h>
#include <stdio.h>
#include <CkCert.h>

#include <iostream>

void ChilkatSample(void);
void ChilkatVerify(void);
void ChilkatVerify_2(CkSocket &socket);
void ChilkatClientCertificate(void);
void AsyncSSLTest(std::string sIPAddress, int port);
	
int _tmain(int argc, _TCHAR* argv[])
{

	WSADATA wsaData;
	WORD version;
	int error;

	version = MAKEWORD( 2, 0 );
	error = WSAStartup( version, &wsaData );

  const char* const host = "Jetro-Mathieu.omnitech.co.il" ;
  const hostent* host_info = 0 ;
  host_info = gethostbyname(host) ;


  if(host_info)
  {
    in_addr * address = (in_addr * )host_info->h_addr;
    std::string ip_address = inet_ntoa(* address);

	AsyncSSLTest(ip_address, 8123);
  }

  
	//ChilkatSample();

	//ChilkatVerify();

	//ChilkatClientCertificate();

	return 0;
}


void ChilkatSample(void)
{
    CkSocket socket;

    bool success;
    success = socket.UnlockComponent("Anything for 30-day trial");
    if (success != true) {
        printf("%s\n",socket.lastErrorText());
        return;
    }

	wprintf(_T("socket.UnlockComponent suceeded\n"));
	ATLTRACE(_T("socket.UnlockComponent suceeded\n"));

    bool ssl;
    ssl = true;
    long maxWaitMillisec;
    maxWaitMillisec = 20000;

    //  The SSL server hostname may be an IP address, a domain name,
    //  or "localhost".  You'll need to change this:
    const char * sslServerHost;
    //sslServerHost = "172.28.2.246";
	sslServerHost = "Jetro-Mathieu.omnitech.co.il";

    long sslServerPort;
    sslServerPort = 8123;

	printf("trying to connect to %s\n", sslServerHost);
	ATLTRACE("trying to connect to %s\n", sslServerHost);

    //  Connect to the SSL server:
    success = socket.Connect(sslServerHost,sslServerPort,ssl,maxWaitMillisec);
    if (success != true) {
        printf("%s\n",socket.lastErrorText());
        return;
    }

	wprintf(_T("socket.Connect suceeded\n"));
	ATLTRACE(_T("socket.Connect suceeded\n"));

	ChilkatVerify_2(socket);

    //  Set maximum timeouts for reading an writing (in millisec)
    socket.put_MaxReadIdleMs(20000);
    socket.put_MaxSendIdleMs(20000);

    //  Send a "Hello Server! -EOM-" message:
    success = socket.SendString("Eli - Hello Server! -EOM-");
    if (success != true) {
        printf("%s\n",socket.lastErrorText());
        return;
    }

    //  The server (in this example) is going to send a "Hello Client! -EOM-"
    //  message.  Read it:
    const char * receivedMsg;
    receivedMsg = socket.receiveUntilMatch("-EOM-");
    if (receivedMsg == 0 ) {
        printf("%s\n",socket.lastErrorText());
        return;
    }

	wprintf(_T("-EOM- RECEIVED FROM SERVER\n"));
	ATLTRACE(_T("-EOM- RECEIVED FROM SERVER\n"));

	wprintf(_T("type any key and enter for exit\n"));
	ATLTRACE(_T("type any key and enter for exit\n"));

	char buff[50]; 
	scanf("%s", buff);

    //  Close the connection with the server
    //  Wait a max of 20 seconds (20000 millsec)
    socket.Close(20000);

    printf("%s\n",receivedMsg);
}


void ChilkatVerify(void)
{
    CkSocket socket;

    bool success;
    success = socket.UnlockComponent("Anything for 30-day trial");
    if (success != true) {
        printf("%s\n",socket.lastErrorText());
        return;
    }

    bool ssl;
    ssl = true;
    long maxWaitMillisec;
    maxWaitMillisec = 20000;

    //  The SSL server hostname may be an IP address, a domain name,
    //  or "localhost".
    const char * sslServerHost;
    //sslServerHost = "www.paypal.com";
    sslServerHost = "172.28.2.191";

    long sslServerPort;
    // sslServerPort = 443;
	sslServerPort = 8123;

    //  Connect to the SSL server:
    success = socket.Connect(sslServerHost,sslServerPort,ssl,maxWaitMillisec);
    if (success != true) {
        printf("%s\n",socket.lastErrorText());
        return;
    }

    CkCert *cert = 0;

    bool bExpired;
    bool bRevoked;
    bool bSignatureVerified;
    bool bTrustedRoot;

    cert = socket.GetSslServerCert();
    if (!(cert == 0 )) {

        printf("Server Certificate:\n");
        printf("Distinguished Name: %s\n",cert->subjectDN());
        printf("Common Name: %s\n",cert->subjectCN());
        printf("Issuer Distinguished Name: %s\n"
            ,cert->issuerDN());
        printf("Issuer Common Name: %s\n",cert->issuerCN());

        bExpired = cert->get_Expired();
        bRevoked = cert->get_Revoked();
        bSignatureVerified = cert->get_SignatureVerified();
        bTrustedRoot = cert->get_TrustedRoot();

        printf("Expired: %d\n",bExpired);
        printf("Revoked: %d\n",bRevoked);
        printf("Signature Verified: %d\n",bSignatureVerified);
        printf("Trusted Root: %d\n",bTrustedRoot);

        delete cert;
    }

    //  Close the connection with the server
    //  Wait a max of 20 seconds (20000 millsec)
    socket.Close(20000);

}




void ChilkatVerify_2(CkSocket &socket)
{

    CkCert *cert = 0;

    bool bExpired;
    bool bRevoked;
    bool bSignatureVerified;
    bool bTrustedRoot;

    cert = socket.GetSslServerCert();
    if (!(cert == 0 )) {

        printf("Server Certificate:\n");
        printf("Distinguished Name: %s\n",cert->subjectDN());
        printf("Common Name: %s\n",cert->subjectCN());
        printf("Issuer Distinguished Name: %s\n"
            ,cert->issuerDN());
        printf("Issuer Common Name: %s\n",cert->issuerCN());

        bExpired = cert->get_Expired();
        bRevoked = cert->get_Revoked();
        bSignatureVerified = cert->get_SignatureVerified();
        bTrustedRoot = cert->get_TrustedRoot();

        printf("Expired: %d\n",bExpired);
        printf("Revoked: %d\n",bRevoked);
        printf("Signature Verified: %d\n",bSignatureVerified);
        printf("Trusted Root: %d\n",bTrustedRoot);

        delete cert;
    }

}
