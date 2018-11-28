CFLAGS  = -Wall -O2

TARGET  = IntervalHello

all: $(TARGET)

IntervalHello: IntervalHello.c
	$(CC) $(CFLAGS) $< -o $@
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
