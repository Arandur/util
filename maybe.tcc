#include <stdexcept>

class bad_maybe_access :
  public std::logic_error
{
  using std::logic_error::logic_error;
};

template <typename T>
template <typename... Args>
constexpr
auto maybe<T>::just(Args&&... args) ->
maybe<T>
{
  maybe<T> res;
  res.template set<0>(std::forward<Args&&>(args)...);
  return res;
}

template <typename T>
constexpr
auto maybe<T>::nothing() ->
maybe<T>
{
  return maybe<T>();
}

template <typename T>
constexpr
auto maybe<T>::has_value() const ->
bool
{
  return this->template is<0>();
}

template <typename T>
constexpr
auto maybe<T>::get_value() ->
T&
{
  if (this->template is<0>())
    return this->template get<0>();
  else
    throw bad_maybe_access("Tried to access value where there was no value");
}

template <typename T>
constexpr
auto maybe<T>::get_value() const ->
const T&
{
  if (this->template is<0>())
    return this->template get<0>();
  else
    throw bad_maybe_access("Trued to access value where there was no value");
}

template <typename T>
template <typename... Args>
constexpr
void maybe<T>::set_value(Args&&... args)
{
  this->template set<0>(std::forward<Args&&>(args)...);
}

template <typename T>
constexpr
auto maybe<T>::get_value_or(T& alt) ->
T&
{
  if (this->template is<0>())
    return this->template get<0>();
  else
    return alt;
}

template <typename T>
template <typename... Args>
constexpr
auto maybe<T>::get_value_or(Args&&... args) const->
const T&
{
  if (this->template is<0>())
    return this->template get<0>();
  else
    return T(std::forward<Args&&>(args)...);
}

template <typename T>
template <typename F>
constexpr
auto maybe<T>::get_value_or_else(F&& f) const ->
const T&
{
  if (this->template is<0>())
    return this->template get<0>();
  else
    return f();
}

template <typename T>
template <typename F>
constexpr
auto maybe<T>::fmap(F&& f) const ->
maybe<std::result_of_t<F(T)>>
{
  if (this->template is<0>())
    return maybe<std::result_of_t<F(T)>>::just(f(this->template get<0>()));
  else
    return maybe<std::result_of_t<F(T)>>::nothing();
}

template <typename T>
template <typename F>
constexpr
auto maybe<T>::flatmap(F&& f) const ->
std::result_of_t<F(T)>
{
  if (this->template is<0>())
    return f(this->template get<0>());
  else
    return std::result_of_t<F(T)>::nothing();
}
