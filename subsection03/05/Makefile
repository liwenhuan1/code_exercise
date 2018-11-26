CFLAGS  = -Wall -O2

TARGET  = selfIncrease

all: $(TARGET)

selfIncrease: selfIncrease.c
	$(CC) $(CFLAGS) $< -o $@ -lpthread
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
