template <typename... Args>
constexpr
variant<Args...>::variant()
{}

template <typename... Args>
variant<Args...>::~variant()
{
  reset();
}

template <typename... Args>
template <size_t N>
constexpr
auto variant<Args...>::is() const ->
bool
{
  return N + 1 == i;
}

template <typename... Args>
constexpr
auto variant<Args...>::has_value() const ->
bool
{
  return i != 0;
}

template <typename... Args>
template <size_t N>
constexpr
auto variant<Args...>::get() ->
nth_t<N, Args...>&
{
  return reinterpret_cast<nth_t<N, Args...>&>(buffer);
}

template <typename... Args>
template <size_t N>
constexpr
auto variant<Args...>::get() const ->
const nth_t<N, Args...>&
{
  return reinterpret_cast<const nth_t<N, Args...>&>(buffer);
}

template <typename... Args>
template <size_t N, typename... cArgs>
constexpr
void variant<Args...>::set(cArgs&&... args)
{
  this->~variant();
  new (&buffer) nth_t<N, Args...>(std::forward<cArgs&&>(args)...);
  i = N + 1;
  destructor = [this] { reinterpret_cast<nth_t<N, Args...>&>(buffer).~nth_t<N, Args...>(); };
}

template <typename... Args>
constexpr
void variant<Args...>::reset()
{
  if (i) {
    i = 0;
    destructor();
  }
}
