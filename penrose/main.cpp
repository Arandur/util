#include "penrose.h"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <cmath>
#include <iostream>

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

auto draw (
    cv::Mat& m, const half_kite& hk, 
    const cv::Scalar& color = CV_RGB(0, 0, 0)) ->
cv::Mat&
{
  cv::Point a (hk.a.real(), hk.a.imag()),
            b (hk.b.real(), hk.b.imag()),
            c (hk.c.real(), hk.c.imag());

  cv::line(m, b, c, color);
  cv::line(m, c, a, color);

  return m;
}

int main()
{
  std::set<half_kite> half_kites;
  std::set<half_dart> half_darts;

  std::tie(half_kites, half_darts) = penrose_sun(point(256, 256), 256);

  cv::Mat img = cv::Mat::zeros(512, 512, CV_8UC3);

  for (const auto& hk : half_kites) {
    draw(img, hk);
  }

  cv::imwrite("penrose.png", img);

  return 0;
}
