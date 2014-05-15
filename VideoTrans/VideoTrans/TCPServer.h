#define CV_2
#ifdef CV_2
#include <iostream>
#pragma comment( lib, "ws2_32.lib" )
#include <Windows.h>
#include <conio.h>
class TCPServer
{
	WSADATA wsaData;	
	SOCKET s;
	SOCKET n;
	int p;
public:
	TCPServer(char* add, int port)
	{
		p=port;
		WSAStartup(MAKEWORD(2,0),&wsaData);
		//s=socket(AF_INET, /*SOCK_DGRAM*/ SOCK_STREAM, IPPROTO_TCP);
		s=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	
		if (s == SOCKET_ERROR)
			return;
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port =htons(port);
		addr.sin_addr.s_addr= htonl(INADDR_ANY);
		bind(s,(SOCKADDR*)&addr, sizeof(addr));
		if(listen(s, 5) == SOCKET_ERROR) 
			return;
		int isize= sizeof(addr);
		n = accept(s, (struct sockaddr*)&addr, &isize);
	}
	int SendTo(char* buf, int size)
	{
		return send(n, buf, size, 0);
	}
	int Recv(char* buf, int len)
	{
		return recv(n, buf, len,0);
	}
	~TCPServer()
	{
		closesocket(s);
		WSACleanup();
	}
};
#endif