#include <random>

class regex_generator
{
public:

  virtual auto operator () () ->
    std::string = 0;

protected:

  static std::default_random_engine g;
};

std::default_random_engine regex_generator::g (
    std::random_device{}());
