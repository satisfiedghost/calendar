#include <curses.h>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <vector>

#include "display/display.h"
#include "util/date_strings.h"

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

  mvwprintw(window, static_cast<int>(m_settings.y), static_cast<int>(m_settings.x), "%s", ((bold) ? BOX_TOP_L_BOLD : BOX_TOP_L));

  for (unsigned int i = 0; i < m_settings.w - 2; i++) {
    wprintw(window, "%s", ((bold) ? BOX_SIDE_H_BOLD : BOX_SIDE_H));
  }
  wprintw(window, "%s", ((bold) ? BOX_TOP_R_BOLD : BOX_TOP_R));

  //// draw sides of box
  for (unsigned int i = 0; i < m_settings.h - 2; i++) {
    mvwprintw(window, static_cast<int>(m_settings.y + i + 1), static_cast<int>(m_settings.x), "%s", ((bold) ? BOX_SIDE_V_BOLD : BOX_SIDE_V));

    // on first iteration, put day in top left
    if (i == 0) {
      mvwprintw(window, static_cast<int>(m_settings.y + 1), static_cast<int>(m_settings.x + 1), "%02d", m_day);
    }
    mvwprintw(window, static_cast<int>(m_settings.y + i + 1), static_cast<int>(m_settings.x + m_settings.w - 1), "%s", ((bold) ? BOX_SIDE_V_BOLD : BOX_SIDE_V));
  }

  //// draw bottom of box
  mvwprintw(window, static_cast<int>(m_settings.y + (m_settings.h - 1)), static_cast<int>(m_settings.x), "%s", ((bold) ? BOX_BOT_L_BOLD : BOX_BOT_L));
  for (unsigned int i = 0; i < m_settings.w - 2; i++) {
    wprintw(window, "%s", ((bold) ? BOX_SIDE_H_BOLD : BOX_SIDE_H));
  }
  wprintw(window, "%s", ((bold) ? BOX_BOT_R_BOLD : BOX_BOT_R));

}

void Box::clear(WINDOW* window) const {
  for (unsigned int i = 0; i < m_settings.w; i++) {
    for (unsigned int j = 0; j < m_settings.h; j++) {
      mvwprintw(window, static_cast<int>(m_settings.x + 1), static_cast<int>(m_settings.y + j), "%s", " ");
    }
  }
}

void Display::select_up() {
  if (m_selected_idx >= 7) {
    m_selected_idx -= 7;
  }
}

void Display::select_down() {
  if (m_selected_idx <= (m_boxes.size() - 7 - 1)) {
    m_selected_idx += 7;
  }
}

void Display::select_left() {
  if (m_selected_idx > 0) {
    m_selected_idx--;
  }
}

void Display::select_right() {
  if (m_selected_idx < m_boxes.size() - 1) {
    m_selected_idx++;
  }
}

void Display::draw_calendar(std::chrono::year year, std::chrono::month month) {
  curs_set(0);
  // find box dimensions (5 rows of 7 each)
  unsigned int h, w;
  getmaxyx(m_display_window, h, w);

  unsigned int box_height = h / 6;
  unsigned int box_width = box_height * 2;
  wclear(m_display_window);

  mvwprintw(m_display_window, 0, 0, "%s, %u", DateStrings::month_to_str(month).c_str(), static_cast<int>(year));
  mvwhline_set(m_display_window, 1, 0, WACS_HLINE, getmaxx(m_display_window));

  for (unsigned int i = 1; i < 8; i++) {
    auto weekday = std::chrono::weekday{i};

    mvwprintw(m_display_window, 4, static_cast<int>(5 + (i - 1) * box_width), "%s", DateStrings::weekday_to_str(weekday).c_str());
  }

  if (year != m_displayed_year or month != m_displayed_month) {
    m_boxes.clear();
    unsigned int days_in_month = static_cast<unsigned int>((year/month/std::chrono::last).day());
    unsigned int weekday_start = std::chrono::weekday{std::chrono::sys_days{year/month/1}}.iso_encoding();

    for (unsigned int j = 0; j < 5; j++) {
      for (unsigned int i = 0; i < 7; i++) {
        unsigned int day = i + j * 7 + 1;
        if (day < weekday_start) {
          continue;
        } else if (day > days_in_month) {
          goto done;
        }
        m_boxes.emplace_back(BoxSettings{5 + i * box_width, 5 + j * box_height, box_width, box_height}, (i + j * 7 + 1) - weekday_start + 1);
      }
    }
  }
  done:

  for (auto& box : m_boxes) {
    box.draw(m_display_window, false);
  }
  m_boxes[m_selected_idx].draw(m_display_window, true);


  wrefresh(m_display_window);
}

void Display::set_windows(WINDOW * display_window, WINDOW* info_window) {
  m_display_window = display_window;
  m_info_window = info_window;
}

Display::Display(const Calendar::Calendar& calendar)
  : m_selected_idx(0) 
  , m_calendar(calendar){
  m_displayed_year = std::chrono::year(1970);
  m_displayed_month = std::chrono::month(1);
}

}

