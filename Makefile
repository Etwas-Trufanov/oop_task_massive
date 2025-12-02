# Компилятор
CXX := g++

# Флаги компилятора
CXXFLAGS := -std=c++11 -g -Wall -Wextra

# Директории
SRC_DIR := src
BUILD_DIR := build
TARGET := $(BUILD_DIR)/out

# Исходные файлы
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Целевая задача
all: $(TARGET)

# Правило для сборки исполняемого файла
$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Правило для сборки объектных файлов
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Целевая задача для очистки
clean:
	rm -rf $(BUILD_DIR)
	@echo "Директория $(BUILD_DIR) очищена, но папка .zed осталась нетронутой"

.PHONY: all clean
