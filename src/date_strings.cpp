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

std::string month_to_str(month month) {
  switch(static_cast<unsigned int>(month)) {
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

std::string weekday_to_str(weekday weekday) {
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

std::string hhmmss_to_str(std::chrono::hh_mm_ss<std::chrono::seconds> hhmmss) {
  return std::to_string(hhmmss.hours().count()) + ":" +
         std::to_string(hhmmss.minutes().count()) + ":" + "00";
}


}
