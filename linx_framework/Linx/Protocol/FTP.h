#pragma once

#include <string>
#include "Linx/IO/Network/TcpClientSocket.h"
#include "Linx/Core.h"

namespace Linx
{
	/**
	 * A simple FTP protocol that can connect to
	 * a server, and download and upload the file
	 * between the server and the local.
	 */
	class FTP
	{
	public:
		/**
		 * Connect to the target address and login.
		 * If Username is null, will not login in.
		 */
		bool Connect(const std::string& InTargetAddr, const std::string& Username = "", const std::string& Password = "");

		/** Connect in anonymous mode. */
		bool ConnectInAnonymous(const std::string& InTargetAddr);

		/**
		 * Enter username to login.
		 * @param Username		The username to login.
		 * @param Response		An array pointer that receives the returned string.
		 * @param ResponseSize	Size of the array that receives the returned string.
		 */
		inline bool LoginUser(const std::string& Username, char* Response = nullptr, size_t ResponseSize = 0)
		{
			return SendCommand("USER " + std::string(Username) + "\r\n", Response, ResponseSize);
		}

		/**
		 * Enter password to login.
		 * @param Password		The password to login.
		 * @param Response		An array pointer that receives the returned string.
		 * @param ResponseSize	Size of the array that receives the returned string.
		 */
		inline bool EnterPassword(const std::string& Password, char* Response = nullptr, size_t ResponseSize = 0)
		{
			return SendCommand("PASS " + std::string(Password) + "\r\n", Response, ResponseSize);
		}

		/** Login without username and password. */
		void AnonymousLogin(char* Response = nullptr, size_t ResponseSize = 0);

		/** Download a file from the server to the buffer or local file. */
		void DownloadFile(const std::string& ServerPath, char* RecvBuf, size_t BufSize);
		void DownloadFile(const std::string& ServerPath, const std::string& LocalPath);

		/** Upload a file from the buffer or local file to the server. */
		void UploadFile(const std::string& ServerPath, char* RecvBuf, size_t BufSize);
		void UploadFile(const std::string& ServerPath, const std::string& LocalPath);

		/** Send the command and receive the response. */
		bool SendCommand(const std::string& Cmd, char* Response = nullptr, size_t ResponseSize = 0);

		/** Returns the control socket. */
		constexpr TcpClientSocket& GetControlSocket() noexcept { return ControlSocket; }

	private:
		/** Connect in passive mode with a transmitted socket. */
		TcpClientSocket ConnectInPassiveMode(char* Response = nullptr, size_t ResponseSize = 0);

		/** Send a download request to the server. */
		inline bool RequestDownload(const std::string& ServerPath)
		{
			return SendCommand("RETR " + ServerPath + "\r\n");
		}
		
		/** Send a upload request to the server. */
		inline bool RequestUpload(const std::string& ServerPath)
		{
			return SendCommand("STOR " + ServerPath + "\r\n");
		}

	private:
		TcpClientSocket ControlSocket;

		std::string TargetAddr;
	};
}