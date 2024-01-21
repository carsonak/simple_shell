#!/usr/bin/make -f

# Final binary executable
BINARY := $(PWD)/s_sh
# Directory with source files
SRC_DIR := $(PWD)/src
# Library directories
LIB_DIR := $(shell ls -d $(shell find '$(PWD)' -mount -name 'lib*' -type d))
# Directory to place object files
BUILD_DIR := ./build

# Directories with header files
INCL_DIRS := $(shell dirname $(shell find '$(PWD)' -type f -name '*.h') | sort -u)
# Include flags
INCL_FLAGS := $(addprefix -I,'$(INCL_DIRS)')
# Header files
HDR_FILES := $(foreach dir,'$(INCL_DIRS)',$(shell find '$(dir)' -mount -name '*.h' -type f))

# Source files
SRC_FILES := $(shell find '$(SRC_DIR)' -mount -name '*.c' -type f | sort)
# Object files are stored in ./build
OBJ_FILES := $(foreach obj_file,$(SRC_FILES:%.c=%.o),$(BUILD_DIR)/$(notdir $(obj_file)))
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

# Making executable
# Create build folder if it don't exist
$(BUILD_DIR) :
	@mkdir -p '$@'

# $@ - the target. $^ - all the prerequisites
$(BINARY) : $(OBJ_FILES)
	@$(CC) '$<' -o '$@'

# Making object files and moving them to obj dir
# $< - only the first prerequisite
$(OBJ_FILES) : $(SRC_FILES) $(BUILD_DIR)
	@$(CC) '$(CFLAGS)' -c '$<' -o '$@'

# Delete build folder
# @ - silence printing of the command
clean :
	@rm -rd --preserve-root '$(BUILD_DIR)'

# Clean Workspace folder
clean-wksp :
	@rm $(shell find '$(PWD)' -mount ! \( -path '$(SRC_DIR)' -prune \) -a \( -name '*.h' -o -name '*.c' \))

# Make a copy of all source codes, header files into a back_up folder
up-wksp :
	@cp -fu $(shell find '$(SRC_DIR)' -mount -name '*.c' -type f) $(HDR_FILES) $(PWD)

# Print out header files, source files and object files
show :
	@printf "HEADER FILES\nDIR: %s\n%s\n\n" \
	'$(INCL_DIRS)' \
	'$(notdir $(HDR_FILES))'

	@printf "SOURCE FILES\nDIR: %s\n%s\n\n" \
	'$(shell dirname $(shell find '$(PWD)' -type f -name '*.c') | sort -u)' \
	'$(notdir $(SRC_FILES))'

	@printf "OBJECT FILES\nDIR: %s\n%s\n" \
	'$(shell dirname $(shell find '$(PWD)' -type f -name '*.o') | sort -u)' \
	'$(notdir $(OBJ_FILES))'

# Print out all global variables
all-vars :
	$(foreach V,$(sort $(.VARIABLES)),\
		$(if \
			$(filter-out environment% default automatic, $(origin $V)),\
					$(warning $V=$($V) ($(value $V)))\
		)\
	)

# .PHONY so that the rules work even if a file with the same target-name exists.
.PHONY : all clean clean-wksp upt-wksp show all-vars $(BUILD_DIR)

# Include the dependencies
-include $(DEP_FILES)
