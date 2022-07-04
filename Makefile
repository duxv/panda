SRC_DIR := ./src
SRC_FILES := $(shell find $(SRC_DIR) -name "*.cpp")
SRC_DIRS := $(shell find $(SRC_DIR) -type d)
OBJ_DIR := ./.obj
OBJ_DIRS := $(OBJ_DIR)  # first obj dir has to be the main one
OBJ_DIRS += $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC_DIRS))
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
TARGET := main
CC := clang++
CPPFLAGS := -Wall -pedantic -std=c++20

all: $(OBJ_DIRS) $(TARGET)

$(TARGET): $(OBJ_FILES)
	@echo $@
	@$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo $^
	@$(CC) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIRS):
	@mkdir -p $@

.PHONY: clean

clean:
	rm -rf $(OBJ_FILES) $(TARGET)
