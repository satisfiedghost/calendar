#include "calendar.h"

namespace Calendar {

using namespace std::chrono;

Calendar::Calendar(std::string cal_file)
: m_disk_storage(cal_file) {
  // read all events on disk
  while (auto event = m_disk_storage.load_event()) {
    add_event(*event);
  }
}

// save all events to disk when dtor is called
// TODO - intermediate backups
Calendar::~Calendar() {
  for (const auto& e : m_event_store.get_all_events()) {
    if (!e.is_deleted()) {
      m_disk_storage.save(e);
    }
  }
}

void Calendar::add_event(const Events::TodEvent& in_event) {
  // check if this event already exists
  if (auto events = m_event_store.get_events(in_event.get_ymd())) {
    for (const auto& e : *events) {
      // don't insert this if a matching event exists on this day
      if (*e == in_event) {
        return;
      }
    }
  }

  m_event_store.add_event(in_event);
}




}

