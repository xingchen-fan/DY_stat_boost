import FWCore.ParameterSet.Config as cms
process = cms.Process("EventIDFilterDY2018")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring()
                        )
#file:/afs/cern.ch/user/f/fanx/EOS_space/016B6C98-203F-1447-B9B8-D2122D01D94A.root

process.phIDf = cms.EDFilter('EventIDFilterDY2018photon')

process.p1 = cms.Path(process.phIDf)
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string("eventmatch_DY_18photon.root"),
                               SelectEvents = cms.untracked.PSet(
                                   SelectEvents = cms.vstring('p1')
                               )
)
process.outpath = cms.EndPath(process.out)

