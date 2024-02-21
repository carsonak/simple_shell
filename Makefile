#!/usr/bin/env make

# Initialising global variables. No trailing spaces
# Final binary executable
BINARY := $(PWD)/s_sh
# Directory with source files
SRC_DIR := $(PWD)/src
# Directory to place object files
BUILD_DIR := ./build
# Compiled library directory
LIBC_DIR := $(BUILD_DIR)/$(LIB_NAME)_lib
# Library directories
LIB_SRC_DIR := $(shell ls -d $(shell find '$(SRC_DIR)' -mount -iname 'lib*' -type d) | sort -u)
# Directories with header files
INCL_DIRS := $(shell dirname $(shell find '$(PWD)' -mount -name '*.h' -type f ) | sort -u)

# Other source files
SRC_FILES := $(shell find '$(SRC_DIR)' -mount ! \( -path '$(LIB_SRC_DIR)' -prune \) -a \( -name '*.c' -type f \) | sort)
# Object files are stored in ./build
OBJ_FILES := $(foreach obj_file,$(SRC_FILES:%.c=%.o),$(BUILD_DIR)/$(notdir $(obj_file)))
# Header files
HDR_FILES := $(foreach dir, $(INCL_DIRS), $(shell find '$(dir)' -maxdepth 1 -name '*.h' -type f))

# Common Library name
LIB_NAME := common
FULL_LIB_NM := lib$(LIB_NAME).a
# Library source  files
LIB_SRC_FILES := $(foreach dir, $(LIB_SRC_DIR), $(shell find '$(dir)' -maxdepth 1 -name '*.c' -type f | sort))
# Library object files
LBO_FILES := $(foreach lib_objf,$(LIB_SRC_FILES:%.c=%.o),$(LIBC_DIR)/$(notdir $(lib_objf)))
# Dependency files for make
DEP_FILES := $(LBO_FILES:.o=.d) $(OBJ_FILES:.o=.d)

# Linker flags
LDIRS := -L$(LIBC_DIR)
LNAMES := -l$(LIB_NAME)
# Include flags
INCL_FLAGS := $(addprefix -I,'$(INCL_DIRS)')

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

# Create build folder
$(LIBC_DIR) $(BUILD_DIR) :
	@mkdir -p $(LIBC_DIR)

# Making executable
# $^ - all the prerequisites
$(BINARY) : $(OBJ_FILES) $(LIBC_DIR)/$(FULL_LIB_NM)
	@$(CC) $(LDIRS) $(LNAMES) $(OBJ_FILES) -o $@

# Making object files and moving them to obj dir
# $< - only the first prerequisite
$(OBJ_FILES) : $(SRC_FILES) $(BUILD_DIR)
	@$(CC) $(CFLAGS) -c $(SRC_FILES) -o $@

# $@ - the target
$(LBO_FILES) : $(LIB_SRC_FILES) $(LIBC_DIR)
	@$(CC) $(CFLAGS) -c $(LIB_SRC_FILES) -o $@

# Updating members of the library
# @ - silence printing of the command
$(LIBC_DIR)/$(FULL_LIB_NM) : $(LBO_FILES)
	@ar -Urcus '$@' $(LBO_FILES)

# Delete build folder
clean :
	@rm -rd --preserve-root '$(BUILD_DIR)'

# Clean Workspace folder
clean-wksp :
	@rm $(shell find '$(PWD)' -mount -maxdepth 1  -name '*.h' -o -name '*.c')

# Make a copy of all source codes, header files into a back_up folder
up-wksp :
	@cp -fu $(shell find '$(SRC_DIR)' -mount -name '*.c' -type f) $(HDR_FILES) $(PWD)

# Print out various files in the project
show :
	@printf "HEADER FILES\nDIR: %s\n%s\n\n" \
	'$(INCL_DIRS)' \
	'$(notdir $(HDR_FILES))'

	@printf "LIBRARY FILES\nDIR: %s\n%s\n\n" \
	'$(LIB_SRC_DIR)' \
	'$(notdir $(LIB_SRC_FILES))'

	@printf "SOURCE FILES\nDIR: %s\n%s\n\n" \
	'$(shell dirname $(SRC_FILES) | sort -u)' \
	'$(notdir $(SRC_FILES))'

	@printf "OBJECT FILES\nDIR: %s\n%s\n\n" \
	'$(shell find $(BUILD_DIR) -type d)' \
	'$(notdir $(shell find $(BUILD_DIR) -name '*.o' -type f))'

	@printf "LIBRARY ARCHIVE:\nNAME: %s\n%s\n\n" \
	'$(LIBC_DIR)/$(FULL_LIB_NM)' \
	'$(shell ar -t '$(LIBC_DIR)/$(FULL_LIB_NM)')'

	@printf "DEPENDENCIES\nDIR: %s\n%s\n\n" \
	'$(BUILD_DIR)' \
	'$(notdir $(shell find $(BUILD_DIR) -name '*.d' -type f))'

# Print out all user set global variables
all-vars :
	$(foreach V,$(sort $(.VARIABLES)),\
		$(if \
			$(filter-out environment% default automatic, \
				$(origin $V)),\
				$(info $V = $(value $V))\
		)\
	)

# .PHONY so that the rules work even if a file with the same target-name exists.
.PHONY : all clean clean-wksp upt-wksp show all-vars

# Include the dependencies
-include $(DEP_FILES)
