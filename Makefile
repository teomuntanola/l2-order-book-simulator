CXX      ?= g++
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra -Iinclude -UNDEBUG

all: demo run_tests
demo: src/main.cpp src/order_book.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@
run_tests: tests/test_book.cpp src/order_book.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@
test: run_tests
	./run_tests
clean:
	rm -f demo run_tests
