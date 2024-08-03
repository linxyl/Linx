#include "Thread.h"

using namespace std;

void Linx::Thread::Start()
{
	ThreadHandle = thread(&Thread::Run, this);
}