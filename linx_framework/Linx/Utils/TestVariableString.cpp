#include "Linx/TestConfig.h"
#ifdef TEST_VARIABLE_STRING
#include "Linx/Utils/VariableString.h"
#include <iostream>

using namespace std;
using namespace Linx;

int main()
{
	VariableString v;

	v = true;
	cout << (bool)v << endl;
	v = -146;
	cout << (int)v << endl;
	cout << (long long)v << endl;
	v = 76;
	cout << (unsigned short)v << endl;
	v = "sz";
	cout << (string)v << endl;
	v = "string"s;
	cout << (string)v << endl;

	getchar();
}

#endif