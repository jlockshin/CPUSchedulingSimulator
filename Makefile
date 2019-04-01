# Jane Lockshin
# Makefile for Project 1
NAME = simulator
CPPFLAGS += -Wall -MMD -MP -Isrc -g -std=c++11

SRCS = $(shell find src -name '*.cpp')
OBJS = $(SRCS:src/%.cpp=bin/%.o)
DEPS = $(SRCS:src/%.cpp=bin/%.d)

$(NAME): $(OBJS)
	$(CXX) $(CPP_FLAGS) $^ -o $(NAME)

run: $(NAME)
	./$(NAME) example_simulation*

clean:
	rm -rf $(NAME)* bin/

$(SRCS): | bin

bin:
	mkdir -p $(shell find src -type d | sed "s/src/bin/")

bin/%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $< -c -o $@

-include $(DEPS)
