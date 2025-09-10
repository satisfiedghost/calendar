#pragma once

#include <unordered_map>
#include <chrono>
#include <optional>
#include <vector>
#include <deque>
#include <functional>

#include "events/tod_event.h"
#include "util/date_hash.h"

namespace Events {
  
// manages events in memory
class EventStore {
public:
  EventStore();

  using TodPtr = const TodEvent*;
  using EventView = std::vector<TodPtr>;
  using OptionalEventView = const std::optional<EventView>;

  // add an event to the store
  void add_event(TodEvent);

  // get views of events by ymd, ym, and y
  OptionalEventView get_events(const std::chrono::year_month_day) const;
  OptionalEventView get_events(const std::chrono::year_month) const;
  OptionalEventView get_events(const std::chrono::year) const;

private:

  // update internal views
  void update_views(const TodEvent&);

  // we use a dequeue because it will realloc non-contiguously and is guaranteed not to reseat pointers,
  // which would break our different time-based vector views (could be left dangling)
  std::deque<TodEvent> m_tod_events;

  // map a ymd, ym, or y to a vector containing the vector for that view
  std::unordered_map<std::chrono::year_month_day, std::vector<TodPtr>, Util::YmdHash> m_ymd_view;
  std::unordered_map<std::chrono::year_month, std::vector<TodPtr>, Util::YearMonthHash> m_year_month_view;
  std::unordered_map<std::chrono::year, std::vector<TodPtr>, Util::YearHash> m_year_view;
};

}
