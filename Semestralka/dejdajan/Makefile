EXEC = ./dejdajan

CXX = g++
CXXFLAGS = -Wall -pedantic -Wno-long-long -O0 -ggdb -std=c++11

DXFILE = Doxyfile

RM = rm -rf
MKDIR = mkdir -p

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

# --- Compile and create documentation --- #

  all: 
	make compile
	make doc

# --------------- Run only --------------- #

  run:
	$(EXEC) -o Output -d 2 www.olsak.net

# ------------- Compile only ------------- # 

  compile: $(OBJS)
	   $(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

# -------- Generate documentation -------- #

  doc: 
	cd src;	doxygen $(DXFILE)

# ------- Clean all generated files ------ # 
# - (objects, documentation, executable) - #

  clean:
	$(RM) $(OBJS) $(EXEC) doc/


# Automatically generated dependencies

CArguments.o: CArguments.cpp CArguments.h EMissingArgument.h EException.h \
 ESyntaxError.h
CArguments.o: CArguments.h EMissingArgument.h EException.h ESyntaxError.h
CDownload.o: CDownload.cpp CDownload.h CPage.h CObject.h CArguments.h \
 EMissingArgument.h EException.h ESyntaxError.h
CDownload.o: CDownload.h CPage.h CObject.h CArguments.h \
 EMissingArgument.h EException.h ESyntaxError.h
CImage.o: CImage.cpp CImage.h CObject.h CArguments.h EMissingArgument.h \
 EException.h ESyntaxError.h CDownload.h CPage.h
CImage.o: CImage.h CObject.h CArguments.h EMissingArgument.h EException.h \
 ESyntaxError.h CDownload.h CPage.h
CObject.o: CObject.cpp CObject.h CArguments.h EMissingArgument.h \
 EException.h ESyntaxError.h
CObject.o: CObject.h CArguments.h EMissingArgument.h EException.h \
 ESyntaxError.h
CPage.o: CPage.cpp CPage.h CObject.h CArguments.h EMissingArgument.h \
 EException.h ESyntaxError.h CImage.h CDownload.h
CPage.o: CPage.h CObject.h CArguments.h EMissingArgument.h EException.h \
 ESyntaxError.h
EException.o: EException.cpp EException.h
EException.o: EException.h
EMissingArgument.o: EMissingArgument.cpp EMissingArgument.h EException.h
EMissingArgument.o: EMissingArgument.h EException.h
ESyntaxError.o: ESyntaxError.cpp ESyntaxError.h EException.h
ESyntaxError.o: ESyntaxError.h EException.h
main.o: main.cpp CDownload.h CPage.h CObject.h CArguments.h \
 EMissingArgument.h EException.h ESyntaxError.h
