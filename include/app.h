#include <ncursesw/ncurses.h>

#include "cli/cli.h"
#include "display/display.h"

namespace App {
// Top level application class to manage ncurses windows and
// run application logic
class Application {
public:
Application(CLI::Display&, CLI::CLIParser&);

void run();

private:
CLI::Display& m_display;
CLI::CLIParser& m_parser;
WINDOW* m_io_window_frame;
WINDOW* m_io_window;
WINDOW* m_display_window;
};

}

