# tool macros
CC := g++
CFLAGS := -std=c++20 -Wall -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -Iollama-hpp/singleheader
DBGFLAGS := -g
COBJFLAGS := $(CFLAGS) -c

# path macros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src
DBG_PATH := debug
TST_PATH := tests

# compile macros
TARGET_NAME := incroyable
TARGET := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DEBUG := $(DBG_PATH)/$(TARGET_NAME)
TARGET_TEST := $(DBG_PATH)/tests

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
# test files
TST := $(foreach x, $(TST_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ_TEST := $(filter-out $(OBJ_PATH)/main.o, $(OBJ)) $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(TST)))))

# default rule
default: makedir all

$(TARGET_TEST): makedir $(OBJ_TEST)
	$(CC) -o $@ $(OBJ_TEST) $(CFLAGS)

# non-phony targets
$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(COBJFLAGS) -o $@ $<

$(DBG_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(COBJFLAGS) $(DBGFLAGS) -o $@ $<

$(OBJ_PATH)/%.o: $(TST_PATH)/%.c*
	$(CC) $(COBJFLAGS) -o $@ $<

$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CC) $(CFLAGS) $(DBGFLAGS) $(OBJ_DEBUG) -o $@

# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: test
test: $(TARGET_TEST)

.PHONY: debug
debug: $(TARGET_DEBUG)

.PHONY: clean
clean:
	@echo "Le gros ménage"
	rm -rf $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

.PHONY: run
run: default
	./$(TARGET)

.PHONY: testrun
testrun: test
	./$(TARGET_TEST)

.PHONY: uml
uml:
	clang-uml
	plantuml -tsvg docs/diagrams/*.puml
