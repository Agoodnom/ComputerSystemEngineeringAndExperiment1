CC = g++
CFLAGS = -W -g
TARGET = main

OBJECTS = str.o main.o

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

.PHONY: clean
clean:
	rm $(OBJECTS) $(TARGET)
