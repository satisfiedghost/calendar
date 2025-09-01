#include <array>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <vector>
#include <ncursesw/ncurses.h>
#include "calendar.h"

#include "cli/cli.h"
#include "events/tod_event.h"

namespace CLI {

using namespace std::chrono;

static WINDOW* io_window;
constexpr int CTRL_U = 21;

static std::string description_prompt = "Description: ";
static std::string command_prompt = "Command: ";
static std::string year_prompt = "Year (YYYY): ";
static std::string month_prompt = "Month (MM): ";
static std::string day_prompt = "Day (DD): ";
static std::string hour_prompt = "Hour (HH): ";
static std::string minute_prompt = "Minute (MM): ";

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

static int term_h, term_w;

static void get_input_line(std::string& input_buffer, const std::string& input_prompt) {
  wprintw(io_window, "%s", input_prompt.c_str());

  int ch = 0;
  while (ch != '\n') {
    ch = wgetch(io_window);
    if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
      if (!input_buffer.empty()) {
        input_buffer.pop_back();
      }
    } else if (ch == CTRL_U) { // clear whole line
      input_buffer.clear();
    } else if (ch >= 32 && ch <= 127) { // printable ascii
      input_buffer.push_back(static_cast<char>(ch));
    }
    wmove(io_window, getcury(io_window), static_cast<int>(input_prompt.size()));
    wclrtoeol(io_window);
    wprintw(io_window, "%s", input_buffer.c_str());
    wrefresh(io_window);
  }
  wprintw(io_window, "\n");
  wrefresh(io_window);
}

template<typename T>
static bool get_int(T& value, std::string& input_buffer, const std::string& prompt) {
  char* strtol_end;
  get_input_line(input_buffer, prompt);
  value = static_cast<T>(strtol(input_buffer.c_str(), &strtol_end, 10));
  if (strtol_end == input_buffer.c_str()) {
    wprintw(io_window, "Invalid year\n");
    wrefresh(io_window);
    return false;
  }
  input_buffer.clear();
  return true;
}

static void io_thread(CLIParser& parser, Calendar::Calendar& c) {
  std::istringstream input_stream;

  werase(io_window);
  mvwprintw(io_window, getmaxy(io_window) - 1, 0, "%s", commands.c_str());
  wrefresh(io_window);

  auto print_events = [&](auto s) {
    wclrtobot(io_window);
    std::ostringstream event_stream;
    auto events = c.get_events(s);

    if (events) {
      for (const auto& e : *events) {
        event_stream << *e << "\n";
      }
      wprintw(io_window, "%s\n", event_stream.str().c_str());
      wrefresh(io_window);
      event_stream.clear();
    } else {
      wprintw(io_window, "%s\n", "No events found!");
    }
  };

  bool cont = true;
  int y;
  unsigned int m, d;
  while (cont) {
    std::string input_buffer;
    input_buffer.clear();
    get_input_line(input_buffer, command_prompt);

    auto cmd = parser.get_user_cmd(input_buffer);
    input_buffer.clear();

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
        if (!e) {
          wprintw(io_window, "%s\n", "Unable to create event!");
          continue;
        }
        c.add_event(*e);
      }
      break;
      
      case CLI::Commands::YEAR_SEARCH:
        if (!get_int(y, input_buffer, year_prompt)) {
          continue;
        }

        wprintw(io_window, "Searching for events in %u.\n", y);
        print_events(year{y});
      break;
      
      case CLI::Commands::YEAR_MONTH_SEARCH:
        if (!get_int(y, input_buffer, year_prompt)) {
          continue;
        }
        if (!get_int(m, input_buffer, month_prompt)) {
          continue;
        }

        wprintw(io_window, "Searching for events in %i-%u.\n", m, y);
        print_events(year_month{year{y}, month{m}});
      break;

      case CLI::Commands::YEAR_MONTH_DAY_SEARCH:
        if (!get_int(y, input_buffer, year_prompt)) {
          continue;
        }
        if (!get_int(m, input_buffer, month_prompt)) {
          continue;
        }
        if (!get_int(d, input_buffer, day_prompt)) {
          continue;
        }

        wprintw(io_window, "Searching for events in %i-%i-%u.\n", d, m, y);
        print_events(year_month_day{year{y}, month{m}, day{d}});
      break;

      case CLI::Commands::PRINT_COMMANDS:
        werase(io_window);
        mvwprintw(io_window, getmaxy(io_window) - 1, 0, "%s", commands.c_str());
        wprintw(io_window, "%s\n", commands.c_str());
        wrefresh(io_window);
      break;

      default:
        std::cout << "Command handler not implemented!" << std::endl;
      break;
    }
  }

  endwin();
}

static void create_io_window() {
  getmaxyx(stdscr, term_h, term_w);
  int h_top = (term_h * 2) / 3;
  int h_io = term_h - h_top;

  if (io_window) {
    delwin(io_window);
  }

  io_window = newwin(h_io, term_w, h_top, 0);

  scrollok(io_window, true);
  wmove(io_window, 1, 1);
  wrefresh(io_window);
}

CLIParser::CLIParser(Calendar::Calendar& calendar) {
  initscr();
  keypad(stdscr, true);
  create_io_window();
  noecho();
  m_io_thread = std::thread(io_thread, std::ref(*this), std::ref(calendar));
}

void CLIParser::join() {
  m_io_thread.join();
}

std::optional<Commands> CLIParser::get_user_cmd(std::string user_input) {
  for (size_t i = 0; i < COMMAND_STR.size(); i++) {
    if (matches_prefix(user_input, COMMAND_STR[i])) {
      return static_cast<Commands>(i);
    }
  }

  return std::nullopt;
}

std::optional<Events::TodEvent> CLIParser::create_event() {
  std::string input_buffer;
  std::string description;
  int y, m, h;
  unsigned  mo, d;

  get_input_line(description, description_prompt);
  if (!get_int(y, input_buffer, year_prompt)) {
    return std::nullopt;
  }
  if (!get_int(mo, input_buffer, month_prompt)) {
    return std::nullopt;
  }
  if (!get_int(d, input_buffer, day_prompt)) {
    return std::nullopt;
  }
  if (!get_int(h, input_buffer, hour_prompt)) {
    return std::nullopt;
  }
  if (!get_int(m, input_buffer, minute_prompt)) {
    return std::nullopt;
  }

  wprintw(io_window, "Creating event for %02i-%02i-%04u, @%02i:%02i\n", d, mo, y, h, m);
  year_month_day ymd{year{y}, month{mo}, day{d}};
  hh_mm_ss<seconds> mmhh{hours{h} + minutes{m}};

  return Events::TodEvent(description, ymd, mmhh);
}

}

