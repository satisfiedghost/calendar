#include <chrono>
#include <list>
#include <string>

#include "day.h"

namespace Calendar {

class Month {
public:
  Month(std::chrono::year_month year_month);

private:
  size_t days_in_month();
  std::list<Day> m_days;
  const std::chrono::year_month m_year_month;
};

}
