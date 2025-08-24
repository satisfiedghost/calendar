#include "ymd/day.h"
#include <stdexcept>

namespace Calendar {

using namespace std::chrono;

Day::Day(year_month_day year_month_day)
  : m_year_month_day(year_month_day)
  , m_sys_days(year_month_day)
  , m_weekday(m_sys_days) {
  if (!m_year_month_day.ok()) {
    throw std::domain_error("Creating day with invalid args.");
  }

}


}

