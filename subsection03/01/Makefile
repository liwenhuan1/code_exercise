CFLAGS  = -Wall -O2

TARGET  = FileStat

all: $(TARGET)

FileStat: FileStat.c
	$(CC) $(CFLAGS) $< -o $@ 
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
