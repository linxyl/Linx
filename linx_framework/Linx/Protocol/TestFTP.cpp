#include "Linx/TestConfig.h"
#ifdef TEST_FTP
#include <iostream>
#include "FTP.h"

using namespace Linx;
using namespace std;

#define ANONYMOUS
#define IP			"192.168.0.30"
#define USERNAME	"linx"
#define PASSWORD	"123456"
#define SERVER_FILE	"/home/linx/workspace/example.txt"

int main()
{
	try
	{
		FTP ftp;
#ifndef ANONYMOUS
		ftp.Connect(IP, USERNAME, PASSWORD);
#else
		ftp.ConnectInAnonymous(IP);
#endif
		ftp.UploadFile(SERVER_FILE, "upload.txt");
		ftp.DownloadFile(SERVER_FILE, "download.txt");

		cout << "Complete ftp test" << endl;
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}

	(void)getchar();

	return 0;
}
#endif
