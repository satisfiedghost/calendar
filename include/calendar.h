#include <chrono>
#include <optional>
#include <functional>
#include <list>

#include "events/event_store.h"
#include "events/tod_event.h"
#include "disk/disk_storage.h"
#include "ymd/year.h"

namespace Calendar {


class Calendar {
public:
  Calendar(std::string cal_file);

  void add_event(const std::chrono::year_month_day, const Events::TodEvent&, bool writethrough=true);
  std::optional<std::reference_wrapper<const std::vector<Events::TodEvent>>> get_events(const std::chrono::year_month_day);

private:
  std::list<Year> m_years;
  Events::EventStore m_event_store;
  Storage::DiskStorage m_disk_storage;
};

}
