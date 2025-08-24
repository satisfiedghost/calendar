#include <chrono>
#include <optional>
#include <functional>
#include <list>

#include "events/event_store.h"
#include "events/tod_event.h"
#include "ymd/year.h"

namespace Calendar {


class Calendar {
public:
  Calendar();

  void add_event(const std::chrono::year_month_day, const Events::TodEvent&);
  std::optional<std::reference_wrapper<const std::vector<Events::TodEvent>>> get_events(const std::chrono::year_month_day);

private:
  std::list<Year> m_years;
  Events::EventStore m_event_store;
};

}
