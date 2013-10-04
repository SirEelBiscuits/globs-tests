# Generic GNUMakefile
 
# Just a snippet to stop executing under other make(1) commands
# that won't understand these lines
ifneq (,)
This makefile requires GNU Make.
endif
 
PROGRAM = glTest
SRCDIR = src
OBJDIR = intermediate
OUTDIR = bin
CPP_FILES :=# $(wildcard $(SRCDIR)/*.cpp)

include inc.mk

OBJS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(CPP_FILES))
LIBBASE = Z:\\Windows\\ManualInstalls\\dev-libs
CXX = g++
CPPFLAGS += -Wall -pedantic -std=c++11 -I$(LIBBASE)\\include -static-libgcc -static-libstdc++ -Isrc/globs
LDFLAGS += -L$(LIBBASE)\\lib -lGLFW -lglew32s -lopengl32 -lglu32 -lDevIL -lILU -lILUT
 
all: version $(PROGRAM) 

nolink: version $(OBJS)
 
$(PROGRAM): .depend $(OBJS)
	$(CXX) $(CPPFLAGS) $(OBJS) $(LDFLAGS) -o $(OUTDIR)/$(PROGRAM)
 
depend: .depend
 
.depend: cmd = $(CXX) -std=c++11 -MM -MF depend $(var); cat depend >> .depend;
.depend:
	@echo "Generating dependencies..."
	@$(foreach var, $(CPP_FILES), $(cmd))
	@rm -f depend
 
-include .depend
 
version:
	echo "#define VERSION \"` date +%s `\"" > $(SRCDIR)/version.inc

# These are the pattern matching rules. In addition to the automatic
# variables used here, the variable $* that matches whatever % stands for
# can be useful in special cases.
$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@
 
%: %.cpp
	$(CXX) $(CPPFLAGS) -o $@ $<
 
clean:
	find $(OBJDIR) -regex ".*\.o" -print0 | xargs -0 rm
 
.PHONY: clean depend
