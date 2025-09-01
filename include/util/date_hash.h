#pragma once

#include <chrono>
#include <unordered_map>

namespace Util {

// the STL doesn't provide hash functionality for these by default
struct YearHash {
  size_t operator()(const std::chrono::year& y) const noexcept {
    return std::hash<int>{}(static_cast<int>(y));
  }
};

struct YearMonthHash {
  size_t operator()(const std::chrono::year_month& ym) const noexcept {
    const int y = static_cast<int>(ym.year());
    const unsigned int m = static_cast<unsigned int>(ym.month());
    return std::hash<unsigned int>{}((static_cast<unsigned int>(y) << 8) | m);
  }
};

struct YmdHash {
  size_t operator()(const std::chrono::year_month_day& ymd) const noexcept {

    const auto days = std::chrono::sys_days{ymd}.time_since_epoch().count();
    return std::hash<long long>{}(days);
  }
};

}
