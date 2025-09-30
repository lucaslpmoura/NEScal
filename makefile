CC = gcc
CFLAGS = -Wall -Wextra -O2  -I./src -I./src/emulator $(shell pkg-config --cflags gtk4)

SRC_DIR = src
EMU_DIR = src/emulator
GPH_DIR = src/graphics
BUILD_DIR = build

SRCS = $(SRC_DIR)/main.c \
       $(EMU_DIR)/CPU.c \
       $(EMU_DIR)/instructions.c \
       $(EMU_DIR)/PPU.c \
       $(EMU_DIR)/RAM.c \
       $(EMU_DIR)/ROM.c \
       $(EMU_DIR)/util.c \
       $(GPH_DIR)/window.c

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

TARGET = NEScal

all: $(TARGET)

# Linkagem final
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(shell pkg-config --libs gtk4) -no-pie

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
