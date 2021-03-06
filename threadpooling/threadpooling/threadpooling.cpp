// threadpooling.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <functional>
#include <vector>
#include <thread>
#include <condition_variable>

using namespace std;

class threadpool
{
public:
	explicit threadpool(size_t numthread)
	{
		start(numthread);
	}

	~threadpool()
	{
		stop();
	}


private:
	vector<thread> m_thread;
	condition_variable m_eventVar;
	mutex m_eventMutex;

	bool m_Stopping = false;

	void start(size_t numthread)
	{

		for (auto i = 0u; i < numthread; i++)
		{
			m_thread.emplace_back([=] {

				while (true)
				{
					unique_lock<mutex> lock{ m_eventMutex };

					m_eventVar.wait(lock, [=] {return m_Stopping; });
					
					if (m_Stopping)
						break;
				}

			});
		}

	}

	void stop()noexcept
	{
		{
			unique_lock<mutex> lock{ m_eventMutex };
			m_Stopping = true;
		}
		m_eventVar.notify_all();
		
		for (auto &thread : m_thread)
		{
			thread.join();
		}
	}
};


int main()
{
	threadpool poll{30};

    std::cout << "Hello World!\n"; 
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
