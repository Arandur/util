#pragma once

template <typename L, typename R>
class either
{
public:

  either() = delete;

  template <typename _L, typename _R>
  constexpr
  either(const either<_L, _R>&);
  template <typename _L, typename _R>
  constexpr
  either(either<_L, _R>&&);

  template <typename _L, typename _R>
  constexpr
  auto operator = (const either<_L, _R>&) ->
  either<L, R>&;
  template <typename _L, typename _R>
  constexpr
  auto operator = (either <_L, _R>&&) ->
  either<L, R>&;

  ~either();

  template <typename... Args>
  constexpr
  static
  auto from_left(Args&&...) ->
  either<L, R>;
  template <typename... Args>
  constexpr
  static
  auto from_right(Args&&...) ->
  either<L, R>;

  constexpr
  auto is_left() const noexcept ->
  bool;
  constexpr
  auto is_right() const noexcept ->
  bool;

  constexpr
  auto get_left() ->
  L&;
  constexpr
  auto get_left() const ->
  const L&;
  constexpr
  auto get_right() ->
  R&;
  constexpr
  auto get_right() const ->
  const R&;

private:

  union
  {
    L left;
    R right;
  };
  bool _is_left;

  template <typename... Args>
  constexpr
  either(bool, Args&&...);

  template <typename... Args>
  constexpr
  void construct_left(Args&&...);
  template <typename... Args>
  constexpr
  void construct_right(Args&&...);

  constexpr
  void destroy_left();
  constexpr
  void destroy_right();
  constexpr
  void clear();
};

#include "either.tcc"
