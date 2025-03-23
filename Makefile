# 编译器和编译选项
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

# 目录
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# 依赖库
LDFLAGS = -lleveldb -lpthread

# 源文件和目标文件
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
TARGET = $(BIN_DIR)/kvstore

# 默认目标
all: directories $(TARGET)

# 创建必要的目录
directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BIN_DIR)

# 链接目标文件生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# 编译源文件生成目标文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# 清理生成的文件
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# 运行程序
run: all
	$(TARGET)

.PHONY: all directories clean run