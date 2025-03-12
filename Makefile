#!/usr/bin/make -f
PROJECT_NAME := simple_shell
# Compiler
CC := clang
# Directory for source files
SRC_DIR := .
# Directory for object files
OBJ_DIR := obj
# Directories with header files
INCL_DIRS := $(shell find . -mount -maxdepth 1 -name '*.h' -type f -exec dirname '{}' \;)

# All source files
SRCS := $(shell find $(SRC_DIR) -mount -maxdepth 1 -name '*.c' -type f)
# Object files are stored in the builds directory
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# Dependency files for make
# https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html#index-M
DEP_FILES = $(OBJS:.o=.d)

# Include flags
INCL_FLAGS = $(addprefix -iquote,$(INCL_DIRS))
OPTIMISATION_FLAGS := -O0
# https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html
# https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html#index-MMD
CPPFLAGS := -MMD
# Warnings
C_STANDARD := --std=gnu89
WARN_FLAGS = $(C_STANDARD) -pedantic -Wall -Werror -Wextra
SANITISERS := -fsanitize=undefined -fsanitize=address -fsanitize=leak
# Debugging flags
DEBUG_FLAGS := -g3 -fno-omit-frame-pointer

CFLAGS = $(CPPFLAGS) $(INCL_FLAGS) $(WARN_FLAGS) $(SANITISERS) $(OPTIMISATION_FLAGS) $(DEBUG_FLAGS)

# First rule that will be run by make on default
debug: $(PROJECT_NAME)

release: CPPFLAGS += -DNDEBUG
release: DEBUG_FLAGS :=
release: C_STANDARD := --std=c99
release: SANITISERS :=
release: OPTIMISATION_FLAGS := -O2
release: $(PROJECT_NAME)

# $@ - the target
$(OBJ_DIR):
	@mkdir -vp $@

# $^ - all the prerequisites
$(PROJECT_NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# https://www.gnu.org/software/make/manual/html_node/Prerequisite-Types.html
# $< - name of only the first prequisite
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	@$(RM) -vrd --preserve-root $(OBJ_DIR) $(PROJECT_NAME)

# Include the dependencies
-include $(DEP_FILES)
