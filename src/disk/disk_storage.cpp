#include <chrono>
#include <filesystem>
#include <sstream>

#include "disk/disk_storage.h"
#include "events/tod_event.h"

constexpr char VERSION_TAG[] = "V";
constexpr char DELIM = '\t';
constexpr char NEWLINE = '\n';
constexpr char TOD_CONTEXT_HEADER[] = "Context";
constexpr char TOD_YEAR_HEADER[] = "Year";
constexpr char TOD_MONTH_HEADER[] = "Month";
constexpr char TOD_DAY_HEADER[] = "Day";
constexpr char TOD_HOUR_HEADER[] = "Hour";
constexpr char TOD_MINUTE_HEADER[] = "Minute";

#include <cstdlib>

namespace Storage {

namespace fs = std::filesystem;
using namespace std::chrono;

// for first time creating a save file
static void init_save_file(std::string save_file) {
  std::ofstream init_file(save_file);
  init_file << VERSION_TAG << DELIM << std::to_string(VERSION) << NEWLINE;
  init_file << TOD_CONTEXT_HEADER << DELIM;
  init_file << TOD_YEAR_HEADER << DELIM;
  init_file << TOD_MONTH_HEADER << DELIM;
  init_file << TOD_DAY_HEADER << DELIM;
  init_file << TOD_HOUR_HEADER << DELIM;
  init_file << TOD_MINUTE_HEADER << DELIM;
  init_file << NEWLINE;
}

DiskStorage::DiskStorage(std::string cal_file)
  : m_save_file(cal_file + ".calendar") 
  , m_temp_file(m_save_file + ".tmp") {

  if (!fs::exists(m_save_file)) {
    init_save_file(m_temp_file); // we'll be writing to the tempfile so init it here
    fs::copy_file(m_temp_file, m_save_file, fs::copy_options::overwrite_existing); // as if reading from an empty save file
  } else {
    fs::copy_file(m_save_file, m_temp_file, fs::copy_options::overwrite_existing);
  }

  m_outfile = std::ofstream(m_temp_file, std::ios::app);
  m_infile = std::ifstream(m_save_file);

  std::string v_tag, line;
  int version;
  std::getline(m_infile, line);
  std::istringstream iss(line);

  iss >> v_tag >> version;
  // m_infile now pointing at 2nd row
  // TODO - version aware parsing, just asasume the layout for now and discard the 2nd row
  std::getline(m_infile, line);
}

DiskStorage::~DiskStorage() {
  // flush everything to disk
  m_outfile.close();
  // copy the temp file over the real one
  fs::copy_file(m_temp_file, m_save_file, fs::copy_options::overwrite_existing);
  // remove the tmpfile
  // TODO - should make persistent intermediate backups
  fs::remove(m_temp_file);

}

void DiskStorage::save(const Events::TodEvent& event) {
  m_outfile << event.m_context << DELIM;

  auto ymd = event.get_ymd();
  m_outfile << static_cast<int>(ymd.year()) << DELIM;
  m_outfile << static_cast<unsigned int>(ymd.month()) << DELIM;
  m_outfile << static_cast<unsigned int>(ymd.day()) << DELIM;

  hh_mm_ss hms(event.m_tod);
  m_outfile << hms.hours().count() << DELIM;
  m_outfile << hms.minutes().count() << DELIM;
  m_outfile << NEWLINE;
}

std::optional<Events::TodEvent> DiskStorage::load_event() {
  // TODO - probably don't need to allocate resources on every call
  std::string line;
  std::getline(m_infile, line);

  if (!m_infile or m_infile.eof()) {
    m_infile.close();
    return std::nullopt;
  }

  std::string in_context;
  int in_year;
  unsigned int in_month, in_day, in_hour, in_minute;
  std::istringstream iss(line);
  std::getline(iss, in_context, '\t');

  iss >> in_year >> in_month >> in_day >> in_hour >> in_minute;

  year_month_day&& ymd{year{in_year}, month{in_month}, day{in_day}};
  Events::TodEvent event(in_context, ymd, hours{in_hour}, minutes{in_minute});

  return event;
}

//void DiskStorage::flush() {
//}

}
