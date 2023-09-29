CXX := g++
CXX_FLAGS := -pthread --std=c++2a
VPATH := bin/

SRCS := $(shell find src -name '*.cpp')
BINS := $(patsubst src/%.cpp,%,$(SRCS))
RUNS := $(addprefix run-,$(BINS))

all: $(BINS)

$(BINS): %: src/%.cpp
	@mkdir -p bin
	$(CXX) $(CXX_FLAGS) $< -o bin/$@

$(RUNS): run-%: %
	./bin/$*

clean:
	rm -rf bin

.PHONY: all clean
