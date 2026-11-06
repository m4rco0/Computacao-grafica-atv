CXX ?= g++
CPPFLAGS += -Iinclude
CXXFLAGS ?= -Wall -Wextra
LDLIBS += -lglfw -lGL -lm

TARGET := bin/esfera
SOURCES := src/main.cpp src/esfera.cpp src/tardis.cpp src/iluminacao.cpp src/sol.cpp
OBJECTS := $(patsubst src/%.cpp,obj/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

obj/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)

-include $(DEPS)
