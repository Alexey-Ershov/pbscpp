FUNC_HEADER = functions.hpp
FUNC_IMPL = functions.cpp
FACT_HEADER = factory.hpp
MAIN = main.cpp
OUTPUT = eqsolver
CFLAGS = -O2 -std=c++17 -Wall
COMPILER = g++-9

.PHONY: all clean

all: main

func_impl.o: $(FUNC_HEADER) $(FUNC_IMPL)
	$(COMPILER) $(CFLAGS)  \
	            -c \
	            -o func_impl.o \
	            $(FUNC_IMPL)

main.o: $(FUNC_HEADER) $(FACT_HEADER) $(MAIN)
	$(COMPILER) $(CFLAGS) \
	            -c \
	            -o main.o \
	            $(MAIN)

main: func_impl.o main.o
	$(COMPILER) -o $(OUTPUT) func_impl.o main.o

clean:
	rm -rf $(OUTPUT) *.o
