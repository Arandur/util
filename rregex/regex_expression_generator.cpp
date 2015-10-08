#include "regex_expression_generator.h"

regex_expression_generator::regex_expression_generator (
    std::vector<std::unique_ptr<regex_generator>>&& _alternatives) :
  alternatives(std::move(_alternatives)),
  dist(0, alternatives.size())
{}

regex_expression_generator::regex_expression_generator (
    regex_expression_generator&& source) :
  regex_expression_generator(std::move(source.alternatives))
{}

auto regex_expression_generator::operator = (
    regex_expression_generator&& source) ->
regex_expression_generator&
{
  alternatives.assign (
      std::make_move_iterator(std::begin(source.alternatives)),
      std::make_move_iterator(std::end(source.alternatives)));
  dist.param(typename decltype(dist)::param_type(0, alternatives.size()));

  return *this;
}

auto regex_expression_generator::operator () () ->
std::string
{
  return (*alternatives[dist(g)])();
}
