#include "penrose.h"

static auto vec_proj (const point& a, const point& b, const long double& len) ->
point
{
  const auto v = b - a;
  return a + b / std::abs (v) * len;
}

half_kite::half_kite (const point& _a, const point& _b, const point& _c) :
  a (_a), b (_b), c (_c)
{}

auto half_kite::decompose () const ->
std::tuple<half_kite, half_kite, half_dart>
{
  const point d = vec_proj (a, b, std::abs (c - b)),
              e = vec_proj (c, a, std::abs (c - b));

  return std::make_tuple (
      half_kite (c, d, b), half_kite (c, d, e), half_dart (a, e, d));
}

half_dart::half_dart (const point& _a, const point& _b, const point& _c) :
  a (_a), b (_b), c (_c)
{}

auto half_dart::decompose () const ->
std::tuple<half_kite, half_dart>
{
  const point d = vec_proj (b, c, std::abs (b - a));

  return std::make_tuple (half_kite (b, a, d), half_dart (d, c, a));
}

auto std::less<half_kite>::operator () (
    const half_kite& hk1, const half_kite& hk2) const noexcept ->
bool
{
  return
    std::make_tuple (
        hk1.a.real(), hk1.a.imag(),
        hk1.b.real(), hk1.b.imag(),
        hk1.c.real(), hk1.c.imag()) <
    std::make_tuple (
        hk2.a.real(), hk2.a.imag(),
        hk2.b.real(), hk2.b.imag(),
        hk2.c.real(), hk2.c.imag());
}

auto std::less<half_dart>::operator () (
    const half_dart& hd1, const half_dart& hd2) const noexcept ->
bool
{
  return
    std::make_tuple (
        hd1.a.real(), hd1.a.imag(),
        hd1.b.real(), hd1.b.imag(),
        hd1.c.real(), hd1.c.imag()) <
    std::make_tuple (
        hd2.a.real(), hd2.a.imag(),
        hd2.b.real(), hd2.b.imag(),
        hd2.c.real(), hd2.c.imag());
}

auto penrose_decompose (
    const std::set<half_kite>& old_half_kites,
    const std::set<half_dart>& old_half_darts) ->
std::tuple <
    std::set<half_kite>,
    std::set<half_dart>>
{
  std::set<half_kite> new_half_kites;
  std::set<half_dart> new_half_darts;

  for (const half_kite& hk : old_half_kites) {
    const auto decomp_results = hk.decompose();
    new_half_kites.insert(std::get<0>(decomp_results));
    new_half_kites.insert(std::get<1>(decomp_results));
    new_half_darts.insert(std::get<2>(decomp_results));
  }

  for (const half_dart& hd : old_half_darts) {
    const auto decomp_results = hd.decompose();
    new_half_kites.insert(std::get<0>(decomp_results));
    new_half_darts.insert(std::get<1>(decomp_results));
  }

  return std::make_tuple(new_half_kites, new_half_darts);
}
