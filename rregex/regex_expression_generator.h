#pragma once

#include "regex_generator.h"

#include <string>
#include <vector>
#include <memory>

class regex_expression_generator :
  regex_generator
{
public:

  regex_expression_generator (
      std::vector<std::unique_ptr<regex_generator>>&&);

  regex_expression_generator (
      regex_expression_generator&&);

  auto operator = (
      regex_expression_generator&&) ->
    regex_expression_generator&;

  virtual auto operator () () final override ->
    std::string;

private:

  std::vector<std::unique_ptr<regex_generator>> alternatives;
};