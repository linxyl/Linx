#include "Linx/TestConfig.h"
#ifdef TEST_FTP
#include <iostream>
#include "FTP.h"

using namespace Linx;
using namespace std;

int main()
{
	FTP ftp;
	if (!ftp.Connect("192.168.8.196", "test", "test"))
	{
		cout << "Connect failed" << endl;
		return -1;
	}

	if (!ftp.UploadFile("example.txt", "upload.txt"))
	{
		cout << "Upload failed" << endl;
		return -1;
	}

	if (!ftp.DownloadFile("example.txt", "download.txt"))
	{
		cout << "Download failed" << endl;
		return -1;
	}

	(void)getchar();

	return 0;
}
#endif

//#include <iostream>
//#include <winsock2.h>
//#include <string>
//
//#pragma comment(lib, "ws2_32.lib")  // 连接 Winsock 库
//
//// 用于发送并接收 FTP 命令的函数
//bool send_ftp_command(SOCKET sock, const std::string& command, const std::string& expected_response) {
//	char buffer[1024];
//	std::string full_command = command + "\r\n";
//
//	// 发送命令
//	int send_result = send(sock, full_command.c_str(), full_command.length(), 0);
//	if (send_result == SOCKET_ERROR) {
//		std::cerr << "Send failed. Error: " << WSAGetLastError() << std::endl;
//		return false;
//	}
//
//	// 接收响应
//	int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
//	if (bytes_received == SOCKET_ERROR) {
//		std::cerr << "Receive failed. Error: " << WSAGetLastError() << std::endl;
//		return false;
//	}
//
//	buffer[bytes_received] = '\0';  // 确保字符串结束
//	std::cout << "Received: " << buffer << std::endl;
//
//	// 检查响应码是否符合预期
//	if (strncmp(buffer, expected_response.c_str(), expected_response.length()) != 0) {
//		std::cerr << "Unexpected response: " << buffer << std::endl;
//		return false;
//	}
//
//	return true;
//}
//
//int main() {
//	WSADATA wsaData;
//	SOCKET sock;
//	sockaddr_in server_addr;
//	const char* server_ip = "192.168.8.196"; // 示例 FTP 服务器
//	const int FTP_PORT = 21;
//
//	// 初始化 Winsock
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//		std::cerr << "WSAStartup failed. Error: " << WSAGetLastError() << std::endl;
//		return 1;
//	}
//
//	// 创建套接字
//	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (sock == INVALID_SOCKET) {
//		std::cerr << "Socket creation failed. Error: " << WSAGetLastError() << std::endl;
//		WSACleanup();
//		return 1;
//	}
//
//	// 设置服务器地址
//	server_addr.sin_family = AF_INET;
//	server_addr.sin_port = htons(FTP_PORT);
//	server_addr.sin_addr.s_addr = inet_addr(server_ip);
//
//	// 连接到 FTP 服务器
//	if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
//		std::cerr << "Connection failed. Error: " << WSAGetLastError() << std::endl;
//		closesocket(sock);
//		WSACleanup();
//		return 1;
//	}
//
//	// 接收服务器欢迎信息（FTP 响应代码 220）
//	char buffer[1024];
//	int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
//	if (bytes_received == SOCKET_ERROR) {
//		std::cerr << "Receive failed. Error: " << WSAGetLastError() << std::endl;
//		closesocket(sock);
//		WSACleanup();
//		return 1;
//	}
//
//	buffer[bytes_received] = '\0';  // 确保字符串结束
//	std::cout << "Received: " << buffer << std::endl;
//
//	// 发送 USER 命令进行匿名登录
//	if (!send_ftp_command(sock, "USER anonymous", "331")) {
//		std::cerr << "Failed to send USER command" << std::endl;
//		closesocket(sock);
//		WSACleanup();
//		return 1;
//	}
//
//	// 发送 PASS 命令，密码可以是任意的，通常是匿名
//	if (!send_ftp_command(sock, "PASS anonymous", "230")) {
//		std::cerr << "Failed to send PASS command" << std::endl;
//		closesocket(sock);
//		WSACleanup();
//		return 1;
//	}
//
//	std::cout << "Logged in anonymously!" << std::endl;
//
//	// 发送 QUIT 命令退出
//	if (!send_ftp_command(sock, "QUIT", "221")) {
//		std::cerr << "Failed to send QUIT command" << std::endl;
//		closesocket(sock);
//		WSACleanup();
//		return 1;
//	}
//
//	std::cout << "Connection closed." << std::endl;
//
//	// 关闭套接字并清理 Winsock
//	closesocket(sock);
//	WSACleanup();
//	return 0;
//}
//#endif