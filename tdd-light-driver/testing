CPPUTEST_HOME := ./cpputest-3.8

TARGET_DIR := target

SRC_DIRS := \
  src/main/Interface \
  src/main/LedDriver \

TEST_DIRS := \
  src/test \
  src/test/LedDriver \

CPP := g++
CPPFLAGS := -std=c++11
CPPFLAGS += -g -Wall
CPPFLAGS += -I$(CPPUTEST_HOME)/include $(foreach dir,$(SRC_DIRS),-I$(dir))

LDFLAGS := -L$(CPPUTEST_HOME)/lib -lCppUTest

SRC_FILES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cc))
SRC_OBJS = $(SRC_FILES:.cc=.o)

TEST_FILES = $(foreach dir,$(TEST_DIRS),$(wildcard $(dir)/*.cpp))
TEST_OBJS = $(TEST_FILES:.cpp=.o)

.PHONY: test
test: $(TEST_OBJS) $(SRC_OBJS)
	$(CPP) -o $(TARGET_DIR)/$@ $^ $(LDFLAGS)
	$(TARGET_DIR)/$@ -v

.PHONY: clean
clean:
	rm -f $(SRC_OBJS) $(TEST_OBJS) $(TARGET_DIR)/*

.PHONY: debug
debug:
	@echo
	@echo "Target Source files:"
	@echo $(SRC_FILES)
	@echo "Traget Object files:"
	@echo $(SRC_OBJS)
