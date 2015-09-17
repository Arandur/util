#pragma once

#include <cstdint>
#include <type_traits>

template <typename L, typename R>
class either
{
public:

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

  std::aligned_union_t<0, L, R> buffer;

  enum class Side_t : uint8_t
  {
    UNINITIALIZED,
    LEFT,
    RIGHT
  } side;

  constexpr
  either();

  template <typename _L, typename _R>
  constexpr
  either(Side_t, const std::aligned_union_t<0, _L, _R>&);

  template <typename _L, typename _R>
  constexpr
  either(Side_t, std::aligned_union_t<0, _L, _R>&&);

  template <typename... Args>
  constexpr
  void construct_left(Args&&...);
  template <typename... Args>
  constexpr
  void construct_right(Args&&...);
};

#include "either.tcc"
