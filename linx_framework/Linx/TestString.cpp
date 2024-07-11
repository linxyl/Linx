#include "TestConfig.h"
#ifdef STRING
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