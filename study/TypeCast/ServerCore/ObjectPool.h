#pragma once

#include "Types.h"
#include "MemoryPool.h"

// Ŭ���� ��ü�� ���� �� �ִ� Pool
template<typename Type>
class ObjectPool
{
public:
	template<typename... Args>
	static Type* Pop(Args&&... args)
	{
// Types.h�� _STOMP ���ǰ� �Ǿ��ִٸ�
#ifdef _STOMP
		MemoryHeader* ptr = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(s_allocSize));
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(ptr, s_allocSize));
// ���ǰ� ���ٸ�
#else
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(s_pool.Pop(), s_allocSize));
#endif

		
		new(memory)Type(forward<Args>(args)...);	// placement new
		return memory;
	}

	static void Push(Type* obj)
	{
		obj->~Type();
#ifdef _STOMP
		StompAllocator::Release(MemoryHeader::DetachHeader(obj));
#else
		s_pool.Push(MemoryHeader::DetachHeader(obj));
#endif
		
	}

	static shared_ptr<Type> MakeShared()
	{
		// �ι�° ���ڷ� ����
		shared_ptr<Type> ptr = { Pop(), Push };
		return ptr;

	}

private:
	static int32		s_allocSize;
	static MemoryPool	s_pool;
};

template<typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool{ s_allocSize };