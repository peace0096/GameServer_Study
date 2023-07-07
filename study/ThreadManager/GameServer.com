#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

CoreGlobal Core;


void ThreadMain()
{
	while (true)
	{
		cout << "Hello ! I am thread" << LThreadId << endl;
		this_thread::sleep_for(1s);
	}
}

int main()
{
	//	CRASH("Test");	// 임의로 크래쉬를 일으키고 싶을 때 사용

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch(ThreadMain);
	}
	GThreadManager->Join();
}

