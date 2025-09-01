#pragma once

#include <chrono>
#include <functional>
#include <list>

#include "month.h"

namespace Calendar {

class Year {
public:
  Year(std::chrono::year year);

private:
  std::list<std::reference_wrapper<Month>> m_months;
  const std::chrono::year m_year;
};

}
