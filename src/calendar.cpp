#include "calendar.h"

namespace Calendar {

using namespace std::chrono;

constexpr year MIN_YEAR{1970};
constexpr year MAX_YEAR{2100};

Calendar::Calendar(std::string cal_file)
: m_disk_storage(cal_file) {
  // read all events on disk
  while (auto time_and_event = m_disk_storage.load_event()) {
    auto& [time, event] = *time_and_event;
    add_event(time, event, false);
  }
}

void Calendar::add_event(const std::chrono::year_month_day ymd, const Events::TodEvent& in_event, bool writethrough) {
  // check if this event already exists
  if (auto events = m_event_store.get_events(ymd)) {
    for (const auto& e : *events) {
    // don't insert this if a matching event exists on this day
      if (*e == in_event) {
        return;
      }
    }
  }

  if (writethrough) {
    m_disk_storage.save(ymd, in_event);
  }
  m_event_store.add_event(ymd, in_event);
}


//std::optional<std::span<Events::EventStore::TodPtr>> Calendar::get_events(const year_month_day ymd) {
//  return m_event_store.get_events(ymd);
//}

}
