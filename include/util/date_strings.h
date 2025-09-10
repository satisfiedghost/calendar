#include <string>
#include <chrono>

namespace DateStrings {

// pretty print any of these chrono objects
std::string date_to_str(std::chrono::year_month_day);
std::string year_to_str(std::chrono::year);
std::string month_to_str(std::chrono::month, bool shorten=false);
std::string day_to_str(std::chrono::day);
std::string weekday_to_str(std::chrono::weekday, bool shortn=false);
std::string hhmmss_to_str(std::chrono::seconds);

constexpr size_t MAX_WEEKDAY_CHAR_LEN = 8;

}
