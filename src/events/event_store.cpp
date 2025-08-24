#include <stdexcept>

#include "events/event_store.h"
#include "events/tod_event.h"

namespace Events {

using namespace std::chrono;

EventStore::EventStore() 
  : m_tod_events()
  {}

void EventStore::add_event(TodEvent event) {
  auto ymd = event.get_ymd();
  if (!ymd.ok()) {
    throw std::domain_error("ymd associated with event is not valid!");
  }

  // move into backing storage and then use a ref to that to index by y/m/d
  m_tod_events.push_back(std::move(event));
  const auto& event_ref = m_tod_events.back();
  update_views(event_ref);
}

void EventStore::update_views(const TodEvent& e_ref) {
  TodPtr e_ptr = &e_ref;
  auto ymd = e_ref.get_ymd();

  // append to the correct span,
  // unordered_map's [] operator calls the default ctor for the value if not found
  m_year_view[ymd.year()].push_back(e_ptr);
  m_year_month_view[year_month{ymd.year(), ymd.month()}].push_back(e_ptr);
  m_ymd_view[ymd].push_back(e_ptr);
}

std::optional<std::span<EventStore::TodPtr>> EventStore::get_events(const year_month_day ymd) {
  if (auto it = m_ymd_view.find(ymd); it != m_ymd_view.end()) {
    return it->second;
  }
  return std::nullopt;
}

std::optional<std::span<EventStore::TodPtr>> EventStore::get_events(const year_month ym) {
  if (auto it = m_year_month_view.find(ym); it != m_year_month_view.end()) {
    return it->second;
  }
  return std::nullopt;
}

std::optional<std::span<EventStore::TodPtr>> EventStore::get_events(const year y) {
  if (auto it = m_year_view.find(y); it != m_year_view.end()) {
    return it->second;
  }
  return std::nullopt;
}

}
