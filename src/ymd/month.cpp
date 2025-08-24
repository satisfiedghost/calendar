#include "ymd/month.h"

namespace Calendar {

using namespace std::chrono;

Month::Month(year_month year_month)
  : m_year_month(year_month)
  {
  for (size_t i = 1; i <= days_in_month(); i++) {
    year_month_day ymd{m_year_month.year(), m_year_month.month(), day(static_cast<unsigned int>(i))};
    m_days.emplace_back(Day(ymd));
  }

  if (!m_year_month.ok()) {
    throw std::domain_error("Creating day with invalid args.");
  }

}

size_t Month::days_in_month() {
  const bool leap = m_year_month.year().is_leap();

  switch(static_cast<unsigned int>(m_year_month.month())) {
    case 1:
      return 31;
    case 2:
      return leap ? 29 : 28;
    case 3:
      return 31;
    case 4:
      return 30;
    case 5:
      return 31;
    case 6:
      return 30;
    case 7:
      return 31;
    case 8:
      return 31;
    case 9:
      return 30;
    case 10:
      return 31;
    case 11:
      return 30;
    case 12:
      return 31;
  }
  return 0; //??
}

}
