import CRABClient
from CRABClient.UserUtilities import config 

config = config()

config.General.requestName = 'DY2017_3_8'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True

#config.JobType.pluginName = 'Analysis'
config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = 'ConfigDY.py'
config.JobType.scriptExe  = '2017DY.sh'
config.JobType.scriptArgs = ['Nevents=10000', 'Tag=fanx_8', 'DIR=fanx3/2017']
config.JobType.inputFiles = ['2017_degipremix_files_disk.txt','FrameworkJobReport.xml']
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
