CXX = g++
OPT = -g -O0
RM = rm

# ソースとヘッダーファイルのディレクトリパスを設定
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# 各オブジェクトファイルのパスを指定
DEPENDENCIES = $(BUILD_DIR)/item.o $(BUILD_DIR)/bin.o $(BUILD_DIR)/knapsack.o $(BUILD_DIR)/gap.o $(BUILD_DIR)/test.o

# 実行ファイルの名前
TARGET = bin/gap

# 各ソースファイルとオブジェクトファイルの対応を指定
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.h
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OPT) -I$(INC_DIR) -c -o $@ $<

# デフォルトのターゲット
all: $(TARGET)

# 実行ファイルの生成ルール
$(TARGET): $(SRC_DIR)/main.cpp $(DEPENDENCIES)
	$(CXX) $(OPT) -I$(INC_DIR) -o $(TARGET) $(SRC_DIR)/main.cpp $(DEPENDENCIES)

# 依存関係をすべてビルド
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CXX) $(OPT) -I$(INC_DIR) -c -o $@ $<

# クリーンアップ
clean:
	$(RM) -rf $(BUILD_DIR) $(TARGET)

# 個別ファイルを削除するためのターゲット
clean-objects:
	$(RM) -f $(BUILD_DIR)/*.o
