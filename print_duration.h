#pragma once

#include <chrono>
#include <iostream>
#include <sstream>

namespace detail
{
  template <typename T>
  struct abbv;

  template <>
  struct abbv<std::chrono::hours>
  {
    static constexpr auto value = "h";
  };

  template <>
  struct abbv<std::chrono::minutes>
  {
    static constexpr auto value = "m";
  };

  template <>
  struct abbv<std::chrono::seconds>
  {
    static constexpr auto value = "s";
  };

  template <>
  struct abbv<std::chrono::milliseconds>
  {
    static constexpr auto value = "ms";
  };

  template <>
  struct abbv<std::chrono::microseconds>
  {
    static constexpr auto value = "us";
  };

  template <>
  struct abbv<std::chrono::nanoseconds>
  {
    static constexpr auto value = "ns";
  };
}

template <typename Rep, typename Period>
auto operator << (
    std::ostream& out, 
    const std::chrono::duration<Rep, Period>& duration) ->
std::ostream&
{
  std::ostringstream oss;
  oss << duration.count() << " " << detail::abbv<std::chrono::duration<Rep, Period>>::value;
  return out << oss.str();
}
