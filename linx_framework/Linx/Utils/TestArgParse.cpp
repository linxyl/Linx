#include "Linx/TestConfig.h"
#ifdef TEST_ARG_PARSER
#include "ArgParser.h"

using namespace std;
using namespace Linx;

int IntTest;
string StringTest;
char CharTest[16];

void func(int a)
{
	cout << "Call func:" << a << endl;
}

BEGIN_MAKE_ARG_PARSER(MainArgParser)
	BEGIN_ARG_PARSER_ADD_OPT
		BIND_ARG_PARSER_ONE_OPT("-i", IntTest)
		BIND_ARG_PARSER_TWO_OPT("-s", "--string", StringTest)
		BIND_ARG_PARSER_ONE_OPT("-c", CharTest)
		BIND_ARG_PARSER_FUNCTION("-f", func, 3)
	END_ARG_PARSER_ADD_OPT
	SET_ARG_PARSER_HELP_STRING(\
		"Help string\n"
		"Enter -i -s -c -f"
	)
END_MAKE_ARG_PARSER

//#define USING_MAIN_ARG

int main(int argc, char** argv)
{
	char* argv2[] = {"", "-i", "0x14", "--string", "abc", "-c", "123", "-f"};
	int argc2 = sizeof(argv2) / sizeof(argv2[0]);

#ifdef USING_MAIN_ARG
	MainArgParser(argc, argv);
#else
	MainArgParser(argc2, argv2);
#endif

	getchar();
}

#endif