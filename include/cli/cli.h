#include "events/tod_event.h"
#include <chrono>

namespace CLI {

class CLIParser {
public:
  // prompt the user to create an event
  Events::TodEvent create_event();
};

}

