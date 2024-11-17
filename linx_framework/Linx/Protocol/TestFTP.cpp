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
//#pragma comment(lib, "ws2_32.lib")  // ���� Winsock ��
//
//// ���ڷ��Ͳ����� FTP ����ĺ���
//bool send_ftp_command(SOCKET sock, const std::string& command, const std::string& expected_response) {
//	char buffer[1024];
//	std::string full_command = command + "\r\n";
//
//	// ��������
//	int send_result = send(sock, full_command.c_str(), full_command.length(), 0);
//	if (send_result == SOCKET_ERROR) {
//		std::cerr << "Send failed. Error: " << WSAGetLastError() << std::endl;
//		return false;
//	}
//
//	// ������Ӧ
//	int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
//	if (bytes_received == SOCKET_ERROR) {
//		std::cerr << "Receive failed. Error: " << WSAGetLastError() << std::endl;
//		return false;
//	}
//
//	buffer[bytes_received] = '\0';  // ȷ���ַ�������
//	std::cout << "Received: " << buffer << std::endl;
//
//	// �����Ӧ���Ƿ����Ԥ��
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
//	const char* server_ip = "192.168.8.196"; // ʾ�� FTP ������
//	const int FTP_PORT = 21;
//
//	// ��ʼ�� Winsock
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//		std::cerr << "WSAStartup failed. Error: " << WSAGetLastError() << std::endl;
//		return 1;
//	}
//
//	// �����׽���
//	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (sock == INVALID_SOCKET) {
//		std::cerr << "Socket creation failed. Error: " << WSAGetLastError() << std::endl;
//		WSACleanup();
//		return 1;
//	}
//
//	// ���÷�������ַ
//	server_addr.sin_family = AF_INET;
//	server_addr.sin_port = htons(FTP_PORT);
//	server_addr.sin_addr.s_addr = inet_addr(server_ip);
//
//	// ���ӵ� FTP ������
//	if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
//		std::cerr << "Connection failed. Error: " << WSAGetLastError() << std::endl;
//		closesocket(sock);
//		WSACleanup();
//		return 1;
//	}
//
//	// ���շ�������ӭ��Ϣ��FTP ��Ӧ���� 220��
//	char buffer[1024];
//	int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
//	if (bytes_received == SOCKET_ERROR) {
//		std::cerr << "Receive failed. Error: " << WSAGetLastError() << std::endl;
//		closesocket(sock);
//		WSACleanup();
//		return 1;
//	}
//
//	buffer[bytes_received] = '\0';  // ȷ���ַ�������
//	std::cout << "Received: " << buffer << std::endl;
//
//	// ���� USER �������������¼
//	if (!send_ftp_command(sock, "USER anonymous", "331")) {
//		std::cerr << "Failed to send USER command" << std::endl;
//		closesocket(sock);
//		WSACleanup();
//		return 1;
//	}
//
//	// ���� PASS ����������������ģ�ͨ��������
//	if (!send_ftp_command(sock, "PASS anonymous", "230")) {
//		std::cerr << "Failed to send PASS command" << std::endl;
//		closesocket(sock);
//		WSACleanup();
//		return 1;
//	}
//
//	std::cout << "Logged in anonymously!" << std::endl;
//
//	// ���� QUIT �����˳�
//	if (!send_ftp_command(sock, "QUIT", "221")) {
//		std::cerr << "Failed to send QUIT command" << std::endl;
//		closesocket(sock);
//		WSACleanup();
//		return 1;
//	}
//
//	std::cout << "Connection closed." << std::endl;
//
//	// �ر��׽��ֲ����� Winsock
//	closesocket(sock);
//	WSACleanup();
//	return 0;
//}
//#endif