#include "Linx/TestConfig.h"
#ifdef TEST_INI
#include "Linx/Protocol/Ini.h"
#include "Linx/IOStream/FileStream.h"
#include <iostream>

using namespace std;
using namespace Linx;

int main()
{
	try
	{
		IniFile file("test.ini");
		file.Set({ "Test", "Key1" }, 3);
		file.Set({ "Test", "Key2" }, "abc");
		file.Set({ "II", "Key" }, 123);
		file.Set({ "321", "k" }, 100);
		int i = file.Get({ "321", "k" });
		file.Set({ "321", "k" }, i + 1);
		file.Set({ "111", "k" }, 6);
	}
	catch(const exception& e)
	{
		cout << e.what() << endl;
	}

	getchar();
}

#endif