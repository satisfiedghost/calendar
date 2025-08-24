#include <string>
#include <chrono>

namespace DateStrings {

// pretty print any of these chrono objects
std::string date_to_str(std::chrono::year_month_day);
std::string year_to_str(std::chrono::year);
std::string month_to_str(std::chrono::month);
std::string day_to_str(std::chrono::day);
std::string weekday_to_str(std::chrono::weekday);
std::string hhmmss_to_str(std::chrono::seconds);

}
