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

  int y;
  unsigned int m, d;

  parser.print_commands();

  auto cont = true;
  while (cont) {
    auto cmd = parser.get_user_cmd();

    if (!cmd) {
      std::cout << "Command not recognized!" << std::endl;
      continue;
    }

    switch (*cmd) {
      case CLI::Commands::QUIT:
        std::cout << "Quitting." << std::endl;
        cont = false;
      break;

      case CLI::Commands::CREATE_EVENT: {
        auto e = parser.create_event();
        c.add_event(e);
      }
      break;
      
      case CLI::Commands::YEAR_SEARCH:
        CLI::CLIParser::get_user_ints({"Year (YYYY): "}, y);
        print_events(year{y});
      break;
      
      case CLI::Commands::YEAR_MONTH_SEARCH:
        CLI::CLIParser::get_user_ints({"Year (YYYY): ", "Month (MM): "}, y, m);
        print_events(year_month{year{y}, month{m}});
      break;

      case CLI::Commands::YEAR_MONTH_DAY_SEARCH:
        CLI::CLIParser::get_user_ints({"Year (YYYY): ", "Month (MM): ", "Day (DD): "}, y, m, d);
        print_events(year_month_day{year{y}, month{m}, day{d}});
      break;

      case CLI::Commands::PRINT_COMMANDS:
        parser.print_commands();
      break;

      default:
        std::cout << "Command handler not implemented!" << std::endl;
      break;
    }
  }

  return 0;
}
