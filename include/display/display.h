#pragma once

#include "cli/cli.h"
#include <sys/ioctl.h>
#include <cstddef>
#include <string>

namespace CLI {

class Display {
public:
  Display();
private:

  struct winsize m_ws;
  size_t m_cli_row;
};

class Box {
public:
  Box(size_t x, size_t y, size_t w, size_t h)
    : m_x(x)
    , m_y(y)
    , m_w(w)
    , m_h(h)
  {}

  // draw the box with x, y as the top left coords
  void draw(bool bold=false) const;

  // clear the area on which the box is drawn
  void clear() const;
private:
  size_t m_x;
  size_t m_y;
  size_t m_w;
  size_t m_h;

  static const std::string BOX_TOP_R;
  static const std::string BOX_BOT_L;
  static const std::string BOX_TOP_L;
  static const std::string BOX_BOT_R;
  static const std::string BOX_SIDE_V;
  static const std::string BOX_SIDE_H;

  static const std::string BOX_TOP_R_BOLD;
  static const std::string BOX_BOT_L_BOLD;
  static const std::string BOX_TOP_L_BOLD;
  static const std::string BOX_BOT_R_BOLD;
  static const std::string BOX_SIDE_V_BOLD;
  static const std::string BOX_SIDE_H_BOLD;
};

}
