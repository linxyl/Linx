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

	str = " 123\n";
	Trim(str);
	cout << str << endl;

	str = " te\n st\t";
	TrimAll(str);
	cout << str << endl;

	getchar();
}
#endif
