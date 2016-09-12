# Project: MiniCSV

CPP      = clang++ -stdlib=libstdc++ -lstdc++ -std=c++11
#CPP      = g++ -lstdc++ -std=c++11
CC       = gcc
OBJ      = BenchmarkFloatConv.o $(RES)
LINKOBJ  = BenchmarkFloatConv.o $(RES)
BIN      = BenchmarkFloatConv
CXXFLAGS = -Wall -g -O1 
CFLAGS   = -Wall -g -O1  
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before BenchmarkFloatConv all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "BenchmarkFloatConv" $(LIBS)

BenchmarkFloatConv.o: BenchmarkFloatConv.cpp
	$(CPP) -c BenchmarkFloatConv.cpp -o BenchmarkFloatConv.o $(CXXFLAGS)
