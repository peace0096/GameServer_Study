#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include "AccountManager.h"
#include "UserManager.h"

void Func1()
{
	for (int32 i = 0; i < 1000; i++)
	{
		UserManager::Instance()->ProcessSave();
	}
}

void Func2()
{
	for (int32 i = 0; i < 1000; i++)
	{
		AccountManager::Instance()->ProcessLogin();
	}
}


int main()
{
	thread t1(Func1);
	thread t2(Func2);

	t1.join();
	t2.join();

	cout << "Jobs Done" << endl;
}

/*
Readme
누군가가 이미 화장실을 사용하고 있을 때의 해결방법
1. 그냥 기다린다.
2. 일단 다른 곳에 갔다가, 나중에 다시 온다(단, 순서 밀릴수도 있음)
3. 자리 찜하고 다른 곳 갔다가 다시 온다.

2,3번은 이동하는데에 리소스가 소모됨.
*/
