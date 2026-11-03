CXX = g++
CXXFLAGS = -Wall -Wextra
LDLIBS = -lglfw -lGL -lm

TARGET = esfera
OBJECTS = main.o esfera.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

main.o: main.cpp esfera.h
esfera.o: esfera.cpp esfera.h

clean:
	rm -f $(TARGET) $(OBJECTS)
