#pragma once

#include <string>
#include "Linx/IO/Network/TcpClientSocket.h"
#include "Linx/Core.h"

using namespace std::literals::string_literals;

namespace Linx
{
	class FTP
	{
	public:
		bool Connect(const std::string& InTargetAddr, const std::string& Username = "", const std::string& Password = "");

		bool LoginUser(const std::string& Username, char* Response = nullptr, size_t ResponseSize = 0);

		bool EnterPassword(const std::string& Password, char* Response = nullptr, size_t ResponseSize = 0);

		bool DownloadFile(const std::string& ServerPath, char* RecvBuf, size_t BufSize);
		bool DownloadFile(const std::string& ServerPath, const std::string& LocalPath);

		bool UploadFile(const std::string& ServerPath, char* RecvBuf, size_t BufSize);
		bool UploadFile(const std::string& ServerPath, const std::string& LocalPath);

		bool SendCommand(const std::string& Cmd, char* Response = nullptr, size_t ResponseSize = 0);

		constexpr TcpClientSocket& GetControlIO() noexcept { return ControlIO; }
		constexpr TcpClientSocket& GetTransmitIO() noexcept { return TransmitIO; }

	private:
		bool ConnectInPassiveMode(char* Response = nullptr, size_t ResponseSize = 0);
		bool RequestDownload(const std::string& ServerPath);
		bool RequestUpload(const std::string& ServerPath);

	private:
		TcpClientSocket ControlIO;
		TcpClientSocket TransmitIO;

		std::string TargetAddr;

		int ServerTransmitPort = -1;
	};
}