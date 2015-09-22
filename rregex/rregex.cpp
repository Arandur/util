#include "rregex.h"

#include "../monad/either.h"
#include "regex_generator.h"
#include "lex_regex.h"

#include <functional>
#include <string>

namespace rregex
{
  template <typename InputIt>
  struct parse_result
  {
    using result_type = std::either<std::unique_ptr<regex_generator>, std::string>;
    InputIt pos;
    result_type result;
  };

  template <
    typename InputIt
    typename Sentinel>
  auto parse_alternative (
      InputIt first,
      Sentinel last) ->
  parse_result<InputIt>
  {
    std::vector<std::unique_ptr<regex_generator>> items;

    while (first != last) {
      auto item = parse_item(first, last);
      auto& result = item.result;

      if (result.is_left()) {
        items.push_back(std::move(result.get_left()));
      } else {
        return item;
      }

      first = item.pos;
    }

    return {
      first,
      typename parse_result<InputIt>::result_type::from_left (
          std::move(items))};
  }
  
  template <
    typename InputIt,
    typename Sentinel>
  auto parse_expression (
      InputIt first,
      Sentinel last) ->
  parse_result<InputIt>
  {
    std::vector<std::unique_ptr<regex_generator>> alternatives;

    while (first != last) {
      auto alternative = parse_alternative(first, last);
      auto& result = alternative.result;

      if (result.is_left()) {
        alternatives.push_back(std::move(result.get_left()));
      } else {
        return alternative;
      }

      first = alternative.pos;
    }

    return {
      first, 
      typename parse_result<InputIt>::result_type::from_left (
          std::move(alternatives))};
  }

  template <typename Container>
  auto parse_expression(Container container) ->
  parse_result<typename Container::const_iterator>
  {
    return parse_expression(std::begin(container), std::end(container));
  }
}

auto make_regex_generator(const std::string& regex) ->
std::either<std::unique_ptr<regex_generator>, std::string>
{
  return rregex::parse_expression(rregex::lex(regex)).result;
}
