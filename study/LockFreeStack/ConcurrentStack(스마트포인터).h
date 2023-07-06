#pragma once

// Lock Based Stack

#include <mutex>

// 스마트포인터 대신, 레퍼런스 카운팅을 직접 하는 LockFreeStack
template<typename T>
class LockFreeStack
{
	struct Node;

	struct CountedNodePtr
	{
		int32 externalCount = 0;
		Node* ptr = nullptr;
	};

	struct Node
	{
		Node(const T& value) : data(make_shared<T>(value))
		{

		}

		// 스마트포인터
		shared_ptr<T> data;
		atomic<int32> internalCount = 0;
		CountedNodePtr next;
	};

public:
	void Push(const T& value)
	{
		// 초기화 선언
		CountedNodePtr node;
		node.ptr = new Node(value);
		node.externalCount = 1;

		node.ptr->next = _head;
		while (_head.compare_exchange_weak(node.ptr->next, node) == false)
		{

		}

	}

	shared_ptr<T> TryPop()
	{
		CountedNodePtr oldHead = _head;
		while (true)
		{
			// 참조권 획득
			IncreaseHeadCount(oldHead);
			// 최소한 externalCount >= 2일테니 삭제X (안전하게 접근할 수 있음)
			Node* ptr = oldHead.ptr;

			// 데이터가 없을 때
			if (ptr == nullptr)
				return shared_ptr<T>();

			// 소유권 획득 ( ptr->next로 head를 바꿔치기 한 애가 이김)
			if (_head.compare_exchange_strong(oldHead, ptr->next))
			{
				shared_ptr<T> res;
				res.swap(ptr->data);

				// 나 말고 또 누가 있는가?
				const int32 countIncrease = oldHead.externalCount - 2;

				if (ptr->internalCount.fetch_add(countIncrease) == -countIncrease)
					delete ptr;

				return res;

			}
			else if (ptr->internalCount.fetch_sub(1) == 1)
			{
				// 참조권은 얻었지만, 소유권은 실패 -> 뒷수습은 여기서!
				delete ptr;
			}
			
		}
	}

private:
	void IncreaseHeadCount(CountedNodePtr& oldCounter)
	{
		while (true)
		{
			CountedNodePtr newCounter = oldCounter;
			newCounter.externalCount++;

			if (_head.compare_exchange_strong(oldCounter, newCounter))
			{
				oldCounter.externalCount = newCounter.externalCount;
				break;
			}

		}
	}

private:
	atomic<CountedNodePtr> _head;
};
