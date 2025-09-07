#include "app.h"
#include "cli/cli.h"
#include <thread>
#include <sstream>

namespace App {

Application::Application(CLI::Display& display, CLI::CLIParser& parser, Calendar::Calendar& calendar) 
  : m_display(display)
  , m_parser(parser)
  , m_calendar(calendar) {

  setlocale(LC_ALL, ""); // enable unicode
  initscr();
  keypad(stdscr, true);
  noecho();

  int term_h, term_w;
  getmaxyx(stdscr, term_h, term_w);
  int h_top = (term_h * 2) / 3;
  int h_io = term_h - h_top;

  m_display_window = newwin(h_top, term_w, 0, 0);

  // Create IO window frame
  m_io_window_frame = newwin(h_io, term_w, h_top, 0);
  box_set(m_io_window_frame, WACS_VLINE, WACS_HLINE);
  wrefresh(m_io_window_frame);

  // Create inner IO window (where interactive text is displayed)
  m_io_window = derwin(m_io_window_frame, h_io - 2, term_w - 2, 1, 1);
  scrollok(m_io_window, true);
  wmove(m_io_window, 1, 1);
  wrefresh(m_io_window);

  m_parser.set_windows(m_io_window_frame, m_io_window);
  m_display.set_window(m_display_window);
}

Application::~Application() {
  delwin(m_io_window);
  delwin(m_io_window_frame);
  delwin(m_display_window);
  endwin();
}


void Application::run() {
  std::optional<CLI::Commands> cmd = CLI::Commands::NONE;

  auto print_events = [&](auto time_search) {
    std::ostringstream event_stream;
    auto events = m_calendar.get_events(time_search);

    if (events) {
      for (const auto& e : *events) {
        event_stream << *e << "\n";
      }
      m_parser.print_str(event_stream.str());
      event_stream.clear();
    } else {
      m_parser.print_strln("No events found!");
    }
  };

  while(cmd != CLI::Commands::QUIT) {
    m_display.draw_calendar();
    cmd = m_parser.get_user_cmd();

    if (!cmd) {
      continue;
    }

    switch(*cmd) {
      case CLI::Commands::QUIT:
        m_parser.print_strln("Quitting, goodbye!");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        continue;
      break;

      case CLI::Commands::CREATE_EVENT: {
        auto e = m_parser.create_event();
        if (!e) {
          m_parser.print_strln("Unable to create event!");
          continue;
        }
        m_calendar.add_event(*e);
      }
      break;
      
      case CLI::Commands::YEAR_SEARCH: {
        auto year = m_parser.get_user_year();
        if (!year) {
          m_parser.print_strln("Invalid year.");
          continue;
        }
        m_parser.print_strln("Searching for events in " + std::to_string(static_cast<int>(*year)));
        print_events(*year);
      }
      break;
      
      case CLI::Commands::YEAR_MONTH_SEARCH: {
        auto year_month = m_parser.get_user_year_month();
        if (!year_month) {
          m_parser.print_strln("Invalid year month.");
          continue;
        }

        m_parser.print_strln("Searching for events in " + std::to_string(static_cast<int>((*year_month).year())) + 
                                                          std::to_string(static_cast<unsigned int>((*year_month).month())));
        print_events(*year_month);
      }
      break;

      case CLI::Commands::YEAR_MONTH_DAY_SEARCH: {
        auto ymd = m_parser.get_user_ymd();
        if (!ymd) {
          m_parser.print_strln("Invalid year month day.");
          continue;
        }

        m_parser.print_strln("Searching for events in " + std::to_string(static_cast<int>((*ymd).year())) + 
                                                          std::to_string(static_cast<unsigned int>((*ymd).month())) +
                                                          std::to_string(static_cast<unsigned int>((*ymd).day())));
        print_events(*ymd);
      }
      break;

      case CLI::Commands::PRINT_COMMANDS:
        m_parser.print_cmds();
      break;

      default:
        m_parser.print_strln("Command handler not implemented!");
      break;
    }

  }

}

}
