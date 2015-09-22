#pragma once

#include "regex_expression_generator.h"
#include "../monad/either.h"

#include <functional>
#include <string>

auto make_regex_generator(const std::string&) ->
std::either<std::function<std::string()>, std::string>;
