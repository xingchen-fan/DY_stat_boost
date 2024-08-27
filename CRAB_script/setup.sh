#!/bin/bash

export SCRAM_ARCH=slc7_amd64_gcc700
source /cvmfs/cms.cern.ch/cmsset_default.sh
scram p CMSSW CMSSW_10_6_40
cd CMSSW_10_6_40/src
mkdir -p Configuration/GenProduction/python
mkdir -p GeneratorInterface/GenFilters/interface
mkdir -p GeneratorInterface/GenFilters/src
cp ../gen_filter/genFilterFiles/BuildFile.xml GeneratorInterface/GenFilters
cp ../gen_filter/genFilterFiles/SealModule.cc GeneratorInterface/GenFilters/src
cp ../gen_filter/genFilterFiles/MatchDYFilter.cc GeneratorInterface/GenFilters/src
cp ../gen_filter/genFilterFiles/MatchDYFilter.h GeneratorInterface/GenFilters/interface
cp ../gen_filter/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL16__fragment.py Configuration/GenProduction/python
cp ../gen_filter/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL16APV__fragment.py Configuration/GenProduction/python
cp ../gen_filter/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL17__fragment.py Configuration/GenProduction/python
cp ../gen_filter/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18__fragment.py Configuration/GenProduction/python
scram b
cd ../..
