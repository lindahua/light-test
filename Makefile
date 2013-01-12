# The make file for Light-SIMD

# Detect platform
#--------------------

UNAME := $(shell uname -s)
MACH_TYPE := $(shell uname -m)


# Compiler configuration
#-------------------------

WARNING_FLAGS = -Wall -Wextra -Wconversion -Wformat -Wno-unused-parameter
CPPFLAGS = -I. 

ifeq ($(UNAME), Linux)
	CXX=g++
	CXXFLAGS = -std=c++0x -pedantic -march=native $(WARNING_FLAGS) $(CPPFLAGS) 
endif

ifeq ($(UNAME), Darwin)
	CXX=clang++
	CXXFLAGS = -std=c++0x -stdlib=libc++ -pedantic -march=native $(WARNING_FLAGS) $(CPPFLAGS)
endif



# directory configuration

INC=light_test
SRC=src
BIN=bin


#------ Header groups ----------

HEADERS = \
	$(INC)/base.h \
	$(INC)/test_assertions.h \
	$(INC)/test_units.h \
	$(INC)/test_mon.h \
	$(INC)/test_exec.h \
	${INC}/auto_suite.h \
	$(INC)/tests.h \
	$(INC)/color_printf.h \
	$(INC)/std_test_mon.h
	

#---------- Target groups -------------------

.PHONY: all
all: $(BIN)/example1

.PHONY: clean

clean:
	-rm $(BIN)/*
	

#--------- Target details -----------------

$(BIN)/example1: $(HEADERS) $(SRC)/example1.cpp
	$(CXX) $(CXXFLAGS) $(SRC)/example1.cpp -o $@





	
	
	
