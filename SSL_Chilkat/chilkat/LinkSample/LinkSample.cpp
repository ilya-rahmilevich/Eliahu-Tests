// LinkSample.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include <tchar.h>
#include <CkImap.h>
#include <CkSocket.h>
#include <stdio.h>

void func()
{
	int i = 5;
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

    bool ssl;
    ssl = true;
    long maxWaitMillisec;
    maxWaitMillisec = 20000;

    //  The SSL server hostname may be an IP address, a domain name,
    //  or "localhost".  You'll need to change this:
    const char * sslServerHost;
    sslServerHost = "123.123.88.88";
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
}



void DoNothing(void)
{
	CkImap imap;
	imap.UnlockComponent("Test");
	imap.Connect("imap.gmail.com");

}

int _tmain(int argc, _TCHAR* argv[])
{
	func();

	DoNothing();

	ChilkatSample();

	return 0;
}


