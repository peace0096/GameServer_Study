#include <iostream>

using namespace std;

// MMORPG 동접 1~5만명, 몬스터 1~500만마리
// 몬스터 생성 이벤트 : 5분동안 몬스터가 10배 많이 나옴

// - 스택영역
// 함수가 끝나면 같이 정리되는 불안정한 메모리
// - 메모리 영역
// 프로그램이 실행되는 도중에는 무조건 사용되는 영역

// 희망사항)
// 필요할 때만 사용하고, 필요없으면 반납.
// 그러면서 우리가 생성/소멸 시점을 관리할 수 있어야 함.
// 동적할당과 관련된 함수/연산자 : malloc, free, new, delete, new(), delete()

// malloc
// 할당할 메모리 크기를 건네줌
// 메모리 할당 후 시작 주소를 가리키는 포인터를 반환

// free
// malloc (혹은 calloc, realloc) 을 통해 할당한 영역을 해제
// 힙 관리자가 할당/미할당 여부를 구분해서 관리

// new / delete
// C++에서 추가됨
// malloc/free 함수 / new/delete는 연산자

// new[] / delete[]
// new가 malloc에 비해 좋긴 한데, 배열과 같이 N개 데이터를 같이 할당하려면?

// 사용 편의성은 new/delete를 사용.
// 타입에 상관 없이 특정한 크기의 메모리 영역을 할당받으려면 malloc/free 사용.

// 중요한 차이가 있음
// new/delete는(생성타입이 클래스일 경우), 생성자/소멸자를 호출해준다.
 
class Monster
{
public:
	Monster()
	{
		cout << "Monster()" << endl;
	}

	~Monster()
	{
		cout << "~Monster()" << endl;
	}
public:
	int _hp;
	int _x;
	int _y;
};

int main()
{

	// C++ 에서는 기본적으로 CRT(C런타임 라이브러리)의 힙관리자를 통해 힙 영역 사용
	// 단, 정말 원한다면 우리가 직접 API 를 통해 힙을 생성하고 관리할 수도 있음(MMORPG 서버 메모리 풀링)

	// void*란?
	// *가 있으니 포인터는 포인터임.
	// 타고 가면 void 아무것도 없는 것이 아니라, 뭐가 있는지 모름. 그러니까 너가 정해서 써라

	void* pointer = malloc(1000);

	Monster* m1 = (Monster*)pointer;
	m1->_hp = 100;
	m1->_y = 2;
	m1->_x = 1;

	// Heap Overflow
	// 유효한 힙 범위를 초과해서 사용하는 문제(지정한 용량 이상 넘을 경우 에러 뱉음)

	// 만약 free하지 않으면 메모리 누수 발생
	free(pointer);

	// Double Free
	// 이건 대부분 그냥 크래시만 발생

	//Use - After - Free
	// 해제 후, 메모리 사용을 시도 -> 에러

	// new / delete
	Monster* m2 = new Monster;
	m2->_hp = 200;
	m2->_y = 3;
	m2->_x = 2;
	delete m2;

	Monster* m3 = new Monster[5];	// 5마리의 몬스터 생성(생성자 5번 호출)
	m3->_hp = 200;
	m3->_y = 3;
	m3->_x = 2;

	Monster* m4 = (m3 + 1);		//배열처럼 생성 가능
	m4->_hp = 200;
	m4->_y = 3;
	m4->_x = 2;

	delete[] m3;	//소멸자 5번 호출
	return 0;
}
