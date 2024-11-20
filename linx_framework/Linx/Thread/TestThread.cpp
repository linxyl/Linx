#include "Linx/TestConfig.h"
#ifdef TEST_THREAD
#include <iostream>
#include "ThreadPool.h"
#include "Thread.h"
#include "Linx/Core.h"

using namespace std;
using namespace Linx;

class T1 :public Task
{
public:
	T1(int i)
	{
		a = i;
	}
	void Run() override
	{
		cout << a << endl;
	}
	int a;
};

class ThreadTest : public Thread
{
public:
	using Thread::Thread;

protected:
	virtual void Run() override
	{
		while (!bStopFlag)
		{}
	}
public:
	virtual void Stop() override
	{
		bStopFlag = true;
	}
private:
	bool bStopFlag = false;
};

int main()
{
	// Test Thread
	ThreadTest TT(1);
	TT.Start();
	SleepS(6);
	TT.Stop();
	TT.Join();

	// Test ThreadPool
	ThreadPool TP;
	TP.AddTask(T1(1));
	TP.AddTask(T1(2));
	TP.AddTask(T1(3));
	SleepM(1000);
	TP.Stop();
	SleepM(1000);
	TP.Start(8);
	TP.AddTask(T1(1));
	TP.AddTask(T1(2));
	TP.AddTask(T1(3));
	SleepM(1000);
	TP.Stop();

	getchar();
}

#endif