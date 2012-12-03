# Source files
SRC = $(wildcard Formula/Atomic/*.cpp Formula/Compound/*.cpp Formula/Containers/*.cpp Formula/Containers/Sets/*.cpp Input/*.cpp Algorithm/*.cpp)

OBJ = $(SRC:.cpp=.o)

# Library path
OUT = ./libproof.a

# Compiler flag for C++11
CCFLAGS = -std=c++0x

CCC = g++

.SUFFIXES: .cpp

.cpp.o:
	$(CCC) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)

depend: $(OUT)
	makedepend -- $(CCFLAGS) $(SRC)

clean:
	rm -f $(OBJ) Makefile.bak 
