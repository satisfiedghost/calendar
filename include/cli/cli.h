#pragma once

#include <array>
#include <chrono>
#include <optional>
#include <iostream>
#include <ncursesw/ncurses.h>

#include "calendar.h"
#include "events/tod_event.h"
#include "display/display.h"

namespace CLI {

enum class Commands {
  QUIT = 0,
  CREATE_EVENT = 1,
  YEAR_SEARCH = 2,
  YEAR_MONTH_SEARCH = 3,
  YEAR_MONTH_DAY_SEARCH = 4,
  PRINT_COMMANDS = 5,
  COMMAND_CNT = 6,
};



class CLIParser {
public:
  CLIParser(Calendar::Calendar& calendar, Display& display);
  // parse a command from user input
  // returns nullopt if command wasn't understood
  std::optional<Commands> get_user_cmd(std::string);
  // prompt the user to create an event
  std::optional<Events::TodEvent> create_event();
  // infinite io loop
  void do_io();
private:
  void create_io_window();
  Display& m_display;
  Calendar::Calendar& m_calendar;
  WINDOW* m_io_window_frame;
  WINDOW* m_io_window;
};




}

