FUNC_HEADER = functions.hpp
FUNC_IMPL = functions.cpp
FACT_HEADER = factory.hpp
EQSOLV_HEADER = eqsolution.hpp
EQSOLV_IMPL = eqsolution.cpp
TEST_HEADER = test.hpp
MAIN = main.cpp
OUTPUT = eqsolver
CFLAGS = -O2 -std=c++17 -Wall
LDFLAGS = -lgtest -lpthread
COMPILER = g++-9

.PHONY: all clean

all: main

func_impl.o: $(FUNC_HEADER) $(FUNC_IMPL)
	$(COMPILER) $(CFLAGS)  \
	            -c \
	            -o func_impl.o \
	            $(FUNC_IMPL)

eqsolv.o: $(FUNC_HEADER) $(FUNC_IMPL) $(EQSOLV_HEADER) $(EQSOLV_IMPL)
	$(COMPILER) $(CFLAGS)  \
	            -c \
	            -o eqsolv.o \
	            $(EQSOLV_IMPL)

main.o: $(FUNC_HEADER) $(FACT_HEADER) $(EQSOLV_HEADER) $(TEST_HEADER) $(MAIN)
	$(COMPILER) $(CFLAGS) \
	            -c \
	            -o main.o \
	            $(MAIN)

main: func_impl.o eqsolv.o main.o
	$(COMPILER) -o $(OUTPUT) func_impl.o eqsolv.o main.o $(LDFLAGS)

clean:
	rm -rf $(OUTPUT) *.o
