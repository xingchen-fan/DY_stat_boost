import CRABClient
from CRABClient.UserUtilities import config 

config = config()
config.General.requestName = 'HtoZg_DY_fakePhoton_merge_2018'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True

config.JobType.pluginName = 'Analysis'
#config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = 'ConfigDY2018.py'


config.Data.inputDataset = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18RECO-106X_upgrade2018_realistic_v11_L1v1-v1/AODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 20
config.Data.publication = True
config.Data.outputDatasetTag = 'HtoZg_DY_fakePhoton_merge_2018'
config.Site.storageSite = 'T3_CH_CERNBOX'
