CXX   = g++
FLAGS = -Wall
ASAN  = -fsanitize=address,undefined -g
LIB   = $(filter-out src/main.cpp, $(wildcard src/*.cpp))
TESTS = $(wildcard tests/test_*.cpp)

all:
	$(CXX) $(FLAGS) src/*.cpp -o main

test:
	@for t in $(TESTS); do \
		grep -q "int main" $$t || continue; \
		echo "== $$t"; \
		$(CXX) $(FLAGS) $$t $(LIB) -o test_run || exit 1; \
		./test_run || exit 1; \
	done

asan:
	@for t in $(TESTS); do \
		grep -q "int main" $$t || continue; \
		echo "== $$t (asan)"; \
		$(CXX) $(FLAGS) $(ASAN) $$t $(LIB) -o test_run || exit 1; \
		./test_run || exit 1; \
	done

clean:
	rm -f main test_run *.exe
