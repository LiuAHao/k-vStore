CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I../include -pthread

TARGET = kvstore_test

SRCS = test_kv_store.cpp ../src/KVStore.cpp ../src/LRUCache.cpp ../src/SlabAllocator.cpp ../src/TTLManager.cpp

OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)