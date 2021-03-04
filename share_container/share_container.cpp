#include <iostream>
#include <thread>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
#include <execution> //§tr tke exet:ts{i*tr pclicy
#include <set>
#include <optional>

using namespace std;

class Workload
{
public:
  void doTheWork();
  void prepareTheWork();
  optional<int> getNextToDo();

  list<int> m_itemsToDo;
};


void Workload::doTheWork()
{
  prepareTheWork();

  const int num_workers = 3;
}

void Workload::prepareTheWork()
{
  for(int i = 0; i < 43; ++i)
    m_itemsToDo.push_back(i);
}



optional<int> Workload::getNextToDo()
{
  if(m_itemsToDo.empty())
    return optional<int>();

  optional<int> result(m_itemsToDo.front());
  m_itemsToDo.pop_front();

  return result;
}


int main()
{
  Workload workload;
  workload.doTheWork();
}