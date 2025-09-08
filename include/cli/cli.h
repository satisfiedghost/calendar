#pragma once

#include "events/tod_event.h"
#include <array>
#include <chrono>
#include <optional>
#include <iostream>
#include <ncursesw/ncurses.h>

namespace CLI {

enum class Commands {
  NONE = -1,
  QUIT = 0,
  CREATE_EVENT = 1,
  YEAR_SEARCH = 2,
  YEAR_MONTH_SEARCH = 3,
  YEAR_MONTH_DAY_SEARCH = 4,
  PRINT_COMMANDS = 5,
  MOVE_UP = 6,
  MOVE_DOWN = 7,
  MOVE_LEFT = 8,
  MOVE_RIGHT = 9,
  CHANGE_MONTH = 10,
  USER_COMMAND_CNT = 11,
};



class CLIParser {
public:
  CLIParser() {}
  // parse a command from user input
  // returns nullopt if command wasn't understood
  std::optional<Commands> get_user_cmd(bool display_prompt);
  // set the window pointers
  void set_windows(WINDOW* io_window_frame, WINDOW* io_window);
  // print a string, verbatim
  void print_str(const std::string&);
  // print a string, with a newline
  void print_strln(const std::string&);
  // print commands for user
  void print_cmds() const;
  // get a year from the user
  std::optional<std::chrono::year> get_user_year();
  // get a year_month from the user
  std::optional<std::chrono::year_month> get_user_year_month();
  // get a year_month_day from the user
  std::optional<std::chrono::year_month_day> get_user_ymd();
  // create an event from user input
  std::optional<Events::TodEvent> create_event();
private:
  WINDOW* m_io_window_frame;
  WINDOW* m_io_window;
};




}

