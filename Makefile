BINARY=s_sh # Final binary executable
SRC_DIR := ./ lib/ # Source code directories
INCL_DIR := ./ include/ # Directories to search for include files
DEP_DIR := dependencies/ # Directory to place dependancy files
OBJ_DIR := obj/ # Directory to place obj files
CC = gcc # Compiler to use
OPT = -O0 # Otimisation flags
DEPFLAGS = -MP -MMD # Generate files that encode make rules for the .h dependencies
# Automatically adding the -I onto each include directory
CFLAGS = $(foreach dir, $(INCL_DIR), -I$(dir)) -Wall -Werror -Wextra -pedantic -std=gnu89 -g $(DEPFLAGS)
# Appending appropriate directories for all the source files
CFILES := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)*.c))
# Creating object and dependency files for each C source file in OBJ_DIR & DEP_DIR respectively
OBJ_FILES := $(patsubst $(dir $(CFILES))%.c, $(OBJ_DIR)%.o, $(CFILES))
DEP_FILES := $(patsubst $(dir $(OBJ_FILES))%.o, $(DEP_DIR)%.d, $(CFILES))

# Include the dependencies
-include $(DEP_FILES) $(INCL_DIR)

# First rule that will be run by make on default
all: create-dirs $(BINARY)

# Rule for compiling a final executable file
# $@ - the target. $^ - the prerequisites
$(BINARY): $(OBJ_FILES)
	$(CC) $^ -o $@

# Rule for creating folders if they don't exist
# @ silences the printing of the command
create-dirs:
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)

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
	@cp -fu $(CFILES) *.h Makefile ../

# .PHONY so that the rules work even if a file with the same target-name exists.
.PHONY: all clean distribute diff $(SRC_DIR) $(INCL_DIR)
