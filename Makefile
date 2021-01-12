
CXX ?= g++
CPPFLAGS ?= -pipe -O2 -Wall -Wextra -fPIC -std=c++14

OK_STRING    = "[OK]"
ERROR_STRING = "[ERROR]"
WARN_STRING  = "[WARNING]"
COM_STRING   = "Compiling"

SRC_DIR    := src
HEADER_DIR := include
BIN_DIR    := bin
OBJ_DIR    := $(BIN_DIR)/obj
MAIN       := $(SRC_DIR)/main.cc
SOURCES    := $(filter-out $(SRC_DIR)/main.cc, $(wildcard $(SRC_DIR)/*.cc))
OBJECTS    := $(SOURCES:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)
EXECUTABLE := $(BIN_DIR)/hmacsha256

clean: COUNT=$(words $(wildcard $(OBJ_DIR)/*.o))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(MAIN)
	@script -q -e -c '$(CXX) -I$(HEADER_DIR) -o $@ $(OBJECTS) $(MAIN)' $@.log > /dev/null; \
	RESULT=$$?; \
	sed -i '1d;$$d' $@.log; \
	sed -i '/^[[:space:]]*$$/d' $@.log; \
	if [ $$RESULT -ne 0 ]; then \
		printf "%-60b%b" "Building $@" "$(ERROR_STRING)\n"; \
	elif [ -s $@.log ]; then \
		printf "%-60b%b" "Building $@" "$(WARN_STRING)\n"; \
	else  \
		printf "%-60b%b" "Building $@" "$(OK_STRING)\n"; \
		printf "%b" "Build success\n"; \
	fi; \
	cat $@.log; \
	rm -f $@.log; \

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cc
	@script -q -e -c '$(CXX) $(CPPFLAGS) -c -I$(HEADER_DIR) $< -o $@' $@.log > /dev/null; \
	RESULT=$$?; \
	sed -i '1d;$$d' $@.log; \
	sed -i '/^[[:space:]]*$$/d' $@.log; \
	if [ $$RESULT -ne 0 ]; then \
		printf "%-60b%b" "$(COM_STRING) $@" "$(ERROR_STRING)\n"; \
	elif [ -s $@.log ]; then \
		printf "%-60b%b" "$(COM_STRING) $@" "$(WARN_STRING)\n"; \
	else  \
		printf "%-60b%b" "$(COM_STRING) $@" "$(OK_STRING)\n"; \
	fi; \
	cat $@.log; \
	rm -f $@.log; \

clean:
	@if [ $(COUNT) -ne 0 ] || [ -f  $(EXECUTABLE) ]; then \
	if [ $(COUNT) -ne 0 ]; then \
		printf "%b" "Cleaning objects\n"; \
		rm -f $(OBJ_DIR)/*.o; \
	fi; \
	if [ -f  $(EXECUTABLE) ]; then \
		printf "%b" "Removing executable\n"; \
		rm -f  $(EXECUTABLE); \
	fi; \
	printf "%b" "Clean complete\n"; \
	else \
	printf "%b" "Nothing to clean\n"; \
	fi;
