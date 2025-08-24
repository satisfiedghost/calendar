#include "events/event.h"

namespace Events {

void Event::update_context(const std::string& context) {
  m_context = context;
}

}

