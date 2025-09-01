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

const std::string Box::BOX_TOP_R = "┐";
const std::string Box::BOX_BOT_L = "└";
const std::string Box::BOX_TOP_L = "┌";
const std::string Box::BOX_BOT_R = "┘";
const std::string Box::BOX_SIDE_V = "│";
const std::string Box::BOX_SIDE_H = "─";
const std::string Box::BOX_TOP_R_BOLD = "┓";
const std::string Box::BOX_BOT_L_BOLD = "┗";
const std::string Box::BOX_TOP_L_BOLD = "┏";
const std::string Box::BOX_BOT_R_BOLD = "┛";
const std::string Box::BOX_SIDE_V_BOLD = "┃";
const std::string Box::BOX_SIDE_H_BOLD = "━";

static void goto_row(size_t row) {
  std::cout << ANSI_ESC << row << ";" << 1 << "H";
}

static void goto_xy(size_t x, size_t y) {
  std::cout << ANSI_ESC << y << ";" << x << "H";
}

void hrule(size_t row, size_t width) {
  goto_row(row);
  for (size_t i = 0; i < width; i++) {
    std::cout << "_";
  }
}

void Box::draw(bool bold) const {
  goto_xy(m_x, m_y);

  // draw top of box
  std::cout << ((bold) ? BOX_TOP_L_BOLD : BOX_TOP_L);
  for (size_t i = 0; i < m_w - 2; i++) {
    std::cout << ((bold) ? BOX_SIDE_H_BOLD : BOX_SIDE_H);
  }
  std::cout << ((bold) ? BOX_TOP_R_BOLD : BOX_TOP_R);

  // draw sides of box
  for (size_t i = 0; i < m_h - 2; i++) {
    goto_xy(m_x, m_y + i + 1);
    std::cout << ((bold) ? BOX_SIDE_V_BOLD : BOX_SIDE_V);
    goto_xy(m_x + m_w - 1, m_y + i + 1);
    std::cout << ((bold) ? BOX_SIDE_V_BOLD : BOX_SIDE_V);
  }

  // draw bottom of box
  goto_xy(m_x, m_y + (m_h - 1));
  std::cout << ((bold) ? BOX_BOT_L_BOLD : BOX_BOT_L);
  for (size_t i = 0; i < m_w - 2; i++) {
    std::cout << ((bold) ? BOX_SIDE_H_BOLD : BOX_SIDE_H);
  }
  std::cout << ((bold) ? BOX_BOT_R_BOLD : BOX_BOT_R);
}

void Box::clear() const {
  for (size_t i = 0; i < m_w; i++) {
    for (size_t j = 0; j < m_h; j++) {
      goto_xy(m_x + i, m_y + j);
      std::cout << " ";
    }
  }
}

Display::Display() {
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &m_ws) < 0) {
    perror("ioctl");
    // rude.
    throw std::runtime_error("Your terminal is fucked.");
  }
  // clear everything up to the cli row
  m_cli_row = 2 * m_ws.ws_row / 3;
  for (size_t i = 1; i < m_cli_row; i++) {
    goto_xy(1, i);
    std::cout << ERASE_LINE;
  }

  std::vector<Box> boxes;

  // find box dimensions (4 rows of 7 each)
  size_t box_height = m_cli_row / 4;
  size_t box_width = box_height * 2;

  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 7; j++) {
      boxes.emplace_back(1 + box_width * j, 1 + box_height * i, box_width, box_height);
    }
  }

  for (const auto& box : boxes) {
    box.draw();
  }

  boxes[0].clear();
  boxes[0].draw(true);

  hrule(m_cli_row, m_ws.ws_col);
}

}

