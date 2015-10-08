#pragma once

#include "../monad/either.h"

#include <string>
#include <vector>

#ifdef DEBUG
#include <iostream>
#endif

namespace rregex
{
  enum class token_type;

  struct token
  {
    token_type type;
    std::string value;

    token (
        token_type, 
        const std::string&);

    token (
        token_type,
        char[]);

    token (
        token_type,
        char);
  };

#ifdef DEBUG
  auto operator << (std::ostream&, const token&) ->
  std::ostream&;
#endif

  auto lex(const std::string&) ->
  either<std::vector<token>, std::string>;
}
