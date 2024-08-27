import CRABClient
from CRABClient.UserUtilities import config 

config = config()

config.General.requestName = '2016APVDY'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True

#config.JobType.pluginName = 'Analysis'
config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = 'ConfigDY.py'
config.JobType.scriptExe  = '2016APVDY.sh'
config.JobType.scriptArgs = ['Nevents=10000', 'Index=20000']
config.JobType.inputFiles = ['FrameworkJobReport.xml']
config.JobType.numCores = 4
config.JobType.maxMemoryMB = 9000
config.JobType.maxJobRuntimeMin = 1400

config.Data.splitting = 'EventBased'
config.Data.unitsPerJob = 1
config.Data.totalUnits = 10000

config.Data.outputPrimaryDataset = 'ShellTest'
config.Data.publication = True
config.Data.outputDatasetTag = 'test'

config.Site.storageSite = 'T3_CH_CERNBOX'
