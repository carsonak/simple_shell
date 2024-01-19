#!/usr/bin/make -f

BINARY := ./s_sh # Final binary executable
SRC_DIR := ./
LIB_DIR := lib/ # Library directories
BUILD_DIR := ./build # Directory to place object files
CC := gcc # Compiler
OPT_FLAGS := -O3 # Optimisation flags
DEP_FLAGS := -MP -MMD # Generate files that encode make rules for the .h dependencies
WRN_FLAGS := -pedantic -Wall -Werror -Wextra
F_FLAGS := -fdiagnostics-color=always-fsanitize=undefined -fsanitize-undefined-trap-on-error -fstack-protector-all
LNK_FLAGS := -lm -lrt

INCL_DIRS := $(shell find $(SRC_DIR) -type d) # Directories to search fo header files
INCL_FLAGS := $(addprefix -I,$(INCL_DIRS)) # Include flags for all directories

SRC_FILES := $(shell find $(SRC_DIR) -name '*.c') # All the source files
OBJ_FILES := $(SRC_FILES:%.c=$(BUILD_DIR)/%.o) # Object files stored in ./build
DEP_FILES := $(OBJ_FILES:.o=.d) # Dependency files for make

CFLAGS := $(INCL_FLAGS) -std=c17 $(WARN_FLAGS) $(F_FLAGS) -g $(DEP_FLAGS)

# Include the dependencies
-include $(DEP_FILES)

# First rule that will be run by make on default
all: build-dirs $(BINARY)

# Rule for compiling a final executable file
# $@ - the target. $^ - the prerequisites
$(BINARY): $(OBJ_FILES)
	$(CC) $^ -o $@

# Rule for creating folders if they don't exist
# @ silences the printing of the command
build-dirs:
	@mkdir $(BUILD_DIR)

# Redifing implicit rule for making object files
# $< - only the first prerequisite
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Removes all obj and dep files and their folders plus the binary file
clean:
	@rm -drf --preserve-root $(BINARY) $(OBJ_FILES) $(DEP_FILES) $(OBJ_DIR) $(DEP_DIR)

# shell commands are a set of keystrokes away
distribute: clean
	tar zcvf dist.tgz *

# $(info ...) prints to output
diff:
	$(info Repository status and per-file changes:)
	git status
	@git diff --stat

# Make a copy of all source codes into the parent folder
cpy-src:
	@cp -fu $(SRC_FILES) *.h Makefile ../

# .PHONY so that the rules work even if a file with the same target-name exists.
.PHONY: all clean distribute diff $(SRC_DIR) $(INCL_DIR)
