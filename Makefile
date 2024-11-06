CC = gcc
CFLAGS = -Wall -Wextra -O2

SRC_DIR = src
TEST_DIR = test
TOOLS_DIR = tools

TEST_SRC_FILES = $(SRC_DIR)/second_preim_48.c $(TOOLS_DIR)/tools.c
TEST_OBJ_FILES = $(TEST_SRC_FILES:.c=.o)
TEST_FILES = $(TEST_DIR)/test_speck48_96.c

all: main

main: $(SRC_DIR)/attack.o $(TEST_OBJ_FILES)
	$(CC) $(SRC_DIR)/attack.o -o main $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TOOLS_DIR)/%.o: $(TOOLS_DIR)/%.c $(TOOLS_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_OBJ_FILES) $(TEST_FILES)
	$(CC) $(CFLAGS) $(TEST_FILES) $(TEST_OBJ_FILES) -o test_exec $(LDFLAGS)
	./test_exec

clean:
	rm -f $(SRC_DIR)/*.o $(TOOLS_DIR)/*.o main test_exec

.PHONY: all clean test
