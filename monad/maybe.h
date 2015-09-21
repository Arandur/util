#pragma once

#include "variant.h"

class bad_maybe_access;

template <typename T>
class maybe :
  private variant<T>
{
  typedef variant<T> Base;
public:

  using Base::Base;

  template <typename... Args>
  constexpr
  static
  auto just(Args&&...) ->
  maybe<T>;
  constexpr
  static
  auto nothing() ->
  maybe<T>;

  constexpr
  auto has_value() const ->
  bool;

  constexpr
  auto get_value() ->
  T&;
  constexpr
  auto get_value() const ->
  const T&;

  template <typename... Args>
  constexpr
  void set_value(Args&&...);

  constexpr
  auto get_value_or(T&) ->
  T&;
  template <typename... Args>
  constexpr
  auto get_value_or(Args&&...) const ->
  const T&;

  template <typename F>
  constexpr
  auto get_value_or_else(F&&) const ->
  const T&;

  template <typename F>
  constexpr
  auto fmap(F&&) const ->
  maybe<std::result_of_t<F(T)>>;

  template <typename F>
  constexpr
  auto flatmap(F&&) const ->
  std::result_of_t<F(T)>;

private:

  using Base::is;
  using Base::set;
  using Base::get;
};

#include "maybe.tcc"
