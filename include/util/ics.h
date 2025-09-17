#include <string>
#include <fstream>
#include <optional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "events/tod_event.h"

namespace Util {

// A key-value pair, any line which does not have the key BEGIN or END
// is interpreted as a key (with possible parameters) attached to an enclosing component
struct IcsKeyValue {
  std::string key;
  std::unordered_map<std::string, std::string> key_params;
  std::string value;
  bool complex = false;
};


// An ICS component.
// ICS Components can have nested components and are demarked by the BEGIN / END keys.
// The entire ICS calendar itself is a component marked by the VCALENDAR value
struct IcsComponent {
  std::string name;
  std::unordered_map<std::string, IcsKeyValue> key_values;
  std::unordered_map<std::string, std::vector<std::unique_ptr<IcsComponent>>> children;

  void clear() {
    name = "";
    key_values.clear();
    children.clear();
  }
};

// a timezone standard
struct TimezoneStandard {
  int tz_offset_to;
  int tz_offset_from;

  TimezoneStandard(const IcsComponent& standard)
    : tz_offset_to(std::atoi(standard.key_values.at("TZOFFSETTO").value.c_str()))
    , tz_offset_from(std::atoi(standard.key_values.at("TZOFFSETFROM").value.c_str()))
  {}
};

// a timezone object
struct VTimeZone {
  std::string tzid;
  TimezoneStandard standard;
  TimezoneStandard daylight;

  VTimeZone(const IcsComponent& timezone)
    : tzid(timezone.key_values.at("TZID").value)
    , standard(*timezone.children.at("STANDARD")[0].get())
    , daylight(*timezone.children.at("DAYLIGHT")[0].get())
  {}
};

struct VEvent {
  std::string summary;
};

class IcsParser {
public:
  IcsParser(const std::string& ics_filename);
  ~IcsParser();

  // acts as a generator, returning events until none are left
  std::optional<Events::TodEvent> get_event();
private:
  const std::string m_ics_filename;
  std::ifstream m_ics_file;
};

}

