#include <array>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>

#include "cli/cli.h"
#include "events/tod_event.h"

namespace CLI {

using namespace std::chrono;

static std::string prompt = "Command: ";

static std::string commands = 
  "Available commands:\n"
  "\tq[uit]    - Quit\n"
  "\te[vent]   - Create an event\n"
  "\ty[ear]    - Search for all events in a year.\n"
  "\tm[onth]   - Search for all events in a month.\n"
  "\td[ay]     - Search for all events in a given day.\n"
  "\tc[ommand] - See this list of commands.\n";

static const std::array<std::string, static_cast<size_t>(Commands::COMMAND_CNT)> COMMAND_STR = {
  "quit",
  "event",
  "year",
  "month",
  "day",
  "command",
};

static bool matches_prefix(const std::string_view query, const std::string_view target) {
  return target.starts_with(query);
}

void CLIParser::print_commands() {
  std::cout << commands << std::endl;
}

void CLIParser::discard_remaining_input() {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::optional<Commands> CLIParser::get_user_cmd() {
  std::cout << prompt;
  std::string user_input;
  std::cin >> user_input;
  discard_remaining_input();

  for (size_t i = 0; i < COMMAND_STR.size(); i++) {
    if (matches_prefix(user_input, COMMAND_STR[i])) {
      return static_cast<Commands>(i);
    }
  }

  return std::nullopt;
}

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

  discard_remaining_input();

  year_month_day ymd{year{y}, month{mo}, day{d}};
  hh_mm_ss<seconds> mmhh{hours{h} + minutes{m}};

  return Events::TodEvent(description, ymd, mmhh);
}

}

