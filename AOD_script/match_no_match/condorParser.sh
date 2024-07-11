#!/bin/bash

# Set up CMSSW
cd /eos/user/f/fanx/CMSSW_13_3_3/src/
source /cvmfs/cms.cern.ch/cmsset_default.sh
#cd ~/CMSSW_12_0_0/src/
cmsenv

# cd to this dir, please change if necessary
cd ~/condorStuff/

# Change to other truth matching study if needed
./truth_matching_study_DY.py $1

