#include <array>
#include <chrono>
#include <curses.h>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <vector>
#include "calendar.h"

#include "cli/cli.h"
#include "events/tod_event.h"

namespace CLI {

using namespace std::chrono;

constexpr int CTRL_U = 21;

static std::string description_prompt = "Description: ";
static std::string command_prompt = "$> ";
static std::string year_prompt = "Year (YYYY): ";
static std::string month_prompt = "Month (MM): ";
static std::string day_prompt = "Day (DD): ";
static std::string hour_prompt = "Hour (HH): ";
static std::string minute_prompt = "Minute (MM): ";

static std::string commands = 
  "Available commands:\n"
  "\tq[uit]     - Quit\n"
  "\te[vent]    - Create an event\n"
  "\t[↑↓←→]     - Change day selection.\n"
  "\tShift+[←→] - Change month selection.\n"
  "\td[elete]   - Delete an event.\n"
  "\tsy[ear]    - Search for all events in a year.\n"
  "\tsm[onth]   - Search for all events in a month.\n"
  "\tsd[ay]     - Search for all events in a given day.\n"
  "\tc[hange]   - Change the displayed month.\n"
  "\th[elp]     - See this list of commands.\n";

static const std::array<std::string, static_cast<size_t>(Commands::USER_COMMAND_CNT)> COMMAND_STR = {
  "quit",
  "event",
  "syear",
  "smonth",
  "sday",
  "help",
  "key_up",
  "key_down",
  "key_left",
  "key_right",
  "change",
  "delete",
  "shift_left",
  "shift_right",
};

static bool matches_prefix(const std::string_view query, const std::string_view target) {
  return target.starts_with(query);
}

static std::string get_input_line(const std::string& input_prompt, WINDOW* io_window) {
  static std::string input_buffer;
  std::string user_input;

  auto echo = [&]() {
    wmove(io_window, getcury(io_window), 0);
    wprintw(io_window, "%s", input_prompt.c_str());
    wprintw(io_window, "%s", input_buffer.c_str());
    curs_set(1);
    wrefresh(io_window);
  };

  // print input buffer if returning from e.g. keypress
  echo();

  int ch = 0;
  while (ch != '\n') {
    ch = wgetch(io_window);

    // TODO - don't hardcode strings here
    if (ch == KEY_UP) {
      return "key_up";
    } else if (ch == KEY_DOWN) {
      return "key_down";
    } else if (ch == KEY_LEFT) {
      return "key_left";
    } else if (ch == KEY_RIGHT) {
      return "key_right";
    } else if (ch == KEY_SLEFT) {
      return "shift_left";
    } else if (ch == KEY_SRIGHT) {
      return "shift_right";
    }

    if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
      if (!input_buffer.empty()) {
        wmove(io_window, getcury(io_window), getcurx(io_window) - 1);
        wclrtoeol(io_window);
        input_buffer.pop_back();
      }
    } else if (ch == CTRL_U) { // clear whole line
      wmove(io_window, getcury(io_window), 0);
      input_buffer.clear();
      wclrtoeol(io_window);
    } else if (ch >= 32 && ch <= 127) { // printable ascii
      input_buffer.push_back(static_cast<char>(ch));
    }
    echo();
  }

  wprintw(io_window, "\n");
  wrefresh(io_window);

  user_input = input_buffer;
  input_buffer.clear();
  return user_input;
}

template<typename T>
static bool get_int(T& value, const std::string& prompt, WINDOW* io_window) {
  char* strtol_end;
  auto input_buffer = get_input_line(prompt, io_window);

  value = static_cast<T>(strtol(input_buffer.c_str(), &strtol_end, 10));

  if (strtol_end == input_buffer.c_str()) {
    wprintw(io_window, "Invalid year\n");
    wrefresh(io_window);
    return false;
  }

  input_buffer.clear();
  return true;
}

void CLIParser::print_str(const std::string& str) {
  wprintw(m_io_window, "%s", str.c_str());
  wrefresh(m_io_window);
}

void CLIParser::print_strln(const std::string& str) {
  wprintw(m_io_window, "%s\n", str.c_str());
  wrefresh(m_io_window);
}

std::optional<int> CLIParser::get_user_int_prompted(const std::string& prompt) {
  int val;

  if (!get_int(val, prompt, m_io_window)) {
    return std::nullopt;
  }

  return val;
}

std::optional<std::chrono::year> CLIParser::get_user_year() {
  int y;

  if (!get_int(y, year_prompt, m_io_window)) {
    return std::nullopt;
  }

  year user_year{y};

  if (!user_year.ok()) {
    return std::nullopt;
  }

  return user_year;
}

std::optional<std::chrono::year_month> CLIParser::get_user_year_month() {
  int y;
  unsigned int m;

  if (!get_int(y, year_prompt, m_io_window)) {
    return std::nullopt;
  }
  if (!get_int(m, month_prompt, m_io_window)) {
    return std::nullopt;
  }

  year_month user_year_month{year{y}, month{m}};

  if (!user_year_month.ok()) {
    return std::nullopt;
  }
  
  return user_year_month;
}

std::optional<std::chrono::year_month_day> CLIParser::get_user_ymd() {
  int y;
  unsigned int m, d;

  if (!get_int(y, year_prompt, m_io_window)) {
    return std::nullopt;
  }
  if (!get_int(m, month_prompt, m_io_window)) {
    return std::nullopt;
  }
  if (!get_int(d, day_prompt, m_io_window)) {
    return std::nullopt;
  }

  year_month_day user_ymd{year{y}, month{m}, day{d}};

  if (!user_ymd.ok()) {
    return std::nullopt;
  }

  return user_ymd;
}


void CLIParser::print_cmds() const {
  werase(m_io_window);
  mvwprintw(m_io_window, getmaxy(m_io_window) - 1, 0, "%s", commands.c_str());
  wrefresh(m_io_window);
}



std::optional<Commands> CLIParser::get_user_cmd() {
  auto user_input = get_input_line(command_prompt, m_io_window);

  for (size_t i = 0; i < COMMAND_STR.size(); i++) {
    if (matches_prefix(user_input, COMMAND_STR[i])) {
      return static_cast<Commands>(i);
    }
  }

  return std::nullopt;
}

void CLIParser::set_windows(WINDOW *io_window_frame, WINDOW *io_window) {
  m_io_window_frame = io_window_frame;
  m_io_window = io_window;
  werase(m_io_window);
  mvwprintw(m_io_window, getmaxy(m_io_window) - 1, 0, "%s", commands.c_str());
}

std::optional<Events::TodEvent> CLIParser::create_event(std::chrono::year_month_day ymd) {
  std::string description;
  int m, h;

  description = get_input_line(description_prompt, m_io_window);
  if (!get_int(h, hour_prompt, m_io_window)) {
    return std::nullopt;
  }
  if (!get_int(m, minute_prompt, m_io_window)) {
    return std::nullopt;
  }

  hh_mm_ss<seconds> mmhh{hours{h} + minutes{m}};

  return Events::TodEvent(description, ymd, mmhh);
}

}

