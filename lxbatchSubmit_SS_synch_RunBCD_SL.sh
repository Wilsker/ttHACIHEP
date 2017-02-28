#!/bin/bash
# Lxplus Batch Job Script
# > Don't forget . . . whitespace matters ;)
CMSSW_SRC=$(echo ${CMSSW_BASE})/src
WORKING_DIR=$(echo ${CMSSW_BASE})/src/TTH/ttHACIHEP
export CMSSW_BASE=$CMSSW_SRC
cd $CMSSW_SRC
eval `scramv1 runtime -sh` # Establishes runtime env. of local shell on node.
cd $WORKING_DIR
echo $WORKING_DIR
./SecondStep synch_output_RunBCD_SL
