#include <filesystem>
#include <stdexcept>
#include <iostream>

#include "util/ics.h"

namespace fs = std::filesystem;

namespace Util {

static const std::string BEGIN = "BEGIN";
static const std::string END = "END";
static const std::string VEVENT = "VEVENT"; 
static const std::string SUMMARY = "SUMMARY";
static const std::string DATE_START = "DTSTART";
static const std::string DATE_START_LA = "DTSTART;TZID=America/Los_Angeles";

IcsParser::IcsParser(const std::string& ics_filename) 
  : m_ics_filename(ics_filename) {

  if (!fs::exists(m_ics_filename)) {
    throw std::runtime_error("ICS file does not exist.");
  }

  m_ics_file = std::ifstream(m_ics_filename);
}

IcsParser::~IcsParser() {
  m_ics_file.close();
}

template <typename T, typename U, typename... Args>
static void getline_crlf(T& t, U& u, Args&&... args) {
  u.clear();
  static std::string line = "";
  u += line;
  bool cont = true;

  while (cont) {
    std::getline(t, line, std::forward<Args>(args)...);

    if (line[0] != ' ') {
      cont = false;
    } else {
      line.erase(0, 1);
    }

    if (line.back() == '\r') {
      line.pop_back();
    }

    if (cont) {
      u += line;
    }
  }
}

static IcsKeyValue parse_line(const std::string& line) {
  IcsKeyValue ics_keyval;

  auto delim_pos = line.find(':');

  std::string header = line.substr(0, delim_pos);
  std::string body = line.substr(delim_pos + 1);

  

  delim_pos = header.find(';');
  if (delim_pos == std::string::npos) {
    ics_keyval.key = header;
  } else {
    ics_keyval.key = header.substr(0, delim_pos);

    std::string params = header.substr(delim_pos + 1);
    std::stringstream ss(params);
    std::string param;

    while (std::getline(ss, param, ';')) {
      ics_keyval.key_params.push_back(param);
    }
  }

  delim_pos = body.find(';');
  if (delim_pos == std::string::npos) {
    ics_keyval.value = body;
  } else {
    ics_keyval.value = "COMPLEX";

    ics_keyval.value_params.push_back(body.substr(0, delim_pos));
    std::string params = body.substr(delim_pos + 1);
    std::stringstream ss(params);
    std::string param;

    while (std::getline(ss, param, ';')) {
      ics_keyval.value_params.push_back(param);
    }
  }


  return ics_keyval;
}

static void print_component(IcsComponent& c) {
  std::cout << "Component Name: " << c.name << std::endl;
  std::cout << "Key Values:" << std::endl;

  for (auto& kv : c.key_values) {
    std::cout << "\t Key: " << kv.key << std::endl;
    for (auto & p : kv.key_params) {
      std::cout << "\t\t Key Param: " << p << std::endl;
    }
    std::cout << "\t Value: " << kv.value << std::endl;
    for (auto & p : kv.value_params) {
      std::cout << "\t\t Value Param: " << p << std::endl;
    }
    std::cout << std::endl;
  }
}

std::optional<Events::TodEvent> IcsParser::get_event() {
  if (!m_ics_file or m_ics_file.eof()) {
    return std::nullopt;
  }

  std::string line;
  size_t parse_level = 0;
  IcsComponent component;

  while (true) {
    getline_crlf(m_ics_file, line);
    auto kp = parse_line(line);

    if (kp.key == BEGIN) {
      if (kp.value == "VCALENDAR") {
        continue;
      }
      if (parse_level == 0) {
        component.name = kp.value;
      }
      parse_level++;
    } else if (kp.key == END) {
      if (kp.value == "VCALENDAR") {
        break;
      }
      parse_level--;
      if (parse_level == 0) {
        print_component(component);
        component.clear();
      }
    } else {
      component.key_values.push_back(kp);
    }
  }

  return std::nullopt;
}


}

