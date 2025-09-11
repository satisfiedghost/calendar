#include <chrono>
#include <iostream>

#include "events/tod_event.h"
#include "util/date_strings.h"

namespace Events {

using namespace std::chrono;

std::ostream& operator<<(std::ostream& os, const TodEvent& e) {
  year_month_day ymd(e.m_sys_days);
  os << DateStrings::date_to_str(ymd);
  os << " @ " << DateStrings::hhmmss_to_str(e.m_tod);
  if (e.m_duration.count() > 0) {
    os << " - " << DateStrings::hhmmss_to_str(e.m_tod + e.m_duration);
  }
  os << std::endl;
  os << "\t" << e.m_context << std::endl;
  return os;
}

}
