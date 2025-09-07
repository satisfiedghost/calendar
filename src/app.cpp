#include "app.h"

namespace App {

Application::Application(CLI::Display& display, CLI::CLIParser& parser) 
  : m_display(display)
  , m_parser(parser) {

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

void Application::run() {
  m_parser.do_io();
}

}
