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

//void* operator new(size_t size)
//{
//	cout << "new!" << size << endl;
//	void* ptr = ::malloc(size);
//	return ptr;
//}
//
//void operator delete(void* ptr)
//{
//	cout << "delete!" << endl;
//	::free(ptr);
//}
//
//void* operator new[](size_t size)
//{
//	cout << "new[]!" << size << endl;
//	void* ptr = ::malloc(size);
//	return ptr;
//}
//
//void operator delete[](void* ptr)
//{
//	cout << "delete[]!" << endl;
//	::free(ptr);
//}

int main()
{
	//Knight* k1 = new Knight();
	//k1->_hp = 120;
	//k1->_mp = 50;
	//delete k1;
	//k1 = nullptr;
	//k1->_hp = 100;	// (Use-After-Free) 값이 들어간다. 메모리 오염 발생!

	// StlAllocator
	// Container.h 에서 using구문 참고
	Vector<Knight> v(100);;
	Map<int32, Knight> m;
	m[100] = Knight();

	// StompAllocate
	Knight* knight = xnew<Knight>(100);
	xdelete(knight);
	knight->_hp = 50;	// 에러 발생
	
}

