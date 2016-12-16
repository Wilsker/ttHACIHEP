#!/bin/bash

# Lxplus Batch Job Script
# > Don't forget . . . whitespace matters ;)

CMSSW_SRC=$(echo ${CMSSW_BASE})/src
WORKING_DIR=$(echo ${CMSSW_BASE})/src/TTH/ttHACIHEP

export CMSSW_BASE=$CMSSW_SRC
cd $CMSSW_SRC
eval `scramv1 runtime -sh`

cd $WORKING_DIR
echo $WORKING_DIR

#====== cmsRun Config File =======
#sleep 10

eval ./SecondStep Data_RunsB-G_DL
eval ./SecondStep Data_RunsB-G_SL
