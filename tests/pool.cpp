/*
Copyright (c) 2012 Jared Duke

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

#include <tinythread_pool.h>

#include "timer.h"

using namespace tthread;
using namespace std;

///////////////////////////////////////////////////////////////////////////

template <int m, int n> 
struct ackermann_t {
	enum { value = ackermann_t<m-1, ackermann_t<m, n-1>::value>::value }; 
}; 

template <int m> struct ackermann_t<m, 0> {
	enum { value = ackermann_t<m-1, 1>::value }; 
}; 

template <int n> struct ackermann_t<0, n> { 
	enum { value = n + 1 }; 
};

///////////////////////////////////////////////////////////////////////////

int ackermann(int m, int n) {
	if (m == 0) return n + 1;
	if (n == 0) return ackermann(m - 1, 1);
	return ackermann(m - 1, ackermann(m, n - 1));
}

///////////////////////////////////////////////////////////////////////////

int ackermann2(int m, int n) {
  while (m > 0) {
    if (n == 0)
      n = 1;
    else {
      n = ackermann2(m, n - 1);
    }
    m = m - 1;
  }

  return n + 1;
}
///////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {

  std::cout << "AckermanT: " << ackermann_t<3, 6>::value << std::endl;

	unsigned pool_size = thread_pool::DEFAULT_POOL_SIZE;
	if (argc > 1) {
		std::istringstream ss(argv[1]);
		ss >> pool_size;
	}

	thread_pool pool(pool_size);

	{
		vector<future<void>> mFutures;

		timed_run session("Ackerman 10");

		for (int i = 0; i < 10; ++i) {
			mFutures.emplace_back(
				pool.submit_task([]() {
					std::cout << "Ackerman(" << this_thread::get_id() << ") = " 
						<< ackermann(3, 11) 
						<< std::endl;
			}));
		}

		for (auto it = begin(mFutures); it != end(mFutures); ++it) {
			it->wait();
		}
	}

	{
		vector<future<int>> mFutures;

		timed_run session("SimpleThread 500000");

		for (int i = 0; i < 500000; ++i) {
			mFutures.emplace_back(pool.submit_task([]() { return ackermann(3,4); }));
		}

		for (auto it = begin(mFutures); it != end(mFutures); ++it) {
			it->wait();
		}
	}
}
