CFLAGS  = -Wall -O2

TARGET  = SortLocale

all: $(TARGET)

SortLocale: SortLocale.c
	$(CC) $(CFLAGS) $< -o $@ 
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
