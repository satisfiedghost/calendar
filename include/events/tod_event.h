#pragma once
#include <chrono>
#include <iosfwd>

#include "event.h"

namespace Storage {
  class DiskStorage; // forward decl so this can save an event to disk
}


namespace Events {

// Time of Day event, the most common
class TodEvent : protected Event {
  friend class Storage::DiskStorage;

public:
  TodEvent(std::string context, unsigned hour, unsigned minute) 
    : Event(context) 
    , m_tod{std::chrono::seconds{hour * 3600 + minute * 60}} {
  }

  friend std::ostream& operator<<(std::ostream&, const TodEvent&);
private:
  std::chrono::hh_mm_ss<std::chrono::seconds> m_tod;
};

}
