#include <chrono>
#include <vector>

#include "calendar.h"
#include "events/tod_event.h"
#include "util/date_strings.h"
#include "util/chrono_literal_extensions.h"

using namespace std::chrono;
using namespace std::chrono_literals;

int main() {
  Calendar::Calendar c;
  Events::TodEvent e("something to do", 18, 30);
  year_month_day ymd{2025y, 8_mo, 30d};

  c.add_event(ymd, e);

  auto events = c.get_events(ymd);

  if (events) {
    for (const auto& e : events->get()) {
      std::cout << e << std::endl;
    }
  }

  return 0;
}
