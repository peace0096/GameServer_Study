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
#include "Allocator.h"


using TL = TypeList <class Player, class Mage, class Knight, class Archer>;

class Player
{
public:
	Player()
	{
		INIT_TL(Player);
	}

	virtual ~Player() {}

	DECLARE_TL;
};

class Knight : public Player
{
public:
	Knight() { INIT_TL(Knight); }
};

class Mage : public Player
{
public:
	Mage() { INIT_TL(Mage); }
};

class Archer : public Player
{
public:
	Archer() { INIT_TL(Archer); }
};

class Dog
{

};

int main()
{
	// TypeCast 구현 계기
	/*
	{
		Player* p1 = new Knight();
		Player* p2 = new Mage();

		// 만약 Knight와 Player가 각 필요한 메모리 용량이 다를경우, 메모리 오염 발생할 수 있음.
		Knight* k1 = static_cast<Knight*>(p2);

		// 캐스팅이 되지 않는다면, nullptr로 반환함. 그러나 느림.
		Knight* k2 = static_cast<Knight*>(p2);

		// 그나마 타입을 멤버변수에 명시해주면 이런식으로 캐스팅가능.
		// 아래 작업을 좀 더 안전하고 빠르게 사용할 수 있도록 코딩해야 함!
		if (p1->_type == KNIGHT)
		{
			Knight* k3 = static_cast<Knight*>(p1);
		}
	}
	*/
	
	
	/*
	TypeList<Mage, Knight>::Head whoAMI;	// Mage
	TypeList<Mage, Knight>::Tail whoAMI2;	// Knight

	TypeList<Mage, TypeList<Knight, Archer>>::Head whoAMI3;	// Mage
	TypeList<Mage, TypeList<Knight, Archer>>::Tail::Head whoAMI4;	// Knight
	TypeList<Mage, TypeList<Knight, Archer>>::Tail::Tail whoAMI5;	// Archer

	int32 len1 = Length<TypeList<Mage, Knight>>::value;				// 2
	int32 len2 = Length<TypeList<Mage, Knight, Archer>>::value;		// 3

	using TL = TypeList <Player, Mage, Knight, Archer>;
	TypeAt<TL, 0>::Result whoAMI6;	// Type:Player
	TypeAt<TL, 1>::Result whoAMI7;	// Type:Mage
	TypeAt<TL, 2>::Result whoAMI8;	// Type:Knight

	int32 index1 = IndexOf<TL, Mage>::value;	// 1
	int32 index2 = IndexOf<TL, Archer>::value;	// 2
	int32 index3 = IndexOf<TL, Dog>::value;		// -1

	bool canConvert1 = Conversion<Player, Knight>::exists;	//	0
	bool canConvert2 = Conversion<Knight, Player>::exists;	//	1
	bool canConvert3 = Conversion<Knight, Dog>::exists;		//  0
	*/

	{
		Player* player = new Player();

		bool canCast = CanCast<Knight*>(player);
		Knight* knight = TypeCast<Knight*>(player);

		delete player;
	}

	{
		shared_ptr<Knight> knight = MakeShared<Knight>();
		shared_ptr<Knight> player = MakeShared<Knight>();

		//shared_ptr<Player> player = TypeCast<Archer>(knight);
		bool canCast = CanCast<Player>(knight);
	}

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{

				}
			});
	}

	GThreadManager->Join();
}

