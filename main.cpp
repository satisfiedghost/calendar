#include "day.h"
#include "event.h"
#include <vector>

int main() {
  std::vector<Calendar::Day> days;

  for (size_t i = 1; i <= 31; i++) {
    days.emplace_back(2025, 1, i);
  }

  Events::Event e1("National Bug Day");
  Events::Event e2("Bumblebee's Birthday");
  Events::Event e3("Untold Horrors");
  Events::Event e4("National Bug Day II (Electric Boogaloo)");

  days[3].add_event(e1);
  days[15].add_event(e2);
  days[15].add_event(e3);
  days[21].add_event(e4);

  for (const auto& d : days) {
    std::cout << d << std::endl << std::endl;
  }
}
