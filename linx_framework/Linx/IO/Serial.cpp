#include "Linx/IO/Serial.h"
#ifndef _WIN32
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include "Linx/Utils/Debug.h"

using namespace std;
using namespace Linx;
using namespace SerialParam;

#ifdef _WIN32

/************************************************************************/
/*                       Windows Definition                             */
/************************************************************************/

Linx::Serial::Serial(const char* PortName, SerialConfig InConfig)
{
	Open(PortName, InConfig);
}

bool Linx::Serial::Open(const char* PortName, SerialConfig InConfig)
{
	if (InConfig.bSync)
	{
		Handle = CreateFile(PortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	}
	else
	{
		Handle = CreateFile(PortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	}

	if (Handle == (HANDLE)-1)
	{
		return false;
	}

	if (!SetBufSize(1024, 1024))
	{
		return false;
	}

	DCB p;
	memset(&p, 0, sizeof(p));
	p.DCBlength = sizeof(p);
	p.BaudRate = InConfig.Baudrate;
	p.ByteSize = InConfig.DataBits;

	switch (InConfig.StopBits)
	{
	case OneStopBit:
		p.StopBits = ONESTOPBIT;
		break;
	case One5StopBits:
		p.StopBits = ONE5STOPBITS;
		break;
	case TwoStopBits:
		p.StopBits = TWOSTOPBITS;
		break;
	}

	switch (InConfig.Parity)
	{
	case NoParity:
		p.Parity = NOPARITY;
		break;
	case OddParity:
		p.Parity = ODDPARITY;
		break;
	case EvenParity:
		p.Parity = EVENPARITY;
		break;
	case MarkParity:
		p.Parity = MARKPARITY;
		break;
	case SpaceParity:
		p.Parity = SPACEPARITY;
		break;
	}

	if (!SetCommState(Handle, &p))
	{
		return false;
	}

	COMMTIMEOUTS TempTimeout;
	TempTimeout.ReadIntervalTimeout = Timeout.ReadIntervalTimeout;
	TempTimeout.ReadTotalTimeoutMultiplier = Timeout.ReadTotalTimeoutMultiplier;
	TempTimeout.ReadTotalTimeoutConstant = Timeout.ReadTotalTimeoutConstant;
	TempTimeout.WriteTotalTimeoutMultiplier = Timeout.WriteTotalTimeoutMultiplier;
	TempTimeout.WriteTotalTimeoutConstant = Timeout.WriteTotalTimeoutConstant;
	SetCommTimeouts(Handle, &TempTimeout);

	// Clear buffer
	PurgeComm(Handle, PURGE_TXCLEAR | PURGE_RXCLEAR);

	Config = InConfig;
	return true;
}

void Serial::Close()
{
	if (INVALID_HANDLE_VALUE != Handle)
	{
		CloseHandle(Handle);
		Handle = INVALID_HANDLE_VALUE;
	}
}

long Serial::Read(void* Buf, size_t Size)
{
	if (Config.bSync)
	{
		DWORD wCount; 
		BOOL bReadStat = ReadFile(Handle, Buf, Size, &wCount, NULL);

		if (!bReadStat)
		{
			return -1;
		}
		return wCount;
	}
	else
	{
		DWORD wCount; 
		DWORD dwErrorFlags; 
		COMSTAT comStat; 
		OVERLAPPED m_osRead; 
							 
		memset(&m_osRead, 0, sizeof(m_osRead));
		m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, nullptr);

		ClearCommError(Handle, &dwErrorFlags, &comStat); 
		if (!comStat.cbInQue)
			return 0; 
									   
		BOOL bReadStat = ReadFile(Handle, Buf, Size, &wCount, &m_osRead);

		if (!bReadStat)
		{
			if (GetLastError() == ERROR_IO_PENDING) 
			{
				GetOverlappedResult(Handle, &m_osRead, &wCount, TRUE);
			}
			else
			{
				ClearCommError(Handle, &dwErrorFlags, &comStat); 
				CloseHandle(m_osRead.hEvent); 
				return -1;
			}
		}

		return wCount;
	}
}

long Serial::Write(const void* Buf, size_t Size) noexcept
{
	Super::Write(Buf, Size);

	if (Config.bSync)
	{
		DWORD dwBytesWrite = Size; 
		BOOL bWriteStat = WriteFile( Handle, Buf, dwBytesWrite, &dwBytesWrite, NULL ); 

		if (!bWriteStat)
		{
			return -1;
		}
		return dwBytesWrite;
	}
	else
	{
		DWORD dwBytesWrite = Size; 
		DWORD dwErrorFlags; 
		COMSTAT comStat; 
		OVERLAPPED m_osWrite; 
							  
		memset(&m_osWrite, 0, sizeof(m_osWrite));
		m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, nullptr);

		ClearCommError(Handle, &dwErrorFlags, &comStat); 
		BOOL bWriteStat = WriteFile( Handle, Buf, dwBytesWrite, &dwBytesWrite, &m_osWrite ); 

		if (!bWriteStat)
		{
			if (GetLastError() == ERROR_IO_PENDING) 
			{
				WaitForSingleObject(m_osWrite.hEvent, 1000); 
			}
			else
			{
				ClearCommError(Handle, &dwErrorFlags, &comStat); 
				CloseHandle(m_osWrite.hEvent); 
				return -1;
			}
		}
		return dwBytesWrite;
	}
}

std::vector<std::string> Serial::GetAllSerialNames()
{
	vector<string> Serials;
	HKEY hKey;
	char portName[256], commName[256];
	
	if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm", 0, KEY_READ, &hKey))
	{
		int i = 0;
		DWORD  dwLong, dwSize;

		while (1)
		{
			dwLong = dwSize = sizeof(portName);
			
			if (ERROR_NO_MORE_ITEMS == ::RegEnumValue(hKey, i, portName, &dwLong, NULL, NULL, (PUCHAR)commName, &dwSize))
			{
				break;
			}
			Serials.emplace_back(commName);
			i++;
		}
		RegCloseKey(hKey);

	}
	return Serials;
}

bool Linx::Serial::SetBufSize(uint32_t ReadSize, uint32_t WriteSize) const
{
	return SetupComm(Handle, ReadSize, WriteSize);
}

#else // _WIN32

/************************************************************************/
/*                       Linux Definition                               */
/************************************************************************/

Linx::Serial::Serial(const char* PortName, SerialConfig InConfig)
{
	Open(PortName, InConfig);
}

bool Linx::Serial::Open(const char* PortName, SerialConfig InConfig)
{

	Handle = open(PortName, O_RDWR | O_NOCTTY | O_NDELAY);
	if (Handle == -1)
	{
	    return false;
	}

	if (InConfig.bSync)
	{
		fcntl(Handle, F_SETFL, 0);
	}


	struct termios tty;
	memset(&tty, 0, sizeof tty);
	if (tcgetattr(Handle, &tty) != 0)
	{
	    return false;
	}

	cfsetospeed(&tty, InConfig.Baudrate);
	cfsetispeed(&tty, InConfig.Baudrate);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | InConfig.DataBits;
	tty.c_iflag &= ~IGNBRK;
	tty.c_lflag = 0;
	tty.c_oflag = 0;
	tty.c_cc[VMIN]  = 0;
	tty.c_cc[VTIME] = 5;

	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_cflag |= (CLOCAL | CREAD);

	if (InConfig.Parity == NoParity)
		tty.c_cflag &= ~PARENB;
	else if (InConfig.Parity == OddParity)
		tty.c_cflag |= (PARENB | PARODD);
	else if (InConfig.Parity == EvenParity)
	{
		tty.c_cflag |= PARENB;
		tty.c_cflag &= ~PARODD;
	}
	tty.c_cflag |= 0;
	if (InConfig.StopBits == OneStopBit)
		tty.c_cflag &= ~CSTOPB;
	else if (InConfig.StopBits == TwoStopBits)
		tty.c_cflag |= CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	if (tcsetattr(Handle, TCSANOW, &tty) != 0)
	{
	    return false;
	}

	Config = InConfig;
	return true;
}

void Serial::Close()
{
	if (-1 != Handle)
	{
		close(Handle);
		Handle = (HANDLE)-1;
	}
}

long Serial::Read(void* Buf, size_t Size)
{
	return read(Handle, Buf, Size);
}

long Serial::Write(const void* Buf, size_t Size) noexcept
{
	return write(Handle, Buf, Size);
}

std::vector<std::string> Serial::GetAllSerialNames()
{
	vector<string> Serials;

    FILE *fp;
    char path[1024];

    fp = popen("ls /dev/ttyS* /dev/ttyUSB* /dev/ttyACM*", "r");
    if (fp == NULL)
    {
    	return Serials;
    }
  
    while (fgets(path, sizeof(path)-1, fp))
    {
        path[strcspn(path, "\n")] = 0;
        Serials.emplace_back(path);
    }  

    pclose(fp);

	return Serials;
}

bool Linx::Serial::SetBufSize(uint32_t ReadSize, uint32_t WriteSize) const
{
	return true;
}

#endif // _WIN32
