#include "Linx/TestConfig.h"
#ifdef TEST_UART
#include "Linx/IO/Serial.h"
#include "Linx/Utils/Debug.h"
#include <thread>
#include "Linx/Core.h"

using namespace std;
using namespace Linx;

int main()
{
	char writebuf[4]= "123" ;
	char readbuf[4];
	auto SerialNames = Serial::GetAllSerialNames();
	DebugContainerEx(SerialNames, 4);

	SerialConfig Config;
	Config.bSync = true;
	Serial uart;
	if (!uart.Open(SerialNames.back()))
	{
		cout << "Cannot open uart!\n";
	}
	
	uart.Write(writebuf, sizeof(writebuf));

	SleepM(10);

	uart.Read(readbuf, sizeof(readbuf));
	DebugContainerEx(readbuf);

	getchar();
}

#endif
