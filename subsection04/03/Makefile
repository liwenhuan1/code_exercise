CFLAGS  = -Wall -O2

TARGET  = Client1000

all: $(TARGET)

Client1000: Client1000.c
	$(CC) $(CFLAGS) $< -o $@
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
