#include <iostream>

#include "events/tod_event.h"
#include "util/date_strings.h"

namespace Events {

std::ostream& operator<<(std::ostream& os, const TodEvent& e) {
  os << e.m_context << std::endl << "\t@" << DateStrings::hhmmss_to_str(e.m_tod);
  return os;
}

}
