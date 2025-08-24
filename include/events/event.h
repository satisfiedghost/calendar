#pragma once
#include <string>

namespace Events {

// generic event class
class Event {
public:
  Event(std::string context)
    : m_context(context)
  {}

  virtual const std::string& get_context() const { return m_context; }

  virtual void update_context(const std::string& context);

  auto operator<=>(const Event&) const = default;

protected:
  std::string m_context;
};

}
