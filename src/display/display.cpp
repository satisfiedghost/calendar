#include <algorithm>
#include <curses.h>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <vector>

#include "display/display.h"
#include "events/tod_event.h"
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
  //constexpr unsigned int box_x_offset = 5;

  // Month, Year + hrule + 1 line
  constexpr unsigned int box_y_offset = 3;

  curs_set(0);
  // find box dimensions (5 rows of 7 each)
  unsigned int term_h, term_w;
  getmaxyx(m_display_window, term_h, term_w);

  // TODO - what if terminal aspect ratio is not wide?
  unsigned int box_height = (term_h - box_y_offset) / 5;
  unsigned int box_width = box_height * 2;
  wclear(m_display_window);

  // we want to center the calendar in this window
  auto total_calendar_width = box_width * 7;
  auto box_x_offset = (term_w - total_calendar_width) / 2;

  mvwprintw(m_display_window, 0, 0, "%s, %u", DateStrings::month_to_str(month).c_str(), static_cast<int>(year));
  mvwhline_set(m_display_window, 1, 0, WACS_HLINE, getmaxx(m_display_window));

  for (unsigned int i = 1; i < 8; i++) {
    auto weekday = std::chrono::weekday{i};

    // draw days of week 1 line above calendar boxes
    mvwprintw(m_display_window, box_y_offset - 1, static_cast<int>(box_x_offset + (i - 1) * box_width), "%s", DateStrings::weekday_to_str(weekday).c_str());
  }

  if (year != m_displayed_year or month != m_displayed_month) {
    m_boxes.clear();
    m_selected_idx = 0; // always default to day 1 to prevent out of bounds when switching months
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
        auto day_of_week = (i + j * 7 + 1) - weekday_start + 1;
        auto box_x_start = box_x_offset + i * box_width;
        auto box_y_start = box_y_offset + j * box_height;

        m_boxes.emplace_back(BoxSettings{box_x_start, box_y_start, box_width, box_height}, day_of_week);
      }
    }
  }
  done:

  for (auto& box : m_boxes) {
    box.draw(m_display_window, false);
  }
  m_boxes[m_selected_idx].draw(m_display_window, true);

  m_displayed_year = year;
  m_displayed_month = month;
  wrefresh(m_display_window);

  draw_info_window();
}

void Display::draw_info_window() {
  // this is the selected ymd
  auto ymd = std::chrono::year_month_day{m_displayed_year,
                                         m_displayed_month,
                                         std::chrono::day{static_cast<unsigned int>(m_selected_idx) + 1}};

  wclear(m_info_window);

  auto events = m_calendar.get_events(ymd);

  if (!events) {
    mvwprintw(m_info_window, 0, 0, "No events found.");
  } else {
    std::vector<const Events::TodEvent*> sorted_events((*events).begin(), (*events).end());

    std::sort(sorted_events.begin(), sorted_events.end(),
        [](const auto& a, const auto& b) {
          return (*a).get_tod() < (*b).get_tod();
        });

    std::ostringstream event_stream;
    for (const auto& event : sorted_events) {
      event_stream << *event << "\n";
    }
    mvwprintw(m_info_window, 0, 0, "%s", event_stream.str().c_str());
  }

  wrefresh(m_info_window);
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

