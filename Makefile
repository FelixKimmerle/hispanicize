TARGET_EXEC ?= hispanicize

BUILD_DIR ?= ./build
SRC_DIRS ?= src/
PREFIX ?= /usr/local
CXX ?= g++
MKDIR_P ?= mkdir -p

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -std=c++17 -MMD -MP -Wall -Wextra

#
# Debug build settings
#
DBGDIR := $(BUILD_DIR)/debug
DBGEXE := $(DBGDIR)/$(TARGET_EXEC)
DBGOBJS := $(SRCS:%=$(DBGDIR)/%.o)
DBGCFLAGS := $(CPPFLAGS) -g -O0 -ftrapv -DDEBUG

#
# Release build settings
#
RELDIR := $(BUILD_DIR)/release
RELEXE := $(RELDIR)/$(TARGET_EXEC)
RELOBJS := $(SRCS:%=$(RELDIR)/%.o)
RELCFLAGS := $(CPPFLAGS) -O2 -DRELEASE -DNDEBUG

DEPS := $(RELOBJS:.o=.d) $(DBGOBJS:.o=.d)

.PHONY: clean release debug run rund all remake test install uninstall

all: release debug
remake: clean all

release: $(RELEXE)

$(RELEXE): $(RELOBJS)
	$(MKDIR_P) $(dir $@)
	$(CXX) $(RELOBJS) -o $@ $(LDFLAGS)

$(RELDIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(RELCFLAGS) -c $< -o $@


debug: $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
	$(MKDIR_P) $(dir $@)
	$(CXX) $(DBGOBJS) -o $@ $(LDFLAGS)

$(DBGDIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(DBGCFLAGS) -c $< -o $@


clean:
	$(RM) -r $(BUILD_DIR)

run: $(RELEXE)
	$(RELEXE)

rund: $(DBGEXE)
	$(DBGEXE)

german_text.txt:
	python3 generate.py

test: $(RELEXE) german_text.txt
	valgrind --leak-check=full $(RELEXE) < german_text.txt > /dev/null

install: $(RELEXE)
	install -Dm755 $(RELEXE) $(DESTDIR)$(PREFIX)/bin/$(TARGET_EXEC)

uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/$(TARGET_EXEC)

-include $(DEPS)
