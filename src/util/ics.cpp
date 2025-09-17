#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <string>

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

    if (line.back() == '\r') {
      line.pop_back();
    }

    if (line[0] != ' ') {
      if (u.empty()) {
        u = line;
        line.clear();
      }
      cont = false;
    } else {
      line.erase(0, 1);
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
      auto param_delim = param.find('=');
      ics_keyval.key_params[param.substr(0, param_delim)] = param.substr(param_delim + 1);
    }
  }

  ics_keyval.value = body;

  if (body.find(';') != std::string::npos) {
    ics_keyval.complex = true;
  }


  return ics_keyval;
}

static void print_component(IcsComponent& c, size_t level = 0) {
  std::string indent = "";
  for (size_t i = 0; i < level; i++) {
    indent += "\t";
  }
  std::cout << indent << "Component Name: " << c.name << std::endl;
  std::cout << indent << "Key Values:" << std::endl;

  for (auto& [_, v] : c.key_values) {
    std::cout << indent << "\tKey: " << v.key << std::endl;
    if (v.key_params.size() > 0) {
      std::cout << indent << "\tKey Params: " <<  std::endl;
      for (auto &[k, p] : v.key_params) {
        std::cout << indent << "\t\t" << k << " = " << p << std::endl;
      }
    }
    std::cout << indent << "\tValue: " << v.value << std::endl;
    std::cout << indent << "\tComplex: " << v.complex << std::endl << std::endl;
  }

  if (c.children.size() > 0) {
    std::cout << indent << "\tChildren: " << std::endl;
    for (auto& [k, c] : c.children) {
      std::cout << indent << "\tType: " << k << std::endl;
      for (auto& ch : c) {
        print_component(*ch.get(), level + 1);
      }
    }
  }
}

static IcsComponent parse_component(std::ifstream& ifs, const std::string name) {
  IcsComponent comp;
  comp.name = name;
  std::string line;


  while (true) {
    getline_crlf(ifs, line);
    auto kp = parse_line(line);

    if (kp.key == BEGIN) {
      // recursively add child components
      comp.children[kp.value].push_back(std::make_unique<IcsComponent>(parse_component(ifs, kp.value)));
    } else if (kp.key == END) {
      return comp;
    } else {
      comp.key_values[kp.key] = kp;
    }
  }
}

std::optional<Events::TodEvent> IcsParser::get_event() {
  if (!m_ics_file or m_ics_file.eof()) {
    return std::nullopt;
  }

  std::string line;
  IcsComponent component;

  getline_crlf(m_ics_file, line);
  auto kp = parse_line(line);

  IcsComponent comp = parse_component(m_ics_file, kp.value);
  print_component(comp);
  std::vector<VTimeZone> time_zones;

  for (const auto& tz : comp.children.at("VTIMEZONE")) {
    time_zones.emplace_back(*tz.get());
  }


  //for (const auto& tz : time_zones) {
  //  std::cout << tz.tzid << std::endl;
  //}


  return std::nullopt;
}


}

