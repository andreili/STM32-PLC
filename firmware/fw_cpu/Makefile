CPP=${CROSS_COMPILE}g++
GCC=${CROSS_COMPILE}gcc

CPPFLAGS=-std=c++11 -Wall -fexceptions -std=gnu++11 -O2 -DNDEBUG -DUNICODE
CFLAGS=-Wall -fexceptions -O2
LDFLAGS=

INCLUDES=\
	./inc

INCLUDE=$(addprefix -I,$(INCLUDES))

LIBS=pthread

LINKFLAGS=$(addprefix -l,$(LIBS))

SOURCE=main.cpp plc_control.cpp plc_io.cpp plc_state.cpp plc_mod.cpp \
	plc_platform.cpp plc_bus.cpp plc_fw.cpp plc_data_types.cpp \
	plc_hw.cpp stl_commands.cpp

OBJ_DIR=./out
SRC_DIR=./src

OBJECTS=$(patsubst %,$(OBJ_DIR)/%,$(SOURCE:%.cpp=%.o))

all: fw_cpu

fw_cpu: $(OBJ_DIR) $(OBJECTS)
	${CPP} -o $(OBJ_DIR)/fw_cpu $(LDFLAGS) $(OBJECTS) $(LINKFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	${CPP} ${CPPFLAGS} ${INCLUDE} -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJECTS) $(OBJ_DIR)/fw_cpu
