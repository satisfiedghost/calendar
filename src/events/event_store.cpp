#include <stdexcept>

#include "events/event_store.h"

namespace Events {

using namespace std::chrono;

EventStore::EventStore() 
  : m_tod_events()
  {}

void EventStore::add_event(const year_month_day ymd, const TodEvent& event) {
  if (!ymd.ok()) {
    throw std::domain_error("ymd associated with event is not valid!");
  }

  m_tod_events[ymd].push_back(event);
}

std::optional<std::reference_wrapper<const std::vector<TodEvent>>> EventStore::get_events(const year_month_day ymd) {
  if (m_tod_events.contains(ymd)) {
    return std::cref(m_tod_events[ymd]);
  } else {
    return std::nullopt;
  }
}

}
