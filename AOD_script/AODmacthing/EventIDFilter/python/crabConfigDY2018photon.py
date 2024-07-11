import CRABClient
from CRABClient.UserUtilities import config 

config = config()

config.General.requestName = 'HtoZg_DY_fakePhoton_merge_2018photon'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True

config.JobType.pluginName = 'Analysis'
#config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = 'ConfigDY2018photon.py'


config.Data.inputDataset = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18RECO-106X_upgrade2018_realistic_v11_L1v1-v1/AODSIM'
#config.Data.inputDataset = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_mc2017_realistic_v6-v1/130000/'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
#config.Data.userInputFiles=['file:/eos/user/f/fanx/HZg_AOD/AOD/02907A41-A949-544B-ACAD-7B4E08CD9B79.root']
config.Data.unitsPerJob = 20
#config.Data.totalUnits = 10
config.Data.publication = True
config.Data.outputDatasetTag = 'HtoZg_DY_fakePhoton_merge_2018_photonObjectSel'
#config.Data.outputPrimaryDataset = 'test'
config.Site.storageSite = 'T3_CH_CERNBOX'
