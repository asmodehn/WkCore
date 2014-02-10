//including only what we need to test
#include "UnitTest++/UnitTest++.h"
#include "Core/Thread/Thread.hh"
#include "Core/Thread/Mutex.hh"
#include "Core/Thread/FastMutex.hh"
#include "Core/Thread/AtomicFlag.hh"
#include "Core/Thread/Atomic.hh"
#include "Core/Thread/ConditionVariable.hh"
#include "Core/Thread/LockGuard.hh"

#include "Core/DateTime/ChronoDuration.hh"

//includes needed to run the test and write xml report
#include <iostream>
#include <fstream>
#include <cstring>
#include <list>
#include "UnitTest++/XmlTestReporter.h"

//to signal unimplemented test
bool NOT_IMPLEMENTED = false;

using namespace Core::Thread;

SUITE(Thread)
{

// HACK: Mac OS X and early MinGW do not support thread-local storage
#if defined(__APPLE__) || (defined(__MINGW32__) && (__GNUC__ < 4))
 #define NO_TLS
#endif

// Thread local storage variable
#ifndef NO_TLS
thread_local int gLocalVar;
#endif

// Mutex + global count variable
Mutex gMutex;
FastMutex gFastMutex;
AtomicFlag gFlag(ATOMIC_FLAG_INIT);
int gCount;
Atomic<int> gAtomicCount;

// Condition variable
ConditionVariable gCond;

// Thread function: Thread ID
Core::Thread::Thread::Id tid;
void ThreadIDs(void * aArg)
{
  tid = ThisThread::get_id();
}

#ifndef NO_TLS
// Thread function: Thread-local storage
void ThreadTLS(void * aArg)
{
  gLocalVar = 2;
  std::cout << " My gLocalVar is " << gLocalVar << "." << std::endl;
}
#endif

// Thread function: Mutex locking
void ThreadLock(void * aArg)
{
  for(int i = 0; i < 10000; ++ i)
  {
    LockGuard<Mutex> lock(gMutex);
    ++ gCount;
  }
}

// Thread function: Fast mutex locking
void ThreadLock2(void * aArg)
{
  for(int i = 0; i < 10000; ++ i)
  {
    LockGuard<FastMutex> lock(gFastMutex);
    ++ gCount;
  }
}

// Thread function: Spin-lock locking
void ThreadSpinLock(void * aArg)
{
  for(int i = 0; i < 10000; ++ i)
  {
    // CPU-friendly spin-lock
    while (gFlag.test_and_set())
      ThisThread::yield();

    ++ gCount;

    // Release lock
    gFlag.clear();
  }
}

// Thread function: Atomic count
void ThreadAtomicCount(void * aArg)
{
  for(int i = 0; i < 10000; ++ i)
  {
    ++ gAtomicCount;
  }
}

// Thread function: Condition notifier
void ThreadCondition1(void * aArg)
{
  LockGuard<Mutex> lock(gMutex);
  -- gCount;
  gCond.notify_all();
}

// Thread function: Condition waiter
void ThreadCondition2(void * aArg)
{
  std::cout << " Waiting..." << std::flush;
  LockGuard<Mutex> lock(gMutex);
  while(gCount > 0)
  {
    std::cout << "." << std::flush;
    gCond.wait(gMutex);
  }
  std::cout << "." << std::endl;
}

// Thread function: Yield
void ThreadYield(void * aArg)
{
  // Yield...
  ThisThread::yield();
}

// Thread function: Detach
void ThreadDetach(void * aArg)
{
  // We don't do anything much, just sleep a little...
  ThisThread::sleep_for(Core::DateTime::chrono::milliseconds(100));
  std::cout << " Detached thread finished." << std::endl;
}

// Test 1: Show number of CPU cores in the system
TEST(Thread_HWConcurrency)
{
	unsigned int numproc = 0;
#if defined(_WIN32)
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  numproc = (int) si.dwNumberOfProcessors;
#elif defined(_SC_NPROCESSORS_ONLN)
  numproc = (int) sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(_SC_NPROC_ONLN)
  numproc = (int) sysconf(_SC_NPROC_ONLN);
#else
  // The standard requires this function to return zero if the number of
  // hardware cores could not be determined.
  return 0;
#endif
	CHECK ( Core::Thread::Thread::hardwareConcurrency() == numproc );
}

TEST(Thread_ID)
{
	// Show the main thread ID
    Thread::Id mtid = ThisThread::get_id();

    // Start a bunch of child threads - only run a single thread at a time
    Thread t1(ThreadIDs, 0);
    t1.join();
	CHECK( mtid != tid);

    Thread t2(ThreadIDs, 0);
    t2.join();
	CHECK( mtid != tid);

    Thread t3(ThreadIDs, 0);
    t3.join();
	CHECK( mtid != tid);

    Thread t4(ThreadIDs, 0);
    t4.join();
	CHECK( mtid != tid);
}

// Test 3: thread local storage
TEST(Thread_LocalStorage)
{
#ifndef NO_TLS
    // Clear the TLS variable (it should keep this value after all threads are
    // finished).
    gLocalVar = 1;
    std::cout << " Main gLocalVar is " << gLocalVar << "." << std::endl;

    // Start a child thread that modifies gLocalVar
    Thread t1(ThreadTLS, 0);
    t1.join();

    // Check if the TLS variable has changed
    CHECK( 1 == gLocalVar );
#else
	//TODO : implement TLS...
	std::cout << " TLS is not supported on this platform..." << std::endl;
	CHECK(false && "NOT SUPPORTED")
#endif
}

//Test 4 : Mutex locking
TEST(Thread_MutexLocking)
{
	// Clear the global counter.
    gCount = 0;

    // Start a bunch of child threads
    std::list<Thread *> threadList;
    for(int i = 0; i < 100; ++ i)
      threadList.push_back(new Thread(ThreadLock, 0));

    // Wait for the threads to finish
    std::list<Thread *>::iterator it;
    for(it = threadList.begin(); it != threadList.end(); ++ it)
    {
      Thread * t = *it;
      t->join();
      delete t;
    }

    // Check the global count
    CHECK( 1000000 == gCount );
}

// Test 5: fast_mutex locking
TEST(Thread_FastMutexLocking)
{
	// Clear the global counter.
    gCount = 0;

    // Start a bunch of child threads
    std::list<Thread *> threadList;
    for(int i = 0; i < 100; ++ i)
      threadList.push_back(new Thread(ThreadLock2, 0));

    // Wait for the threads to finish
    std::list<Thread *>::iterator it;
    for(it = threadList.begin(); it != threadList.end(); ++ it)
    {
      Thread * t = *it;
      t->join();
      delete t;
    }

    // Check the global count
    CHECK( 1000000 == gCount );
}


// Test 6: Atomic lock
TEST(Thread_AtomicLock)
{
	    // Clear the global counter.
    gCount = 0;

    // Start a bunch of child threads
    std::list<Thread *> threadList;
    for(int i = 0; i < 100; ++ i)
      threadList.push_back(new Thread(ThreadSpinLock, 0));

    // Wait for the threads to finish
    std::list<Thread *>::iterator it;
    for(it = threadList.begin(); it != threadList.end(); ++ it)
    {
      Thread * t = *it;
      t->join();
      delete t;
    }

    // Check the global count
    CHECK( 1000000 == gCount );
}


  // Test 7: Atomic variable
TEST(Thread_AtomicVariable)
{
	//No need to clear atomic variable, it was inited at 0;

    // Start a bunch of child threads
    std::list<Thread *> threadList;
    for(int i = 0; i < 100; ++ i)
      threadList.push_back(new Thread(ThreadAtomicCount, 0));

    // Wait for the threads to finish
    std::list<Thread *>::iterator it;
    for(it = threadList.begin(); it != threadList.end(); ++ it)
    {
      Thread * t = *it;
      t->join();
      delete t;
    }

    CHECK( 1000000 == gAtomicCount );
}


// Test 8: condition variable
TEST(Thread_ConditionVariable)
{
	// Set the global counter to the number of threads to run.
    gCount = 40;

    // Start the waiting thread (it will wait for gCount to reach zero).
    Thread t1(ThreadCondition2, 0);

    // Start a bunch of child threads (these will decrease gCount by 1 when they
    // finish)
    std::list<Thread *> threadList;
    for(int i = 0; i < 40; ++ i)
      threadList.push_back(new Thread(ThreadCondition1, 0));

    // Wait for the waiting thread to finish
    t1.join();

    // Wait for the other threads to finish
    std::list<Thread *>::iterator it;
    for(it = threadList.begin(); it != threadList.end(); ++ it)
    {
      Thread * t = *it;
      t->join();
      delete t;
    }
}


  // Test 9: yield
TEST(Thread_Yield)
{
	// Start a bunch of child threads
    std::list<Thread *> threadList;
    for(int i = 0; i < 40; ++ i)
      threadList.push_back(new Thread(ThreadYield, 0));

    // Yield...
    ThisThread::yield();

    // Wait for the threads to finish
    std::list<Thread *>::iterator it;
    for(it = threadList.begin(); it != threadList.end(); ++ it)
    {
      Thread * t = *it;
      t->join();
      delete t;
    }
}

//Test 10: sleep
TEST(Thread_Sleep)
{
	// Sleep...
    std::cout << " Sleeping" << std::flush;
    for(int i = 0; i < 10; ++ i)
    {
		ThisThread::sleep_for(Core::DateTime::chrono::milliseconds(100));
      std::cout << "." << std::flush;
    }
    std::cout << std::endl;
}

// Test 11: detach
TEST(Thread_Detach)
{
	Thread t(ThreadDetach, 0);
    t.detach();
    std::cout << " Detached from thread." << std::endl;

    // Give the thread a chanse to finish too...
	ThisThread::sleep_for(Core::DateTime::chrono::milliseconds(400));
}

}


/*SUITE(ThreadedCommand)
{
}
*/

int main(int argc, char * argv [])
{
	if ( argc < 2 )
	{
		return UnitTest::RunAllTests();
	}
	else if ( strcmp(argv[1],"-xml") == 0 )
	{
		std::ofstream f("UnitTest-Thread-result.xml");
		UnitTest::XmlTestReporter reporter(f);
		UnitTest::TestRunner runner(reporter);
		return runner.RunTestsIf(UnitTest::Test::GetTestList(),NULL,UnitTest::True(),0);
	}
	else
	{
		std::cerr << "Usage : " << argv[0] << " [-xml]" << std::endl;
		return 127;
	}
}
