# Define what compiler to use and the flags.
CXX=g++
CCFLAGS= -g -std=c++11 -Wall -Werror
LDLIBS= -lm

# Define your target
all: proj 

# Define how to compile .cpp files into .o files
%.o : %.cpp
	$(CXX) -c $(CCFLAGS) $<

# Collect all .o files into a variable
OBJ = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

# Link the object files into the executable
proj: $(OBJ)
	$(CXX) -o $@ $(OBJ) $(CCFLAGS) $(LDLIBS)

# Define a rule to clean up the build
clean:
	rm -f core $(OBJ) proj


