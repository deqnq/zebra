PKG_MNG 	= dnf
CC	 		= gcc
CFLAGS 		= -Iinclude `pkg-config --cflags gtk+-3.0 vte-2.91`
LDFLAGS 	= `pkg-config --libs gtk+-3.0 vte-2.91`
SRC_DIR 	= zebra/
BUILD		= build/
TARGET		= $(BUILD)zebra
SRC			= $(SRC_DIR)*

all: $(TARGET) setup dependencies run clean

setup:
	mkdir -p $(BUILD)

dependencies: 
	sudo $(PKG_MNG) install gtk3-devel vte291-devel

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: setup $(TARGET)
	$(TARGET)

.PHONY: all dependencies clean run
