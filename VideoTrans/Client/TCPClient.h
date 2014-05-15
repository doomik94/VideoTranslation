#define CV_1

#ifdef CV_1
#include <iostream>
#pragma comment( lib, "ws2_32.lib" )
#include <Windows.h>
#include <conio.h>
#pragma comment(lib, "Ws2_32.lib")
class TCPClient
{
	int p;
	WSADATA wsaData;
	SOCKET s;
	struct sockaddr_in addr;
public:
	TCPClient(char* add, int port)
	{
		p=port;
		WSAStartup(MAKEWORD(2,0),&wsaData);
		s=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	
		addr.sin_family = AF_INET;
		addr.sin_port =htons(port);
		addr.sin_addr.s_addr= inet_addr(add);
		
		connect(s, (SOCKADDR*)&addr, sizeof(SOCKADDR));
	}
	int SenTo(char* buf, int size)
	{
		return send(s, buf, size, 0);
	}
	int Recv(char* buf, int len)
	{
		return recv(s, buf, len, 0);
	}
	~TCPClient()
	{
		closesocket(s);
		WSACleanup();
	}
};
#endif