CFLAGS  = -Wall -O2

TARGET  = UserGetTime

all: $(TARGET)

UserGetTime: UserGetTime.c
	$(CC) $(CFLAGS) $< -o $@
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
