#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"

void UserManager::ProcessSave()
{

	// accountLock
	Account* account = AccountManager::Instance()->GetAccount(100);

	// userLock
	lock_guard<mutex> guard(_mutex);

	// accountLock
	// Account* account = AccountManager::Instance()->GetAccount(100);
	// 이 상태일경우, user와 account 모두 lock이 걸릴 수도 있다.
	// 따라서, accountLock의 순서를 첫번째로 수정하면 deadlock에 걸리지 않는다.
}