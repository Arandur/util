#include "regex_lex.h"

namespace rregex
{
  enum class token_type
  {
    open_paren,
    close_paren,
    open_brace,
    close_brace,
    open_bracket,
    close_bracket,
    vertical_bar,
    question_mark,
    asterisk,
    plus,
    comma,
    integer,
    literal_range,
    literal_string
  };

  enum class lexer_state
  {
    null,
    in_range,
    in_braces,
    in_string
  };

  struct token
  {
    token_type type;
    std::string value;

    token (
        token_type _type, 
        const std::string& _value) :
      type(_type),
      value(_value)
    {}

    token (
        token_type _type,
        char _value[]) :
      type(_type),
      value(_value)
    {}
  };

  auto lex(const std::string& regex) ->
  std::vector<token>
  {
    std::vector<token> tokens;
    tokens.reserve(regex.size());

    std::string literal;
    lexer_state curr_state = lexer_state::null;

    for (const auto& c : regex) {
      switch (curr_state) {
      case lexer_state::null:

        break;
      case lexer_state::in_range:

        break;
      case lexer_state::in_braces:

        break;
      case lexer_state::in_string:

        break;
      }
    }
  }
}
