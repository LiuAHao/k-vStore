CXX = g++
# 添加protobuf头文件路径
CXXFLAGS = -std=c++17 -Wall -Wextra -I. -I/usr/local/include -pthread
# 添加protobuf库路径
LDFLAGS = -L/usr/local/lib -lgrpc++ -lgrpc -lprotobuf -lpthread -lgrpc++_reflection

TARGET = kv_server

SRCS = src/KVServer.cpp ../kvstore.pb.cc ../kvstore.grpc.pb.cc
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)