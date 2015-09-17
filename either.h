#pragma once

#include "variant.h"

template <typename L, typename R>
class either :
  private variant<L, R>
{
public:

  using variant<L, R>::variant();
};
