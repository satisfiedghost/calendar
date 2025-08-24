#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>

#include "cli/cli.h"
#include "events/tod_event.h"

namespace CLI {

using namespace std::chrono;


Events::TodEvent CLIParser::create_event() {
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

  // trim whitespace and trailing \n
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  year_month_day ymd{year{y}, month{mo}, day{d}};
  hh_mm_ss<seconds> mmhh{hours{h} + minutes{m}};

  return Events::TodEvent(description, ymd, mmhh);
}

}

