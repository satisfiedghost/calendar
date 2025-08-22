#pragma once
#include <string>

namespace Events {

class Event {
public:
  Event(std::string context)
    : m_context(context)
  {}

  const std::string& get_context() const { return m_context; }

protected:
  std::string m_context;
};

}
