CFLAGS  = -Wall -O2

TARGET  = intervalHello

all: $(TARGET)

intervalHello: intervalHello.c
	$(CC) $(CFLAGS) $< -o $@
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
