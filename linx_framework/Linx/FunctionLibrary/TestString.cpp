#include "Linx/TestConfig.h"
#ifdef TEST_STRING
#include <iostream>
#include "StringLibrary.h"

using namespace Linx;
using namespace std;

int main()
{
	std::string str("Hello World String");

	Replace(str, "String", "LString");
	cout << str << endl;

	getchar();
}
#endif
