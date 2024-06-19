# Compiler settings - Can change based on your needs
CXX = g++
CXXFLAGS = -Wall -O2 -std=c++11

# Linker flags
LDFLAGS = -lsndfile -lfftw3

# Source files
SOURCES = main.cpp
# Object files
OBJECTS = $(SOURCES:.cpp=.o)
# Executable name
EXECUTABLE = main

# Default target
all: $(EXECUTABLE)

# Linking the executable from the object files
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Compiling the source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Phony targets
.PHONY: clean
