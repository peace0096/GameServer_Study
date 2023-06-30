#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>

// 두 스레드에서 수정을 하면 이상한 값이 나옴. 덮어쓰기 때문.
// 동기화 필요!

// Atomic : All-or-Nothing
atomic<int32> sum = 0;

void Add()
{
	for (int32 i = 0; i < 100'0000; i++)
	{
		sum.fetch_add(1);
		/*
		int32 eax = sum;
		// eax = 0
		eax = eax + 1;
		// eax = 1
		sum = eax;
		// sum = 1
		*/
	}
}

void Sub()
{
	for (int32 i = 0; i < 100'0000; i++)
	{
		sum.fetch_add(-1);
		/*
		int32 eax = sum;
		// eax = 0
		eax = eax - 1;
		// eax = -1
		sum = eax;
		// sum = -1
		*/
	}
}


int main()
{

	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();
	cout << sum << endl;

    return 0;

}
