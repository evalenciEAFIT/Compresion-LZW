# Makefile para compilar el programa LZW en C++

CXX = g++
CXXFLAGS = -std=c++17 -Wall
TARGET = lzw_program
OBJ = main.o LZWCompressor.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

main.o: main.cpp LZWCompressor.h
	$(CXX) $(CXXFLAGS) -c main.cpp

LZWCompressor.o: LZWCompressor.cpp LZWCompressor.h
	$(CXX) $(CXXFLAGS) -c LZWCompressor.cpp

clean:
	rm -f $(TARGET) $(OBJ)
