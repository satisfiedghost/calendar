CXX := g++
CXXFLAGS := -std=c++20 -MP -MMD -Wall -Wextra -Werror -Wconversion -O2

.PHONY: clean clena all
INC_DIRS := include
SRC_DIRS := src src/events src/ymd src/disk
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
	$(CXX) $(CXXFLAGS) $^ -o $@

# compile
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clena: clean

clean:
	rm -frv $(OBJ_DIR) $(TARGET)

-include $(DEPS)
