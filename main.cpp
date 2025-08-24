#include <chrono>
#include <string>
#include <iostream>
#include <vector>

#include "calendar.h"
#include "events/tod_event.h"
#include "util/date_strings.h"
#include "util/chrono_literal_extensions.h"
#include "cli/cli.h"

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
  CLI::CLIParser parser;

  auto print_events = [&](auto s) {
    auto events = c.get_events(s);

    if (events) {
      for (const auto& e : *events) {
        std::cout << *e << std::endl;
      }
    } else {
      std::cout << "no events found!" << std::endl;
    }
  };

  char cmd;
  int y;
  unsigned int m, d;
  
  std::cout << "Enter a command (q for quit, e for create event)" << std::endl;
  std::cout << "(y for year-search, m for month-search, d for day-search): ";
  while (std::cin >> cmd && cmd != 'q') {
    switch(cmd) {
      case 'e': {
        auto e = parser.create_event();
        c.add_event(e);
      }
      break;
      case 'y':
        std::cout << "enter a year (YYYY): ";
        std::cin >> y;
        print_events(year{y});
      break;
      case 'm':
        std::cout << "enter a year month (YYYY MM): ";
        std::cin >> y >> m;
        print_events(year_month{year{y}, month{m}});
      break;
      case'd':
        std::cout << "enter a year month day (YYYY MM DD): ";
        std::cin >> y >> m >> d;
        print_events(year_month_day{year{y}, month{m}, day{d}});
      break;
      default:
        std::cout << "unknown command: " << cmd << std::endl;
      break;

    }
    std::cout << "Enter a command (q for quit, e for create event)" << std::endl;
    std::cout << "(y for year-search, m for month-search, d for day-search): ";
  }

  return 0;
}
