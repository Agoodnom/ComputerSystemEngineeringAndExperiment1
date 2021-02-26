CC = g++
CFLAGS = -W -g
TARGET = main

OBJECTS = main.o

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

.PHONY: clean
clean:
	rm $(OBJECTS) $(TARGET)
