#include "calendar.h"

namespace Calendar {

using namespace std::chrono;

constexpr year MIN_YEAR{1970};
constexpr year MAX_YEAR{2100};

Calendar::Calendar() {
  for (auto y = MIN_YEAR; y <= MAX_YEAR; y++) {
    m_years.emplace_back(Year(y));
  }
}

void Calendar::add_event(const std::chrono::year_month_day ymd, const Events::TodEvent& e) {
  m_event_store.add_event(ymd, e);
}


std::optional<std::reference_wrapper<const std::vector<Events::TodEvent>>> Calendar::get_events(const year_month_day ymd) {
  return m_event_store.get_events(ymd);
}

}
