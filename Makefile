########################################
#				MakeFile - SecondStep.cc			 #
########################################
#				Joshuha Thomas-Wilsker
#		Institute of High Energy Physics
########################################
# Makefile that compiles SecondStep.cc
# against root6 with dependencies from
# CommonClassifier and MEMClassifier
# packages. Code invokes GCC, and does
# preprocessing, compilation, assembly
# and linking (combining all the
# object files into an executable).
########################################

# CXXFLAGS are the options passed to the compiler.
# USEFULL DEBUG CXX FLAGS:
## Can use '-ggdb' flag as it is most expressive for gdb debug tool.
## '-Wall' flag will turn on all warnings g++ can tell you.
## '-Werror' flag (wouldnt advise unless being very strict) treat warnings as errors.
## '-Wextra' enable extra warnings.
CXXFLAGS   = -g $(shell root-config --cflags)

# INCL variable contains all include directories.
# Run Directory:
INCL  := -I/afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/ttHACIHEP
# External stuff:
INCL   += -I${ROOTSYS}/include
INCL   += -I/afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src
#INCL   += -I/afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/CommonClassifier/interface
#INCL   += -I/afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/MEIntegratorStandalone/interface
INCL	 += -I${ROOTSYS}/../../../external/lhapdf/6.1.6-giojec/include/LHAPDF

# Variable containing flags passed through the linker (LDFLAGS).
# Mostly libs as they are only needed at linking stage (not during assembly).
LDFLAGS    = $(shell root-config --libs) -lMinuit
LDFLAGS		+= $(shell root-config --ldflags)
LDFLAGS   += -lHistFactory -lRooStats -lRooFit -lRooFitCore -lMathMore -lTMVA
LDFLAGS   += -L/afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/lib/slc6_amd64_gcc493
LDFLAGS		+= -lopenloops
LDFLAGS	  += -L${ROOTSYS}/../../../external/lhapdf/6.1.6-giojec/lib
LDFLAGS 	+= -lLHAPDF

# Following lines have format of 'target: dependencies'.
# Indented lines should be run if dependecies are newer than target so that target is updated and rebuilt.
# '-c' : Assemble source files but do not link. Outputs object files. Removing this options means the
# compiler will assemble source files and link resulting files together (also with libraries).
# '-o' : Output files.

# Some universal variables:
# '$@' is the name of the file being generated (target).
# '$<' expands to the name of the FIRST dependency.
# '$^' expands to a list of all dependencies.

all: obj/SecondStep.o SecondStep
SRCS := /afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/CommonClassifier/src/BlrBDTClassifier.cpp
SRCS += /afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/CommonClassifier/src/MEMClassifier.cc
SRCS += /afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/CommonClassifier/src/CommonBDTvars.cpp
SRCS += /afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/MEIntegratorStandalone/src/JetLikelihood.cpp
SRCS += /afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/MEIntegratorStandalone/src/Integrand.cpp
SRCS += /afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/MEIntegratorStandalone/src/Parameters.cpp
SRCS += Root/newPUWeight.cc Root/eleFuncs.cc Root/muFuncs.cc Root/jetFuncs.cc Root/bdtVarCalculator.cc

DEPS := /afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/CommonClassifier/interface/MEMClassifier.h
DEPS += /afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/CommonClassifier/interface/BlrBDTClassifier.h
DEPS += /afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/CommonClassifier/interface/CommonBDTvars.h
DEPS += /afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/MEIntegratorStandalone/interface/Integrand.h
DEPS += /afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/MEIntegratorStandalone/interface/JetLikelihood.h
DEPS += /afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/MEIntegratorStandalone/interface/Utils.h
DEPS += /afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/MEIntegratorStandalone/interface/Parameters.h
DEPS += interface/SecondStep.h interface/newPUWeight.h interface/eleFuncs.h interface/muFuncs.h interface/jetFuncs.h interface/bdtVarCalculator.h

SecondStep: obj/SecondStep.o $(SRCS)
	@echo '<<<<< Second Step Executable <<<<<<'
	@echo 'Target: $@ '
	@echo 'Dependancies: '
	@echo $^
	g++ $(CXXFLAGS) $(LDFLAGS) $(SRCS) $(INCL) $< -o $@

obj/SecondStep.o: Root/SecondStep.cc $(DEPS)
	@echo '>>>>> SecondStep >>>>>'
	@echo 'Target: $@ '
	@echo 'Dependancies:'
	@echo $<
	g++ $(CXXFLAGS) $(INCL) -c $< -o $@

# Could compile individual .cpp file in directory listed beneath
# using pattern rule e.g. $> make obj/BDTClassifier.o ?

clean:
	rm -rf SecondStep obj/SecondStep.o obj/*.o
