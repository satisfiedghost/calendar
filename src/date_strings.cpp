#include <iomanip>
#include <sstream>

#include "util/date_strings.h"

namespace DateStrings {

using namespace std::chrono;

std::string date_to_str(year_month_day ymd) {
  return weekday_to_str(sys_days(ymd)) + " " +
         month_to_str(ymd.month()) + " " +
         day_to_str(ymd.day()) + ", " +
         year_to_str(ymd.year());
}

std::string year_to_str(year year) {
  return std::to_string(static_cast<int>(year));
}

std::string month_to_str(month month, bool shorten) {
  switch(static_cast<unsigned int>(month)) {
    case 1:
      return shorten ? "Jan" : "January";
    case 2:
      return shorten ? "Feb" : "February";
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
      return shorten ? "Aug" : "August";
    case 9:
      return shorten ? "Sept" : "September";
    case 10:
      return shorten ? "Oct" : "October";
    case 11:
      return shorten ? "Nov" : "November";
    case 12:
      return shorten ? "Dec" : "December";
    default:
      return "???";
  }
}


std::string day_to_str(day day) {
  auto day_num = static_cast<unsigned int>(day);
  std::string str = std::to_string(day_num);

  if (day_num >= 10 and day_num <= 19) {
    return str + "th";
  }

  switch(day_num % 10) {
    case 1:
      return str + "st";
    case 2:
      return str + "nd";
    case 3:
      return str + "rd";
    default:
      return str + "th";
  }
}

std::string weekday_to_str(weekday weekday, bool shorten) {
  switch(weekday.iso_encoding()) {
    case 1:
      return shorten ? "Mon" :  "Monday";
    case 2:
      return shorten ? "Tues" : "Tuesday";
    case 3:
      return shorten ? "Wed" : "Wednesday";
    case 4:
      return shorten ? "Thurs" : "Thursday";
    case 5:
      return shorten ? "Fri" : "Friday";
    case 6:
      return shorten ? "Sat" : "Saturday";
    case 7:
      return shorten ? "Sun" : "Sunday";
    default:
      return "???";
  }
}

std::string hhmmss_to_str(std::chrono::seconds seconds) {
  std::chrono::hh_mm_ss hms(seconds);
  std::ostringstream oss;
  oss << std::setfill('0') << std::setw(2) << hms.hours().count() << ":";
  oss << std::setfill('0') << std::setw(2) << hms.minutes().count() << ":" << "00";

  return oss.str();
}


}
