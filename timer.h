#pragma once

#include <chrono>
#include <tuple>

namespace detail
{
  template <typename Fn>
  struct timer_t
  {
    timer_t(Fn&& fn) :
      function(fn)
    {}

    template <typename... Args>
    auto operator () (Args&&... args) ->
    std::tuple <
        std::result_of_t<Fn(Args&&...)>, 
        std::chrono::steady_clock::duration>
    {
      auto t1 = std::chrono::steady_clock::now();
      auto&& res = function(std::forward<Args&&>(args)...);
      auto t2 = std::chrono::steady_clock::now();

      return std::make_tuple(res, t2 - t1);
    }

    Fn&& function;
  };
}

/**
 * Wraps a function with a timer, counting how long it takes for the function
 * to complete. Returns a std::function<> returning a tuple whose first member 
 * is the return value of the function, and whose second value is the duration 
 * of the call.
 */
template <typename Fn>
auto timer(Fn&& fn)
{
  return detail::timer_t<Fn>(std::forward<Fn&&>(fn));
}
