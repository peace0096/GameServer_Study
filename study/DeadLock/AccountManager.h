#pragma once
#include <mutex>

class Account
{
	// TODO
};

class AccountManager
{
public:
	// 싱글톤
	static AccountManager* Instance()
	{
		static AccountManager instance;
		return &instance;
	}

	Account* GetAccount(int32 id)
	{
		lock_guard<mutex> guard(_mutex);
		// 특정 데이터를 가져올 것임
		return nullptr;
	}

	void ProcessLogin();

private:
	mutex _mutex;
	// map<int32, Account*> _accounts;
};

