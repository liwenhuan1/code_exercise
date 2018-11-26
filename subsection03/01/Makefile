CFLAGS  = -Wall -O2

TARGET  = fileStat

all: $(TARGET)

fileStat: fileStat.c
	$(CC) $(CFLAGS) $< -o $@ 
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
