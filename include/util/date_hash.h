#include <chrono>
#include <unordered_map>

namespace Util {

struct YmdHash {
  size_t operator()(const std::chrono::year_month_day& ymd) const {

    const auto days = std::chrono::sys_days{ymd}.time_since_epoch().count();
    return std::hash<long long>{}(days);
  }
};

struct YmdEq {
  bool operator()(const std::chrono::year_month_day& a,
                  const std::chrono::year_month_day& b) const noexcept {
    return a == b;
  }
};
}
