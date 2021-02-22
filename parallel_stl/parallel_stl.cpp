#include <iostream>
#include <thread>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
#include <execution> //§tr tke exet:ts{i*tr pclicy
#include <set>

using namespace std;

class A
{
public:
  A() : m_i(++A::g_cnt) {}
  string hello() const
  { 
    auto id = this_thread::get_id();
    g_threadIds.insert(id);

    ostringstream os;
    os << m_i << " hello concurrent world=" << m_i << " threadId=" << id << "|";
    return os.str();
  }

  static const set<thread::id>& thread_ids() { return g_threadIds; }

private:
  int m_i;
  static int g_cnt;
  static set<thread::id> g_threadIds;
};
int A::g_cnt = 0;
set<thread::id> A::g_threadIds;

void hello(const A& obj)
{
  cout << obj.hello() << "\n";
}


int main()
{
  vector<A> container;
  for(int i = 0; i < 42; ++i)
    container.emplace_back(A());

  for_each(std::execution::par,
    container.begin(), container.end(),
    [](auto& val) { hello(val); });

  cout << "\n\n#threads=" << A::thread_ids().size() << "\n";
}