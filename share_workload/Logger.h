#pragma once
#include <ostream>
#include <memory>

class Logger : public std::basic_ostream<char, std::char_traits<char>>
{
public:
  Logger();
  ~Logger();

private:
  struct Impl;
  Impl* m_pimpl;
};