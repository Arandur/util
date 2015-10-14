#include "penrose.h"

#include <cmath>

auto penrose_sun (
    const point& center,
    long double radius) ->
std::tuple <
    std::set<half_kite>,
    std::set<half_dart>>
{
  using namespace std::complex_literals;

  const point i_radius (0.0L, radius);
  const static point alpha (0.0L, M_PI / 5);

  std::set<half_kite> half_kites;
  
  for (long double i = 1; i < 10; i += 2) {
    half_kites.emplace (
        center,
        center + i_radius * std::exp((i - 1) * alpha),
        center + i_radius * std::exp( i      * alpha));
    half_kites.emplace (
        center,
        center + i_radius * std::exp((i + 1) * alpha),
        center + i_radius * std::exp( i      * alpha));
  }

  return std::make_tuple (
      half_kites,
      std::set<half_dart> {});
}

int main()
{
  return 0;
}
