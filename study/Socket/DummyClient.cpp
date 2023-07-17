#include "pch.h"
#include <iostream>

#include <winsock2.h>
#include <mswsock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{

	// 윈속 초기화 (ws2_32 라이브러리 초기화)
	// 관련 정보가 wsaData에 채워짐
	WSAData wasData;
	if (::WSAStartup(MAKEWORD(2, 2), &wasData) != 0)
		return 0;

	// ad : Address Family (AF_INET = IPv4, AF_INET6 = IPv6)
	// type : TCP(SOCK_STREAM)신뢰있는 애 vs UDP(SOCK_DGRAM)빠른 애
	// protocol : 0
	// return : descriptor
	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Socket ErrorCode : " << errCode << endl;
		return 0;
	}

	// 연결할 목적지는? (IP주소 + Port) ->  XX아파트 YY 호 말 그대로 주소!
	SOCKADDR_IN serverAddr;	// IPv4
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	// serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1"); deprecated 요즘 이건 안씀
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	// host to network short
	// Little-Endian vs Big-Endian
	// ex) 0x12345678 이라는 4바이트 정수가 있을 때,
	// low [0x78][0x56][0x34][0x12] high < little
	// low [0x12][0x34][0x56][0x78[ high < big = network
	// htons는 liitle-endian을 big-edian으로 바꿔줌
	// 정수 표현식은 little이 디폴트고, 네트워크 포트는 big을 사용하기 때문.

	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Connect ErrorCode : " << errCode << endl;
		return 0;
	}
	// -------------------------
	// 연결성공! 아래부터 데이터 송수신 가능

	cout << "Connected To Server ! " << endl;

	while (true)
	{
		// TODO

		this_thread::sleep_for(1s);
	}

	// 소켓 리소스 반환
	::closesocket(clientSocket);

	// 윈속 종료
	::WSACleanup();
}
