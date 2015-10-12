template <typename T>
template <typename... Args>
constexpr
auto result<T>::from_ok(Args&&... args)
  noexcept(noexcept(T(std::declval<Args&&>()...))) ->
result<T>
{
  return static_cast<result<T>>(Base::from_left(std::forward<Args&&>(args)...));
}

template <typename T>
template <typename... Args>
constexpr
auto result<T>::from_err(Args&&... args)
  noexcept(noexcept(std::string(std::declval<Args&&>()...))) ->
result<T>
{
  return Base::from_right(std::forward<Args&&>(args)...);
}

template <typename T>
constexpr
auto result<T>::is_ok() const
  noexcept ->
bool
{
  return Base::is_left();
}

template <typename T>
constexpr
auto result<T>::is_err() const
  noexcept ->
bool
{
  return Base::is_right();
}

template <typename T>
constexpr
auto result<T>::expect (
    const std::string& msg) ->
T&
{
  return Base::expect_left(msg);
}

template <typename T>
constexpr
auto result<T>::expect (
    const std::string& msg) const ->
const T&
{
  return Base::expect_left(msg);
}

template <typename T>
constexpr
auto result<T>::expect_err (
    const std::string& msg) ->
std::string&
{
  return Base::expect_right(msg);
}

template <typename T>
constexpr
auto result<T>::expect_err (
    const std::string& msg) const ->
const std::string&
{
  return Base::expect_right(msg);
}

template <typename T>
constexpr
auto result<T>::unwrap() ->
T&
{
  return expect("Expected value where there was no value");
}

template <typename T>
constexpr
auto result<T>::unwrap() const ->
const T&
{
  return expect("Expected value where there was no value");
}

template <typename T>
constexpr
auto result<T>::unwrap_err() ->
std::string&
{
  return expect_err("Expected error where there was no error");
}

template <typename T>
constexpr
auto result<T>::unwrap_err() const ->
const std::string&
{
  return expect_err("Expected error where there was no error");
}

template <typename T>
constexpr
auto result<T>::unwrap_or(T& alt)
  noexcept ->
T&
{
  return Base::unwrap_left(alt);
}

template <typename T>
template <typename... Args>
constexpr
auto result<T>::unwrap_or(Args&&... args) const
  noexcept(noexcept(T(std::declval<Args&&>()...))) ->
const T&
{
  return Base::unwrap_left(std::forward<Args&&>(args)...);
}

template <typename T>
template <typename F>
constexpr
auto result<T>::unwrap_or_else(F&& f) const
  noexcept(noexcept(std::declval<F>()())) ->
const T&
{
  return Base::unwrap_left_or_else(std::forward<F&&>(f));
}

template <typename T>
template <typename F>
constexpr
auto result<T>::map(F&& f) const
  noexcept(noexcept(std::declval<F>()(std::declval<T>()))) ->
result<std::result_of_t<F(T)>>
{
  return Base::map_left(std::forward<F&&>(f));
}

template <typename T>
template <typename F>
constexpr
auto result<T>::map_err(F&& f) const
  noexcept(noexcept(std::declval<F>()(std::declval<std::string>()))) ->
either<T, std::result_of_t<F(std::string)>>
{
  return Base::map_right(std::forward<F&&>(f));
}

template <typename T>
constexpr
auto result<T>::get() const
  noexcept ->
optional<T>
{
  if (is_ok()) {
    return optional<T>::from_some(this->template get<0>());
  } else {
    return optional<T>::from_none();
  }
}

template <typename T>
constexpr
auto result<T>::get_err() const
  noexcept ->
optional<std::string>
{
  if (is_ok()) {
    return optional<T>::from_none();
  } else {
    return optional<T>::from_some(this->template get<1>());
  }
}

template <typename T>
template <typename F>
constexpr
auto result<T>::flatmap(F&& f) const
  noexcept(noexcept(std::declval<F>()(std::declval<T>()))) ->
std::result_of_t<F(T)>
{
  return Base::flatmap_left(f);
}

template <typename T>
template <typename F>
constexpr
auto result<T>::flatmap_err(F&& f) const
  noexcept(noexcept(std::declval<F>()(std::declval<std::string>()))) ->
std::result_of_t<F(std::string)>
{
  return Base::flatmap_right(f);
}
