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

template <typename... Args>
class variant
{
public:

  constexpr
  variant();

  ~variant();

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

  template <size_t N, typename... cArgs>
  constexpr
  void set(cArgs&&...);

  constexpr
  void reset();

private:

  std::aligned_union_t<0, Args...> buffer;
  size_t i = 0;
  std::function<void()> destructor;
};

#include "variant.tcc"
