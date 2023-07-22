#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include <winsock2.h>
#include <mswsock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;
}

const int32 BUFSIZE = 1000;

struct Session
{
	// 시작주소는 Overlapped 구조체를 가리킴
	WSAOVERLAPPED overlapped = {};
	SOCKET socket = INVALID_SOCKET;
	char recvBuffer[BUFSIZE] = {};
	int32 recvBytes = 0;
};

void CALLBACK RecvCallBack(DWORD error, DWORD recvLen, LPWSAOVERLAPPED overlapped, DWORD flags)
{
	cout << "Data Recv Len Callback = " << recvLen << endl;
	// TODO : 에코 서버를 만든다면

	// 시작값이 overlapped 구조체이므로, 이렇게 캐스팅해서 보낼 수 있음.
	Session* session = (Session*)overlapped;
}

int main()
{
	WSAData wasData;
	if (::WSAStartup(MAKEWORD(2, 2), &wasData) != 0)
		return 0;

	// 블로킹(Blocking) 소켓
	// accept -> 접속한 클라가 있을 때
	// connect -> 서버 접속 성공했을 때
	// send, sendto -> 요청한 데이터를 송신 버퍼에 복사했을 때
	// recv, recvfrom -> 수신 버퍼에 도착한 데이터가 있고 이를 유저레벨 버퍼에 복사했을 때

	// 논블로킹(Non-Blocking)

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	return 0;

	u_long on = 1;
	// 논블로킹 설정
	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
	return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	return 0;

	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	return 0;

	cout << "Accept" << endl;

	// Overlapped 모델 (이벤트 기반)
	// - 비동기 입출력 지원하는 소켓 생성
	// - 비동기 입출력 함수 호출 (1에서 만든 이벤트 객체를 같이 넘겨줌)
	// - 비동기 작업이 바로 완료되지 않으면, WSA_IO_PENDING 오류 코드
	// - 비동기 입출력 함수 호출한 쓰레드를 -> Alertable Wait 상태로 만든다
	// ex) WaitForSingleObjectEx, WaitForMultipleObjectsEx, SleepEx, WSAWaitForMultipleEvents
	// - 비동기 IO 완료되면, 운영체제는 완료 루틴 호출
	// - 완료 루틴 호출이 모두 끝나면, 쓰레드는 Alertable Wait 상태에서 빠져나온다.

	// 1) 오류 발생 시 0 아닌 값
	// 2) 전송 바이트 수
	// 3) 비동기 입출력 함수 호출 시 넘겨준 WSAOVERLAPPED 구조체의 주소값
	// 4) 0
	// void CompletionRoutine()

	// Select 모델
	// - 장점) 윈도우/리눅스 공통
	// - 단점) 선응 최하 (매번 등록 비용), 64개 제한
	// WSAEventSelect 모델
	// - 장점) 비교적 뛰어난 성능
	// - 단점) 64개 제한
	// Overlapped(이벤트 기반)
	// - 장점) 비교적 뛰어난 성능
	// - 단점) 64개 제한
	// Overlapped(콜백 기반)
	// - 장점) 성능
	// - 단점) 모든 비동기 소켓 함수에서 사용 가능하진 않음(accept). 빈번한 Alertable Wait으로 인한 성능 하락
	// IOCP 앞으로 배울 것!

	// Reactor Pattern (뒤늦게 논블로킹 소켓. 소켓 상태 확인 후 -> 뒤늦게 recv send 호출)
	// Proactor Pattern (미리 recv send 호출. Overlapped WSA)

	while (true)
	{
		SOCKADDR_IN clientAddr;
		int32 addrLen = sizeof(clientAddr);

		SOCKET clientSocket;

		while (true)
		{
			clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
				break;

			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			// 그외 오류 상황 -> 종료
			return 0;
		}

		Session session = Session{ clientSocket };
		WSAEVENT wsaEvent = ::WSACreateEvent();
		

		cout << "Client Connected" << endl;

		while (true)
		{
			WSABUF wsaBuf;
			wsaBuf.buf = session.recvBuffer;
			wsaBuf.len = BUFSIZE;

			DWORD recvLen = 0;
			DWORD flags = 0;

			if (::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &session.overlapped, RecvCallBack) == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSA_IO_PENDING)
				{
					// Pending
					//Alertable Wait
					::SleepEx(INFINITE, TRUE);
					//::WSAWaitForMMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, TRUE);
				}
				else
				{
					// TODO : 문제있는 상황
					break;
				}

			}
			cout << "Data Recv Len = " << recvLen << endl;

		}
	}

		

			
		
	

	// 윈속 종료
	::WSACleanup();
}

