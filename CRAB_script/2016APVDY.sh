echo "this is not a test"

echo $(pwd)

ARG=$2
INDEX=$3
DIR=$4
NEVENTS=${ARG#*=}
NJOB=$(($1+${INDEX#*=}))
TAG="DY2016APV"
NANOAOD_NAME="DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL16NanoAODAPVv2-106X_mcRun2_asymptotic_preVFP_v9-v1__privateProduction"
Fragment_filename=DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL16APV__fragment.py

echo ---------------------------GEN-------------------------
Output_filename=$TAG"_"$NJOB"__LHE.root"
cmsDriver.py Configuration/GenProduction/python/$Fragment_filename --python_filename $TAG"__LHE__cfg_"$NJOB".py" --eventcontent RAWSIM,LHE --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN,LHE --fileout file:$Output_filename --conditions 106X_mcRun2_asymptotic_preVFP_v8 --beamspot Realistic25ns13TeV2016Collision --customise_commands "from IOMC.RandomEngine.RandomServiceHelper import RandomNumberServiceHelper ; randSvc = RandomNumberServiceHelper(process.RandomNumberGeneratorService) ; randSvc.populate()\nprocess.source.numberEventsInLuminosityBlock = cms.untracked.uint32(100)" --step LHE,GEN --geometry DB:Extended --era Run2_2016_HIPM --no_exec --mc -n $NEVENTS
cmsRun $TAG"__LHE__cfg_"$NJOB".py"

echo ---------------------------SIM-------------------------
Input_filename=$TAG"_"$NJOB"__LHE.root"
Output_filename=$TAG"_"$NJOB"__SIM.root"
cmsDriver.py --python_filename $TAG"__SIM__cfg_"$NJOB".py" --eventcontent RAWSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM --fileout file:$Output_filename --conditions 106X_mcRun2_asymptotic_preVFP_v8 --beamspot Realistic25ns13TeV2016Collision --step SIM --geometry DB:Extended --filein file:$Input_filename --era Run2_2016_HIPM --runUnscheduled --no_exec --mc -n -1
cmsRun $TAG"__SIM__cfg_"$NJOB".py"

echo ---------------------------DIGIPREMIX-------------------------
Input_filename=$TAG"_"$NJOB"__SIM.root"
Output_filename=$TAG"_"$NJOB"__DIGIPREMIX.root"
cmsDriver.py --python_filename $TAG"__DIGIPREMIX__cfg_"$NJOB".py" --eventcontent PREMIXRAW --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-DIGI --fileout file:$Output_filename --pileup_input "dbs:/Neutrino_E-10_gun/RunIISummer20ULPrePremix-UL16_106X_mcRun2_asymptotic_v13-v1/PREMIX" --conditions 106X_mcRun2_asymptotic_preVFP_v8 --step DIGI,DATAMIX,L1,DIGI2RAW --procModifiers premix_stage2 --geometry DB:Extended --filein file:$Input_filename --datamix PreMix --era Run2_2016_HIPM --runUnscheduled --no_exec --mc -n -1
cmsRun $TAG"__DIGIPREMIX__cfg_"$NJOB".py"

echo ---------------------------HLT-------------------------
Input_filename=$TAG"_"$NJOB"__DIGIPREMIX.root"
Output_filename=$TAG"_"$NJOB"__HLT.root"
scram p CMSSW CMSSW_8_0_33_UL
cd CMSSW_8_0_33_UL/src
cmsenv
cd ../..
cmsDriver.py --python_filename $TAG"__HLT__cfg_"$NJOB".py" --eventcontent RAWSIM --outputCommand "keep *_mix_*_*,keep *_genPUProtons_*_*" --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-RAW --inputCommands "keep *","drop *_*_BMTF_*","drop *PixelFEDChannel*_*_*_*" --fileout file:$Output_filename --conditions 80X_mcRun2_asymptotic_2016_TrancheIV_v6 --customise_commands 'process.source.bypassVersionCheck = cms.untracked.bool(True)' --step HLT:25ns15e33_v4 --geometry DB:Extended --filein file:$Input_filename --era Run2_2016 --no_exec --mc -n -1
cmsRun $TAG"__HLT__cfg_"$NJOB".py"
rm -rf CMSSW_8_0_33_UL

echo ---------------------------AOD-------------------------
Input_filename=$TAG"_"$NJOB"__HLT.root"
Output_filename=$TAG"_"$NJOB"__AOD.root"
scram p CMSSW CMSSW_10_6_40
cd CMSSW_10_6_40/src
cmsenv
cd ../..
cmsDriver.py --python_filename $TAG"__AOD__cfg_"$NJOB".py" --eventcontent AODSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier AODSIM --fileout file:$Output_filename --conditions 106X_mcRun2_asymptotic_preVFP_v8 --step RAW2DIGI,L1Reco,RECO,RECOSIM --geometry DB:Extended --filein file:$Input_filename --era Run2_2016_HIPM --runUnscheduled --no_exec --mc -n -1
cmsRun $TAG"__AOD__cfg_"$NJOB".py"

echo ---------------------------MINIAOD-------------------------
Input_filename=$TAG"_"$NJOB"__AOD.root"
Output_filename=$TAG"_"$NJOB"__MINIAOD.root"
cmsDriver.py  --python_filename $TAG"__MINIAOD__cfg_"$NJOB".py" --eventcontent MINIAODSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier MINIAODSIM --fileout file:$Output_filename --conditions 106X_mcRun2_asymptotic_preVFP_v11 --step PAT --procModifiers run2_miniAOD_UL --geometry DB:Extended --filein file:$Input_filename --era Run2_2016_HIPM --runUnscheduled --no_exec --mc -n -1
cmsRun $TAG"__MINIAOD__cfg_"$NJOB".py"

echo ---------------------------NANOAOD-------------------------
Input_filename=$TAG"_"$NJOB"__MINIAOD.root"
Output_filename=$NANOAOD_NAME"__job-"$NJOB.root
cmsDriver.py --python_filename $TAG"__NANOAOD__cfg_"$NJOB".py" --eventcontent NANOAODSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier NANOAODSIM --fileout file:$Output_filename --conditions 106X_mcRun2_asymptotic_preVFP_v11 --step NANO --filein file:$Input_filename --era Run2_2016_HIPM,run2_nanoAOD_106Xv2 --no_exec --mc -n -1
cmsRun $TAG"__NANOAOD__cfg_"$NJOB".py"

rm -f $TAG"_"$NJOB"__LHE.root"
rm -f *inLHE.root
rm -f $TAG"_"$NJOB"__SIM.root"
rm -f $TAG"_"$NJOB"__DIGIPREMIX.root"
rm -f $TAG"_"$NJOB"__HLT.root"
rm -f $TAG"_"$NJOB"__AOD.root"
rm -f $TAG"_"$NJOB"__MINIAOD.root"

xrdcp  $NANOAOD_NAME"__job-"$NJOB.root root://eosuser.cern.ch//eos/project/h/htozg-dy-privatemc/fanx/$DIR/.
rm -f $NANOAOD_NAME"__job-"$NJOB.root
rm -rf CMSSW_10_6_40

