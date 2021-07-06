space := $(subst ,, )
PROJECT_NAME := $(notdir $(subst $(space),_,$(CURDIR)))

CXX = g++
CC = $(CXX)
INCLUDE_DIRS = -I include
SRC = src
BIN = bin
ifeq ($(OS),Windows_NT)
	INCLUDE_DIRS := $(INCLUDE_DIRS) -I freeglut/include
	LIB_FLAGS = -L freeglut\lib\x64 -lfreeglut -lopengl32 -lglu32
	OBJ = obj/Windows
	EXE = $(BIN)/$(PROJECT_NAME).exe
else
	LIB_FLAGS = -lGL -lGLU -lglut -ldl
	OBJ = obj/Linux
	EXE = $(BIN)/$(PROJECT_NAME)
endif


CXX_FLAGS = -std=c++17 -g $(INCLUDE_DIRS)

CPPSOURCEFILES = $(wildcard $(SRC)/*.cpp)
SOURCEFILES = $(notdir $(CPPSOURCEFILES))
OBJECTFILES = $(CPPSOURCEFILES:%.cpp=$(OBJ)/%.o)
DEP = $(OBJECTFILES:%.o=%.d)

# all2:
# 	echo $(CXX_FLAGS)

all: $(EXE)

$(EXE): $(OBJECTFILES)
	mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) -c $(SRC)/glad.c -o $(OBJ)/glad.o $(LIB_FLAGS)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJ)/glad.o $^ $(LIB_FLAGS)

-include $(DEP)

$(OBJ)/%.o : %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) -MMD -c $< -o $@

.PHONY : clean all .FORCE
clean :
	# This should remove all generated files.
	-rm $(EXE) $(OBJECTFILES) $(DEP) $(OBJ)/glad.o






# BIN = mybin
# # Put all auto generated stuff to this build dir.
# BUILD_DIR = ./build

# # List of all .cpp source files.
# CPP = main.cpp $(wildcard dir1/*.cpp) $(wildcard dir2/*.cpp)

# # All .o files go to build dir.
# OBJ = $(CPP:%.cpp=$(BUILD_DIR)/%.o)
# # Gcc/Clang will create these .d files containing dependencies.
# DEP = $(OBJ:%.o=%.d)

# # Default target named after the binary.
# $(BIN) : $(BUILD_DIR)/$(BIN)

# # Actual target of the binary - depends on all .o files.
# $(BUILD_DIR)/$(BIN) : $(OBJ)
#     # Create build directories - same structure as sources.
#     mkdir -p $(@D)
#     # Just link all the object files.
#     $(CXX) $(CXX_FLAGS) $^ -o $@

# # Include all .d files
# -include $(DEP)

# # Build target for every single object file.
# # The potential dependency on header files is covered
# # by calling `-include $(DEP)`.
# $(BUILD_DIR)/%.o : %.cpp
#     mkdir -p $(@D)
#     # The -MMD flags additionaly creates a .d file with
#     # the same name as the .o file.
#     $(CXX) $(CXX_FLAGS) -MMD -c $< -o $@

# .PHONY : clean
# clean :
#     # This should remove all generated files.
#     -rm $(BUILD_DIR)/$(BIN) $(OBJ) $(DEP)