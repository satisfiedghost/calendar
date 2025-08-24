#include <string>
#include <iostream>
#include <chrono>
#include <iomanip>

namespace Calendar {


class Day {
public:
  Day(std::chrono::year_month_day year_month_day);

  std::string to_string();

private:
  const std::chrono::year_month_day m_year_month_day;
  const std::chrono::sys_days m_sys_days;
  const std::chrono::weekday m_weekday;
};


}

