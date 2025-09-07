#include <array>
#include <chrono>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <vector>
#include "calendar.h"

#include "cli/cli.h"
#include "events/tod_event.h"

namespace CLI {

using namespace std::chrono;

static WINDOW* display_window;
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

static void get_input_line(std::string& input_buffer, const std::string& input_prompt, WINDOW* io_window) {
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
static bool get_int(T& value, std::string& input_buffer, const std::string& prompt, WINDOW* io_window) {
  char* strtol_end;
  get_input_line(input_buffer, prompt, io_window);
  value = static_cast<T>(strtol(input_buffer.c_str(), &strtol_end, 10));
  if (strtol_end == input_buffer.c_str()) {
    wprintw(io_window, "Invalid year\n");
    wrefresh(io_window);
    return false;
  }
  input_buffer.clear();
  return true;
}

void CLIParser::do_io() {
  std::istringstream input_stream;

  werase(m_io_window);
  mvwprintw(m_io_window, getmaxy(m_io_window) - 1, 0, "%s", commands.c_str());

  auto print_events = [&](auto s) {
    wclrtobot(m_io_window);
    std::ostringstream event_stream;
    auto events = m_calendar.get_events(s);

    if (events) {
      for (const auto& e : *events) {
        event_stream << *e << "\n";
      }
      wprintw(m_io_window, "%s\n", event_stream.str().c_str());
      wrefresh(m_io_window);
      event_stream.clear();
    } else {
      wprintw(m_io_window, "%s\n", "No events found!");
    }
  };

  bool cont = true;
  int y;
  unsigned int m, d;
  while (cont) {
    // calendar draw logic
    m_display.draw_calendar();

    std::string input_buffer;
    input_buffer.clear();
    get_input_line(input_buffer, command_prompt, m_io_window);

    auto cmd = get_user_cmd(input_buffer);
    input_buffer.clear();

    if (!cmd) {
      wprintw(m_io_window, "%s\n", "Command not recognized.");
      continue;
    }

    switch (*cmd) {
      case CLI::Commands::QUIT:
        wprintw(m_io_window, "%s\n", "Quitting!");
        cont = false;
      break;

      case CLI::Commands::CREATE_EVENT: {
        auto e = create_event();
        if (!e) {
          wprintw(m_io_window, "%s\n", "Unable to create event!");
          continue;
        }
        m_calendar.add_event(*e);
      }
      break;
      
      case CLI::Commands::YEAR_SEARCH:
        if (!get_int(y, input_buffer, year_prompt, m_io_window)) {
          continue;
        }

        wprintw(m_io_window, "Searching for events in %u.\n", y);
        print_events(year{y});
      break;
      
      case CLI::Commands::YEAR_MONTH_SEARCH:
        if (!get_int(y, input_buffer, year_prompt, m_io_window)) {
          continue;
        }
        if (!get_int(m, input_buffer, month_prompt, m_io_window)) {
          continue;
        }

        wprintw(m_io_window, "Searching for events in %i-%u.\n", m, y);
        print_events(year_month{year{y}, month{m}});
      break;

      case CLI::Commands::YEAR_MONTH_DAY_SEARCH:
        if (!get_int(y, input_buffer, year_prompt, m_io_window)) {
          continue;
        }
        if (!get_int(m, input_buffer, month_prompt, m_io_window)) {
          continue;
        }
        if (!get_int(d, input_buffer, day_prompt, m_io_window)) {
          continue;
        }

        wprintw(m_io_window, "Searching for events in %i-%i-%u.\n", d, m, y);
        print_events(year_month_day{year{y}, month{m}, day{d}});
      break;

      case CLI::Commands::PRINT_COMMANDS:
        werase(m_io_window);
        mvwprintw(m_io_window, getmaxy(m_io_window) - 1, 0, "%s", commands.c_str());
        wprintw(m_io_window, "%s\n", commands.c_str());
        wrefresh(m_io_window);
      break;

      default:
        wprintw(m_io_window, "Command handler not implemented!");
      break;
    }

  }

  endwin();
}

void CLIParser::create_io_window() {
  getmaxyx(stdscr, term_h, term_w);
  int h_top = (term_h * 2) / 3;
  int h_io = term_h - h_top;

  display_window = newwin(h_top, term_w, 0, 0);

  // Create IO window frame
  m_io_window_frame = newwin(h_io, term_w, h_top, 0);
  box_set(m_io_window_frame, WACS_VLINE, WACS_HLINE);
  wrefresh(m_io_window_frame);

  // Create inner IO window (where interactive text is displayed)
  m_io_window = derwin(m_io_window_frame, h_io - 2, term_w - 2, 1, 1);
  scrollok(m_io_window, true);
  wmove(m_io_window, 1, 1);
  wrefresh(m_io_window);
}


CLIParser::CLIParser(Calendar::Calendar& calendar, Display& display)
  : m_display(display) 
  , m_calendar(calendar) {
  setlocale(LC_ALL, ""); // enable unicode
  initscr();
  keypad(stdscr, true);
  create_io_window();
  noecho();
  display.set_window(display_window);
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

  get_input_line(description, description_prompt, m_io_window);
  if (!get_int(y, input_buffer, year_prompt, m_io_window)) {
    return std::nullopt;
  }
  if (!get_int(mo, input_buffer, month_prompt, m_io_window)) {
    return std::nullopt;
  }
  if (!get_int(d, input_buffer, day_prompt, m_io_window)) {
    return std::nullopt;
  }
  if (!get_int(h, input_buffer, hour_prompt, m_io_window)) {
    return std::nullopt;
  }
  if (!get_int(m, input_buffer, minute_prompt, m_io_window)) {
    return std::nullopt;
  }

  wprintw(m_io_window, "Creating event for %02i-%02i-%04u, @%02i:%02i\n", d, mo, y, h, m);
  year_month_day ymd{year{y}, month{mo}, day{d}};
  hh_mm_ss<seconds> mmhh{hours{h} + minutes{m}};

  return Events::TodEvent(description, ymd, mmhh);
}

}

