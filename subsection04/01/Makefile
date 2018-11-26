CFLAGS  = -Wall -O2

TARGET  = netMessageGet

all: $(TARGET)

netMessageGet: netMessageGet.c
	$(CC) $(CFLAGS) $< -o $@
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
