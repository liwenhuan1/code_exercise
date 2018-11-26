CFLAGS  = -Wall -O2

TARGET  = Client1000 Client1000SinglePro

all: $(TARGET)

Client1000: Client1000.c
	$(CC) $(CFLAGS) $< -o $@
	rm -rf *.o

Client1000SinglePro: Client1000SinglePro.c
	$(CC) $(CFLAGS) $< -o $@
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
