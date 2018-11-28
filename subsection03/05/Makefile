CFLAGS  = -Wall -O2

TARGET  = SelfIncC SelfIncCXX

all: $(TARGET)

SelfIncC: SelfInc.c
	$(CC) $(CFLAGS) $< -o $@ -lpthread
	rm -rf *.o

SelfIncCXX: SelfInc.cc
	$(CXX) $(CXXFLAGS) $< -o $@ -std=c++11 -lpthread
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)

.PHONY: $(PHONY) clean
