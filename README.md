# DY_stat_boost
End-to-end scripts to generate more events for DY+fake events
## Pico script
Scripts use UCSB pico data format files to select certain events from 2016-2018 UL samples.

`producer.c`: Select DY+fake baseline events. BDT evaluation requires a weight xml file.

`DY_photon_producer.c`: Select events only using photon object selection

`DY_Z_producer.c`: Select events only using Z related selection

## AOD script
To do truth matching of the recon photons, we need to pin down the events we want using event ID matching between pico and AOD.
### EDFilter setup
AOD has a format of Event Data Model (EDM) which requires us to use CMSSW to access and manipulate. To obtain the event ID of each AOD event, an EDFilter is needed. Please refer to this [twiki](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideSkeletonCodeGenerator) on how to set up a filter.

In short, the setup code is as such
```
cd CMSSW/src
cmsenv
mkdir AODmacthing
mkedfltr EventIDFilter
```

In the filter, the event ID of each AOD event is compared with that of all the slected pico events. If the ID matched, the AOD event will be saved in an output EDM file. Submit CRAB jobs to run the filter over a full set of dataset in `AODmacthing/EventIDFilter/python`. Two configuration files are needed, one for CMSSW to run and the other for CRAB job submission. Out put files will be aved in the CERN box as specified in the CRAB config file.

### Match and no match event slection
The truth matching of the pico recon photon with the AOD truth particles happens here. Due to the large number of events we need to go through, we use CERN batch system, HTCondor, for the process.

`truth_matching_study_DY.py`: Truth matching DY+fake baseline events and output no match events.

`truth_matching_study_DY_match_ph.py`: Truth matching DY+fake baseline events and output match truth photon (pion mother) events.

`truth_matching_study_photon_only.py`: Truth matching events only using photon object selection and output no match events.

test

