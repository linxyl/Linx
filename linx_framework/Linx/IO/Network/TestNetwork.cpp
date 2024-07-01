#include "TestConfig.h"
#ifdef NETWORK
#include <thread>
#include <iostream>
#include <mutex>

#include "TcpClientSocket.h"
#include "TcpServerSocket.h"
#include "UdpSocket.h"

using namespace std;
using namespace Linx;

#ifdef __linux__
#include <unistd.h>
	__inline__ void Sleep(int x) { usleep(1000 * x); }
#endif

void TestTcpClient();
void TestTcpServer();
void TestUdpSocket();

mutex m;

int main()
{
	cout << "TCP test" << endl;

	thread threadTC(TestTcpClient);
	thread threadTS(TestTcpServer);

	threadTS.join();
	threadTC.join();

	cout << "\nUDP test" << endl;

	TestUdpSocket();

	getchar();
}

void TestTcpClient()
{
	TcpClientSocket tcs;

	while(!tcs.Connect("127.0.0.1", 5004))
	{
	}

	////////////////////////////////////////
	if(tcs.Send("123", 4) <= 0)
	{
		cout<<"tcs send failed\n";
		return;
	}
	m.lock();
	cout << "tcs send 123\n";
	m.unlock();
	Sleep(10);

	m.lock();
	char buf[4];
	if(tcs.Recv(buf, 4) <= 0)
	{
		cout<<"tcs recv failed\n";
		return;
	}
	cout << "tcs recv " << buf << endl;
	m.unlock();
	///////////////////////////////////////

	tcs.Close();
}

void TestTcpServer()
{
	TcpServerSocket tss;

	if(!tss.Bind(5004))
	{
		cout<<"Bind failed\n";
		return;
	}

	if(!tss.Listen())
	{
		cout<<"Listen failed\n";
		return;
	}
	
	if(!tss.Accept())
	{
		cout<<"Accept failed\n";
		return;
	}
	////////////////////////////////////////
	m.lock();
	char buf[4];
	if(tss.Recv(buf, 4) <= 0)
	{
		cout<<"tss recv failed\n";
		return;
	}
	cout << "tss recv " << buf << endl;
	m.unlock();

	Sleep(10);

	if(tss.Send("456", 4) <= 0)
	{
		cout<<"tss send failed\n";
		return;
	}
	m.lock();
	cout << "tss send 456\n";
	m.unlock();
	///////////////////////////////////////

	tss.Close();
}

void TestUdpSocket()
{
	UdpSocket us;

	us.SetTargetAddr("127.0.0.1", 5006);

	us.Bind(5006);

	us.Send("123", 4);
	cout << "us send 123\n";
	char buf[4];
	us.Recv(buf, 4);
	cout << "us recv " << buf << endl;

	us.Close();
}
#endif
