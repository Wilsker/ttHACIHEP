#####################################################################
# MakeFile - SecondStep.cc
#####################################################################
# Joshuha Thomas-Wilsker
# Institute of High Energy Physics
#####################################################################
# Makefile that compiles IHEP ttH Analysis Code against root6 with
# dependencies from  CommonClassifier and MEMClassifier packages.
# To run, invoke make command in the run directory:
#	${CMSSW_BASE}/src/TTH/ttHACIHEP
#
# 'make' will find the Makefile, preprocess, compile, assemble
# and link (combining all the object files into an executable).
#####################################################################
# Environment Setup
#####################################################################
# 1.) Working version of CMSSW required for ${CMSSW_BASE}
# 2.) CommonClassifier (kit80x) and MEIntegratorStandalone compiled
#			against CMSSW. Required to be in directory ${CMSSW_BASE}/src/TTH
# 	  To setup CommonClassifer see:
#			https://github.com/cms-ttH/CommonClassifier


# CXXFLAGS are the options passed to the compiler.
# USEFULL DEBUG CXX FLAGS:
## Can use '-ggdb' flag as it is most expressive for gdb debug tool.
## '-Wall' flag will turn on all warnings g++ can tell you.
## '-Werror' flag (wouldnt advise unless being very strict) treat warnings as errors.
## '-Wextra' enable extra warnings.
CXXFLAGS   = -ggdb $(shell root-config --cflags)

# INCL variable contains all include directories.
# Run Directory:
INCL  := -I${PWD}
# External stuff:
INCL   += -I${ROOTSYS}/include
#INCL   += -I${CMSSW_BASE}/src
#INCL	 += -I${LHAPDF_DATA_PATH}/../../../6.1.6-giojec/include/LHAPDF

# 'LDFLAGS' variable containing flags passed through the linker.
# Mostly libs as they are only needed at linking stage (not during assembly).

# !!!! HEED THIS WARNING !!!! scram b clean will clear openloops ME libs from local CMSSW
# You will see this error:
# 		[OpenLoops] ERROR: register_process: proclib folder not found, check install_path or install libraries.
#
# To resolve, copy all libs from MEIntegratorStandalone back to $SCRAM_ARCH:
#			$> cd ${CMSSW_BASE}/src
# 		$> cp -R TTH/MEIntegratorStandalone/libs/* ../lib/$SCRAM_ARCH/


LDFLAGS    = $(shell root-config --libs) -lMinuit
LDFLAGS		+= $(shell root-config --ldflags)
LDFLAGS   += -lHistFactory -lRooStats -lRooFit -lRooFitCore -lMathMore -lTMVA
#LDFLAGS   += -L${CMSSW_BASE}/lib/slc6_amd64_gcc493
#LDFLAGS	  += -L${LHAPDF_DATA_PATH}/../../../6.1.6-giojec/lib
#LDFLAGS 	+= -lLHAPDF
#LDFLAGS		+= -L${CMS_OPENLOOPS_PREFIX}/lib
#LDFLAGS		+= -lopenloops

# Following lines have format of 'target: dependencies'.
# Indented lines should be run if dependecies are newer than target so that target is updated and rebuilt.
# '-c' : Assemble source files but do not link. Outputs object files. Removing this options means the
# compiler will assemble source files and link resulting files together (also with libraries).
# '-o' : Output files.

# Some universal variables:
# '$@' is the name of the file being generated (target).
# '$<' expands to the name of the FIRST dependency.
# '$^' expands to a list of all dependencies.

# I know, I know! The following could be simplified by using the correct include directories above.

all: obj/SecondStep.o SecondStep
#SRCS := ${PWD}/../CommonClassifier/src/BlrBDTClassifier.cpp
#SRCS += ${PWD}/../CommonClassifier/src/MEMClassifier.cc
#SRCS += ${PWD}/../CommonClassifier/src/CommonBDTvars.cpp
#SRCS += ${PWD}/../MEIntegratorStandalone/src/JetLikelihood.cpp
#SRCS += ${PWD}/../MEIntegratorStandalone/src/Integrand.cpp
#SRCS += ${PWD}/../MEIntegratorStandalone/src/Parameters.cpp
SRCS := Root/newPUWeight.cc Root/eleFuncs.cc Root/muFuncs.cc Root/jetFuncs.cc Root/triggerSFs.cc
#SRCS += Root/bdtVarCalculator.cc

#DEPS := ${PWD}/../CommonClassifier/interface/MEMClassifier.h
#DEPS += ${PWD}/../CommonClassifier/interface/BlrBDTClassifier.h
#DEPS += ${PWD}/../CommonClassifier/interface/CommonBDTvars.h
#DEPS += ${PWD}/../MEIntegratorStandalone/interface/Integrand.h
#DEPS += ${PWD}/../MEIntegratorStandalone/interface/JetLikelihood.h
#DEPS += ${PWD}/../MEIntegratorStandalone/interface/Utils.h
#DEPS += ${PWD}/../MEIntegratorStandalone/interface/Parameters.h
DEPS := interface/SecondStep.h interface/newPUWeight.h interface/eleFuncs.h interface/muFuncs.h interface/jetFuncs.h interface/triggerSFs.h
#DEPS += interface/bdtVarCalculator.h

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
