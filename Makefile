#!/usr/bin/make

# Final binary executable
BINARY := ./s_sh
SRC_DIR := ./src_sh
# Library directories
LIB_DIR := lib/
# Directory to place object files
BUILD_DIR := ./build

# Directories to search fo header files
INCL_DIRS := $(shell find $(SRC_DIR) -type d)
# Include flags for all directories
INCL_FLAGS := $(addprefix -I,$(INCL_DIRS))

# All the source files
SRC_FILES := $(notdir $(shell find $(SRC_DIR) -name '*.c'))
# Object files stored in ./build
OBJ_FILES := $(SRC_FILES:%.c=$(BUILD_DIR)/%.o)
# Dependency files for make
DEP_FILES := $(OBJ_FILES:.o=.d)

# Compiler
CC := gcc
# Optimisation flags
OPT_FLAGS := -O3
# Generate files that encode make rules for the .h dependencies
DEP_FLAGS := -MP -MMD
# Warnings
WRN_FLAGS := -pedantic -Wall -Werror -Wextra
# Debugging flags
F_FLAGS := -fdiagnostics-color=always -fsanitize=undefined -fsanitize-undefined-trap-on-error -fstack-protector-all -g
# Libraries to link
LNK_FLAGS := -lm -lrt

CFLAGS := $(INCL_FLAGS) -std=c17 $(WARN_FLAGS) $(F_FLAGS) $(DEP_FLAGS)

# First rule that will be run by make on default
all : $(BINARY)

# Create folder if it don't exist
$(BUILD_DIR) :
	@mkdir -p $@

# Rule for compiling a final executable file
# $@ - the target. $^ - the prerequisites
$(BINARY) : $(OBJ_FILES)
	@$(CC) $^ -o $@ $(LNK_FLAGS)

# Redifing implicit rule for making object files
# $< - only the first prerequisite
$(OBJ_FILES) : $(SRC_FILES) $(BUILD_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# Removes all obj and dep files and their folders plus the binary file
# @ silences the printing of the command
clean :
	@rm -rd --preserve-root $(BUILD_DIR)

# Make a copy of all source codes, header files and Makefile into a back_up folder
cpy-src :
	@cp -fu $(SRC_FILES) $(shell find $(SRC_DIR) -name '*.h') Makefile ./back_up

# .PHONY so that the rules work even if a file with the same target-name exists.
.PHONY : all clean cpy-src $(BUILD_DIR)

# Include the dependencies
-include $(DEP_FILES)
