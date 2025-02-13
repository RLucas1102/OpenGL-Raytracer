objects := vecTest.o
src := ./src
inc := ../include
program := test

CFLAGS = -I$(inc)

all: $(program)

$(program): $(objects)
	$(CXX) -o $@ $^

$(objects): %.o : $(src)/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -f *.o
	rm -f test