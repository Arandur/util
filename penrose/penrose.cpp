#include <complex>
#include <set>
#include <tuple>
#include <cmath> // M_PI
#include <iostream>

struct half_dart;

struct half_kite
{
  half_kite (
      const std::complex<long double>&,
      const std::complex<long double>&,
      const std::complex<long double>&);

  const std::complex<long double> a, b, c;

  auto decompose() const ->
    std::tuple<half_kite, half_kite, half_dart>;
};

struct half_dart
{
  half_dart (
      const std::complex<long double>&,
      const std::complex<long double>&,
      const std::complex<long double>&);

  const std::complex<long double> a, b, c;

  auto decompose() const ->
    std::tuple<half_kite, half_dart>;
};

template <typename T>
static auto vec_proj (
    const std::complex<T>& a,
    const std::complex<T>& b,
    const T& len) ->
std::complex<T>
{
  const auto v = b - a;

  return v / std::abs(v) * len;
}

half_kite::half_kite (
    const std::complex<long double>& _a,
    const std::complex<long double>& _b,
    const std::complex<long double>& _c) :
  a(_a), b(_b), c(_c)
{}

auto half_kite::decompose() const ->
std::tuple<half_kite, half_kite, half_dart>
{
  const std::complex<long double> d = vec_proj(a, b, std::abs(c - b));
  const std::complex<long double> e = vec_proj(c, a, std::abs(c - b));

  return std::make_tuple(half_kite(c, d, b), half_kite(c, d, e), half_dart(a, e, d));
}

half_dart::half_dart (
    const std::complex<long double>& _a,
    const std::complex<long double>& _b,
    const std::complex<long double>& _c) :
  a(_a), b(_b), c(_c)
{}

auto half_dart::decompose() const ->
std::tuple<half_kite, half_dart>
{
  const std::complex<long double> d = vec_proj(b, c, std::abs(b - a));

  return std::make_tuple(half_kite(b, a, d), half_dart(d, c, a));
}

struct half_kite_cmp
{
  auto operator () (const half_kite& hk1, const half_kite& hk2) const ->
  bool
  {
    return 
      std::tie (
          hk1.a.real(), hk1.a.imag(), 
          hk1.b.real(), hk1.b.imag(), 
          hk1.c.real(), hk1.c.imag()) < 
      std::tie (
          hk2.a.real(), hk2.a.imag(), 
          hk2.b.real(), hk2.b.imag(), 
          hk2.c.real(), hk2.c.imag());
  }
};

struct half_dart_cmp
{
  auto operator () (const half_dart& hd1, const half_dart& hd2) const ->
  bool
  {
    return
      std::tie (
          hd1.a.real(), hd1.a.imag(), 
          hd1.b.real(), hd1.b.imag(), 
          hd1.c.real(), hd1.c.imag()) < 
      std::tie (
          hd2.a.real(), hd2.a.imag(), 
          hd2.b.real(), hd2.b.imag(), 
          hd2.c.real(), hd2.c.imag());
  }
};


template <
    typename ContainerHalfKites,
    typename ContainerHalfDarts>
auto penrose_decompose (
    const ContainerHalfKites& old_half_kites,
    const ContainerHalfDarts& old_half_darts) ->
std::tuple <
    std::set<half_kite, half_kite_cmp>,
    std::set<half_dart, half_dart_cmp>>
{
  std::set<half_kite, half_kite_cmp> new_half_kites;
  std::set<half_dart, half_dart_cmp> new_half_darts;

  for (const half_kite& hk : old_half_kites) {
    const auto children = hk.decompose();
    new_half_kites.insert(std::get<0>(children));
    new_half_kites.insert(std::get<1>(children));
    new_half_darts.insert(std::get<2>(children));
  }

  for (const half_dart& hd : old_half_darts) {
    const auto children = hd.decompose();
    new_half_kites.insert(std::get<0>(children));
    new_half_darts.insert(std::get<1>(children));
  }

  return std::make_tuple(new_half_kites, new_half_darts);
}

auto penrose_sun (
    const std::complex<long double>& center,
    long double radius) ->
std::tuple <
    std::set<half_kite, half_kite_cmp>,
    std::set<half_dart, half_dart_cmp>>
{
  using namespace std::complex_literals;

  const static std::complex<long double> 1i;
  const static long double alpha = M_PI / 5;

  return std::make_tuple (
      std::set<half_kite, half_kite_cmp> {
        half_kite ( center, 
            center + 1i, 
            center + 1i * radius * std::exp(1i *     alpha) ),
        half_kite ( center,
            center + 1i * radius * std::exp(1i * 2 * alpha),
            center + 1i * radius * std::exp(1i *     alpha) ),
        half_kite ( center,
            center + 1i * radius * std::exp(1i * 2 * alpha),
            center + 1i * radius * std::exp(1i * 3 * alpha) ),
        half_kite ( center,
            center + 1i * radius * std::exp(1i * 4 * alpha),
            center + 1i * radius * std::exp(1i * 3 * alpha) ),
        half_kite ( center,
            center + 1i * radius * std::exp(1i * 4 * alpha),
            center + 1i * radius * std::exp(1i * 5 * alpha) ),
        half_kite ( center,
            center + 1i * radius * std::exp(1i * 6 * alpha),
            center + 1i * radius * std::exp(1i * 5 * alpha) ),
        half_kite ( center,
            center + 1i * radius * std::exp(1i * 6 * alpha),
            center + 1i * radius * std::exp(1i * 7 * alpha) ),
        half_kite ( center,
            center + 1i * radius * std::exp(1i * 8 * alpha),
            center + 1i * radius * std::exp(1i * 7 * alpha) ),
        half_kite ( center,
            center + 1i * radius * std::exp(1i * 8 * alpha),
            center + 1i * radius * std::exp(1i * 9 * alpha) ),
        half_kite ( center,
            center + 1i,
            center + 1i * radius * std::exp(1i * 9 * alpha) ),
      }, 
      std::set<half_dart, half_dart_cmp> {});
}

int main()
{
  std::set<half_kite, half_kite_cmp> half_kites;
  std::set<half_dart, half_dart_cmp> half_darts;

  std::tie(half_kites, half_darts) = penrose_sun(0, 10);

  for (const auto& hk : half_kites) {
    std::cout << "(" 
              << hk.a << ", " 
              << hk.b << ", " 
              << hk.c << ")" << std::endl;
  }

  return 0;
}
