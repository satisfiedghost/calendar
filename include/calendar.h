#pragma once

#include <chrono>
#include <optional>
#include <functional>
#include <span>

#include "events/event_store.h"
#include "events/tod_event.h"
#include "disk/disk_storage.h"
#include "ymd/year.h"

namespace Calendar {

// primary interaction with the event store and interfacing with the user
class Calendar {
public:
  Calendar(std::string cal_file);

  // add an event to the calendar
  // set writethrough=false to avoid writing to disk
  void add_event(const Events::TodEvent&, bool writethrough=true);

  // get an event span for a given ymd, ym, or y
  // or nullopt if none are present for the requested date
  template<typename T>
  const std::optional<std::span<const Events::EventStore::TodPtr>> get_events(const T t) const { return m_event_store.get_events(t); }

private:
  Events::EventStore m_event_store;
  Storage::DiskStorage m_disk_storage;
};

}
