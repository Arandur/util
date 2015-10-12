#pragma once

#include "either.h"

#include <stdexcept>

class bad_result_access :
  public std::logic_error
{
  using std::logic_error::logic_error;
};

template <typename T>
class result :
  public either<T, std::string>
{
  using Base = either<T, std::string>;

public:

  template <typename... Args>
  constexpr
  static
  auto from_ok(Args&&...)
  noexcept(noexcept(T(std::declval<Args&&>()...))) ->
  result<T>;
  template <typename... Args>
  constexpr
  static
  auto from_err(Args&&...)
  noexcept(noexcept(std::string(std::declval<Args&&>()...))) ->
  result<T>;

  constexpr
  auto is_ok() const
  noexcept ->
  bool;
  constexpr
  auto is_err() const
  noexcept ->
  bool;

  constexpr
  auto expect(const std::string&) ->
  T&;
  constexpr
  auto expect(const std::string&) const ->
  const T&;
  constexpr
  auto expect_err(const std::string&) ->
  std::string&;
  constexpr
  auto expect_err(const std::string&) const ->
  const std::string&;

  constexpr
  auto unwrap() ->
  T&;
  constexpr
  auto unwrap() const ->
  const T&;
  constexpr
  auto unwrap_err() ->
  std::string&;
  constexpr
  auto unwrap_err() const ->
  const std::string&;

  constexpr
  auto unwrap_or(T&)
  noexcept ->
  T&;
  template <typename... Args>
  constexpr
  auto unwrap_or(Args&&...) const
  noexcept(noexcept(T(std::declval<Args&&>()...))) ->
  const T&;

  template <typename F>
  constexpr
  auto unwrap_or_else(F&&) const
  noexcept(noexcept(std::declval<F>()())) ->
  const T&;

  template <typename F>
  constexpr
  auto map(F&&) const
  noexcept(noexcept(std::declval<F>()(std::declval<T>()))) ->
  result<std::result_of_t<F(T)>>;
  template <typename F>
  constexpr
  auto map_err(F&&) const
  noexcept(noexcept(std::declval<F>()(std::declval<std::string>()))) ->
  either<T, std::result_of_t<F(std::string)>>;

  constexpr
  auto get() const
  noexcept ->
  optional<T>;
  constexpr
  auto get_err() const
  noexcept ->
  optional<std::string>;

  template <typename F>
  constexpr
  auto flatmap(F&&) const
  noexcept(noexcept(std::declval<F>()(std::declval<T>()))) ->
  std::result_of_t<F(T)>;
  template <typename F>
  constexpr
  auto flatmap_err(F&&) const
  noexcept(noexcept(std::declval<F>()(std::declval<std::string>()))) ->
  std::result_of_t<F(std::string)>;

private:

  using Base::Base;
  using Base::get;

  result(const Base& base) :
    Base(base)
  {}

  result(Base&& base) :
    Base(base)
  {}
};
