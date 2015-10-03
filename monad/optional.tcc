template <typename T>
template <typename... Args>
constexpr
auto optional<T>::from_some(Args&&... args)
  noexcept(noexcept(T(std::declval<Args&&>()...))) ->
optional<T>
{
  optional<T> res;
  res.template set<0>(std::forward<Args&&>(args)...);
  return res;
}

template <typename T>
constexpr
auto optional<T>::from_none()
  noexcept ->
optional<T>
{
  return optional<T>();
}

template <typename T>
constexpr
optional<T>::operator bool () const
	noexcept
{
  return is_some();
}

template <typename T>
constexpr
auto optional<T>::operator ! () const
	noexcept ->
bool
{
  return ! static_cast<bool>(*this);
}

template <typename T>
constexpr
auto optional<T>::is_some() const
  noexcept ->
bool
{
  return this->template is<0>();
}

template <typename T>
constexpr
auto optional<T>::is_none() const
  noexcept ->
bool
{
  return ! is_some();
}

template <typename T>
constexpr
auto optional<T>::expect (
    const std::string& msg) ->
T&
{
  if (is_some()) {
    return this->template get<0>();
  } else {
    throw bad_optional_access(msg);
  }
}

template <typename T>
constexpr
auto optional<T>::expect (
    const std::string& msg) const ->
const T&
{
  if (is_some()) {
    return this->template get<0>();
  } else {
    throw bad_optional_access(msg);
  }
}

template <typename T>
constexpr
auto optional<T>::unwrap() ->
T&
{
  return expect("Tried to access empty optional value");
}

template <typename T>
constexpr
auto optional<T>::unwrap() const ->
const T&
{
  return expect("Tried to access empty optional value");
}

template <typename T>
constexpr
auto optional<T>::unwrap_or(T& alt)
  noexcept ->
T&
{
  if (is_some()) {
    return this->template get<0>();
  } else {
    return alt;
  }
}

template <typename T>
template <typename... Args>
constexpr
auto optional<T>::unwrap_or(Args&&... args) const
  noexcept(noexcept(T(std::declval<Args&&>()...))) ->
const T&
{
  if (is_some()) {
    return this->template get<0>();
  } else {
    return T(std::forward<Args&&>(args)...);
  }
}

template <typename T>
template <typename F>
constexpr
auto optional<T>::unwrap_or_else(F&& f) const
  noexcept(noexcept(std::declval<F>()())) ->
const T&
{
  if (is_some()) {
    return this->template get<0>();
  } else {
    return f();
  }
}

template <typename T>
template <typename F>
constexpr
auto optional<T>::map(F&& f) const
  noexcept(noexcept(std::declval<F>()(std::declval<T>()))) ->
optional<std::result_of_t<F(T)>>
{
  using result_type = optional<std::result_of_t<F(T)>>;
  if (is_some()) {
    return result_type::from_some(f(this->template get<0>()));
  } else {
    return result_type::from_none();
  }
}

template <typename T>
template <typename F>
constexpr
auto optional<T>::map_or(const T& alt, F&& f) const
  noexcept(noexcept(std::declval<F>()(std::declval<T>()))) ->
std::result_of_t<F(T)>
{
  if (is_some()) {
    return f(this->template get<0>());
  } else { 
    return f(alt);
  }
}

template <typename T>
template <typename F, typename D>
constexpr
auto optional<T>::map_or_else(D&& d, F&& f) const
  noexcept(noexcept(std::declval<F>()(std::declval<T>())) && 
           noexcept(std::declval<D>()())) ->
std::result_of_t<F(T)>
{
  if (is_some()) {
    return f(this->template get<0>());
  } else {
    return f(d());
  }
}

template <typename T>
template <typename R>
constexpr
auto optional<T>::left_or(R&& right) const
  noexcept ->
either<T, R>
{
  if (is_some()) {
    return either<T, R>::from_left(this->template get<0>());
  } else {
    return either<T, R>::from_right(std::forward<R&&>(right));
  }
}

template <typename T>
template <typename F>
constexpr
auto optional<T>::left_or_else(F&& f) const
  noexcept(noexcept(std::declval<F>()())) ->
either<T, std::result_of_t<F()>>
{
  using result_type = either<T, std::result_of_t<F()>>;
  if (is_some()) {
    return result_type::from_left(this->template get<0>());
  } else {
    return result_type::from_right(f());
  }
}

template <typename T>
template <typename... Args>
constexpr
auto optional<T>::ok_or(Args&&... args) const
  noexcept(noexcept(result<T>::from_err(std::declval<Args&&>()...))) ->
result<T>
{
  if (is_some()) {
    return result<T>::from_ok(this->template get<0>());
  } else {
    return result<T>::from_err(std::forward<Args&&>(args)...);
  }
}

template <typename T>
template <typename F>
constexpr
auto optional<T>::ok_or_else(F&& f) const
  noexcept(noexcept(std::declval<F>()())) ->
result<T>
{
  if (is_some()) {
    return result<T>::from_ok(this->template get<0>());
  } else {
    return result<T>::from_err(f());
  }
}

template <typename T>
template <typename F>
constexpr
auto optional<T>::flatmap(F&& f) const 
  noexcept(noexcept(std::declval<F>()(std::declval<T>()))) ->
std::result_of_t<F(T)>
{
  using result_type = std::result_of_t<F(T)>;
  if (is_some()) {
    return f(this->template get<0>());
  } else {
    return result_type::from_none();
  }
}
