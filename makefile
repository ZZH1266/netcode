# 指定编译器
CXX = g++

# 编译选项
CXXFLAGS = -Wall -Wextra -O2

# 目标文件
TARGETS = out/server out/client

# 源文件目录
SRC_DIR = src

# 输出目录
OUT_DIR = out

# 默认目标
all: $(TARGETS) clean_objs

# 创建输出目录
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# server目标
out/server: server.o | $(OUT_DIR)
	$(CXX) $(CXXFLAGS) -o out/server server.o

server.o: $(SRC_DIR)/server.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/server.cpp -o server.o

# client目标
out/client: client.o | $(OUT_DIR)
	$(CXX) $(CXXFLAGS) -o out/client client.o

client.o: $(SRC_DIR)/client.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/client.cpp -o client.o

# 清理生成的文件
clean:
	rm -f *.o $(TARGETS)

# 伪目标，避免与实际文件名冲突
.PHONY: all clean clean_objs

# 删除中间.o文件
clean_objs:
	rm -f *.o