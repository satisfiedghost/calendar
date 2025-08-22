CPPFLAGS := -std=c++20 -Iinclude -MP -MMD -Wall -Wextra -Werror -Wconversion

.PHONY: clean clena

calendar:
	$(CXX) $(CPPFLAGS) main.cpp day.cpp -o calendar

clena: clean

clean:
	rm -frv calendar
