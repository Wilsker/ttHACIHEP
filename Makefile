#####################################################################
#									MakeFile - SecondStep.cc
#####################################################################
#									Joshuha Thomas-Wilsker
#							Institute of High Energy Physics
#####################################################################
# Makefile that compiles IHEP ttH Analysis Code against root6 with
# dependencies from  CommonClassifier and MEMClassifier packages.
# Code invokes make and does preprocessing, compilation, assembly
# and linking (combining all the object files into an executable).
#####################################################################
# 										 				Setup
# 1.) Working version of CMSSW required for ${CMSSW_BASE}
# 2.) CommonClassifier and MEIntegratorStandalone compiled against
#  	  CMSSW. Required to be in directory ${CMSSW_BASE}/src/TTH


# CXXFLAGS are the options passed to the compiler.
# USEFULL DEBUG CXX FLAGS:
## Can use '-ggdb' flag as it is most expressive for gdb debug tool.
## '-Wall' flag will turn on all warnings g++ can tell you.
## '-Werror' flag (wouldnt advise unless being very strict) treat warnings as errors.
## '-Wextra' enable extra warnings.
CXXFLAGS   = -g $(shell root-config --cflags)

# INCL variable contains all include directories.
# Run Directory:
INCL  := -I${CMSSW_BASE}/src/TTH/ttHACIHEP
# External stuff:
INCL   += -I${ROOTSYS}/include
INCL   += -I${CMSSW_BASE}/src
INCL	 += -I${LHAPDF_DATA_PATH}/../../../6.1.6-giojec/include/LHAPDF

# 'LDFLAGS' variable containing flags passed through the linker.
# Mostly libs as they are only needed at linking stage (not during assembly).
LDFLAGS    = $(shell root-config --libs) -lMinuit
LDFLAGS		+= $(shell root-config --ldflags)
LDFLAGS   += -lHistFactory -lRooStats -lRooFit -lRooFitCore -lMathMore -lTMVA
LDFLAGS   += -L${CMSSW_BASE}/lib/slc6_amd64_gcc493
LDFLAGS		+= -lopenloops
LDFLAGS	  += -L${LHAPDF_DATA_PATH}/../../../6.1.6-giojec/lib
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
SRCS := ${CMSSW_BASE}/src/TTH/CommonClassifier/src/BlrBDTClassifier.cpp
SRCS += ${CMSSW_BASE}/src/TTH/CommonClassifier/src/MEMClassifier.cc
SRCS += ${CMSSW_BASE}/src/TTH/CommonClassifier/src/CommonBDTvars.cpp
SRCS += ${CMSSW_BASE}/src/TTH/MEIntegratorStandalone/src/JetLikelihood.cpp
SRCS += ${CMSSW_BASE}/src/TTH/MEIntegratorStandalone/src/Integrand.cpp
SRCS += ${CMSSW_BASE}/src/TTH/MEIntegratorStandalone/src/Parameters.cpp
SRCS += Root/newPUWeight.cc Root/eleFuncs.cc Root/muFuncs.cc Root/jetFuncs.cc Root/bdtVarCalculator.cc

DEPS := ${CMSSW_BASE}/src/TTH/CommonClassifier/interface/MEMClassifier.h
DEPS += ${CMSSW_BASE}/src/TTH/CommonClassifier/interface/BlrBDTClassifier.h
DEPS += ${CMSSW_BASE}/src/TTH/CommonClassifier/interface/CommonBDTvars.h
DEPS += ${CMSSW_BASE}/src/TTH/MEIntegratorStandalone/interface/Integrand.h
DEPS += ${CMSSW_BASE}/src/TTH/MEIntegratorStandalone/interface/JetLikelihood.h
DEPS += ${CMSSW_BASE}/src/TTH/MEIntegratorStandalone/interface/Utils.h
DEPS += ${CMSSW_BASE}/src/TTH/MEIntegratorStandalone/interface/Parameters.h
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

clean:
	rm -rf SecondStep obj/SecondStep.o obj/*.o
