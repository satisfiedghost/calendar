#include "ymd/year.h"
#include "util/date_strings.h"

namespace Calendar {

using namespace std::chrono;

Year::Year(year y)
  : m_year(y) {
  if (!m_year.ok()) {
    throw std::domain_error("Creating year with invalid arg: " + DateStrings::year_to_str(m_year));
  }
}

}
