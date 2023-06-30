#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

class SpinLock
{
public:
	void lock()
	{
		// CAS (Compare-And-Swap)
		bool expected = false;	// 예상값. 문이 열려있다면?
		bool desired = true;	// 만약 예상값이라면, 해당 desired로 바꿔줌

		
		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;
		}

	}

	void unlock()
	{
		//_locked = false;
		_locked.store(false);
	}

private:
	// volatile : 최적화하지 말아달라고 요청. 근데 쓸 일은 별로 없을 거임.
	// atomic 원자적으로 사용하겠다고 선언.
	atomic<bool> _locked = false;	// 잠긴 상태인가?
};


int32 sum = 0;
mutex m;
SpinLock spinLock;

void Add()
{
	for (int32 i = 0; i < 10'0000; i++)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum++;
	}
}

void Sub()
{
	for (int32 i = 0; i < 10'0000; i++)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum--;
	}
}

int main()
{

	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;

}


/*
	스핀락
	
	동시에 들어와서 자물쇠에 걸어버림.
	문제를 해결하려면 작업을 시작하고 메모리 점유와 자물쇠 잠금이 동시에 일어나야 함.

	_locked.compare_exchange_strong 을 통해 기다리다가 들어간다.
*/
