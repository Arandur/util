#pragma once

#include <string>

namespace rregex
{
  struct token;

  auto lex(const std::string&) ->
    std::vector<token>;
}
