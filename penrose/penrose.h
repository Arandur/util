#include <complex>
#include <vector>
#include <tuple>
#include <functional>
#include <set>

using std::complex;

using point = complex<long double>;

struct half_kite;
struct half_dart;

struct half_kite
{
  half_kite(const point&, const point&, const point&);

  const point a, b, c;

  auto decompose() const ->
    std::tuple<half_kite, half_kite, half_dart>;
};

struct half_dart
{
  half_dart(const point&, const point&, const point&);

  const point a, b, c;

  auto decompose() const ->
    std::tuple<half_kite, half_dart>;
};

namespace std
{
  template <>
  struct less<half_kite>
  {
    auto operator () (
        const half_kite&, const half_kite&) const noexcept ->
    bool;
  };

  template <>
  struct less<half_dart>
  {
    auto operator () (
        const half_dart&, const half_dart&) const noexcept ->
    bool;
  };
}

auto penrose_decompose (
    const std::set<half_kite>&,
    const std::set<half_dart>&) ->
std::tuple <
    std::set<half_kite>,
    std::set<half_dart>>;
