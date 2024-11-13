# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -I./tools -I./src

# Directories
SRC_DIR = src
TEST_DIR = test
TOOLS_DIR = tools
ROOT_DIR = .

# Source files and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(TOOLS_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:.c=.o)

# Test files and executables
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_EXEC = $(patsubst $(TEST_DIR)/%.c, $(ROOT_DIR)/%, $(TEST_FILES))

# Default target: build all test executables
all: $(TEST_EXEC)

# Rule to compile each test executable
$(TEST_EXEC): $(OBJ_FILES) $(TEST_FILES)
	@for test_file in $(TEST_FILES); do \
		test_exec=$(ROOT_DIR)/$$(basename $$test_file .c); \
		$(CC) $(CFLAGS) -o $$test_exec $(OBJ_FILES) $$test_file; \
	done

# Run all test executables
test: $(TEST_EXEC)
	@for exec in $(TEST_EXEC); do \
		echo "Running $$exec..."; \
		./$$exec; \
	done

# Clean up object files and test executables
clean:
	rm -f $(OBJ_FILES) $(TEST_EXEC)

.PHONY: all clean test
