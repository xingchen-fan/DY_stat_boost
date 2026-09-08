# DY_stat_boost
End-to-end scripts to generate more events for DY+fake events
## Pico script
Scripts use UCSB pico data format files to select certain events from 2016-2018 UL samples. Three event producers are:

* `producer.c`: Select DY+fake baseline events. BDT evaluation requires a weight xml file.
  * Output _baseline_pico.root_ file.
* `DY_photon_producer.c`: Select events only using photon object selection.
  * Output _photon_pico.root_ file.
* `DY_Z_producer.c`: Select events only using Z related selection.
  * Output _Z_pico.root_ file.

## AOD script
To do truth matching of the recon photons, we need to pin down the events we want using event ID, run number and lumiblock matching between pico and AOD (or miniAOD).
### EDFilter setup
AOD (or miniAOD) has a format of Event Data Model (EDM) which requires us to use CMSSW to access and manipulate. To obtain the event ID of each AOD event, an EDFilter is needed. Please refer to this [twiki](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideSkeletonCodeGenerator) on how to set up a filter.

In short, the setup code is as such
```
cd CMSSW/src
cmsenv
mkdir AODmacthing
mkedfltr EventIDFilter
```

In the filter, the event ID, run number, lumiblock of each AOD (or miiniAOD) event is compared with those of all the slected pico events. If the values matched, the AOD (or miniAOD) event will be saved in an output EDM file. Submit CRAB jobs to run the filter over a full set of dataset in `AODmacthing/EventIDFilter/python`. Two configuration files are needed, one for CMSSW to run and the other for CRAB job submission. Output files will be saved in the CERN box as specified in the CRAB config file.

Equivalent EDM files to the pico will be output 
  * ie. _baseline_AOD.root_ and _photon_AOD.root_.

One example of such event matching filter is at AOD_script/EventIDFilter22` where we find the corresponding miniAOD events to the selected pico events for 2022 DY events passing the baseline selection.

For reference, the dataset versions of Run3 eras are the following:

|Era|Datasets|
|-|-|
|2022| `/DYto2L-2Jets_MLL-50_TuneCP5_13p6TeV_amcatnloFXFX-pythia8/Run3Summer22MiniAODv4-130X_mcRun3_2022_realistic_v5-v2/MINIAODSIM`, `/DYto2L-2Jets_MLL-50_TuneCP5_13p6TeV_amcatnloFXFX-pythia8/Run3Summer22MiniAODv4-130X_mcRun3_2022_realistic_v5_ext1-v1/MINIAODSIM`|
|2022EE|`/DYto2L-2Jets_MLL-50_TuneCP5_13p6TeV_amcatnloFXFX-pythia8/Run3Summer22EEMiniAODv4-130X_mcRun3_2022_realistic_postEE_v6-v2/MINIAODSIM`, `/DYto2L-2Jets_MLL-50_TuneCP5_13p6TeV_amcatnloFXFX-pythia8/Run3Summer22EEMiniAODv4-130X_mcRun3_2022_realistic_postEE_v6_ext1-v1/MINIAODSIM`|
|2023|`/DYto2L-2Jets_MLL-50_TuneCP5_13p6TeV_amcatnloFXFX-pythia8/Run3Summer23MiniAODv4-130X_mcRun3_2023_realistic_v14-v1/MINIAODSIM`|
|2023BPix|`/DYto2L-2Jets_MLL-50_TuneCP5_13p6TeV_amcatnloFXFX-pythia8/Run3Summer23MiniAODv4-130X_mcRun3_2023_realistic_v14-v1/MINIAODSIM`|
|2024|`/DYto2E-2Jets_Bin-MLL-50_TuneCP5_13p6TeV_amcatnloFXFX-pythia8/RunIII2024Summer24MiniAODv6-150X_mcRun3_2024_realistic_v2-v4/MINIAODSIM`, `/DYto2Mu-2Jets_Bin-MLL-50_TuneCP5_13p6TeV_amcatnloFXFX-pythia8/RunIII2024Summer24MiniAODv6-150X_mcRun3_2024_realistic_v2-v6/MINIAODSIM`|

### DY event classification 
The truth matching of the pico recon photon with the AOD truth particles happens here. Due to the large number of events we need to go through, we use CERN batch system, HTCondor, for the process.

Several truth matching processes are:

* `truth_matching_study_DY.py`: Truth matching DY+fake baseline events and output no match events.
  * Output _baseline_nomatch_pico.root_ file.
* `truth_matching_study_DY_match_ph.py`: Truth matching DY+fake baseline events and output match truth photon (pion mother) events.
  * Output _baseline_match_photon__(_mom_pion_)__pico.root_ file.
* `truth_matching_study_photon_only.py`: Truth matching events only using photon object selection and output no match events.
  * Output _nomatch_photon_pico.root_ file.

## Pile-up events - Event mixing
To generate more no match events, we mix the Z candicates from _Z_pico.root_ events with no match photons from _nomatch_photon_pico.root_ events. To separate GGF and Dijet events, use `print_event_id.py` to print out the event IDs of the corresponding events, and plot them separately by event ID matching.

The mixed events' error bars need extra cares and please use `plot_mixing_corrErrBar.c` to plot the corrected error bar.

## Jet photon events - Generator filter (CERN Condor)
Codes are originally from Jae-Bak's [repository](https://github.com/jaebak/produceMC/tree/UL). Refer to it for environment setup. I modified scripts to suit my need of mass generation.

Initialize your VOMS with
```
voms-proxy-init --voms cms --out $(pwd)/voms_proxy.txt -valid 172:0
```

The idea is plain and simple: use the same script that generates the central produced DY MC samples to generate more events, but apply a filter on the generator level to save time on the simulation and reconstruction. Go to the [DAS](https://cmsweb.cern.ch/das/) page to find the corresponding dataset and grap its production code from McM.

There are three files per production:
* `.sh`: where the generation happens, from GEN to NanoAOD.
* `.env`: where output and input file names reside.
* `.sub`: where Condor job settings are specified.

[Multithreading](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookGenMultithread) is turned on for Madgraph and Pythia8. Import the gen filter files into the CMSSW and compile. 

__Notice__:
* No job submission is allowed from any EOS directory, so please put the repo in your AFS/workspace.
* The current gen filter only works with `CMSSW_10_6_X` and some changes are required for newer version of CMSSW.
* Right now the output NanoAOD files will be transferred to my EOS space. Please change `OUTPUT_DIR` to yours in `.env`.
* The job number is used as the random seed for event generation. Please change it to some other numbers in `.sub`.

## Match events - Generator filter (CRAB)
Need to have a CentOs7 container that supports `CMSSW10` and all the CRAB jobs should be submitted in that environment.
On lxplus, use a command like this ([reference](https://cms-sw.github.io/singularity.html)):
```
cmssw-el7
```
To set up the CMSSW:
* **RECOMMENDED:** cd to `/eos/project-h/htozg-dy-privatemc/CMSSW_10_6_40/src` and run `cmsenv; scram b`.
* ALTERNATIVE: Install `CMSSW10` and compile the generator filter and fragment files by running `setup.sh`.

Each era has a set of files. Take 2016 as an example, let's look at them:
* `2016DY.sh` : where the generation of new events only happens.
* `ConfigDY.py` : A minimal skeleton CMSSW config file, required by the CRAB config file and shared by all eras.
* `FrameworkJobReport.xml` : A minimal report file required by the CRAB config file and shared by all eras.
* `crabConfig2016.py` : the CRAB config file that you need to modify.

There is no need to modify the skeleton CMSSW config file or the shell script, but the CRAB config file.

In our shared CERN box, please create a directory for yourself, in my case, `/fanx`, and per year, create a dedicated folder such as `/2017`.

Inside the CRAB config file, there are some key lines and parameters that I will explain in the following, and the lines need modifications are in ***italics***:
* `config.General.workArea` : A directory will be create to hold the log files of your submissions.
* ***`config.General.requestName`*** : Need to be a unique name for each submission.
* `config.JobType.scriptExe` : Specify the shell script run in jobs.
* `config.Data.totalUnits = 10000` : Number of jobs = 10000. Each job will have a job number `njob` from 1 to 10k. CRAB only allows up to 10k jobs per submission.
* ***`config.JobType.scriptArgs = ['Nevents=10000', 'Tag=fanx1', 'DIR=YOURDIR/YEAR']`*** : `Nevents` should always be 10000 per job, but there is a tag at the end of the output NanoAOD file to distinguish different jobs, so please make a different tag for each submission. `DIR` is the directory that you want to transfer the outputs of this submission to.

Finally, submit the jobs to the CRAB:

```
crab submit -c crabConfig2016.py
```
## Guidance for the number of jobs

According to my rough estimation, 50k events requested will give us one event after the baseline and truth matching. I have this table for 1 fold of statistics:
|Era|Existing events after baseline|Number of jobs (10k events/job)|
|-|-|-|
|2016|5128|26000|
|2016APV|6439|33000|
|2017|13045|70000|
|2018|14056|80000|

For Run3, assuming jet photon events makes up 55% of total DY after baseline
|Era|Existing events after baseline|Number of jobs (10k events/job)|
|-|-|-|
|2022|13500|68000|
|2022EE|43700|219000|
|2023|15000|75000|
|2023BPix|10000|50000|
|2024|141000|707000|
## CRAB Job Guide (No-brainer Version)

**IMPORTANT:** You need to have your grid certificate installed, please go to this [page](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookStartingGrid) and follow the steps. 

**NOTICE:** Another [repo](https://github.com/xingchen-fan/DY_filter_MC_submission/tree/main) is created just for Run3 MC job submission. Please go there!

Take a 2017 submission as an example:

1. Login to LXPLUS
2. git clone this repo.
3. Run `cmssw-el7`
4. `cd` to the prepared CMSSW dir in our CERNBOX
   ```
   cd /eos/project/h/htozg-dy-privatemc/CMSSW_10_6_40/src/
   ```
   and run `cmsenv; scram b`
5. Craete your own folder in our CERNBOX. Then create one folder per submission in your folder, i.e. `YOURDIR/2017`.
6. Back to the `CRAB_script/` of your local repo dir and run `submitter.sh`:
   ```
   ./submitter.sh crabConfig2017.py n DY2017 YOURDIR/2017 MYTAG
   ```
   where there are 5 arguments:
   * The CRAB config file you want to use.
   * The number of submissions you want to submit. `n` means **n*10k** jobs submitted here.
   * The name of the submission for monitoring. In each submission, a number will be added at the end of it so that the names are different.
   * The subdirectory in our CERNBOX where you want to download the output files to.
   * The tag at the end of the output root files. In each submission, a number will be added at the end of it so that the tags are different.
   

Now, you successfully submit **n** submissions of jobs and there will eventually be about **n***9500 output files (5% failure rate) in `/YOURDIR/2017` if everything goes as intended.

Three useful CRAB commands are
```
crab status -d crab_projects/crab_DY2017_1
```
which is to monitor the jobs. This command will provide some URL that you can open in your broswer such that you can monitor the jobs in a more intuitive way. If you keep that URL open, you don't need to log in to LXPLUS to monitor the jobs.
```
crab kill -d crab_projects/crab_DY2017_1
```
which is to kill a submission.

```
crab resubmit -d crab_projects/crab_DY2017_1
```
which is to resubmit failed jobs in a submission.


As I list in the table previously, we need 7 submissions (10k jobs each) to have one fold of 2017 DY statistics. Likewise, 3 submissions for 2016, 4 submissions for 2016APV and 7 submssions for 2018. DO NOT submit all these jobs together and you will lose your priority! Finish one year at a time and move on to the next year. 

If you finish all the years, congratulations! You help us increase the statistics by 1 fold! And you can create a sub-folder like `/1fold` in `/YOURDIR` and move all the root files to it, cleaning up `/YOURDIR`. Then you can move on to the next fold. 
