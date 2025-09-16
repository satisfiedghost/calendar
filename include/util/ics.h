#include <string>
#include <fstream>
#include <optional>
#include <vector>

#include "events/tod_event.h"

namespace Util {


struct IcsKeyValue {
  std::string key;
  std::vector<std::string> key_params;
  std::string value;
  std::vector<std::string> value_params;
};

struct IcsComponent {
  std::string name;
  std::vector<IcsKeyValue> key_values;
  std::vector<IcsComponent> children;

  void clear() {
    name = "";
    key_values.clear();
    children.clear();
  }
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

