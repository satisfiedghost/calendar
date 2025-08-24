#include <chrono>
#include <fstream>
#include <functional>
#include <optional>
#include <string>

#include "events/tod_event.h"

constexpr unsigned short VERSION = 1;

namespace Storage {

class DiskStorage {
public:
  DiskStorage(std::string save_file);
  ~DiskStorage();

  std::optional<Events::TodEvent> load_event();
  void save(const Events::TodEvent&);
  //void flush();

private:
  const std::string m_save_file;
  const std::string m_temp_file;
  std::ifstream m_infile;
  std::ofstream m_outfile;
  unsigned short m_save_file_version;
};

}
