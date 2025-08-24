#include <unordered_map>
#include <chrono>
#include <optional>
#include <vector>
#include <functional>

#include "events/tod_event.h"
#include "util/date_hash.h"

namespace Events {
  

class EventStore {
public:
  EventStore();

  void add_event(const std::chrono::year_month_day, const TodEvent&);
  std::optional<std::reference_wrapper<const std::vector<TodEvent>>> get_events(const std::chrono::year_month_day);

private:
  std::unordered_map<std::chrono::year_month_day, std::vector<TodEvent>, Util::YmdHash, Util::YmdEq> m_tod_events;
};

}
