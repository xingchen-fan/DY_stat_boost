# DY_stat_boost
End-to-end scripts to generate more events for DY+fake events
## Pico script
Scripts use UCSB pico data format files to select certain events from 2016-2018 UL samples.

`producer.c`: Select DY+fake baseline events. BDT evaluation requires a weight xml file.

`DY_photon_producer.c`: Select events only using photon object selection

`DY_Z_producer.c`: Select events only using Z related selection
