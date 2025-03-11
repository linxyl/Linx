#include "Linx/TestConfig.h"
#ifdef TEST_NETWORK
#include <thread>
#include <iostream>
#include <mutex>

#include "TcpClientSocket.h"
#include "TcpServerSocket.h"
#include "UdpSocket.h"
#include "IcmpSocket.h"
#include "Linx/IO/File.h"
#include "Linx/Core.h"

//#define TEST_SENDFILE

using namespace std;
using namespace Linx;

void TestTcpClient();
void TestTcpServer();
void TestUdpSocket();
void TestIcmpSocket();

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

	cout << "\nIcmp test" << endl;

	TestIcmpSocket();

	getchar();
}

void TestTcpClient()
{
	TcpClientSocket tcs;

	while (!tcs.Connect("127.0.0.1", 5004))
	{
	}

	////////////////////////////////////////
	if (tcs.Send("123", 4) <= 0)
	{
		cout << "tcs send failed\n";
		return;
	}
	m.lock();
	cout << "tcs send 123\n";
	m.unlock();
	SleepM(10);

	m.lock();
	char buf[4];
	if (tcs.Recv(buf, 4) <= 0)
	{
		cout << "tcs recv failed\n";
		return;
	}

	cout << "tcs recv " << buf << endl;
	m.unlock();

#ifdef TEST_SENDFILE
	if (tcs.Recv(buf, 4) <= 0)
	{
		cout << "tcs recv file failed\n";
		return;
	}
	cout << "tcs recv file: " << buf << endl;
#endif
	///////////////////////////////////////

	tcs.Close();
}

void TestTcpServer()
{
	TcpServerSocket tss;

	if (!tss.Bind(5004))
	{
		cout << "Bind failed\n";
		return;
	}

	if (!tss.Listen())
	{
		cout << "Listen failed\n";
		return;
	}

	auto ts = tss.Accept();
	if (!ts.IsValid())
	{
		cout << "Accept failed\n";
		return;
	}
	////////////////////////////////////////
	m.lock();
	char buf[4];
	if (ts.Recv(buf, 4) <= 0)
	{
		cout << "tss recv failed\n";
		return;
	}
	cout << "tss recv " << buf << endl;
	m.unlock();

	SleepM(10);

	if (ts.Send("456", 4) <= 0)
	{
		cout << "tss send failed\n";
		return;
	}
	m.lock();
	cout << "tss send 456\n";
	m.unlock();

#ifdef TEST_SENDFILE
	File file("sendfile.txt");
	m.lock();
	if (ts.SendFile(file, 3) <= 0)
	{
		cout << "tss send file failed\n";
		return;
	}
	cout << "tss send file: abc\n";
	m.unlock();
#endif
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

void TestIcmpSocket()
{
	if (!IcmpSocket::PingTest("127.0.0.1"))
	{
		cout << "Ping failed" << endl;
	}
}
#endif
