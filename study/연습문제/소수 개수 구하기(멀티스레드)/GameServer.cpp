#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include <vector>
#include <thread>

bool isPrime(int number)
{
	int primeCount = 0;
	for (int i = 1; i < number; i++)
	{
		if (number % i == 0)
			primeCount++;
		if (primeCount >= 2)
			return false;
	}
	if (primeCount == 1)
		return true;

	return false;
}

int getPrimeCount(int start, int end)
{
	int count = 0;
	
	for (int i = start; i <= end; i++)
	{
		if (isPrime(i))
			count++;
	}
	return count;
}

int main()
{
	const int MAX_NUMBER = 100'0000;
	vector<thread> threads;
	int primeCount = 0;

	int coreCount = thread::hardware_concurrency();
	int jobCount = MAX_NUMBER / coreCount + 1;
	
	for (int i = 0; i < coreCount; i++)
	{
		int start = (i * jobCount) + 1;
		int end = min(MAX_NUMBER, ((i + 1) * jobCount));

		threads.push_back(thread([start, end, &primeCount]()
			{
				primeCount += getPrimeCount(start, end);

			}));
	}

	for (thread& t : threads)
		t.join();


	cout << primeCount << endl;
}

