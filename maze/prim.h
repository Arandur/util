#include <vector>
#include <cstdint>
#include <utility>
#include <random>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <set>

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

struct vertex_cmp
{
  auto operator () (const vertex& v1, const vertex& v2) noexcept ->
  bool
  {
    return std::tie(v1.x, v1.y) < std::tie(v2.x, v2.y);
  }
};

template <typename URNG>
auto random_edge (
    const uint32_t width, const uint32_t height,
    URNG&& g) ->
edge
{
  std::uniform_int_distribution<uint8_t>
    b_dist(0, 1);

  if (b_dist(g)) { // Horizontal
    std::uniform_int_distribution<uint32_t>
      x_dist(0, width - 2),
      y_dist(0, height - 1);

    vertex v { x_dist(g), y_dist(g) };

    return {v, {v.x + 1, v.y}};
  } else { // Vertical
    std::uniform_int_distribution<uint32_t>
      x_dist(0, width - 1),
      y_dist(0, height - 2);

    vertex v { x_dist(g), y_dist(g) };

    return {v, {v.x, v.y + 1}};
  }
}

auto calculate_boundary (
    const uint32_t width, const uint32_t height,
    const std::set<vertex, vertex_cmp>& vertices_in_map) ->
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
  std::set<vertex, vertex_cmp> vertices_in_map;

  edges.reserve(width * height - 1);

  edges.push_back(random_edge(width, height, g));
  vertices_in_map.insert(edges.back().a);
  vertices_in_map.insert(edges.back().b);

  while (true) {
    const std::vector<edge> boundary = 
      calculate_boundary(width, height, vertices_in_map);

    if (boundary.empty()) {
      break;
    }

    edges.push_back(random_choice(boundary, g));
    vertices_in_map.insert(edges.back().a);
    vertices_in_map.insert(edges.back().b);
  }

  return edges;
}
