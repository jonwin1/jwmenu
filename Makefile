CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -g
LDFLAGS =
LIBS =

TARGET = jwmenu

SRC = src/main.c src/menu.c src/config.c src/util.c lib/tomlc17.c
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Link object files to make the final executable
$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@ $(LIBS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -fv $(TARGET) $(OBJ)
