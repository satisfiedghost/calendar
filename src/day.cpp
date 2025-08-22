#include "day.h"

namespace Calendar {

std::ostream& operator<<(std::ostream& os, const Day& day) {
  os << day.m_weekday_str << ", " <<
    day.m_month_str << " " <<
    std::setw(2) << std::setfill('0') << static_cast<unsigned>(day.m_year_month_day.day()) << " " <<
    static_cast<int>(day.m_year_month_day.year());

  if (day.m_events.size()) {
    os << std::endl << "Events:" << std::endl;
    for (auto it = day.m_events.begin(); it != day.m_events.end(); it++) {
      os << "\t" << (*it).get_context();
      if (std::next(it) != day.m_events.end()) {
        os << std::endl;
      }
    }
  }

  return os;
}

std::string Day::weekday_to_str(std::chrono::weekday weekday) {
  switch(weekday.iso_encoding()) {
    case 1:
      return "Monday";
    case 2:
      return "Tuesday";
    case 3:
      return "Wednesday";
    case 4:
      return "Thursday";
    case 5:
      return "Friday";
    case 6:
      return "Saturday";
    case 7:
      return "Sunday";
    default:
      return "???";
  }
}


std::string Day::month_to_str(std::chrono::month month) {
  switch(static_cast<unsigned>(month)) {
    case 1:
      return "January";
    case 2:
      return "February";
    case 3:
      return "March";
    case 4:
      return "April";
    case 5:
      return "May";
    case 6:
      return "June";
    case 7:
      return "July";
    case 8:
      return "August";
    case 9:
      return "September";
    case 10:
      return "October";
    case 11:
      return "November";
    case 12:
      return "December";
    default:
      return "???";
  }
}

void Day::add_event(Events::Event &event) {
  m_events.push_back(event);
}

}

