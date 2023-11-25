BINARY=s_sh # Final binary executable
SRC_DIR := . lib # Source code directories
INCL_DIR := . include # Directories to search for include files
DEP_DIR := dependencies # Directory to place dependancy files
OBJ_DIR := obj # Directory to place obj files
CC = gcc # Compiler to use
OPT = -O0 # Otimisation flags
DEPFLAGS = -MP -MMD # Generate files that encode make rules for the .h dependencies
# Automatically adding the -I onto each include directory
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu89 -g $(foreach dir, $(INCL_DIR), -I$(dir)) $(DEPFLAGS)
# Appending appropriate directories for all the source files
CFILES := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
# Creating object and dependency files for each C source file in OBJ_DIR & DEP_DIR respectively
OBJ_FILES := $(patsubst $(foreach dir, $(SRC_DIR), $(dir))/%.c, $(OBJ_DIR)/%.o, $(CFILES))
DEP_FILES := $(patsubst $(foreach dir, $(OBJ_DIR), $(dir))/%.o, $(DEP_DIR)/%.d, $(CFILES))

all: $(BINARY)

$(BINARY): $(OBJ_FILES)
	$(CC) $^ -o $@

# Create folders if they don't exist
# @ silences the printing of the command
create-dirs:
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)

# only want the .c file dependency here, thus $< instead of $^.
%.o: %.c | create-dirs
	$(CC) $(CFLAGS) -c $< -o $@

# Removes all obj and dep files and their folders plus the binary file
clean:
	@rm -drf --preserve-root $(BINARY) $(OBJ_FILES) $(DEP_FILES) $(OBJ_DIR) $(DEP_DIR)

# shell commands are a set of keystrokes away
distribute: clean
	tar zcvf dist.tgz *

# $(info ...) prints output
diff:
	$(info Repository status and per-file changes:)
	git status
	@git diff --stat

# include the dependencies
-include $(DEP_FILES)

# add .PHONY so that the non-targetfile - rules work even if a file with the same name exists.
.PHONY: all clean distribute diff
