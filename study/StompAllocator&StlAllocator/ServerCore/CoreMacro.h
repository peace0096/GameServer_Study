#pragma once

#define OUT

/*----------------------
	  Lock
----------------------*/

#define USE_MANY_LOCKS(count)	Lock _locks[count];
#define USE_LOCK				USE_MANY_LOCKS(1);
#define READ_LOCK_IDX(idx)		ReadLockGuard readLockGuard_##idx(_locks[idx], typeid(this).name());
#define READ_LOCK				READ_LOCK_IDX(0);
#define WRITE_LOCK_IDX(idx)		WriteLockGuard writeLockGuard_##idx(_locks[idx], typeid(this).name());
#define WRITE_LOCK				WRITE_LOCK_IDX(0);

/*----------------------
	  Memory
----------------------*/

#ifdef _DEBUG
#define Xalloc(size)		StompAllocator::Alloc(size)
#define Xrelease(size)		StompAllocator::Release(size)
#else
#define Xalloc(size)		BaseAllocator::Alloc(size)
#define Xrelease(size)		BaseAllocator::Release(size)
#endif

/*----------------------
	  Crash
----------------------*/
#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	_Analysis_assume_(crash != nullptr);	\
	*crash = 0XDEADBEEF;					\
}

#define ASSERT_CRASH(expr)					\
{											\
	if (!(expr))							\
	{										\
		CRASH("ASSERT_CRASH");				\
		_Analysis_assume_(expr);			\
	}										\
}