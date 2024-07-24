#include "Linx/TestConfig.h"
#ifdef TEST_UART
#include "Linx/IO/Uart.h"
#include "Linx/Utils/Debug.h"
#include <thread>
#include "Linx/Core.h"

using namespace std;
using namespace Linx;

int main()
{
	char writebuf[4]= "123" ;
	char readbuf[4];
	auto UartNames = Uart::GetAllUartNames();
	DebugContainerEx(UartNames, 4);

	UartConfig Config;
	Config.bSync = true;
	Uart uart;
	if (!uart.Open(UartNames.back()))
	{
		cout << "Cannot open uart!\n";
	}
	
	uart.Write(writebuf, sizeof(writebuf));

	Sleep(10);

	uart.Read(readbuf, sizeof(readbuf));
	DebugContainerEx(readbuf);

	getchar();
}

#endif
