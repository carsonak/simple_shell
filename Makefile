# Initialising global variables. No trailing spaces
# Final binary executable
BINARY := s_sh
# Main directory with all the source files
SRC_DIR := src
# Main directory for all the object files
BUILD_DIR := build
# Library source file directories
LIB_SRC_DIR := $(shell find . -mount -iname "lib*" -type d | sort)
# Compiled library directory
LIB_OBJS_DIR := $(LIB_SRC_DIR:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
# Directories with header files
INCL_DIRS := $(shell find . -mount -name '*.h' -type f -exec dirname {} \; | sort -u)

# Extracting source files
SRC := $(shell find "$(SRC_DIR)" -mount ! \( -iname "lib*" -type d \) -a \( -name '*.c' -type f \) | sort)
# Object files are stored in the builds directory
OBJ := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
# Header files
HEADERS := $(shell find $(INCL_DIRS) -name '*.h' -type f | sort)

# lib archive names
LIB_ARCV := $(foreach dir, $(LIB_OBJS_DIR), lib$(subst /,-,$(subst $(SRC_DIR),,$(dir))).a)
# Library source  files
LIB_SRC := $(foreach dir, $(LIB_SRC_DIR), $(shell find '$(dir)' -name '*.c' -type f | sort))
# Library object files
LBO_FILES := $(LIB_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
# Dependency files for make
DEP_FILES := $(LBO_FILES:.o=.d) $(OBJ:.o=.d)

# Linker flags
LDLIBS := -l$(LIB_NAME)
LDFLAGS := -L$(LIB_OBJS_DIR)
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
DBG_FLAGS := -fsanitize=undefined -fsanitize-undefined-trap-on-error -fstack-protector-all -g
CFLAGS := $(INCL_FLAGS) -std=c17 $(WARN_FLAGS) $(DBG_FLAGS) $(DEP_FLAGS)

# First rule that will be run by make on default
all : $(BINARY)

# Create build folder
$(LIB_OBJS_DIR) $(BUILD_DIR) :
	@mkdir -p $@

# $^ - all the prerequisites
# Making executable
$(BINARY) : $(LIB_ARCV) $(BUILD_DIR) $(OBJ)
	@$(CC) $(LDLIBS) $(LDFLAGS) $(OBJ) -o $@

# @< - Every item in prequisite is processed one by one
# $@ - the target
# Making object files and moving them to the obj dir
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@mv -u $(shell basename $< .c)?? $(dir $@)

# @ - silence printing of the command being run
# Updating members of the library
$(LIB_ARCV) : $(LIB_OBJS_DIR) $(LBO_FILES)
	@ar -Urcus '$@' $(LBO_FILES)

# Delete build folder
clean :
	@rm -rd --preserve-root '$(BUILD_DIR)'

# Clean Workspace folder
clean-wksp :
	@rm $(shell find '$(PWD)' -mount -maxdepth 1  -name '*.h' -o -name '*.c')

# Make a copy of all source codes, header files into a back_up folder
up-wksp :
	@cp -fu $(shell find '$(SRC_DIR)' -mount -name '*.c' -type f) $(HEADERS) $(PWD)

# Print out various files in the project
show :
	@printf "HEADER FILES\nDIR: %s\n%s\n\n" \
	'$(INCL_DIRS)' \
	'$(notdir $(HEADERS))'

	@printf "LIBRARY FILES\nDIR: %s\n%s\n\n" \
	'$(LIB_SRC_DIR)' \
	'$(notdir $(LIB_SRC))'

	@printf "SOURCE FILES\nDIR: %s\n%s\n\n" \
	'$(shell dirname $(SRC) | sort -u)' \
	'$(notdir $(SRC))'

	@printf "OBJECT FILES\nDIR: %s\n%s\n\n" \
	'$(shell find $(BUILD_DIR) -type d)' \
	'$(notdir $(shell find $(BUILD_DIR) -name '*.o' -type f))'

	@printf "LIBRARY ARCHIVE:\nNAME: %s\n%s\n\n" \
	'$(LIB_ARCV)' \
	'$(shell ar -t '$(LIB_ARCV)')'

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
