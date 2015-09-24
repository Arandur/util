#pragma once

#include "variant.h"

#include <stdexcept>

template <typename L, typename R>
class either;
template <typename T>
class result;

class bad_optional_access :
  public std::logic_error
{
  using std::logic_error::logic_error;
};

template <typename T>
class optional :
  private variant<T>
{
  typedef variant<T> Base;

  using Base::Base;

public:

  template <typename... Args>
  constexpr
  static
  auto from_some(Args&&...) 
  noexcept(noexcept(T(std::declval<Args&&>()...))) ->
  optional<T>;
  constexpr
  static
  auto from_none() 
  noexcept ->
  optional<T>;

  constexpr
  operator bool () const
  noexcept;

  constexpr
  auto operator ! () const 
  noexcept ->
  bool;

  constexpr
  auto is_some() const 
  noexcept ->
  bool;
  constexpr
  auto is_none() const 
  noexcept ->
  bool;

  constexpr
  auto expect(const std::string&) ->
  T&;
  constexpr
  auto expect(const std::string&) const ->
  const T&;

  constexpr
  auto unwrap() ->
  T&;
  constexpr
  auto unwrap() const ->
  const T&;

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
  optional<std::result_of_t<F(T)>>;

  template <typename F>
  constexpr
  auto map_or(const T&, F&&) const
  noexcept(noexcept(std::declval<F>()(std::declval<T>()))) ->
  std::result_of_t<F(T)>;

  template <typename F, typename D>
  constexpr
  auto map_or_else(D&&, F&&) const
  noexcept(noexcept(std::declval<F>()(std::declval<T>())) && 
           noexcept(std::declval<D>()())) ->
  std::result_of_t<F(T)>;

  template <typename R>
  constexpr
  auto left_or(R&&) const
  noexcept ->
  either<T, R>;

  template <typename F>
  constexpr
  auto left_or_else(F&&) const
  noexcept(noexcept(std::declval<F>()())) ->
  either<T, std::result_of_t<F()>>;

  template <typename... Args>
  constexpr
  auto ok_or(Args&&...) const
  noexcept(noexcept(result<T>::from_err(std::declval<Args&&>()...))) ->
  result<T>;

  template <typename F>
  constexpr
  auto ok_or_else(F&&) const
  noexcept(noexcept(std::declval<F>()())) ->
  result<T>;

  template <typename F>
  constexpr
  auto flatmap(F&&) const
  noexcept(noexcept(std::declval<F>()(std::declval<T>()))) ->
  std::result_of_t<F(T)>;
  
private:

  using Base::is;
  using Base::set;
  using Base::get;
};
