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
either<L, R>::either(const either<_L, _R>& rhs) :
  either(rhs.side, rhs.buffer)
{}

template <typename L, typename R>
template <typename _L, typename _R>
constexpr
either<L, R>::either(either<_L, _R>&& rhs) :
  either(rhs.side, rhs.buffer)
{}

template <typename L, typename R>
template <typename _L, typename _R>
constexpr
auto either<L, R>::operator = (const either<_L, _R>& rhs) ->
either<L, R>&
{
  ~either();

  switch (rhs.side) {
  case Side_t::UNINITIALIZED:
    break;
  case Side_t::LEFT:
    construct_left(reinterpret_cast<const _L&>(rhs.buffer));
    break;
  case Side_t::RIGHT:
    construct_right(reinterpret_cast<const _R&>(rhs.buffer));
    break;
  }

  return *this;
}

template <typename L, typename R>
template <typename _L, typename _R>
constexpr
auto either<L, R>::operator = (either<_L, _R>&& rhs) ->
either<L, R>&
{
  ~either();

  switch (rhs.side) {
  case Side_t::UNINITIALIZED:
    break;
  case Side_t::LEFT:
    construct_left(reinterpret_cast<_L&&>(rhs.buffer));
    break;
  case Side_t::RIGHT:
    construct_right(reinterpret_cast<_R&&>(rhs.buffer));
    break;
  }

  return *this;
}

template <typename L, typename R>
either<L, R>::~either()
{
  switch (side) {
  case Side_t::UNINITIALIZED:
    break;
  case Side_t::LEFT:
    reinterpret_cast<L&>(buffer).~L();
    break;
  case Side_t::RIGHT:
    reinterpret_cast<R&>(buffer).~R();
    break;
  }

  side = Side_t::UNINITIALIZED;
}

template <typename L, typename R>
template <typename... Args>
constexpr
auto either<L, R>::from_left(Args&&... args) ->
either<L, R>
{
  either<L, R> res;
  res.construct_left(std::forward<Args&&>(args)...);
  return res;
}

template <typename L, typename R>
template <typename... Args>
constexpr
auto either<L, R>::from_right(Args&&... args) ->
either<L, R>
{
  either<L, R> res;
  res.construct_right(std::forward<Args&&>(args)...);
  return res;
}

template <typename L, typename R>
constexpr
auto either<L, R>::is_left() const noexcept ->
bool
{
  return side == Side_t::LEFT;
}

template <typename L, typename R>
constexpr
auto either<L, R>::is_right() const noexcept ->
bool
{
  return side == Side_t::RIGHT;
}

template <typename L, typename R>
constexpr
auto either<L, R>::get_left() ->
L&
{
  if (is_left())
    return reinterpret_cast<L&>(buffer);
  else
    throw bad_access_exception("Attempted to access left when no left exists");
}

template <typename L, typename R>
constexpr
auto either<L, R>::get_left() const ->
const L&
{
  if (is_left())
    return reinterpret_cast<const L&>(buffer);
  else
    throw bad_access_exception("Attempted to access left when no left exists");
}

template <typename L, typename R>
constexpr
auto either<L, R>::get_right() ->
R&
{
  if (is_right())
    return reinterpret_cast<R&>(buffer);
  else
    throw bad_access_exception("Attempted to access right when no right exists");
}

template <typename L, typename R>
constexpr
auto either<L, R>::get_right() const ->
const R&
{
  if (is_right())
    return reinterpret_cast<const R&>(buffer);
  else
    throw bad_access_exception("Attempted to access right when no right exists");
}

template <typename L, typename R>
constexpr
either<L, R>::either() :
  side(Side_t::UNINITIALIZED)
{}

template <typename L, typename R>
template <typename _L, typename _R>
constexpr
either<L, R>::either(Side_t _side, const std::aligned_union_t<0, _L, _R>& buffer) :
  side(_side)
{
  switch (side) {
  case Side_t::UNINITIALIZED:
    break;
  case Side_t::LEFT:
    construct_left(reinterpret_cast<const _L&>(buffer));
    break;
  case Side_t::RIGHT:
    construct_right(reinterpret_cast<const _R&>(buffer));
    break;
  }
}

template <typename L, typename R>
template <typename _L, typename _R>
constexpr
either<L, R>::either(Side_t _side, std::aligned_union_t<0, _L, _R>&& buffer) :
  side(_side)
{
  switch (side) {
  case Side_t::UNINITIALIZED:
    break;
  case Side_t::LEFT:
    construct_left(reinterpret_cast<_L&&>(buffer));
    break;
  case Side_t::RIGHT:
    construct_right(reinterpret_cast<_R&&>(buffer));
    break;
  }
}

template <typename L, typename R>
template <typename... Args>
constexpr
void either<L, R>::construct_left(Args&&... args)
{
  new (&buffer) L(std::forward<Args&&>(args)...);
  side = Side_t::LEFT;
}

template <typename L, typename R>
template <typename... Args>
constexpr
void either<L, R>::construct_right(Args&&... args)
{
  new (&buffer) R(std::forward<Args&&>(args)...);
  side = Side_t::RIGHT;
}
