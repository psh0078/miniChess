CC = gcc
TARGET = bitboard
SRC = bitboard.c movegen.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean

