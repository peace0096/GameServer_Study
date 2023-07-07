#include "pch.h"
#include "Lock.h"

void Lock::WriteLock()
{
	// ������ �����尡 �����ϰ� �ִٸ� ������ ����
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_writeCount++;
		return;
	}

	// �ƹ��� ���� �� �����ϰ� ���� ���� ��, �����ؼ� �������� ��´�.
	// 32��Ʈ Flag ��� ��� ���� ��!
	// 16��Ʈ �̵� ��, WriteFlag �ۼ�
	const int64 beginTick = ::GetTickCount64();
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
	
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = EMPTY_FLAG;	// Lock�� Ǯ������ ���� ��(��� ����)
			if (_lockFlag.compare_exchange_strong(OUT expected, desired))
			{
				// Write ���� !
				_writeCount++;
				return;
			}
		}

		// �ð� �ʰ�
		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");


		// ��� ����
		this_thread::yield();
	}
}

void Lock::WriteUnlock()
{
	// ReadLock�� �� Ǯ�� ������ WriteUnLock �Ұ���
	if ((_lockFlag.load() & READ_COUNT_MASK) != 0)
		CRASH("INVALID_UNLOCK_ORDER");

	const int32 lockCount = --_writeCount;
	if (lockCount == 0)
		_lockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock()
{
	// ������ �����尡 �����ϰ� �ִٸ� ������ ����
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	// �ƹ��� �����ϰ� ���� ���� �� �����ؼ� ���� ī��Ʈ�� �ø���.
	const int64 beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
			if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
				return;
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::ReadUnlock()
{
	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		CRASH("MULTIPLE_UNLOCK");
}