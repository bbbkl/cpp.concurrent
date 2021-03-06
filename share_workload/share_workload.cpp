#include <iostream>
#include <thread>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
#include <execution> //�tr tke exet:ts{i*tr pclicy
#include <set>
#include <optional>

#include "Logger.h"

using namespace std;

#if 1
#define OUT Logger()
#else
#define OUT cout
#endif



class WorkloadContainer
{
public:
  void doTheWorkOverall();
  void prepareTheWork(int workItemCount);
  void showWhatToDo() const;
  int getNextToDo();
  void doTheWorkInThread();

  vector<string> m_itemsToDo;
  atomic<int> m_nextItemIdx = -1;
};

void WorkloadContainer::showWhatToDo() const
{
  cout << "Work to do #=" << m_itemsToDo.size();
  for(int i = 0; i < m_itemsToDo.size(); ++i)
    cout << "\n\t" << m_itemsToDo[i];
  cout << "\n\n";
}

void WorkloadContainer::prepareTheWork(int workItemCount)
{
  for(int i = 0; i < workItemCount; ++i)
  {
    ostringstream os;
    os << "workload_" << i;
    m_itemsToDo.push_back(os.str());
  }
  m_nextItemIdx = 0;
}

int WorkloadContainer::getNextToDo()
{
  return m_nextItemIdx++;
}

void WorkloadContainer::doTheWorkInThread()
{
  auto tid = this_thread::get_id();
  OUT << "start_of_thread=" << tid << endl;

  int cnt = 0;
  size_t endIdx = m_itemsToDo.size();

  auto idx = getNextToDo();
  while(idx >= 0 && idx < endIdx)
  {
    ostringstream os;
    os << tid << "/" << ++cnt;
    auto thisTurn = os.str();

    const auto& itemToDo = m_itemsToDo[idx];
    OUT << thisTurn << " ... " << itemToDo << " - done by " << thisTurn << endl;
    
    idx = getNextToDo();
  }

  OUT << "termination_of_thread=" << tid << " count=" << cnt << endl;
}

void WorkloadContainer::doTheWorkOverall()
{
  int workItemCount = 42;
  prepareTheWork(workItemCount);
  showWhatToDo();

#if 1
  const int num_workers = 3;
  list<thread> threads;
  for(int i = 0; i < num_workers; ++i)
    threads.emplace_back(thread(&WorkloadContainer::doTheWorkInThread, this));

  for(auto& t : threads)
    t.join();
#else
  doIt(); // non-thread version
#endif
}


int main()
{
  WorkloadContainer workload;
  workload.doTheWorkOverall();
}