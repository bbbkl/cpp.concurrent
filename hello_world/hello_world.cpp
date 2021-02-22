#include <iostream>
#include <thread>
#include <sstream>
#include <list>

using namespace std;

class A
{
public:
  A() : m_i(++A::g_cnt) {}
  string hello() 
  { 
    ostringstream os;
    os << m_i << " hello concurrent world=" << m_i << " threadId=" << this_thread::get_id() << "|";
    return os.str();
  }

private:
  int m_i;
  static int g_cnt;
};
int A::g_cnt = 0;

void hello()
{
  A obj;
  std::cout << obj.hello() << "\n";
}

class Background_task
{
public:
  void operator()() const
  {
    A obj;
    cout << obj.hello() << "\n";
  }
};


int main()
{
  std::list<std::thread> threads;

  for(int i = 0; i < 42; ++i)
  {
    //threads.emplace_back(std::thread(hello));
    threads.emplace_back(std::thread(Background_task()));
  }

  for(auto& t : threads)
    t.join();
}