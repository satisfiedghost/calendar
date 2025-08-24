#include <unordered_map>
#include <chrono>
#include <optional>
#include <vector>
#include <deque>
#include <functional>
#include <span>

#include "events/tod_event.h"
#include "util/date_hash.h"

namespace Events {
  
// manages events in memory
class EventStore {
public:
  EventStore();

  using TodPtr = const TodEvent*;

  // add an event to the store
  void add_event(TodEvent);

  // get views of events by ymd, ym, and y
  std::optional<std::span<TodPtr>> get_events(const std::chrono::year_month_day);
  std::optional<std::span<TodPtr>> get_events(const std::chrono::year_month);
  std::optional<std::span<TodPtr>> get_events(const std::chrono::year);

private:

  // update internal views
  void update_views(const TodEvent&);

  // we use a dequeue because it will realloc non-contiguously and is guaranteed not to reseat pointers,
  // which would break our span views
  std::deque<TodEvent> m_tod_events;

  // map a ymd, ym, or y to a vector containing the span for that view
  std::unordered_map<std::chrono::year_month_day, std::vector<TodPtr>, Util::YmdHash> m_ymd_view;
  std::unordered_map<std::chrono::year_month, std::vector<TodPtr>, Util::YearMonthHash> m_year_month_view;
  std::unordered_map<std::chrono::year, std::vector<TodPtr>, Util::YearHash> m_year_view;
};

}
