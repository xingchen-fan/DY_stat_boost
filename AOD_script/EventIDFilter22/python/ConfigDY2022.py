import FWCore.ParameterSet.Config as cms
process = cms.Process("EventIDFilter22")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring()
                        )

process.phIDf = cms.EDFilter('EventIDFilter22')

process.p1 = cms.Path(process.phIDf)
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string("eventmatch_DY_22.root"),
                               SelectEvents = cms.untracked.PSet(
                                   SelectEvents = cms.vstring('p1')
                               )
)
process.outpath = cms.EndPath(process.out)
