#include <string>
#include <iostream>
#include <chrono>
#include <iomanip>
#include "event.h"
#include <vector>

namespace Calendar {


class Day {
public:
  Day(int year, unsigned int month, unsigned int day) 
    : m_year_month_day{std::chrono::year{year}, std::chrono::month{month}, std::chrono::day{day}}
    , m_sys_days(m_year_month_day)
    , m_weekday(m_sys_days)
    , m_weekday_str(weekday_to_str(m_weekday))
    , m_month_str(month_to_str(m_year_month_day.month()))
    , m_events()
  {}

  friend std::ostream& operator<<(std::ostream& os, const Day& d);

  void add_event(Events::Event& event);

private:

  std::string weekday_to_str(std::chrono::weekday weekday);
  std::string month_to_str(std::chrono::month month);

  const std::chrono::year_month_day m_year_month_day;
  const std::chrono::sys_days m_sys_days;
  const std::chrono::weekday m_weekday;
  const std::string m_weekday_str;
  const std::string m_month_str;

  std::vector<Events::Event> m_events;
};


}

