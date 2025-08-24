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
  

class EventStore {
public:
  EventStore();
  using TodPtr = const TodEvent*;

  void add_event(const std::chrono::year_month_day, TodEvent);
  std::optional<std::span<TodPtr>> get_events(const std::chrono::year_month_day);
  std::optional<std::span<TodPtr>> get_events(const std::chrono::year_month);
  std::optional<std::span<TodPtr>> get_events(const std::chrono::year);

private:


  void update_views(const std::chrono::year_month_day ymd, const TodEvent&);

  std::deque<TodEvent> m_tod_events;
  std::unordered_map<std::chrono::year, std::vector<TodPtr>, Util::YearHash> m_year_view;
  std::unordered_map<std::chrono::year_month, std::vector<TodPtr>, Util::YearMonthHash> m_year_month_view;
  std::unordered_map<std::chrono::year_month_day, std::vector<TodPtr>, Util::YmdHash> m_ymd_view;
};

}
