#include <functional>
#include <chrono>
#include <type_traits>

namespace detail
{
  template <typename Fn>
  class timer_fn
  {
  public:

    timer_fn(Fn&& fn) :
      function(fn)
    {}

    template <typename... Args>
    auto operator () (Args&&... args) ->
    std::conditional_t <
      std::is_void<std::result_of_t<Fn(Args&&...)>>::value,
      std::tuple<std::chrono::steady_clock::duration>,
      std::tuple<std::chrono::steady_clock::duration, std::result_of_t<Fn(Args&&...)>>>
    {
      return run(
          std::is_void<std::result_of_t<Fn(Args&&...)>>(),
          std::forward<Args&&>(args)...);
    }


  private:

    Fn&& function;

    template <typename... Args>
    auto run (
        std::true_type, /* void */
        Args&&... args) ->
    std::tuple<std::chrono::steady_clock::duration>
    {
      auto t1 = std::chrono::steady_clock::now();
      function(std::forward<Args&&>(args)...);
      auto t2 = std::chrono::steady_clock::now();

      return std::make_tuple(t2 - t1);
    }

    template <typename... Args>
    auto run (
        std::false_type, /* not void */
        Args&&... args) ->
    std::tuple <
        std::chrono::steady_clock::duration,
        std::result_of_t<Fn(Args&&...)>>
    {
      auto   t1 = std::chrono::steady_clock::now();
      auto&& res = function(std::forward<Args&&>(args)...);
      auto   t2 = std::chrono::steady_clock::now();

      return std::make_tuple(t2 - t1, res);
    }
  };
}

template <typename Fn>
auto timer(Fn&& fn) ->
detail::timer_fn<Fn>
{
  return detail::timer_fn<Fn>(std::forward<Fn&&>(fn));
}
