#include "events/tod_event.h"
#include <tuple>
#include <chrono>

namespace CLI {

class CLIParser {
public:
  std::tuple<std::chrono::year_month_day, Events::TodEvent> create_event();
};

}

