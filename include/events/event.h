#pragma once
#include <string>

namespace Events {

class Event {
public:
  Event(std::string context)
    : m_context(context)
  {}

  virtual const std::string& get_context() const { return m_context; }

  virtual void update_context(const std::string& context);

protected:
  std::string m_context;
};

}
