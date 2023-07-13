#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "RefCountable.h"
#include "Memory.h"
#include "LockFreeStack.h"

class Data	// : public SListEntry
{
public:
	SListEntry _entry;

	int64 _rand = rand() % 1000;
};

class Knight
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}

	Knight(int32 hp) : _hp(hp)
	{
		cout << "Knight(hp)" << endl;
	}

	~Knight()
	{
		cout << "~Knight()" << endl;
	}

	int32 _hp = 100;
	int32 _mp = 30;
};

SListHeader* GHeader;

int main()
{

	// memory pool #1
	//for (int32 i = 0; i < 5; i++)
	//{
	//	GThreadManager->Launch([]()
	//		{
	//			while (true)
	//			{
	//				Vector<Knight> v(10);

	//				Map<int32, Knight> m;
	//				m[100] = Knight();

	//				this_thread::sleep_for(10ms);
	//			}
	//		});
	//}
	//GThreadManager->Join();

	// memory pool #2
	//SListHeader header;
	//InitializeHead(&header);

	//Data* data = new Data();
	//data->_hp = 10;
	//data->_mp = 20;
	//PushEntrySList(&header, (SListEntry*)data);

	//Data* popData = (Data*)PopEntrySList(&header);

	// memory pool (ABA문제 해결)
	GHeader = new SListHeader();
	ASSERT_CRASH( ((uint64)GHeader % 16) == 0);
	InitializeHead(GHeader);
	
	for (int32 i = 0; i < 3; i++)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
					Data* data = new Data();
					ASSERT_CRASH(((uint64)GHeader % 16) == 0);

					PushEntrySList(GHeader, (SListEntry*)data);
					this_thread::sleep_for(10ms);
				}
			});
	}

	for (int32 i = 0; i < 3; i++)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
					Data* pop = nullptr;
					pop = (Data*)PopEntrySList(GHeader);

					if (pop)
					{
						cout << pop->_rand << endl;
						delete pop;
					}
					else
					{
						cout << "NONE" << endl;
					}
				}
			});
	}
	GThreadManager->Join();
}

