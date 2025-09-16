CXX := g++
CXXFLAGS := -std=c++20 -MP -MMD -Wall -Wextra -Werror -Wconversion -g
LDLIBS := -lncursesw

.PHONY: clean clena all
INC_DIRS := include
SRC_DIRS := src src/events src/ymd src/disk src/cli src/display src/util
OBJ_DIR := build

CXXFLAGS += $(addprefix -I,$(INC_DIRS))

# find all .cpp files + main
SRCS := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.cpp)) main.cpp

# each TU needs a .o
# pattern replacement $(VAR:pattern=replacement)
OBJS := $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

# Dependency files
DEPS := $(OBJS:%.o=%.d)

# target executable
TARGET := calendar

all: $(TARGET)

# link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@

# compile
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clena: clean

clean:
	rm -frv $(OBJ_DIR) $(TARGET)

-include $(DEPS)
