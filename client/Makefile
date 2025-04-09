CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I. -I/usr/local/include -pthread
LDFLAGS = -L/usr/local/lib -lgrpc++ -lgrpc -lprotobuf -lpthread -lgrpc++_reflection

TARGET = kv_client

SRCS = src/KVClient.cc ../kvstore.pb.cc ../kvstore.grpc.pb.cc
OBJS = $(SRCS:.cc=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)