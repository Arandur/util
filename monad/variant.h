#pragma once

#include <type_traits>
#include <memory>
#include <functional>

template <size_t N, typename T, typename... Args>
struct nth
{
  typedef typename nth<N - 1, Args...>::type type;
};

template <typename T, typename... Args>
struct nth<0, T, Args...>
{
  typedef T type;
};

template <size_t N, typename... Args>
using nth_t = typename nth<N, Args...>::type;

template <template <typename> class Pred, typename T, typename... Args>
struct all
{
  constexpr static bool value = Pred<T>::value && all<Pred, Args...>::value;
};

template <template <typename> class Pred, typename T>
struct all<Pred, T>
{
  constexpr static bool value = Pred<T>::value;
};

template <template <typename> class Pred, typename... Args>
constexpr auto all_v = all<Pred, Args...>::value;

template <
  template <typename...> class Derived,
  bool ProvideDestructor,
  typename... Args>
class variant_base
{
public:

  constexpr
  variant_base() {}

  constexpr
  variant_base(const variant_base&) = default;
  constexpr
  variant_base(variant_base&&) = default;

  constexpr
  auto operator = (const variant_base& rhs) ->
  variant_base&
  {
    reset();
    buffer = rhs.buffer;
    i = rhs.i;
    destructor = rhs.destructor;

    return *this;
  }

  constexpr
  auto operator = (variant_base&& rhs) ->
  variant_base&
  {
    reset();
    buffer = std::move(rhs.buffer);
    i = std::move(rhs.i);
    destructor = std::move(rhs.destructor);

    return *this;
  }

  ~variant_base()
  {
    reset();
  }

  template <size_t N, typename... cArgs>
  constexpr
  void set(cArgs&&... args)
  {
    reset();
    new (&buffer) nth_t<N, Args...>(std::forward<cArgs&&>(args)...);
    i = N + 1;
    destructor = [this] { reinterpret_cast<nth_t<N, Args...>&>(buffer).~nth_t<N, Args...>(); };
  }

  constexpr
  void reset()
  {
    if (i != 0) 
      i = 0;

    if (destructor)
      destructor();
  }

protected:

  std::aligned_union_t<0, Args...> buffer;
  size_t i = 0;

private:
  std::function<void()> destructor;
};

template <
  template <typename> class Derived,
  typename... Args>
class variant_base<Derived, false, Args...>
{
public:

  constexpr
  variant_base() {}

  template <size_t N, typename... cArgs>
  constexpr
  void set(cArgs&&... args)
  {
    new (&buffer) nth_t<N, Args...>(std::forward<cArgs&&>(args)...);
    i = N + 1;
  }

  constexpr
  void reset()
  {
    i = 0;
  }

protected:

  std::aligned_union_t<0, Args...> buffer;
  size_t i = 0;
};

template <
  typename... Args>
class variant :
  public variant_base<variant, ! all<std::is_trivially_destructible, Args...>::value, Args...>
{
  typedef variant_base<variant, ! all<std::is_trivially_destructible, Args...>::value, Args...> Base;
public:

  constexpr
  variant();

  template <size_t N>
  constexpr
  auto is() const ->
  bool;

  constexpr
  auto has_value() const ->
  bool;

  template <size_t N>
  constexpr
  auto get() ->
  nth_t<N, Args...>&;

  template <size_t N>
  constexpr
  auto get() const ->
  const nth_t<N, Args...>&;

  using Base::set;
  using Base::reset;

private:

  using Base::buffer;
  using Base::i;
};

#include "variant.tcc"
