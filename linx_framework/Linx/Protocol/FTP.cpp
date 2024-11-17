#include "FTP.h"
#include "Linx/IO/File.h"

using namespace Linx;

bool FTP::Connect(const std::string& InTargetAddr, const std::string& Username, const std::string& Password)
{
	TargetAddr = InTargetAddr;
	if (!ControlIO.Connect(TargetAddr, 21))
	{
		return false;
	}

	if (!Username.empty() && !LoginUser(Username))
	{
		return false;
	}

	if (!Password.empty() && !EnterPassword(Password))
	{
		return false;
	}

	return true;
}

bool FTP::LoginUser(const std::string& Username, char* Response, size_t ResponseSize)
{
	return SendCommand("USER "s + std::string(Username) + "\r\n"s, Response, ResponseSize);
}

bool FTP::EnterPassword(const std::string& Password, char* Response, size_t ResponseSize)
{
	if (!SendCommand("PASS "s + std::string(Password) + "\r\n"s, Response, ResponseSize))
	{
		return false;
	}

	if (!ConnectInPassiveMode())
	{
		return false;
	}
	return true;
}

bool FTP::DownloadFile(const std::string& ServerPath, char* RecvBuf, size_t BufSize)
{
	if (!RequestDownload(ServerPath))
	{
		return false;
	}

	while (TransmitIO.Recv(RecvBuf, BufSize) > 0)
	{
	}

	return true;
}

bool FTP::DownloadFile(const std::string& ServerPath, const std::string& LocalPath)
{
	if (!RequestDownload(ServerPath))
	{
		return false;
	}

	File file(LocalPath);
	char Buf[1024];
	while (true)
	{
		auto Ret = TransmitIO.Recv(Buf, sizeof(Buf));
		if (Ret <= 0)
		{
			break;
		}

		if (file.Write(Buf, Ret) <= 0)
		{
			break;
		}
	}

	return true;
}

bool FTP::UploadFile(const std::string& ServerPath, char* RecvBuf, size_t BufSize)
{
	if (!RequestUpload(ServerPath))
	{
		return false;
	}

	while (TransmitIO.Send(RecvBuf, BufSize) > 0)
	{
	}

	return true;
}

bool FTP::UploadFile(const std::string& ServerPath, const std::string& LocalPath)
{
	if (!RequestUpload(ServerPath))
	{
		return false;
	}

	File file(LocalPath);
	char Buf[1024];
	while (true)
	{
		auto Ret = file.Read(Buf, sizeof(Buf));
		if (Ret <= 0)
		{
			break;
		}

		if (TransmitIO.Send(Buf, Ret) <= 0)
		{
			break;
		}
	}

	return true;
}

bool FTP::SendCommand(const std::string& Cmd, char* Response, size_t ResponseSize)
{
	if (ControlIO.Send(Cmd.c_str(), Cmd.size()) <= 0)
	{
		return false;
	}

	char Buf[1024];
	if (!Response)
	{
		Response = Buf;
		ResponseSize = sizeof(Buf);
	}

	SleepM(10);

	auto Ret = ControlIO.Recv(Response, ResponseSize);
	if (Ret <= 0)
	{
		return false;
	}

	if (Ret < ResponseSize)
	{
		Response[Ret] = '\0';
	}
	return true;
}
bool FTP::ConnectInPassiveMode(char* Response, size_t ResponseSize)
{
	char Buf[1024];
	if (!Response)
	{
		Response = Buf;
		ResponseSize = sizeof(Buf);
	}

	if (!SendCommand("PASV\r\n"s, Response, ResponseSize))
	{
		return false;
	}

	int p1, p2;
	if (sscanf(Response, "227 Entering Passive Mode (%*d,%*d,%*d,%*d,%d,%d)", &p1, &p2) != 2)
	{
		return false;
	}

	ServerTransmitPort = p1 * 256 + p2;

	return true;
}

bool FTP::RequestDownload(const std::string& ServerPath)
{
	if (!TransmitIO.Connect(TargetAddr, ServerTransmitPort))
	{
		auto a = TransmitIO.GetLastError();
		return false;
	}

	if (!SendCommand("RETR "s + ServerPath + "\r\n"s))
	{
		return false;
	}

	return true;
}

bool FTP::RequestUpload(const std::string& ServerPath)
{
	if (!TransmitIO.Connect(TargetAddr, ServerTransmitPort))
	{
		return false;
	}

	if (!SendCommand("STOR "s + ServerPath + "\r\n"s))
	{
		return false;
	}

	return true;
}
