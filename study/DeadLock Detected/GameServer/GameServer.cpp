#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "PlayerManager.h"
#include "AccountManager.h"

int main()
{
	GThreadManager->Launch([=]
		{
			while (true)
			{
				cout << "PlayerThenAccount" << endl;
				GPlayerManager.PlayerThenAccount();
				this_thread::sleep_for(1000ms);
			}
		});
	GThreadManager->Launch([=]
		{
			while (true)
			{
				cout << "AccountThenManager" << endl;
				GAccountManager.AccountThenPlayer();
				this_thread::sleep_for(1000ms);
			}
		});

	GThreadManager->Join();
}

