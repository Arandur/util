#pragma once

#include "variant.h"

class bad_either_access;

template <typename L, typename R>
class either :
  private variant<L, R>
{
  typedef variant<L, R> Base;
public:

  using Base::Base;

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
  auto is_left() const ->
  bool;
  constexpr
  auto is_right() const ->
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

  template <typename... Args>
  constexpr
  void set_left(Args&&...);
  template <typename... Args>
  constexpr
  void set_right(Args&&...);

private:

  using Base::is;
  using Base::set;
  using Base::get;
};

#include "either.tcc"
