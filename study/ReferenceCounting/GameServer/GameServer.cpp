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

class Wraight : public RefCountable
{
public:
	int _hp = 150;
	int _poxX = 0;
	int _poxY = 0;
};

using WraightRef = TSharedPtr<Wraight>;


class Missile : public RefCountable
{
public:
	void SetTarget(WraightRef target)
	{
		_target = target;
		// target->AddRef();
	}

	bool Update()
	{
		if (_target == nullptr)
			return true;
		int posX = _target->_poxX;
		int posY = _target->_poxY;

		// TODO : 쫓아간다.

		if (_target->_hp == 0)
		{
			_target->ReleaseRef();
			_target = nullptr;
			return true;
		}
			
		return false;
	}

	WraightRef _target = nullptr;

};


using MissileRef = TSharedPtr<Missile>;

int main()
{
	WraightRef wraight(new Wraight());
	wraight->ReleaseRef();
	MissileRef missile(new Missile());
	missile->ReleaseRef();

	missile->SetTarget(wraight);

	// 레이스가 피격당함
	wraight->_hp = 0;
	// wraight->ReleaseRef();
	wraight = WraightRef(nullptr);	// TSharedPtr& operator= 에서 Release()를 통해 알아서 메모리 비워줌
	// delete wraight;	// 어 이러면 미사일이 가리키는 포인터도 삭제되지 않나..

	while (true)
	{
		if (missile)
		{
			if (missile->Update())
			{
				missile->ReleaseRef();
				missile = nullptr;

			}
		}
	}

	missile->ReleaseRef();
	missile = nullptr;
	//delete missile;
}

