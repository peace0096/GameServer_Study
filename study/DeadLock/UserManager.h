#pragma once
#include <mutex>

class User
{

};


class UserManager
{
public:
	static UserManager* Instance()
	{
		static UserManager instance;
		return &instance;
	}

	User* GetUser(int32 id)
	{
		lock_guard<mutex> guard(_mutex);
		// 특정 유저를 가져옴
		return nullptr;
	}

	void ProcessSave();

private:
	mutex _mutex;
};

