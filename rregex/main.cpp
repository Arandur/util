#include "rregex.h"

#include <cstdlib>
#include <cstdio>
#include <utility>
#include <string>

// TODO: This should be spun out to a proper formatter (or use an existing
// library), since fprintf() doesn't handle std::strings.
template <typename... Args>
[[noreturn]]
void exit(int exit_code, Args&&... args)
{
  fprintf(stderr, std::forward<Args&&>(args)...);
  std::exit(exit_code);
}

int main(int argc, char** argv)
{
  const std::string regex = [=]
  {
    if (argc < 2) {
      exit(1, "Usage: %s regex [n]\n", argv[0]);
    }

    return argv[1];
  }();

  const auto n = [=]
  {
    if (argc < 3) {
      return 1;
    }

    try {
      return std::stoull(argv[2]);
    } catch (const std::invalid_argument& e) {
      exit(1, "Usage: %s regex [n]\n", argv[0]);
    } catch (const std::out_of_range& e) {
      std::cerr << "Argument out of range: " << argv[2]
                << "; defaulting to 1" << std::endl;
    }
  }();

  auto regex_generator = make_regex_generator(regex);

  if (regex_generator.is_left()) {
    auto& generator = *regex_generator.get_left();
    std::generate_n (
        std::ostream_iterator<std::string>(std::cin, '\n'),
        std::ostream_iterator<std::string>(),
        generator);
  } else {
    exit(1, regex_generator.is_right().c_str());
  }

  return 0;
}
