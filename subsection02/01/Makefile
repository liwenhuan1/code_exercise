CFLAGS  = -Wall -O2

TARGET  = SortInput

all: $(TARGET)

SortInput: SortInput.c
	$(CC) $(CFLAGS) $< -o $@ 
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
