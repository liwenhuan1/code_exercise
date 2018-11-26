CFLAGS  = -Wall -O2

TARGET  = userGetTime

all: $(TARGET)

userGetTime: userGetTime.c
	$(CC) $(CFLAGS) $< -o $@
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
