CFLAGS  = -Wall -O2

TARGET  = Server Client 

all: $(TARGET)

Server: Server.c
	$(CC) $(CFLAGS) $< -o $@
	rm -rf *.o

Client: Client.c
	$(CC) $(CFLAGS) $< -o $@
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
