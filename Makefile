# Makefile for student allocation for Experimental Research Project

CXX = g++  # for macs - otherwise get c++ = clang
CXXFLAGS = -Wall -g -O2

# also arrange for fortran support
FC = gfortran
FFLAGS = -Wall -O2
CXXFLAGS += -std=c++20
LDFLAGS += -std=c++20

INCLUDE +=
LIBRARIES  +=

INCLUDE += $(LCLINCLUDE)

COMMONSRC =
F77SRC =
COMMONOBJ =


PROGSRC = generate_pseudodata.cc matchmaking.cc matchmakingNoRandom.cc process_CSV_qualtrics.cc randomize_top_picks.cc visualize_survey.cc
PROGOBJ = generate_pseudodata.o matchmaking.o matchmakingNoRandom.o process_CSV_qualtrics.o randomize_top_picks.o visualize_survey.o

all : generate_pseudodata matchmaking matchmakingNoRandom process_CSV_qualtrics randomize_top_picks visualize_survey

INCLUDE += `root-config --cflags`
LIBRARIES  += `root-config --glibs`

generate_pseudodata : generate_pseudodata.o $(COMMONOBJ)
	$(CXX) $(LDFLAGS) -o $@ $@.o $(COMMONOBJ) $(LIBRARIES)

matchmaking : matchmaking.o $(COMMONOBJ)
	$(CXX) $(LDFLAGS) -o $@ $@.o $(COMMONOBJ) $(LIBRARIES)

matchmakingNoRandom : matchmakingNoRandom.o $(COMMONOBJ)
	$(CXX) $(LDFLAGS) -o $@ $@.o $(COMMONOBJ) $(LIBRARIES)

process_CSV_qualtrics : process_CSV_qualtrics.o $(COMMONOBJ)
	$(CXX) $(LDFLAGS) -o $@ $@.o $(COMMONOBJ) $(LIBRARIES)

randomize_top_picks : randomize_top_picks.o $(COMMONOBJ)
	$(CXX) $(LDFLAGS) -o $@ $@.o $(COMMONOBJ) $(LIBRARIES)

visualize_survey : visualize_survey.o $(COMMONOBJ)
	$(CXX) $(LDFLAGS) -o $@ $@.o $(COMMONOBJ) $(LIBRARIES)

clean:
	rm -vf $(COMMONOBJ) $(PROGOBJ)

realclean: clean
	rm -vf generate_pseudodata matchmaking matchmakingNoRandom process_CSV_qualtrics randomize_top_picks visualize_survey

.cc.o:         $<
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
.cpp.o:         $<
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
.C.o:         $<
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
.f.o:         $<
	$(FC) $(FFLAGS) -c $< -o $@
.f90.o:         $<
	$(FC) $(FFLAGS) -c $< -o $@

depend:
	makedepend  $(LCLINCLUDE) -Y --   -- $(COMMONSRC) $(PROGSRC)

# DO NOT DELETE

generate_pseudodata.o: ProgressBar.h

