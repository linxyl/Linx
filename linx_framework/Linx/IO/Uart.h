#pragma once

namespace Linx
{
	class Uart
	{
#ifndef _WIN32
		typedef int HANDLE;
#endif

	public:
		Uart() {};
		Uart(const char* PortName);
		~Uart();

		bool Open(const char* PortName);

	private:

	};
}
