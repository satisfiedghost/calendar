#include "events/tod_event.h"
#include <chrono>

namespace CLI {

class CLIParser {
public:
  Events::TodEvent create_event();
};

}

