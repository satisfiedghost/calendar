#include <curses.h>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <vector>

#include "display/display.h"

namespace CLI {

std::string ANSI_ESC = "\x1b[";
std::string SAVE_CURSOR = ANSI_ESC + "s";
std::string RESTORE_CURSOR = ANSI_ESC + "u";
std::string HIDE_CURSOR = ANSI_ESC + "?25l";
std::string SHOW_CURSOR = ANSI_ESC + "?25h";
std::string RETURN = "\r";
std::string ERASE_LINE = ANSI_ESC + "2K";

const char* Box::BOX_TOP_R = "┐";
const char* Box::BOX_BOT_L = "└";
const char* Box::BOX_TOP_L = "┌";
const char* Box::BOX_BOT_R = "┘";
const char* Box::BOX_SIDE_V = "│";
const char* Box::BOX_SIDE_H = "─";
const char* Box::BOX_TOP_R_BOLD = "┓";
const char* Box::BOX_BOT_L_BOLD = "┗";
const char* Box::BOX_TOP_L_BOLD = "┏";
const char* Box::BOX_BOT_R_BOLD = "┛";
const char* Box::BOX_SIDE_V_BOLD = "┃";
const char* Box::BOX_SIDE_H_BOLD = "━";

void Box::draw(WINDOW* window, bool bold) const {
  //wmove(window, m_x, m_y);

  // draw top of box

  mvwprintw(window, m_y, m_x, "%s", ((bold) ? BOX_TOP_L_BOLD : BOX_TOP_L));

  for (int i = 0; i < m_w - 2; i++) {
    wprintw(window, "%s", ((bold) ? BOX_SIDE_H_BOLD : BOX_SIDE_H));
  }
  wprintw(window, "%s", ((bold) ? BOX_TOP_R_BOLD : BOX_TOP_R));

  //// draw sides of box
  for (int i = 0; i < m_h - 2; i++) {
    mvwprintw(window, m_y + i + 1, m_x, "%s", ((bold) ? BOX_SIDE_V_BOLD : BOX_SIDE_V));
    mvwprintw(window, m_y + i + 1, m_x + m_w - 1, "%s", ((bold) ? BOX_SIDE_V_BOLD : BOX_SIDE_V));
  }

  //// draw bottom of box
  mvwprintw(window, m_y + (m_h - 1), m_x, "%s", ((bold) ? BOX_BOT_L_BOLD : BOX_BOT_L));
  for (int i = 0; i < m_w - 2; i++) {
    wprintw(window, "%s", ((bold) ? BOX_SIDE_H_BOLD : BOX_SIDE_H));
  }
  wprintw(window, "%s", ((bold) ? BOX_BOT_R_BOLD : BOX_BOT_R));

}

void Box::clear(WINDOW* window) const {
  for (int i = 0; i < m_w; i++) {
    for (int j = 0; j < m_h; j++) {
      mvwprintw(window, m_x + 1, m_y + j, "%s", " ");
    }
  }
}

void Display::draw_calendar() {
  // find box dimensions (4 rows of 7 each)
  int h, w;
  getmaxyx(m_window, h, w);
  int box_height = h / 5;
  int box_width = box_height * 2;
  wclear(m_window);

  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 7; i++) {
      m_boxes.emplace_back(5 + i * box_width, 5 + j * box_height, box_width, box_height);
    }
  }

  for (auto& box : m_boxes) {
    box.draw(m_window, false);
  }
  box_set(m_window, WACS_VLINE, WACS_HLINE);
  wrefresh(m_window);
}

void Display::set_window(WINDOW * window) {
  m_window = window;
}

Display::Display(){}
}

