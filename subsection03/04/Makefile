CFLAGS  = -Wall -O2

TARGET  = TestMqueue

all: $(TARGET)

TestMqueue: TestMqueue.c
	$(CC) $(CFLAGS) $< -o $@ -lrt
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
