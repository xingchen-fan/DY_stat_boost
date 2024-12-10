import CRABClient
from CRABClient.UserUtilities import config 

config = config()

config.General.requestName = 'DY2016_3_3'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True

#config.JobType.pluginName = 'Analysis'
config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = 'ConfigDY.py'
config.JobType.scriptExe  = '2016DY.sh'
config.JobType.scriptArgs = ['Nevents=10000', 'Tag=fanx_3', 'DIR=fanx3/2016']
#config.JobType.inputFiles = ['FrameworkJobReport.xml']
config.JobType.numCores = 4
config.JobType.maxMemoryMB = 6300
config.JobType.maxJobRuntimeMin = 1400

config.Data.splitting = 'EventBased'
config.Data.unitsPerJob = 1
config.Data.totalUnits = 10000

config.Data.outputPrimaryDataset = 'ShellTest'
config.Data.publication = True
config.Data.outputDatasetTag = 'test'

config.Site.storageSite = 'T3_CH_CERNBOX'
