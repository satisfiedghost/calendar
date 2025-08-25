#include <array>
#include <chrono>
#include <optional>
#include <iostream>

#include "events/tod_event.h"

namespace CLI {

enum class Commands {
  QUIT = 0,
  CREATE_EVENT = 1,
  YEAR_SEARCH = 2,
  YEAR_MONTH_SEARCH = 3,
  YEAR_MONTH_DAY_SEARCH = 4,
  PRINT_COMMANDS = 5,
  COMMAND_CNT = 6,
};



class CLIParser {
public:
  // print available commands
  void print_commands();
  // parse a command from user input
  // returns nullopt if command wasn't understood
  std::optional<Commands> get_user_cmd();
  // prompt the user to create an event
  Events::TodEvent create_event();

  template<typename I>
  static void get_user_int(std::string prompt, I& i) {
    while (true) {
      std::cout << prompt;

      if (std::cin >> i) {
        break;
      } else {
        std::cout << "Not an integer?" << std::endl;
        discard_remaining_input();
      }
    }
  }

  //template<class... Ints>
  //static void get_user_ints(std::string prompt, Ints&... ints) {
  //  (get_user_int(prompt, ints), ...);
  //}
  template<class... Ints>
  static void get_user_ints(const std::array<std::string, sizeof...(Ints)>& prompts, Ints&... ints) {
    size_t idx = 0;
    (get_user_int(prompts[idx++], ints), ...);
  }
private:
  static void discard_remaining_input();
};




}

