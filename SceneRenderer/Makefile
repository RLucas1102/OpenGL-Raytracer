src := ./src
inc := ./../include
lib := ./../../pckgs/glfw-3.4/build/src/libglfw3.a # I am doing static linking
program := renderer.out

CPP_SOURCES := $(wildcard $(src)/*.cpp)
CPP_OBJECTS := $(CPP_SOURCES:$(src)/%.cpp=%.o)

C_SOURCES := $(wildcard $(src)/*.c)
C_OBJECTS := $(C_SOURCES:$(src)/%.c=%.o)

CC = g++
CFLAGS = -I $(inc)
LFLAGS = $(lib)

$(program) : $(CPP_OBJECTS) $(C_OBJECTS)
	$(CC) $^ $(LFLAGS) -o $@

$(CPP_OBJECTS) : %.o : $(src)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(C_OBJECTS) : %.o : $(src)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(CPP_OBJECTS)
	rm -f $(C_OBJECTS)
	rm -f $(program)