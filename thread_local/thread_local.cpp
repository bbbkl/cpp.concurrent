#include <iostream>
#include <thread>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
#include <execution>
#include <set>
#include <optional>

#include "..\share_workload\Logger.h"

using namespace std;

#if 0
#define ENDL "\n"
#else
#define ENDL endl
#endif


#if 1
#define OUT Logger()
#else
#define OUT cout
#endif

class A
{
  static atomic<int> g_ctorCount;
  static atomic<int> g_dtorCount;

public:
  A() { OUT << "A::ctor=" << ++g_ctorCount << ENDL;  }
  ~A() { OUT << "A::dtor=" << ++g_dtorCount << ENDL; }

  static pair<int, int> getCounters() 
  { 
    return make_pair<int, int>(g_ctorCount, g_dtorCount); 
  }
};

atomic<int> A::g_ctorCount = 0;
atomic<int> A::g_dtorCount = 0;

class HideMe
{
public:
  string getIt() const
  {
    static thread_local A a;
    const auto& counters = A::getCounters();
    ostringstream os;
    os << "HideMe ctor=" << counters.first << ", dtor=" << counters.second;
    return os.str();
  }
};


void doTheWorkInThread()
{
  auto tid = this_thread::get_id();
  OUT << "start_of_thread=" << tid << ENDL;
  OUT << HideMe().getIt() << ENDL;
  OUT << "termination_of_thread=" << tid << ENDL;
}

void doTheWorkOverall()
{
  const int num_workers = 3;

  for(int turns = 0; turns < 3; ++turns)
  {
    list<thread> threads;
    for(int i = 0; i < num_workers; ++i)
      threads.emplace_back(thread(doTheWorkInThread));

    for(auto& t : threads)
      t.join();
  }
}


int main()
{
  OUT << "main program" << ENDL;
  doTheWorkOverall();

  auto counter = A::getCounters();
  OUT << "at end ctor=" << counter.first << ", dtor=" << counter.second << ENDL;
}