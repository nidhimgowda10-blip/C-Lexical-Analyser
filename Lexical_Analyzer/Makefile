CC = gcc
CFLAGS = -Wall -Wextra -I include

TARGET = lexer

SRC = src/main.c src/lexer.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run:
	./$(TARGET) input/test.c

clean:
	rm -f $(TARGET)