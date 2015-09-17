#include <stdexcept>
#include <memory>

class bad_access_exception :
  public std::logic_error
{
public:

  using std::logic_error::logic_error;
};

template <typename L, typename R>
template <typename _L, typename _R>
constexpr
either<L, R>::either(const either<_L, _R>& rhs)
{
  if (rhs.is_left())
    construct_left(rhs.left);
  else
    construct_right(rhs.right);
}

template <typename L, typename R>
template <typename _L, typename _R>
constexpr
either<L, R>::either(either<_L, _R>&& rhs)
{
  if (rhs.is_left())
    construct_left(std::move(rhs.left));
  else
    construct_right(std::move(rhs.right));
}

template <typename L, typename R>
template <typename _L, typename _R>
constexpr
auto either<L, R>::operator = (const either<_L, _R>& rhs) ->
either<L, R>&
{
  clear();
  if (rhs.is_left())
    construct_left(rhs.left);
  else
    construct_right(rhs.right);

  return *this;
}

template <typename L, typename R>
template <typename _L, typename _R>
constexpr
auto either<L, R>::operator = (either<_L, _R>&& rhs) ->
either<L, R>&
{
  clear();
  if (rhs.is_left())
    construct_left(std::move(rhs.left));
  else
    construct_right(std::move(rhs.right));

  return *this;
}

template <typename L, typename R>
either<L, R>::~either()
{
  clear();
}

template <typename L, typename R>
template <typename... Args>
constexpr
auto either<L, R>::from_left(Args&&... args) ->
either<L, R>
{
  return either<L, R>(true, std::forward<Args&&>(args)...);
}

template <typename L, typename R>
template <typename... Args>
constexpr
auto either<L, R>::from_right(Args&&... args) ->
either<L, R>
{
  return either<L, R>(false, std::forward<Args&&>(args)...);
}

template <typename L, typename R>
constexpr
auto either<L, R>::is_left() const noexcept ->
bool
{
  return _is_left;
}

template <typename L, typename R>
constexpr
auto either<L, R>::is_right() const noexcept ->
bool
{
  return ! _is_left;
}

template <typename L, typename R>
constexpr
auto either<L, R>::get_left() ->
L&
{
  if (is_left())
    return left;
  else
    throw bad_access_exception("Attempted to access left when no left exists");
}

template <typename L, typename R>
constexpr
auto either<L, R>::get_left() const ->
const L&
{
  if (is_left())
    return left;
  else
    throw bad_access_exception("Attempted to access left when no left exists");
}

template <typename L, typename R>
constexpr
auto either<L, R>::get_right() ->
R&
{
  if (is_right())
    return right;
  else
    throw bad_access_exception("Attempted to access right when no right exists");
}

template <typename L, typename R>
constexpr
auto either<L, R>::get_right() const ->
const R&
{
  if (is_right())
    return right;
  else
    throw bad_access_exception("Attempted to access right when no right exists");
}

template <typename L, typename R>
template <typename... Args>
constexpr
either<L, R>::either(bool __is_left, Args&&... args)
{
  if (__is_left)
    construct_left(std::forward<Args&&>(args)...);
  else
    construct_right(std::forward<Args&&>(args)...);
}

template <typename L, typename R>
template <typename... Args>
constexpr
void either<L, R>::construct_left(Args&&... args)
{
  new (std::addressof(left)) L(std::forward<Args&&>(args)...);
  _is_left = true;
}

template <typename L, typename R>
template <typename... Args>
constexpr
void either<L, R>::construct_right(Args&&... args)
{
  new (std::addressof(right)) R(std::forward<Args&&>(args)...);
  _is_left = false;
}

template <typename L, typename R>
constexpr
void either<L, R>::destroy_left()
{
  left.~L();
}

template <typename L, typename R>
constexpr
void either<L, R>::destroy_right()
{
  right.~R();
}

template <typename L, typename R>
constexpr
void either<L, R>::clear()
{
  if (_is_left)
    destroy_left();
  else
    destroy_right();
}
