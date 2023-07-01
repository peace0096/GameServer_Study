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

void Producer()
{
	while (true)
	{
		unique_lock<mutex> lock(m);
		q.push(100);
	}
	::SetEvent(handle);
	//this_thread::sleep_for(100ms);
}

void Consumer()
{
	while (true)
	{
		::WaitForSingleObject(handle, INFINITE);

		unique_lock<mutex> lock(m);
		if (q.empty() == false)
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
