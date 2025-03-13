objects := main.o
src := ./src
inc := ./inc
program := test

CFLAGS = -I$(inc) -ggdb

all: $(program)

$(program): $(objects)
	$(CXX) -o $@ $^

$(objects): %.o : $(src)/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -f *.o
	rm -f test
	rm *.ppm