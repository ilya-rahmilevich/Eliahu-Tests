#include "stdafx.h"
#include <CkSocket.h>
#include <CkCertStore.h>
#include <CkCert.h>
#include "stdio.h"

void ChilkatClientCertificate(void)
{
    CkSocket socket;

    bool success;
    success = socket.UnlockComponent("Anything for 30-day trial");
    if (success != true) {
        printf("%s\n",socket.lastErrorText());
        return;
    }

    //  Create an instance of a certificate store object, load a PFX file,
    //  locate the certificate we need, and use it for signing.
    //  (a PFX file may contain more than one certificate.)
    CkCertStore certStore;
    //  The 1st argument is the filename, the 2nd arg is the
    //  PFX file's password:
	success = certStore.LoadPfxFile("c:\\projects\\ssl\\eli_cer.pfx","Welcome!");
    if (success != true) {
        printf("%s\n",certStore.lastErrorText());
        return;
    }

    CkCert *cert = 0;
    cert = certStore.FindCertBySubjectCN("user");
    if (cert == 0 ) {
        printf("%s\n",certStore.lastErrorText());
        return;
    }

    socket.SetSslClientCert(*cert);

    bool ssl;
    ssl = true;
    long maxWaitMillisec;
    maxWaitMillisec = 20000;

    //  The SSL server hostname may be an IP address, a domain name,
    //  or "localhost".  You'll need to change this:
    const char * sslServerHost;
    //sslServerHost = "123.123.88.88";
    //sslServerPort = 8123;

    sslServerHost = "172.28.2.246";
    long sslServerPort;
    sslServerPort = 8123;


    //  Connect to the SSL server:
    success = socket.Connect(sslServerHost,sslServerPort,ssl,maxWaitMillisec);
    if (success != true) {
        printf("%s\n",socket.lastErrorText());
        return;
    }

    //  Set maximum timeouts for reading an writing (in millisec)
    socket.put_MaxReadIdleMs(20000);
    socket.put_MaxSendIdleMs(20000);

    //  Send a "Hello Server! -EOM-" message:
    success = socket.SendString("Hello Server! -EOM-");
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

    //  Close the connection with the server
    //  Wait a max of 20 seconds (20000 millsec)
    socket.Close(20000);

    printf("%s\n",receivedMsg);

	wprintf(_T("type any key and enter for exit\n"));
	ATLTRACE(_T("type any key and enter for exit\n"));

	char buff[50]; 
	scanf("%s", buff);

}


