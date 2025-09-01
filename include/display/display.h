#pragma once

#include <sys/ioctl.h>
#include <ncursesw/ncurses.h>
#include <cstddef>
#include <string>
#include <vector>

namespace CLI {

class Box; // forward decl

class Display {
public:
  Display();

  void set_window(WINDOW*);

  void draw_calendar();
private:
  WINDOW* m_window;
  std::vector<Box> m_boxes;
};

class Box {
public:
  Box(int x, int y, int w, int h)
    : m_x(x)
    , m_y(y)
    , m_w(w)
    , m_h(h)
  {}

  // draw the box with x, y as the top left coords
  void draw(WINDOW* window, bool bold=false) const;

  // clear the area on which the box is drawn
  void clear(WINDOW* window) const;
private:

  int m_x;
  int m_y;
  int m_w;
  int m_h;

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
