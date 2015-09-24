#pragma once

#include "variant.h"

#include <stdexcept>

class bad_either_access :
  public std::logic_error
{
  using std::logic_error::logic_error;
};

template <typename T>
class optional;

template <typename L, typename R>
class either :
  private variant<L, R>
{
  typedef variant<L, R> Base;

  using Base::Base;

public:

  template <typename... Args>
  constexpr
  static
  auto from_left(Args&&...)
  noexcept(noexcept(L(std::declval<Args&&>()...))) ->
  either<L, R>;
  template <typename... Args>
  constexpr
  static
  auto from_right(Args&&...)
  noexcept(noexcept(R(std::declval<Args&&>()...))) ->
  either<L, R>;

  constexpr
  auto is_left() const 
  noexcept ->
  bool;
  constexpr
  auto is_right() const 
  noexcept ->
  bool;

  constexpr
  auto expect_left(const std::string&) ->
  L&;
  constexpr
  auto expect_left(const std::string&) const ->
  const L&;
  constexpr
  auto expect_right(const std::string&) ->
  R&;
  constexpr
  auto expect_right(const std::string&) const ->
  const R&;

  constexpr
  auto unwrap_left() ->
  L&;
  constexpr
  auto unwrap_left() const ->
  const L&;
  constexpr
  auto unwrap_right() ->
  R&;
  constexpr
  auto unwrap_right() const ->
  const R&;

  constexpr
  auto unwrap_left_or(L&)
  noexcept ->
  L&;
  template <typename... Args>
  constexpr
  auto unwrap_left_or(Args&&...) const
  noexcept(noexcept(L(std::declval<Args&&>()...))) ->
  const L&;
  constexpr
  auto unwrap_right_or(R&)
  noexcept ->
  R&;
  template <typename... Args>
  constexpr
  auto unwrap_right_or(Args&&...) const
  noexcept(noexcept(R(std::declval<Args&&>()...))) ->
  const R&;

  template <typename F>
  constexpr
  auto unwrap_left_or_else(F&&) const
  noexcept(noexcept(std::declval<F>())) ->
  const L&;
  template <typename F>
  constexpr
  auto unwrap_right_or_else(F&&) const
  noexcept(noexcept(std::declval<F>())) ->
  const R&;

  template <typename F>
  constexpr
  auto map_left(F&&) const 
  noexcept(noexcept(std::declval<F>()(std::declval<L>()))) ->
  either<std::result_of_t<F(L)>, R>;
  template <typename F>
  constexpr
  auto map_right(F&&) const 
  noexcept(noexcept(std::declval<F>()(std::declval<R>()))) ->
  either<L, std::result_of_t<F(R)>>;

  template <typename F>
  constexpr
  auto map_left_or(const L&, F&&) const
  noexcept(noexcept(std::declval<F>()(std::declval<L>()))) ->
  std::result_of_t<F(L)>;
  template <typename F>
  constexpr
  auto map_right_or(const R&, F&&) const
  noexcept(noexcept(std::declval<F>()(std::declval<R>()))) ->
  std::result_of_t<F(R)>;

  template <typename F, typename D>
  constexpr
  auto map_left_or_else(D&&, F&&) const
  noexcept(noexcept(std::declval<F>()(std::declval<L>())) and
           noexcept(std::declval<D>()())) ->
  std::result_of_t<F(L)>;
  template <typename F, typename D>
  constexpr
  auto map_right_or_else(D&&, F&&) const
  noexcept(noexcept(std::declval<F>()(std::declval<R>())) and
           noexcept(std::declval<D>()())) ->
  std::result_of_t<F(R)>;

  constexpr
  auto get_left() const
  noexcept ->
  optional<L>;
  constexpr
  auto get_right() const
  noexcept ->
  optional<R>;

  template <typename F>
  constexpr
  auto flatmap_left(F&&) const 
  noexcept(noexcept(std::declval<F>()(std::declval<L>()))) ->
  std::result_of_t<F(L)>;
  template <typename F>
  constexpr
  auto flatmap_right(F&&) const
  noexcept(noexcept(std::declval<F>()(std::declval<R>()))) ->
  std::result_of_t<F(R)>;

protected:

  using Base::get;

private:

  using Base::is;
  using Base::set;
};
