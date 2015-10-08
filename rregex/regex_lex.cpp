#include "regex_lex.h"

#include <algorithm>

#ifdef DEBUG
#include <sstream>
#endif

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
    caret,
    integer,
    literal_range,
    literal_string
  };

#ifdef DEBUG
  auto operator << (std::ostream& os, const token_type& t) ->
  std::ostream&
  {
    switch (t) {
    case token_type::open_paren:
      return os << "open_paren";
    case token_type::close_paren:
      return os << "close_paren";
    case token_type::open_brace:
      return os << "open_brace";
    case token_type::close_brace:
      return os << "close_brace";
    case token_type::open_bracket:
      return os << "open_bracket";
    case token_type::close_bracket:
      return os << "close_bracket";
    case token_type::vertical_bar:
      return os << "vertical_bar";
    case token_type::question_mark:
      return os << "question_mark";
    case token_type::asterisk:
      return os << "asterisk";
    case token_type::plus:
      return os << "plus";
    case token_type::comma:
      return os << "comma";
    case token_type::caret:
      return os << "caret";
    case token_type::integer:
      return os << "integer";
    case token_type::literal_range:
      return os << "literal_range";
    case token_type::literal_string:
      return os << "literal_string";
    }

    __builtin_unreachable();
  }
#endif

  token::token (
      token_type _type, 
      const std::string& _value) :
    type(_type),
    value(_value)
  {}

  token::token (
      token_type _type,
      char _value[]) :
    type(_type),
    value(_value)
  {}

  token::token (
      token_type _type,
      char _value) :
    type(_type),
    value(1, _value)
  {}

#ifdef DEBUG
  auto operator << (std::ostream& os, const token& t) ->
  std::ostream&
  {
    std::ostringstream ss;
    ss << "token { type: " << t.type << "; value: \"" << t.value << "\" };";
    return os << ss.str();
  }
#endif

  template <
    typename InputIt,
    typename Sentinel>
  auto lex (
      InputIt first,
      Sentinel last) ->
  either<std::vector<token>, std::string>
  {
    std::vector<token> tokens;
    tokens.reserve(std::distance(first, last));

    while (first != last) {
      switch (*first) {
      case '(':
        tokens.emplace_back(token_type::open_paren, *first++);
        break;
      case ')':
        tokens.emplace_back(token_type::close_paren, *first++);
        break;
      case '[':
        tokens.emplace_back(token_type::open_bracket, *first++);
        
        if (*first == '^') {
          tokens.emplace_back(token_type::caret, *first++);
        }

        for (; *first != ']' and first != last; ++first) {
          if (*first == '\\')
            ++first;
          tokens.emplace_back(token_type::literal_string, *first);
        }

        if (first == last) {
          return either<std::vector<token>, std::string>::from_right (
              "End of regex reached; closing bracket not found");
        }

        tokens.emplace_back(token_type::close_bracket, *first++);
        break;
      case '{':
        {
          tokens.emplace_back(token_type::open_brace, *first++);

          const static char s[] = ",}";
          auto it = std::find_first_of (
              first, last, std::begin(s), std::end(s));

          if (it == last) {
            return either<std::vector<token>, std::string>::from_right (
                "End of regex reached; closing brace not found");
          }

          tokens.emplace_back(token_type::integer, std::string(first, it));

          if (*it == ',') {
            tokens.emplace_back(token_type::comma, *it);
            first = ++it;
            if (first != last and *first != '}') {
              it = std::find(first, last, '}');
              tokens.emplace_back(token_type::integer, std::string(first, it));
            }
          }

          if (it == last) {
            return either<std::vector<token>, std::string>::from_right (
                "End of regex reached; closing brace not found");
          }

          first = it;
          tokens.emplace_back(token_type::close_brace, *first++);
        }
        break;
      case '+':
        tokens.emplace_back(token_type::plus, *first++);
        break;
      case '?':
        tokens.emplace_back(token_type::question_mark, *first++);
        break;
      case '*':
        tokens.emplace_back(token_type::asterisk, *first++);
        break;
      case '|':
        tokens.emplace_back(token_type::vertical_bar, *first++);
        break;
      case '\\':
        // TODO: Make this handle explicit ranges and other character escapes
        ++first;
        // fallthrough
      default: 
        {
          const static char s[] = "()\\+?*[{|";
          auto it = std::find_first_of (
              first, last, std::begin(s), std::end(s));

          if (it != last) {
            switch (*it) {
            case '*':
            case '?':
            case '+':
            case '{':
              --it;
              if (first != it) {
                tokens.emplace_back(token_type::literal_string, std::string(first, it));
              }
              tokens.emplace_back(token_type::literal_string, *it++);
              break;
            default:
              tokens.emplace_back(token_type::literal_string, std::string(first, it));
            }
          }

          first = it;
        }
      }
    }

    return either<std::vector<token>, std::string>::from_left(tokens);
  }

  auto lex(const std::string& regex) ->
  either<std::vector<token>, std::string>
  {
    return lex(std::begin(regex), std::end(regex));
  }
}
