#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>

#include "cli/cli.h"
#include "events/tod_event.h"

namespace CLI {

using namespace std::chrono;


std::tuple<std::chrono::year_month_day, Events::TodEvent> CLIParser::create_event() {
  std::string description;
  std::cout << "Enter a description: ";
  std::getline(std::cin >> std::ws, description);

  int y;
  unsigned  mo, d;
  std::cout << "Enter a date (YYYY MM DD): ";
  std::cin >> y >> mo >> d;

  int h, m;
  std::cout << "Enter a time (HH MM): ";
  std::cin >> h >> m;

  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  year_month_day ymd{year{y}, month{mo}, day{d}};
  hh_mm_ss<seconds> mmhh{hours{h} + minutes{m}};

  auto tup = std::make_tuple(ymd, Events::TodEvent(description, (unsigned)mmhh.hours().count(), (unsigned)mmhh.minutes().count()));

  return tup;
}

}

