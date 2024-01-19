#!/usr/bin/make

BINARY := ./s_sh # Final binary executable
SRC_DIR := ./scrap
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
OBJ_FILES := $(SRC_FILES:$(notdir %.c)=$(BUILD_DIR)/%.o) # Object files stored in ./build
DEP_FILES := $(OBJ_FILES:.o=.d) # Dependency files for make

CFLAGS := $(INCL_FLAGS) -std=c17 $(WARN_FLAGS) $(F_FLAGS) -g $(DEP_FLAGS)

# First rule that will be run by make on default
all: $(BINARY)

# Rule for compiling a final executable file
# $@ - the target. $^ - the prerequisites
$(BINARY): $(OBJ_FILES) $(BUILD_DIR)
	$(CC) $^ -o $@ $(LNK_FLAGS)

# Create folder if it don't exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Redifing implicit rule for making object files
# $< - only the first prerequisite
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Removes all obj and dep files and their folders plus the binary file
# @ silences the printing of the command
clean:
	@rm -rd --preserve-root $(BUILD_DIR)

# Make a copy of all source codes, header files and Makefile into a back_up folder
cpy-src:
	@cp -fu $(SRC_FILES) $(shell find $(SRC_DIR) -name '*.h') Makefile ./back_up

# .PHONY so that the rules work even if a file with the same target-name exists.
.PHONY: all clean cpy-src $(BUILD_DIR)

# Include the dependencies
-include $(DEP_FILES)
