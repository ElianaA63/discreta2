CC = gcc
CFLAGS = -Wall -Wextra -Werror -O3 -std=c99 -g
SOURCES = main.c APIG24.c API2024Parte2.c
OBJECTS = $(SOURCES:.c=.o)
INCLUDES = -I.
TARGET = programa

# Verificar el sistema operativo
ifeq ($(OS),Windows_NT)
    RM = del *.o
	EXECUTABLE_EXTENSION = .exe
    RUN_COMMAND = .\$(TARGET)
else
    RM = rm -f
	EXECUTABLE_EXTENSION =
    RUN_COMMAND = ./$(TARGET)
endif

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJECTS)

run: $(TARGET)
	$(RUN_COMMAND)$(EXECUTABLE_EXTENSION) < $(word 2, $(MAKECMDGOALS))

clean:
	$(RM) $(OBJECTS) $(TARGET)$(EXECUTABLE_EXTENSION)
