#include "Linx/TestConfig.h"
#ifdef TEST_INI
#include "Linx/Protocol/Ini.h"
#include "Linx/IO/FileStream.h"
#include <iostream>

using namespace std;
using namespace Linx;

int main()
{
	try
	{
		IniFile file("test.ini");
		file.Load();
		file.Set({ "Test", "Key1" }, 3);
		file.Set({ "Test", "Key2" }, "abc");
		file.Set({ "II", "Key" }, 123);
		int i = file.Get({ "321", "k" });
		file.Set({ "321", "k" }, i + 1);
		file.Set({ "111", "k" }, 6);
		file.Save();
	}
	catch(const exception& e)
	{
		cout << e.what() << endl;
	}

	getchar();
}

#endif