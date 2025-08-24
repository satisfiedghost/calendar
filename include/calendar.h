#include <chrono>
#include <optional>
#include <functional>
#include <span>

#include "events/event_store.h"
#include "events/tod_event.h"
#include "disk/disk_storage.h"
#include "ymd/year.h"

namespace Calendar {


class Calendar {
public:
  Calendar(std::string cal_file);

  void add_event(const std::chrono::year_month_day, const Events::TodEvent&, bool writethrough=true);

  template<typename T>
  std::optional<std::span<Events::EventStore::TodPtr>> get_events(const T t) { return m_event_store.get_events(t); }

private:
  Events::EventStore m_event_store;
  Storage::DiskStorage m_disk_storage;
};

}
