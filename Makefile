CC = gcc
CFLAGS = -Iinclude
SRC_DIR = zebra/
BUILD = build/
TARGET = $(BUILD)zebra
SRC = $(SRC_DIR)main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
