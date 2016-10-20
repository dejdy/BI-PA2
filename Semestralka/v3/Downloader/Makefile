EXEC = ./podroada

CXX = g++
RM = rm -rf
MKDIR = mkdir -p
DOXYFILE = Doxyfile

CXXFLAGS = -Wall -pedantic -Wno-long-long -O0 -ggdb -std=c++11

SRCS = $(wildcard src/*.cpp wildcard src/exceptions/*.cpp wildcard src/tags/*.cpp)
OBJS = $(SRCS:.cpp=.o)

# ---------- Make all (compile and create documentation) ---------- # 
all:
	make compile
	make doc

# ---------- Only compile ---------- # 
compile: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

# ---------- Only run ---------- # 
run:
	$(EXEC) examples/test_1.html -

# ---------- Clean all generated files (objects, documentation, executable) ---------- # 
clean:
	$(RM) $(OBJS) $(EXEC) doc/

# ---------- Generate documentation ---------- # 
doc:
	cd src; doxygen $(DOXYFILE)

# ---------- Universal rule ---------- # 
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
