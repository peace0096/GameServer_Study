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

using KnightRef = TSharedPtr<class Knight>;
using InventoryRef = TSharedPtr<class Inventory>;

class Knight
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}

	~Knight()
	{
		cout << "~Knight()" << endl;
	}

	void SetTarget(shared_ptr<Knight> target)
	{
		_target = target;
	}


	shared_ptr<Knight> _target = nullptr;
	// 참조카운트 증가하지 않음!
	weak_ptr<Inventory> _inventory;
};

class Inventory
{
public:
	Inventory(weak_ptr<Knight> knight) : _knight(knight)
	{
		cout << "Inventory()" << endl;
	}

	~Inventory()
	{
		cout << "~Inventory()" << endl;
	}

	// 참조카운트 증가하지 않음!
	weak_ptr<Knight> _knight;
};

int main()
{
	// k1 과 Inventory는 각 멤버변수 weak_ptr을 통해 참조한다.
	shared_ptr<Knight> k1 = make_shared<Knight>();
	k1->_inventory = make_shared<Inventory>(k1);

	cout << "done!" << endl;
	
}

