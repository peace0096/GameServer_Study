#pragma once

/*------------------
	  BaseAllocator
-------------------*/

class BaseAllocator
{
public:
	static void* Alloc(int32 size);
	static void Release(void* ptr);
};


// 버그찾는데 유용
/*------------------
	  StompAllocator
-------------------*/

class StompAllocator
{
	enum { PAGE_SIZE = 0x1000 };

public:
	static void* Alloc(int32 size);
	static void Release(void* ptr);
};

/*------------------
	  Pool Allocator
-------------------*/

class PoolAllocator
{

public:
	static void* Alloc(int32 size);
	static void Release(void* ptr);
};

/*------------------
	  STL Allocator
-------------------*/
template<typename T>
class StlAllocator
{
public:
	using value_type = T;

	StlAllocator() {}

	template<typename Other>
	StlAllocator(const StlAllocator<Other>&) { }

	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32>(count * sizeof(T));
		return static_cast<T*>(Xalloc(size));
	}

	void deallocate(T* ptr, size_t count)
	{
		Xrelease(ptr);
	}
};