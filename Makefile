# the compiler to use
CPP = clang++
CPPFLAGS = -std=c++11

# files to link:
LFLAGS =  -lGL -lGLU -lglut

#files to include
INCL = -I includes

# the name to use for the target source file and the output file
TARGET = main
OBJ = mesh
SRC = $(wildcard src/*.cpp )

all: src/$(TARGET).cpp
	$(CPP) -o $(OBJ) $(SRC) $(CPPFLAGS) $(INCL) $(LFLAGS)

clean:
	rm cube
