template <typename L, typename R>
template <typename... Args>
constexpr
auto either<L, R>::from_left(Args&&... args) 
  noexcept(noexcept(L(std::declval<Args&&>()...))) ->
either<L, R>
{
  either<L, R> res;
  res.template set<0>(std::forward<Args&&>(args)...);
  return res;
}

template <typename L, typename R>
template <typename... Args>
constexpr
auto either<L, R>::from_right(Args&&... args) 
  noexcept(noexcept(R(std::declval<Args&&>()...))) ->
either<L, R>
{
  either<L, R> res;
  res.template set<1>(std::forward<Args&&>(args)...);
  return res;
}

template <typename L, typename R>
constexpr
auto either<L, R>::is_left() const 
  noexcept ->
bool
{
  return this->template is<0>();
}

template <typename L, typename R>
constexpr
auto either<L, R>::is_right() const 
  noexcept ->
bool
{
  return this->template is<1>();
}

template <typename L, typename R>
constexpr
auto either<L, R>::expect_left (
    const std::string& msg) ->
L&
{
  if (is_left()) {
    return this->template get<0>();
  } else {
    throw bad_either_access(msg);
  }
}

template <typename L, typename R>
constexpr
auto either<L, R>::expect_left (
    const std::string& msg) const ->
const L&
{
  if (is_left()) {
    return this->template get<0>();
  } else {
    throw bad_either_access(msg);
  }
}

template <typename L, typename R>
constexpr
auto either<L, R>::expect_right (
    const std::string& msg) ->
R&
{
  if (is_right()) {
    return this->template get<1>();
  } else {
    throw bad_either_access(msg);
  }
}

template <typename L, typename R>
constexpr
auto either<L, R>::expect_right (
    const std::string& msg) const ->
const R&
{
  if (is_right()) {
    return this->template get<1>();
  } else {
    throw bad_either_access(msg);
  }
}

template <typename L, typename R>
constexpr
auto either<L, R>::unwrap_left() ->
L&
{
  return expect_left("Expected left where there was no left");
}

template <typename L, typename R>
constexpr
auto either<L, R>::unwrap_left() const ->
const L&
{
  return expect_left("Expected left where there was no left");
}

template <typename L, typename R>
constexpr
auto either<L, R>::unwrap_right() ->
R&
{
  return expect_right("Expected right where there was no right");
}

template <typename L, typename R>
constexpr
auto either<L, R>::unwrap_right() const ->
const R&
{
  return expect_right("Expected right where there was no right");
}

template <typename L, typename R>
constexpr
auto either<L, R>::unwrap_left_or(L& alt)
  noexcept ->
L&
{
  if (is_left()) {
    return this->template get<0>();
  } else {
    return alt;
  }
}

template <typename L, typename R>
template <typename... Args>
constexpr
auto either<L, R>::unwrap_left_or(Args&&... args) const
  noexcept(noexcept(L(std::declval<Args&&>()...))) ->
const L&
{
  if (is_left()) {
    return this->template get<0>();
  } else {
    return L(std::forward<Args&&>()...);
  }
}

template <typename L, typename R>
constexpr
auto either<L, R>::unwrap_right_or(R& alt)
  noexcept ->
R&
{
  if (is_right()) {
    return this->template get<1>();
  } else {
    return alt;
  }
}

template <typename L, typename R>
template <typename... Args>
constexpr
auto either<L, R>::unwrap_right_or(Args&&... args) const
  noexcept(noexcept(R(std::declval<Args&&>()...))) ->
const R&
{
  if (is_right()) {
    return this->template get<1>();
  } else {
    return R(std::forward<Args&&>()...);
  }
}

template <typename L, typename R>
template <typename F>
constexpr
auto either<L, R>::map_left(F&& f) const
  noexcept(noexcept(std::declval<F>()(std::declval<L>()))) ->
either<std::result_of_t<F(L)>, R>
{
  using result_type = either<std::result_of_t<F(L)>, R>;
  if (is_left()) {
    return result_type::from_left(f(this->template get<0>()));
  } else {
    return result_type::from_right(this->template get<1>());
  }
}

template <typename L, typename R>
template <typename F>
constexpr
auto either<L, R>::map_right(F&& f) const
  noexcept(noexcept(std::declval<F>()(std::declval<R>()))) ->
either<L, std::result_of_t<F(R)>>
{
  using result_type = either<L, std::result_of_t<F(R)>>;
  if (is_right()) {
    return result_type::from_right(f(this->template get<1>()));
  } else {
    return result_type::from_left(this->template get<0>());
  }
}

template <typename L, typename R>
template <typename F>
constexpr
auto either<L, R>::map_left_or(const L& alt, F&& f) const
  noexcept(noexcept(std::declval<F>()(std::declval<L>()))) ->
std::result_of_t<F(L)>
{
  if (is_left()) {
    return f(this->template get<0>());
  } else {
    return f(alt);
  }
}

template <typename L, typename R>
template <typename F>
constexpr
auto either<L, R>::map_right_or(const R& alt, F&& f) const
  noexcept(noexcept(std::declval<F>()(std::declval<R>()))) ->
std::result_of_t<F(R)>
{
  if (is_right()) {
    return f(this->template get<1>());
  } else {
    return f(alt);
  }
}

template <typename L, typename R>
template <typename F, typename D>
constexpr
auto either<L, R>::map_left_or_else(D&& d, F&& f) const
  noexcept(noexcept(std::declval<F>()(std::declval<L>())) and
           noexcept(std::declval<D>()())) ->
std::result_of_t<F(L)>
{
  if (is_left()) {
    return f(this->template get<0>());
  } else {
    return f(d());
  }
}

template <typename L, typename R>
template <typename F, typename D>
constexpr
auto either<L, R>::map_right_or_else(D&& d, F&& f) const
  noexcept(noexcept(std::declval<F>()(std::declval<R>())) and
           noexcept(std::declval<D>()())) ->
std::result_of_t<F(R)>
{
  if (is_right()) {
    return f(this->template get<1>());
  } else {
    return f(d());
  }
}

template <typename L, typename R>
constexpr
auto either<L, R>::get_left() const
  noexcept ->
optional<L>
{
  if (is_left()) {
    return optional<L>::from_some(this->template get<0>());
  } else {
    return optional<L>::from_none();
  }
}

template <typename L, typename R>
constexpr
auto either<L, R>::get_right() const
  noexcept ->
optional<R>
{
  if (is_right()) {
    return optional<R>::from_some(this->template get<1>());
  } else {
    return optional<R>::from_none();
  }
}

template <typename L, typename R>
template <typename F>
constexpr
auto either<L, R>::flatmap_left(F&& f) const 
  noexcept(noexcept(std::declval<F>()(std::declval<L>()))) ->
std::result_of_t<F(L)>
{
  using result_type = std::result_of_t<F(L)>;
  if (is_left()) {
    return f(this->template get<0>());
  } else {
    return result_type::from_right(this->template get<1>());
  }
}

template <typename L, typename R>
template <typename F>
constexpr
auto either<L, R>::flatmap_right(F&& f) const
  noexcept(noexcept(std::declval<F>()(std::declval<R>()))) ->
std::result_of_t<F(R)>
{
  using result_type = std::result_of_t<F(R)>;
  if (is_right()) {
    return f(this->template get<1>());
  } else {
    return result_type::from_left(this->template get<0>());
  }
}
