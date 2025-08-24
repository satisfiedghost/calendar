#include "calendar.h"

namespace Calendar {

using namespace std::chrono;

Calendar::Calendar(std::string cal_file)
: m_disk_storage(cal_file) {
  // read all events on disk
  while (auto event = m_disk_storage.load_event()) {
    add_event(*event, false);
  }
}

void Calendar::add_event(const Events::TodEvent& in_event, bool writethrough) {
  // check if this event already exists
  if (auto events = m_event_store.get_events(in_event.get_ymd())) {
    for (const auto& e : *events) {
    // don't insert this if a matching event exists on this day
      if (*e == in_event) {
        return;
      }
    }
  }

  if (writethrough) {
    m_disk_storage.save(in_event);
  }
  m_event_store.add_event(in_event);
}

}
