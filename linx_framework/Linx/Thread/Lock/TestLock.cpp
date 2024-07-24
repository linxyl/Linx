#include "Linx/TestConfig.h"
#ifdef TEST_LOCK
#include <iostream>
#include "Linx/Thread/Lock/Mutex.h"
#include "Linx/Utils/TypeTraits.h"

using namespace std;
using namespace Linx;
	GENERATE_HAS_MEMBER_FUNCTION(Lock)
		GENERATE_HAS_MEMBER_FUNCTION(write)

int main()
{
	auto b = HAS_MEMBER_FUNCTION(MutexLock, Lock);
	cout << b << endl;
}

#endif