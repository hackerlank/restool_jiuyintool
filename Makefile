TARGET := test

SRC1 :=	 $(wildcard src/*.cpp) \
		 $(wildcard src/*/*.cpp) \
		 $(wildcard src/*/*/*.cpp) \
		 $(wildcard src/*/*/*/*.cpp) \
		 $(wildcard src/*/*/*/*/*.cpp)

SRC2 :=  $(wildcard src/*.c) \
		 $(wildcard src/*/*.c) \
		 $(wildcard src/*/*/*.c) \
		 $(wildcard src/*/*/*/*.c) \
		 $(wildcard src/*/*/*/*/*.c)


OBJS := $(SRC2:%.c=obj/%.o)
OBJS += $(SRC1:%.cpp=obj/%.o)

all:$(OBJS)
	@echo [M]Link...
	@mkdir -p bin
	g++ -g $(OBJS) -o bin/$(TARGET)


obj/%.o:%.cpp
	@echo [M]Compile CXX...
	@mkdir -p $(@D)
	g++ -g -c $< -o $@ -Wno-multichar

obj/%.o:%.c
	@echo [M]Compile C...
	@mkdir -p $(@D)
	g++ -g -c $< -o $@

run: all
	./bin/$(TARGET)

clean:
	rm -rf bin
	rm -rf obj
	rm -rf out/*
	rm -rf core















