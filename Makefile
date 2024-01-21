#!/usr/bin/make

# Final binary executable
BINARY := $(PWD)/s_sh
# Current Working directory
SRC_DIR := $(PWD)/src
# Library directories
LIB_DIR := $(shell ls -d $(shell find '$(PWD)' -mount -name 'lib*' -type d))
# Directory to place object files
BUILD_DIR := ./build

# Directories to search fo header files
INCL_DIRS := $(shell ls -d $(shell find '$(SRC_DIR)' -mount -name 'head*' -type d))
# Include flags for all directories
INCL_FLAGS := $(addprefix -I,'$(INCL_DIRS)')
# Header files
HDR_FILES := $(foreach dir,'$(INCL_DIRS)',$(shell find '$(dir)' -mount -name '*.h' -type f))

# All the source files
SRC_FILES := $(notdir $(shell find '$(SRC_DIR)' -mount -name '*.c' -type f))
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

CFLAGS := $(INCL_FLAGS) -std=c17 $(WARN_FLAGS) $(F_FLAGS) $(DEP_FLAGS)

# First rule that will be run by make on default
all : $(BINARY)

# Create folder if it don't exist
$(BUILD_DIR) :
	@mkdir -p '$@'

# Rule for compiling a final executable file
# $@ - the target. $^ - all the prerequisites
$(BINARY) : $(OBJ_FILES)
	@$(CC) '$<' -o '$@'

# Redifing implicit rule for making object files
# $< - only the first prerequisite
$(OBJ_FILES) : $(SRC_FILES) $(BUILD_DIR)
	@$(CC) '$(CFLAGS)' -c '$<' -o '$@'

# Removes all obj and dep files and their folders plus the binary file
# @ silences the printing of the command
clean :
	@rm -rd --preserve-root $(BUILD_DIR)
	@rm $(shell find '$(PWD)' -mount ! \( -path '$(SRC_DIR)' -prune \) -a \( -name '*.h' -o -name '*.c' \))

# Make a copy of all source codes, header files into a back_up folder
up_wksp :
	@cp -fu $(shell find '$(SRC_DIR)' -mount -name '*.c' -type f) $(HDR_FILES) $(PWD)

# .PHONY so that the rules work even if a file with the same target-name exists.
.PHONY : all clean upt_wksp $(BUILD_DIR)

show :
	$(foreach v,$(filter-out $(VARS_OLD) VARS_OLD,$(.VARIABLES)), $(info $(v) = $($(v))))

# Include the dependencies
-include $(DEP_FILES)
