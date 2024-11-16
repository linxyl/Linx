#include "Linx/TestConfig.h"
#ifdef TEST_THREAD
#include "ThreadPool.h"
#include <iostream>
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

int main()
{
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