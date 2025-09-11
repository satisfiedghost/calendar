#pragma once

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

  // loads events from disk, acts as a generator and will return a nullopt when done
  std::optional<Events::TodEvent> load_event();

  // save an event to disk
  void save(const Events::TodEvent&);

private:
  const std::string m_save_file_path;
  std::string m_outfile_path;
  std::ifstream m_infile;
  std::ofstream m_outfile;
  unsigned short m_save_file_version;
};

}
