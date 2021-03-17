CC=gcc

# If you wish, comment the last two flags
# However, the remainder is to always resolve the compiler warnings
CFLAGS=-g -Wall -Wextra -pedantic -D _GNU_SOURCE -Werror -O2

BUILD_DIR = ./build
SRC_DIR = ./src

SRCS := $(shell find $(SRC_DIR) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

all: xmod

xmod: $(OBJS)
	$(CC) $(CFLAGS) -lm $^ -o xmod

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf xmod $(BUILD_DIR)/* *.log

MKDIR_P = mkdir -p
