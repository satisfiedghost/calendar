#include "day.h"
#include <vector>

int main() {
  std::vector<Calendar::Day> days;

  for (size_t i = 1; i <= 31; i++) {
    days.emplace_back(2025, 1, i);
  }

  for (const auto& d : days) {
    std::cout << d << std::endl;
  }

}
