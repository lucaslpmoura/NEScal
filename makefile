CC = gcc
CFLAGS = -Wall -Wextra -O2 -I./src -I./src/emulator


SRC_DIR = src
EMU_DIR = src/emulator
BUILD_DIR = build


SRCS = $(SRC_DIR)/main.c \
       $(EMU_DIR)/CPU.c \
       $(EMU_DIR)/instructions.c \
       $(EMU_DIR)/PPU.c \
       $(EMU_DIR)/RAM.c \
       $(EMU_DIR)/ROM.c \
       $(EMU_DIR)/util.c


OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)


TARGET = NEScal.exe


all: $(TARGET)

# Linkagem final
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^


$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
