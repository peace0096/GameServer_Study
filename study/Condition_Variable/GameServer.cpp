#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>

mutex m;
queue<int32> q;
HANDLE handle;

// 참고) CV는 User-Level Object (커널 오브젝트X)
// 조건부 대기 !(이벤트에 조건성이 합쳐진 개념)
condition_variable cv;

void Producer()
{
	while (true)
	{
		// 1) Lock을 잡고
		// 2) 공유변수값을 수정
		// 3) Lock을 풀고
		// 4) 조건변수 통해 다른 쓰레드에게 통지
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}
		
	}

	cv.notify_one();	// wait 중인 쓰레드가 있으면 딱 1개를 깨운다.
	//::SetEvent(handle);
	//this_thread::sleep_for(100ms);
}

void Consumer()
{
	while (true)
	{
		// ::WaitForSingleObject(handle, INFINITE);
		unique_lock<mutex> lock(m);
		cv.wait(lock, []() { return q.empty() == false; });
		// 1) Lock을 잡고
		// 2) 조건 확인
		// -) 만족O => 빠져 나와서 이어서 코드 실행
		// -) 만족X => Lock을 풀어주고 대기 상태

		// 그런데 notify_one을 했으면 항상 조건식을 만족하는거 아닐까?
		// Spurious WeakUp (가짜 기상?)
		// notify_one할 때, Lock을 잡고 있는 것이 아니기 때문임. 
		
		// while (q.empty() == false)	// CV에서 조건 체크 중이기 때문에 생략 가능
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
}


int main()
{
	// 커널 오브젝트
	// Usage Count
	// Signal (파란불) / Non-Signal (빨간불) << bool
	// Auto / Manual << bool

	handle = ::CreateEvent(NULL/*보안속성*/, FALSE/*bManualReset*/, FALSE/*binitialState*/, NULL);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);

}

/*
* Event
  Producer에서 언제 작업을 끝내는지 모름.
  근데 Consumer는 이를 모른 채, 계속 트라이하기 때문에 cpu점유율이 높아짐.
  이를 해결하기 위해 '이벤트'를 활용.

  프로듀서가 일을 끝냈다고 컨슈머에게 신호를 보냄!


*/
