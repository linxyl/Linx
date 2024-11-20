#include <stdexcept>
#include "FTP.h"
#include "Linx/IO/File.h"

using namespace std;
using namespace std::literals::string_literals;
using namespace Linx;

bool FTP::Connect(const std::string& InTargetAddr, const std::string& Username, const std::string& Password)
{
	TargetAddr = InTargetAddr;
	constexpr int ControlPort = 21;
	if (!ControlSocket.Connect(TargetAddr, ControlPort))
	{
		throw runtime_error("Connect to control port " + to_string(ControlPort) + " failed");
	}
	ControlSocket.SetRecvTimeout(3000);
	ControlSocket.SetSendTimeout(3000);

	if (!Username.empty() && !LoginUser(Username))
	{
		throw runtime_error("Login username failed");
	}

	if (!Password.empty() && !EnterPassword(Password))
	{
		throw runtime_error("Enter password failed");
	}

	return true;
}

bool FTP::ConnectInAnonymous(const std::string& InTargetAddr)
{
	TargetAddr = InTargetAddr;
	constexpr int ControlPort = 21;
	if (!ControlSocket.Connect(TargetAddr, ControlPort))
	{
		throw runtime_error("Connect to control port " + to_string(ControlPort) + " failed");
	}
	ControlSocket.SetRecvTimeout(3000);
	ControlSocket.SetSendTimeout(3000);
	
	AnonymousLogin();
}

void FTP::AnonymousLogin(char* Response /*= nullptr*/, size_t ResponseSize /*= 0*/)
{
	if (!SendCommand("USER anonymous\r\n", Response, ResponseSize))
	{
		throw runtime_error("Send command USER anonymous failed");
	}

	if (!SendCommand("PASS anonymous\r\n", Response, ResponseSize))
	{
		throw runtime_error("Send command PASS anonymous failed");
	}
}

void FTP::DownloadFile(const std::string& ServerPath, char* RecvBuf, size_t BufSize)
{
	// Connect the data transfer port.
	auto TransferSocket = ConnectInPassiveMode(nullptr, 0);

	// Request to download file.
	if (!RequestDownload(ServerPath))
	{
		throw runtime_error("Request download failed");
	}

	// Transfer buffer.
	if (TransferSocket.Recv(RecvBuf, BufSize) < 0)
	{
		throw runtime_error("Receive failed");
	}
}

void FTP::DownloadFile(const std::string& ServerPath, const std::string& LocalPath)
{
	// Connect the data transfer port.
	auto TransferSocket = ConnectInPassiveMode(nullptr, 0);

	// Request to download file.
	if (!RequestDownload(ServerPath))
	{
		throw runtime_error("Request download failed");
	}

	// Transfer file.
	File file(LocalPath);
	char Buf[1024];
	while (true)
	{
		auto Ret = TransferSocket.Recv(Buf, sizeof(Buf));
		if (Ret <= 0)
		{
			break;
		}

		if (file.Write(Buf, Ret) <= 0)
		{
			break;
		}
	}

	// Confirm transfer complete.
	TransferSocket.Close();
}

void FTP::UploadFile(const std::string& ServerPath, char* RecvBuf, size_t BufSize)
{
	// Connect the data transfer port.
	auto TransferSocket = ConnectInPassiveMode(nullptr, 0);

	// Request to upload file.
	if (!RequestUpload(ServerPath))
	{
		throw runtime_error("Request upload failed");
	}

	// Transfer buffer.
	if (TransferSocket.Send(RecvBuf, BufSize) < 0)
	{
		throw runtime_error("Send failed");
	}

	// Confirm transfer complete.
	TransferSocket.Close();
	char Buf[1024];
	auto Ret = ControlSocket.Recv(Buf, sizeof(Buf));
	if (Ret <= 0 || 0 != strncmp(Buf, "226 Transfer complete.\r\n", Ret))
	{
		throw runtime_error("Receive transfer complete failed");
	}
}

void FTP::UploadFile(const std::string& ServerPath, const std::string& LocalPath)
{
	// Connect the data transfer port.
	auto TransferSocket = ConnectInPassiveMode(nullptr, 0);

	// Request to upload file.
	if (!RequestUpload(ServerPath))
	{
		throw runtime_error("Request upload failed");
	}

	// Transfer file.
	File file(LocalPath);
	char Buf[1024];
	while (true)
	{
		auto Ret = file.Read(Buf, sizeof(Buf));
		if (Ret <= 0)
		{
			break;
		}

		if (TransferSocket.Send(Buf, Ret) <= 0)
		{
			break;
		}
	}

	// Confirm transfer complete.
	TransferSocket.Close();
	auto Ret = ControlSocket.Recv(Buf, sizeof(Buf));
	if (Ret <= 0 || 0 != strncmp(Buf, "226 Transfer complete.\r\n", Ret))
	{
		throw runtime_error("Receive transfer complete failed");
	}
}

bool FTP::SendCommand(const std::string& Cmd, char* Response, size_t ResponseSize)
{
	if (ControlSocket.Send(Cmd.c_str(), Cmd.size()) <= 0)
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

	auto Ret = ControlSocket.Recv(Response, ResponseSize);
	if (Ret <= 0 || 0 == strncmp(Response, "550", 3))
	{
		return false;
	}

	if (Ret < ResponseSize)
	{
		Response[Ret] = '\0';
	}
	return true;
}

TcpClientSocket FTP::ConnectInPassiveMode(char* Response, size_t ResponseSize)
{
	char Buf[1024];
	if (!Response)
	{
		Response = Buf;
		ResponseSize = sizeof(Buf);
	}

	if (!SendCommand("PASV\r\n"s, Response, ResponseSize))
	{
		throw runtime_error("Send command PASV failed");
	}

	int p1, p2;
	if (sscanf(Response, "227 Entering Passive Mode (%*d,%*d,%*d,%*d,%d,%d)", &p1, &p2) != 2)
	{
		throw runtime_error("Parse PASV response port failed");
	}
	int ServerTransferPort = p1 * 256 + p2;

	TcpClientSocket TransferSocket;
	if (!TransferSocket.Connect(TargetAddr, ServerTransferPort))
	{
		throw runtime_error("Connect to transfer port failed");
	}
	return TransferSocket;
}