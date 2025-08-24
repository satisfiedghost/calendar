#include <chrono>
#include <string>
#include <iostream>
#include <vector>

#include "calendar.h"
#include "events/tod_event.h"
#include "util/date_strings.h"
#include "util/chrono_literal_extensions.h"

using namespace std::chrono;
using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "supply calendar file?" << std::endl;
    return -1;
  } else if (argc > 2) {
    std::cout << "too many arguments" << std::endl;
    return -1;
  }

  Calendar::Calendar c{std::string(argv[1])};

  Events::TodEvent e1("a cool party", 20, 00);
  year_month_day ymd1{2025y, 8_mo, 22d};
  Events::TodEvent e2("go to the woods and find a bug", 5, 13);
  year_month_day ymd2{2025y, 8_mo, 25d};
  Events::TodEvent e3("stare into the void", 01, 00);
  year_month_day ymd3{1993y, 7_mo, 9d};

  c.add_event(ymd1, e1);
  c.add_event(ymd2, e2);
  c.add_event(ymd3, e3);

  auto events = c.get_events(ymd3);

  if (events) {
    for (const auto& e : events->get()) {
      std::cout << e << std::endl;
    }
  }

  return 0;
}
