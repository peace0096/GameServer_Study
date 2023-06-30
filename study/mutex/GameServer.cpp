#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

vector<int32> v;

// Mutual Exclusive(상호배타적)
mutex m;

// RAII (Resource Acquisition Is Initialization)
template<typename T>
class LockGuard
{
public:
	LockGuard(T& m)
	{
		_mutex = &m;
		_mutex->lock();
	}
	~LockGuard()
	{
		_mutex->unlock();
	}

private:
	T* _mutex;
};

void Push()
{
	for (int32 i = 0; i < 10000; i++)
	{
		// 자물쇠 잠근다. 다른 사람들이 못 쓰게!
		// 자동문임. 생성자에서 락을 건 뒤, 실행이 끝나면 자동으로 소멸되면서 잠금 해제
		LockGuard<mutex> lockguard(m);
		//lock_guard<mutex> lockGuard(m);	// 위와 같음
		//unique_lock<mutex> uniqueLock(m, defer_lock);	// 잠금 시점을 조정할 수 있음. 그 외엔 똑같음.
		//uniqueLock.lock();
		//m.lock();

		v.push_back(i);

		// 자물쇠를 푼다. 이제 다른 사람들도 사용할 수 있다.
		//m.unlock();
	}
		
}

int main()
{
	v.reserve(20000);

	thread t1(Push);
	thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;

}
