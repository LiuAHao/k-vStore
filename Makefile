CXX = g++
CXXFLAGS = -std=c++17 -pthread -I/usr/local/include -I/usr/include/jsoncpp \
           -I. -I$(PROTO_DIR) \
           `pkg-config --cflags protobuf grpc`
LDFLAGS = -L/usr/local/lib -lleveldb -ljsoncpp \
          `pkg-config --libs protobuf grpc++`

PROTO_DIR = protos
PROTO_SRCS = $(PROTO_DIR)/kvstore.pb.cc
PROTO_HDRS = $(PROTO_DIR)/kvstore.pb.h
GRPC_SRCS = $(PROTO_DIR)/kvstore.grpc.pb.cc
GRPC_HDRS = $(PROTO_DIR)/kvstore.grpc.pb.h

SRCS = src/main.cpp \
       src/KvServer.cpp \
       src/KvClient.cpp \
       src/RaftNode.cpp \
       src/PersistentStorage.cpp \
       src/KvStateMachine.cpp \
       src/LogEntry.cpp \
       src/ClientRequest.cpp \
       src/ClientResponse.cpp \
       $(PROTO_SRCS) \
       $(GRPC_SRCS)

TARGET = kvstore

.PHONY: all clean proto
all: $(TARGET)

proto: $(PROTO_DIR)/kvstore.proto
	protoc -I$(PROTO_DIR) --cpp_out=$(PROTO_DIR) $(PROTO_DIR)/kvstore.proto
	protoc -I$(PROTO_DIR) --grpc_out=$(PROTO_DIR) --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` $(PROTO_DIR)/kvstore.proto

$(TARGET): proto $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET) $(PROTO_SRCS) $(PROTO_HDRS) $(GRPC_SRCS) $(GRPC_HDRS)