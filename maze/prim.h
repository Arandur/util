#include <vector>
#include <cstdint>
#include <utility>
#include <random>
#include <algorithm>
#include <iostream>
#include <sstream>

struct vertex {
  uint32_t x, y;
};

auto operator == (const vertex& v1, const vertex& v2) noexcept ->
bool
{
  return v1.x == v2.x and v1.y == v2.y;
}

struct edge {
  vertex a, b;
};

auto operator << (std::ostream& out, const vertex& v) ->
std::ostream&
{
  std::ostringstream oss;
  oss << "(" << v.x << ", " << v.y << ")";
  return out << oss.str();
}

auto operator << (std::ostream& out, const edge& e) ->
std::ostream&
{
  std::ostringstream oss;
  oss << e.a << " - " << e.b;
  return out << oss.str();
}

/**
 * This is highly imperfect; it uniformly chooses from a set of edges excluding
 * those along the bottom and right sides of the graph.
 *
 * This could be fixed by choosing the direction first (down or right), then 
 * choosing uniformly from those vertices which have vertices in those
 * directions.
 */
template <typename URNG>
auto random_edge (
    const uint32_t width, const uint32_t height,
    URNG&& g) ->
edge
{
  std::uniform_int_distribution<uint32_t>
    w_dist(0, width - 2),
    h_dist(0, height - 2);

  std::uniform_int_distribution<uint8_t>
    b_dist(0, 1);

  vertex v { w_dist(g), h_dist(g) };

  if (b_dist(g)) {
    return {v, {v.x + 1, v.y}};
  } else {
    return {v, {v.x, v.y + 1}};
  }
}

auto calculate_boundary (
    const uint32_t width, const uint32_t height,
    const std::vector<vertex>& vertices_in_map) ->
std::vector<edge>
{
  std::vector<edge> boundary;
  boundary.reserve(3 * vertices_in_map.size() + 1);

  for (const auto& v : vertices_in_map) {
    if (v.x != 0) { // Left
      const vertex _v {v.x - 1, v.y};
      if (std::find (
              std::begin(vertices_in_map), 
              std::end(vertices_in_map), 
              _v) == std::end(vertices_in_map)) {
        boundary.push_back({v, _v});
      }
    }
    if (v.y != 0) { // Up
      const vertex _v {v.x, v.y - 1};
      if (std::find (
              std::begin(vertices_in_map), 
              std::end(vertices_in_map), 
              _v) == std::end(vertices_in_map)) {
        boundary.push_back({v, _v});
      }
    }
    if (v.x != width - 1) { // Right
      const vertex _v {v.x + 1, v.y};
      if (std::find (
              std::begin(vertices_in_map), 
              std::end(vertices_in_map), 
              _v) == std::end(vertices_in_map)) {
        boundary.push_back({v, _v});
      }
    }
    if (v.y != height - 1) { // Down
      const vertex _v {v.x, v.y + 1};
      if (std::find (
              std::begin(vertices_in_map), 
              std::end(vertices_in_map), 
              _v) == std::end(vertices_in_map)) {
        boundary.push_back({v, _v});
      }
    }
  }

  return boundary;
}

template <
    typename Container,
    typename URNG>
auto random_choice (
    Container&& c,
    URNG&& g) ->
typename std::decay_t<Container>::const_reference
{
  auto first = std::begin(c);
  const auto last = std::end(c);

  const auto dist = std::distance(first, last);
  std::uniform_int_distribution<decltype(dist)> dist_dist(0, dist - 1);
  const auto i = dist_dist(g);

  std::advance(first, i);

  return *first;
}

template <typename URNG>
auto prim (
    const uint32_t width, const uint32_t height,
    URNG&& g) ->
std::vector<edge>
{
  std::vector<edge> edges;
  std::vector<vertex> vertices_in_map;

  edges.reserve(width * height - 1);
  vertices_in_map.reserve(width * height);

  edges.push_back(random_edge(width, height, g));
  vertices_in_map.push_back(edges.back().a);
  vertices_in_map.push_back(edges.back().b);

  while (true) {
    const std::vector<edge> boundary = 
      calculate_boundary(width, height, vertices_in_map);

    if (boundary.empty()) {
      break;
    }

    edges.push_back(random_choice(boundary, g));
    vertices_in_map.push_back(edges.back().a);
    vertices_in_map.push_back(edges.back().b);
  }

  return edges;
}
