#include "pch.h"
#include <iostream>

#include <winsock2.h>
#include <mswsock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;
}

int main()
{

	// 윈속 초기화 (ws2_32 라이브러리 초기화)
	// 관련 정보가 wsaData에 채워짐
	WSAData wasData;
	if (::WSAStartup(MAKEWORD(2, 2), &wasData) != 0)
		return 0;

	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}

	u_long on = 1;
	// 논블로킹 설정
	if (::ioctlsocket(clientSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	// Connect
	while (true)
	{
		if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		{
			// 원래 블록했어야 했는데 블로킹이라면?
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			// 이미 연결된 상태라면
			if (::WSAGetLastError() == WSAEISCONN)
				break;
			// Error
			break;
		}
		
	}
	cout << "Connected to Server!" << endl;

	char sendBuffer[1000] = "Hello World!";

	// send
	while (true)
	{
		if (::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			// 블로킹 외 다른 오류라면 스탑!
			break;
		}

		cout << "Send Data ! Len = " << sizeof(sendBuffer) << endl;
		// Recv
		while (true)
		{
			char recvBuffer[1000];
			int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen == SOCKET_ERROR)
			{
				// 원래 블록했어야 했는데..
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
					continue;

				// 블로킹 외 다른 오류라면 스탑!
				break;
			}
			else if (recvLen == 0)
			{
				// 연결 끊김
				break;
			}

			cout << "Recv Data Len = " << recvLen << endl;
			break;
		}

		this_thread::sleep_for(1s);
	}


	

	// 소켓 리소스 반환
	::closesocket(clientSocket);

	// 윈속 종료
	::WSACleanup();
}
