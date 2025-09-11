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
  virtual ~TodEvent() = default;

  TodEvent(std::string context, std::chrono::year_month_day ymd, std::chrono::hh_mm_ss<std::chrono::seconds> hms)
    : Event(context) 
    , m_tod{std::chrono::seconds{hms.hours().count() * 3600 + hms.minutes().count() * 60}}
    , m_sys_days(ymd)
    , m_deleted(false)
  {}

  // convenience delegating ctors
  TodEvent(std::string context, std::chrono::year_month_day ymd, std::chrono::seconds tod)
    : TodEvent(context, ymd, std::chrono::hh_mm_ss<std::chrono::seconds>(tod))
    {}

  TodEvent(std::string context, std::chrono::year_month_day ymd, std::chrono::hours h, std::chrono::minutes m)
    : TodEvent(context, ymd, std::chrono::seconds(h.count() * 3600 + m.count() * 60))
  {}

  // get the ymd for this event
  const std::chrono::year_month_day get_ymd() const { return std::chrono::year_month_day(m_sys_days); }

  // teach cout how to pretty print an event
  friend std::ostream& operator<<(std::ostream&, const TodEvent&);

  // make these 3 way comparable (helps to check for duplicates)
  auto operator<=>(const TodEvent&) const = default;

  const std::chrono::seconds get_tod() const { return m_tod; }

  bool is_deleted() const { return m_deleted; }

  void erase() { m_deleted = true; }
private:
  std::chrono::seconds m_tod; // time since midnight
  std::chrono::sys_days m_sys_days;
  bool m_deleted;
};

}
