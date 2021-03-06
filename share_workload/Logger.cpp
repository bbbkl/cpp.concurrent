#include "Logger.h"

#include <mutex>

#include <iostream>
#include <sstream>

using namespace std;

static mutex g_mtx;

struct Logger::Impl : public basic_streambuf<char>
{
  Impl()
    : basic_streambuf<char>()
  {}

  ~Impl() {}

  ostream& operator<< (ostream& (*fcn)(ostream&))
  {
    return (*fcn)(m_buf);
  }

  void handleMessage(const string& message)
  {
    unique_lock<mutex> lock(g_mtx);
    cerr << message << std::flush;
  }

  int sync() override
  {
    string message = m_buf.str();
    handleMessage(message);

    static string emptyString;
    m_buf.str(emptyString); // reset internal string buffer

    return m_buf.good() ? 0 : -1;
  }

  int_type overflow(int_type c) override
  {
    m_buf.put(c);
    return m_buf.good();
  }

  streamsize xsputn(const char* s, streamsize cnt) override
  {
    m_buf.write(s, cnt);
    return m_buf.good() ? cnt : 0;
  }

  ostringstream m_buf;
};

Logger::Logger()
  : basic_ostream<char, char_traits<char>>(nullptr)
  , m_pimpl(new Impl())
{
  rdbuf(m_pimpl);    // attach m_pimpl to own buffer
}

Logger::~Logger()
{
  m_pimpl->sync();
  delete m_pimpl;
}