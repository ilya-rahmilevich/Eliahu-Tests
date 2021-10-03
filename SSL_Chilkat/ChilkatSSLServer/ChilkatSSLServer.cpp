// SSLServer.cpp : Defines the entry point for the console application.
//

// ATLTRACE_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <CkSocket.h>
#include <CkCertStore.h>
#include <CkCert.h>

void ChilkatSample(void)
{
    CkSocket listenSslSocket;

    //  Any string automatically begins a fully-functional 30-day trial.
    bool success;
    success = listenSslSocket.UnlockComponent("Anything for 30-day trial");
    if (success != true) {
        printf("%s\n",listenSslSocket.lastErrorText());
        return;
    }

	wprintf(_T("listenSslSocket.UnlockComponent suceeded\n"));
	ATLTRACE(_T("listenSslSocket.UnlockComponent suceeded\n"));

    //  An SSL server needs a digital certificate.  This example loads it from a PFX file.

    //  Create an instance of a certificate store object, load a PFX file,
    //  locate the certificate we need, and use it.
    //  (a PFX file may contain more than one certificate.)
    CkCertStore certStore;
    //  The 1st argument is the filename, the 2nd arg is the
    //  PFX file's password:
    //success = certStore.LoadPfxFile("chilkat.pfx","test");
	success = certStore.LoadPfxFile("c:\\projects\\ssl\\eli_cer.pfx","Welcome!");
    if (success != true) {
        printf("%s\n",certStore.lastErrorText());
        return;
    }

	wprintf(_T("pfx file with password sucessfully loaded\n"));
	ATLTRACE(_T("pfx file with password sucessfully loaded\n"));

    //  Find the certificate to be used for SSL:
    CkCert *cert = 0;
    //cert = certStore.FindCertBySubject("Chilkat Software, Inc.");
	cert = certStore.FindCertBySubject("user");
    if (cert == 0 ) {
        printf("%s\n",certStore.lastErrorText());
        return;
    }

	wprintf(_T("sertificate found by subject <%s>\n"), _T("user"));
	ATLTRACE(_T("sertificate found by subject <%s>\n"), _T("user"));

    //  Use the certificate:
    success = listenSslSocket.InitSslServer(*cert);
    if (success != true) {
        printf("%s\n",listenSslSocket.lastErrorText());
        return;
    }

	wprintf(_T("listenSslSocket.InitSslServer suceeded\n"));
	ATLTRACE(_T("listenSslSocket.InitSslServer suceeded\n"));

    //  Bind and listen on a port:
    long myPort;
    myPort = 8123;
    //  Allow for a max of 5 queued connect requests.
    long backLog;
    backLog = 5;
    success = listenSslSocket.BindAndListen(myPort,backLog);
    if (success != true) {
        printf("%s\n",listenSslSocket.lastErrorText());
        return;
    }

	wprintf(_T("listenSslSocket.BindAndListen suceeded\n"));
	ATLTRACE(_T("listenSslSocket.BindAndListen suceeded\n"));

    //  If accepting an SSL/TLS connection, the SSL handshake is part of the connection
    //  establishment process. This involves a few back-and-forth messages between the
    //  client and server to establish algorithms and a shared key to create the secure
    //  channel. The sending and receiving of these messages are governed by the
    //  MaxReadIdleMs and MaxSendIdleMs properties. If these properties are set to 0
    //  (and this is the default unless changed by your application), then the
    //  AcceptNextConnection can hang indefinitely during the SSL handshake process.
    //  Make sure these properties are set to appropriate values before calling AcceptNextConnection.

    //  Set a 10 second max for waiting to read/write.  This is for the SSL/TLS handshake establishment.
    listenSslSocket.put_MaxReadIdleMs(10000);
    listenSslSocket.put_MaxSendIdleMs(10000);

	wprintf(_T("trying to accept connection from client\n"));
	ATLTRACE(_T("trying to accept connection from client\n"));

    //  Accept a single client connection and establish the secure SSL/TLS channel:
    CkSocket *clientSock = 0;
    long maxWaitMillisec;
    maxWaitMillisec = 20000;
    clientSock = listenSslSocket.AcceptNextConnection(maxWaitMillisec);
    if (clientSock == 0 ) {
        printf("%s\n",listenSslSocket.lastErrorText());
        return;
    }

	wprintf(_T("after listenSslSocket.AcceptNextConnection\n"));
	ATLTRACE(_T("after listenSslSocket.AcceptNextConnection\n"));

    //  The client (in this example) is going to send a "Hello Server! -EOM-"
    //  message.  Read it:
	wprintf(_T("The client (in this example) is going to send a \"Hello Server! -EOM-\"\n"));
	ATLTRACE(_T("The client (in this example) is going to send a \"Hello Server! -EOM-\"\n"));

    const char * receivedMsg;
    receivedMsg = clientSock->receiveUntilMatch("-EOM-");
    if (receivedMsg == 0 ) {
        printf("%s\n",clientSock->lastErrorText());
        return;
    }

	wprintf(_T("Waiting for message from client\n"));
	ATLTRACE(_T("Waiting for message from client\n"));
    printf("%s\n",receivedMsg);

    //  Send a "Hello Client! -EOM-" message:
    success = clientSock->SendString("(Eli) Hello Client! -EOM-");
    if (success != true) {
        printf("%s\n",clientSock->lastErrorText());
        return;
    }

	wprintf(_T("type any key and enter for exit\n"));
	ATLTRACE(_T("type any key and enter for exit\n"));

	char buff[50]; 
	scanf("%s", buff);

    //  Close the connection with the client
    //  Wait a max of 20 seconds (20000 millsec)
    clientSock->Close(20000);

}




int _tmain(int argc, _TCHAR* argv[])
{
	ChilkatSample();

	return 0;
}

