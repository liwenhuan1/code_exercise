CFLAGS  = -Wall -O2

TARGET  = NetMessageGet

all: $(TARGET)

NetMessageGet: NetMessageGet.c
	$(CC) $(CFLAGS) $< -o $@
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
