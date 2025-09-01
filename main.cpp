#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "calendar.h"
#include "cli/cli.h"
#include "display/display.h"
#include "events/tod_event.h"
#include "util/date_strings.h"
#include "util/chrono_literal_extensions.h"

using namespace std::chrono;
using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "supply calendar file?" << std::endl;
    return -1;
  } else if (argc > 2) {
    std::cout << "too many arguments" << std::endl;
    return -1;
  }

  Calendar::Calendar c{std::string(argv[1])};
  CLI::CLIParser parser(c);


  parser.join();

  return 0;
}
