#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>

void HelloThread()
{
    cout << "Hello World" << endl;
}

void HelloThread2(int32 num)
{
    cout << num << endl;
}

int main()
{
    // Thread 기본
    /*
    
    thread t;

    auto id1 = t.get_id(); // 쓰레드 아이디

    t = thread(HelloThread);

    int32 count = t.hardware_concurrency(); // CPU 코어 개수
    auto id2 = t.get_id(); // 쓰레드 아이디

    if(t.joinable())    // join 가능한지?
        t.join();   // 메인스레드가 먼저 끝날수도 있음. 따라서, join으로 해당 스레드가 끝날 때까지 기다려줘야함.

    // t.detach(); // thread객체에서 실제 쓰레드를 분리(=>백그라운드 전환). 이후에 자원 참조를 못함..
    
    cout << "Hello Main" << endl;
    */

    vector<thread> v;

    for (int32 i = 0; i < 10; i++)
    {
        v.push_back(thread(HelloThread2, i));
    }

    for (int32 i = 0; i < 10; i++)
    {
        if (v[i].joinable())
            v[i].join();
    }

    cout << "Hello Main" << endl;
    


}
