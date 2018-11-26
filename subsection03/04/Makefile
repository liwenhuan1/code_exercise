CFLAGS  = -Wall -O2

TARGET  = testMqueue

all: $(TARGET)

testMqueue: testMqueue.c
	$(CC) $(CFLAGS) $< -o $@ -lrt
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
