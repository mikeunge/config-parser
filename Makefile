CC            = clang
BUILD_FLAGS   = -std=c99 -Wall -DDEBUG -g
SRC           = main.c
TARGET        = parser
BUILD_PATH    = bin
BINS          = $(BUILD_PATH)/$(TARGET)


all: clean build

build:
	mkdir -p $(BUILD_PATH)
	$(CC) $(BUILD_FLAGS) -o $(BINS) $(SRC)

clean:
	rm -rf $(BUILD_PATH)

