#include "stdafx.h"
#include <CkSocket.h>
#include <string>

void AsyncSSLTest(std::string sIPAddress, int port)
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
    //  or "localhost".  You'll need to change this:
    const char * sslServerHost;
    // sslServerHost = "172.28.2.246";
	sslServerHost = sIPAddress.c_str();
    long sslServerPort;
    sslServerPort = port;

    //  Connect to the SSL server asynchronously in a background thread.
    success = socket.AsyncConnectStart(sslServerHost,sslServerPort,ssl,maxWaitMillisec);
    if (success != true) {
        printf("%s\n",socket.lastErrorText());
        return;
    }

    //  Wait for the socket to become connected...
    while ((socket.get_AsyncConnectFinished() != true)) {
        //  Sleep 1 second.
        socket.SleepMs(1000);
    }

    //  Did the connect fail?
    if (socket.get_AsyncConnectSuccess() != true) {
        printf("%s\n",socket.asyncConnectLog());
        return;
    }

    //  Set maximum timeouts for reading an writing (in millisec)
    socket.put_MaxReadIdleMs(20000);
    socket.put_MaxSendIdleMs(20000);

    //  Send a "Hello Server! -EOM-" message:
    success = socket.AsyncSendString("(Eli)Hello Server! -EOM-");
    if (success != true) {
        printf("%s\n",socket.lastErrorText());
        return;
    }

    //  Wait for the send to finish
    while ((socket.get_AsyncSendFinished() != true)) {
        //  Sleep 1 second.
        socket.SleepMs(1000);
    }

    //  Did the send fail?
    if (socket.get_AsyncSendSuccess() != true) {
        printf("%s\n",socket.asyncSendLog());
        return;
    }

    //  The server (in this example) is going to send a "Hello Client! -EOM-"
    //  message.  Begin reading asynchronously in a background thread:
    success = socket.AsyncReceiveUntilMatch("-EOM-");
    if (success != true) {
        printf("%s\n",socket.lastErrorText());
        return;
    }

    //  Wait for the background read to finish
    while ((socket.get_AsyncReceiveFinished() != true)) {
        //  Sleep 1 second.
        socket.SleepMs(1000);
    }

    //  Did the receive fail?
    if (socket.get_AsyncReceiveSuccess() != true) {
        printf("%s\n",socket.asyncReceiveLog());
        return;
    }

    //  Display the received message:
    printf("%s\n",socket.asyncReceivedString());

	char buff[50]; 
	scanf("%s", buff);

    //  Close the connection with the server
    //  Wait a max of 20 seconds (20000 millsec)
    socket.Close(20000);
}


