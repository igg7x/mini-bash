CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = mini_bash

# Esto busca automáticamente todos los .c en src/
SRC = src/mini.c src/utils.c
# Convierte la lista de .c en .o
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Regla genérica: para cada .o, busca su .c correspondiente
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all run clean