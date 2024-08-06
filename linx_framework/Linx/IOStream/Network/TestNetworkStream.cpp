#include "Linx/TestConfig.h"
#ifdef TEST_NETWORK_STREAM
#include <thread>
#include <iostream>
#include <mutex>

#include "TcpClientStream.h"
#include "TcpServerStream.h"
#include "UdpStream.h"
#include "Linx/Core.h"

using namespace std;
using namespace Linx;

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
	TcpClientStream tcs;

	while(!tcs.Connect("127.0.0.1", 5004))
	{
	}

	////////////////////////////////////////
	tcs << 123 << endl;
	m.lock();
	cout << "tcs send 123\n";
	m.unlock();
	Sleep(10);

	m.lock();
	int Val;
	tcs >> Val;
	cout << "tcs recv " << Val << endl;
	m.unlock();
	///////////////////////////////////////

	tcs.Close();
}

void TestTcpServer()
{
	TcpServerStream tss;

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
	int Val;
	tss >> Val;
	cout << "tss recv " << Val << endl;
	m.unlock();

	Sleep(10);

	tss << 456 << endl;
	m.lock();
	cout << "tss send 456\n";
	m.unlock();
	///////////////////////////////////////

	tss.Close();
}

void TestUdpSocket()
{
	UdpStream us;

	us.SetTargetAddr("127.0.0.1", 5006);

	us.Bind(5006);

	us << 123 << endl;
	cout << "us send 123\n";
	int val;
	us >> val;
	cout << "us recv " << val << endl;

	us.Close();
}
#endif
