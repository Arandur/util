#include <stdexcept>

class bad_either_access :
  public std::logic_error
{
  using std::logic_error::logic_error;
};

template <typename L, typename R>
template <typename... Args>
constexpr
auto either<L, R>::from_left(Args&&... args) ->
either<L, R>
{
  either<L, R> res;
  res.template set<0>(std::forward<Args&&>(args)...);
  return res;
}

template <typename L, typename R>
template <typename... Args>
constexpr
auto either<L, R>::from_right(Args&&... args) ->
either<L, R>
{
  either<L, R> res;
  res.template set<1>(std::forward<Args&&>(args)...);
  return res;
}

template <typename L, typename R>
constexpr
auto either<L, R>::is_left() const ->
bool
{
  return this->template is<0>();
}

template <typename L, typename R>
constexpr
auto either<L, R>::is_right() const ->
bool
{
  return this->template is<1>();
}

template <typename L, typename R>
constexpr
auto either<L, R>::get_left() ->
L&
{
  if (this->template is<0>())
    return this->template get<0>();
  else
    throw bad_either_access("Tried to access left where there was no left");
}

template <typename L, typename R>
constexpr
auto either<L, R>::get_left() const ->
const L&
{
  if (this->template is<0>())
    return this->template get<0>();
  else
    throw bad_either_access("Tried to access left where there was no left");
}

template <typename L, typename R>
constexpr
auto either<L, R>::get_right() ->
R&
{
  if (this->template is<1>())
    return this->template get<1>();
  else
    throw bad_either_access("Tried to access right where there was no right");
}

template <typename L, typename R>
constexpr
auto either<L, R>::get_right() const ->
const R&
{
  if (this->template is<1>())
    return this->template get<1>();
  else
    throw bad_either_access("Tried to access right where there was no right");
}

template <typename L, typename R>
template <typename... Args>
constexpr
void either<L, R>::set_left(Args&&... args)
{
  this->template set<0>(std::forward<Args&&>(args)...);
}

template <typename L, typename R>
template <typename... Args>
constexpr
void either<L, R>::set_right(Args&&... args)
{
  this->template set<1>(std::forward<Args&&>(args)...);
}

template <typename L, typename R>
constexpr
auto either<L, R>::get_left_or(L& alt) ->
L&
{
  if (this->template is<0>())
    return this->template get<0>();
  else
    return alt;
}

template <typename L, typename R>
template <typename... Args>
constexpr
auto either<L, R>::get_left_or(Args&&... args) const ->
const L&
{
  if (this->template is<0>())
    return this->template get<0>();
  else
    return L(std::forward<Args&&>(args)...);
}

template <typename L, typename R>
template <typename F>
constexpr
auto either<L, R>::get_left_or_else(F&& f) const ->
const L&
{
  if (this->template is<0>())
    return this->template get<0>();
  else
    return f();
}

template <typename L, typename R>
template <typename F>
constexpr
auto either<L, R>::fmap(F&& f) const ->
either<std::result_of_t<F(L)>, R>
{
  if (this->template is<0>())
    return either<std::result_of_t<F(L)>, R>::from_left(f(this->template get<0>()));
  else
    return either<std::result_of_t<F(L)>, R>::from_right(this->template get<1>());
}

template <typename L, typename R>
template <typename F>
constexpr
auto either<L, R>::flatmap(F&& f) const ->
std::result_of_t<F(L)>
{
  if (this->template is<0>())
    return f(this->template get<0>());
  else
    return std::result_of_t<F(L)>::from_right(this->template get<1>());
}
