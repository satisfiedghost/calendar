#pragma once

#include "calendar.h"
#include "events/event_store.h"
#include <chrono>
#include <sys/ioctl.h>
#include <ncursesw/ncurses.h>
#include <cstddef>
#include <string>
#include <vector>

namespace CLI {

class Box; // forward decl

class Display {
public:
  Display(const Calendar::Calendar&);

  void set_windows(WINDOW*, WINDOW*);

  void select_up();

  void select_down();

  void select_left();

  void select_right();

  size_t get_selected_event_cnt() const { return m_displayed_events.size(); }

  Events::EventStore::TodPtr get_selected_event(size_t sel);

  void draw(std::chrono::year, std::chrono::month);
private:
  void draw_calendar(std::chrono::year, std::chrono::month);

  void draw_info_window();

  WINDOW* m_display_window;
  WINDOW* m_info_window;
  std::vector<Box> m_boxes;
  size_t m_selected_idx;
  const Calendar::Calendar& m_calendar;
  std::chrono::year m_displayed_year;
  std::chrono::month m_displayed_month;
  std::vector<Events::EventStore::TodPtr> m_displayed_events;
};

struct BoxSettings {
  unsigned int x;
  unsigned int y;
  unsigned int w;
  unsigned int h;
};

class Box {
public:
  Box(BoxSettings settings, unsigned int day)
    : m_settings(settings)
    , m_day(day)
  {}

  // draw the box with x, y as the top left coords
  void draw(WINDOW* window, bool bold=false) const;

  // clear the area on which the box is drawn
  void clear(WINDOW* window) const;
private:

  BoxSettings m_settings;
  unsigned int m_day;

  static const char* BOX_TOP_R;
  static const char* BOX_BOT_L;
  static const char* BOX_TOP_L;
  static const char* BOX_BOT_R;
  static const char* BOX_SIDE_V;
  static const char* BOX_SIDE_H;

  static const char* BOX_TOP_R_BOLD;
  static const char* BOX_BOT_L_BOLD;
  static const char* BOX_TOP_L_BOLD;
  static const char* BOX_BOT_R_BOLD;
  static const char* BOX_SIDE_V_BOLD;
  static const char* BOX_SIDE_H_BOLD;
};

}
