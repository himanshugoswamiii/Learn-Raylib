CC = gcc
CFLAGS = -Wall
LDFLAGS = -lraylib

$(TARGET) : $(TARGET).c
	@echo "Building $(TARGET)"
	$(CC) -o $(TARGET) $(TARGET).c $(CFLAGS) $(LDFLAGS)
