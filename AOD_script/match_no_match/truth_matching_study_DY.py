#!/usr/bin/env python3
import ROOT
import sys
import argparse
from array import array
#import pandas as pd
#from IPython.display import display
from DataFormats.FWLite import Handle, Events
parser = argparse.ArgumentParser(description = "AOD file to run over")
parser.add_argument('number')
args = parser.parse_args()
print("It begins")
file0 = ROOT.TFile.Open("/eos/user/f/fanx/DY_output_kingscanyon_v1_2016APV_merge.root", "READ")
tree0 = file0.Get("tree")
#aodDir = '/afs/cern.ch/user/f/fanx/EOS_space/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/HtoZg_DY_fakePhoton_merge/240528_161419/0000/match_DY_'+args.number+'.root'
#aodDir = '/afs/cern.ch/user/f/fanx/EOS_space/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/HtoZg_DY_fakePhoton_merge_2018/240708_234706/0000/eventmatch_DY_18_'+args.number+'.root'
#aodDir = '/afs/cern.ch/user/f/fanx/EOS_space/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/HtoZg_DY_fakePhoton_merge_2016/240709_112013/0000/eventmatch_DY_16_'+args.number+'.root'
aodDir = '/afs/cern.ch/user/f/fanx/EOS_space/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/HtoZg_DY_fakePhoton_merge_2016APV/240709_112108/0000/eventmatch_DY_16APV_'+args.number+'.root'

nEntries = tree0.GetEntries()
truth = open('/eos/user/f/fanx/HZg_AOD/list/match_list_' + args.number + '.txt', 'w')
notruth = open('/eos/user/f/fanx/HZg_AOD/list/nomatch_list_' + args.number + '.txt', 'w')
truthCheckList = []
motherCheckList = []
motherList = []
zero = [0]

aod = Events(aodDir)
handle  = Handle('std::vector<reco::GenParticle>')
label = ("genParticles")
nfound = 0

outfile = ROOT.TFile('/eos/user/f/fanx/HZg_AOD/condor_sample/no_match_DY_16APV_' + args.number + '.root', 'RECREATE')
outtree = ROOT.TTree("tree", "tree")
event_id = array('i', [0])
npv = array('i', [0])
njet = array('i', [0]) 
w_lumi = array('f', [0])
photon_pt = array('f', [0])
photon_eta = array('f', [0])
photon_phi = array('f', [0])
photon_energyErr = array('f', [0])
photon_idmva = array('f', [0])
llphoton_m = array('f', [0])
llphoton_pt = array('f', [0])
llphoton_costheta = array('f', [0])
llphoton_cosTheta = array('f', [0])
llphoton_psi = array('f', [0])
photon_drmin = array('f', [0])

outtree.Branch('event', event_id, 'event/I')
outtree.Branch('npv', npv, 'npv/I')
outtree.Branch('njet', njet, 'njet/I')
outtree.Branch('w_lumi', w_lumi, 'w_lumi/F')
outtree.Branch('photon_pt', photon_pt, 'photon_pt/F')
outtree.Branch('photon_eta', photon_eta, 'photon_eta/F')
outtree.Branch('photon_phi', photon_phi, 'photon_phi/F')
outtree.Branch('photon_energyErr', photon_energyErr, 'photon_energyErr/F')
outtree.Branch('photon_idmva', photon_idmva, 'photon_idmva/F')
outtree.Branch('photon_drmin', photon_drmin, 'photon_drmin/F')
outtree.Branch('llphoton_m', llphoton_m, 'llphoton_m/F')
outtree.Branch('llphoton_pt', llphoton_pt, 'llphoton_pt/F')
outtree.Branch('llphoton_costheta', llphoton_costheta, 'llphoton_costheta/F')
outtree.Branch('llphoton_cosTheta', llphoton_cosTheta, 'llphoton_cosTheta/F')
outtree.Branch('llphoton_psi', llphoton_psi, 'llphoton_psi/F')

for inx, event in enumerate(aod):
    if inx > 10000: break
    if inx%100 == 0: print("Processing ", inx)
    aodEventId = event.eventAuxiliary().event()
    event.getByLabel(label, handle)
    genpart = handle.product()
    choseId = 9999
    truthPDG = 0
    motherPDG = 0
    picoPh = ROOT.TLorentzVector()
    w_lumi_ = 0.
    photon_energyErr_ = 0.
    photon_idmva_ = -2.
    llg_m_ = 0.
    llg_pt_ = 0.
    costheta_ = -2.
    cosTheta_ = -2.
    psi_ = -5.
    mindR_ = 0.
    npv_ = -1
    njet_ = -1
    for i in range(nEntries):
        tree0.GetEntry(i)
        found = False
        pico_entry = tree0.event
        mindR = 999.
        if aodEventId == pico_entry: 
            nfound +=1
            found = True
            mindR = 999.
            picoPh = ROOT.TLorentzVector()
            picoPh.SetPtEtaPhiM(tree0.photon_pt[0], tree0.photon_eta[0], tree0.photon_phi[0], 0.)
            w_lumi_ = tree0.w_lumi
            photon_idmva_ = tree0.photon_idmva[0]
            photon_energyErr_ = tree0.photon_energyErr[0]
            llg_m_ = tree0.llphoton_m[0]
            llg_pt_ = tree0.llphoton_pt[0]
            costheta_ = tree0.llphoton_costheta[0]
            cosTheta_ = tree0.llphoton_cosTheta[0]
            psi_ = tree0.llphoton_psi[0]
            mindR_ = tree0.photon_drmin[0]
            npv_ = tree0.npv
            njet_ =  tree0.njet
            for idx, part in enumerate(genpart):
                aodPh = ROOT.TLorentzVector()
                aodPh.SetPtEtaPhiM(part.pt(), part.eta(), part.phi(), part.mass())
                somedR = aodPh.DeltaR(picoPh)
                #if part.statusFlags().isPrompt()<1 or part.statusFlags().isHardProcess()<1:
                #    continue
                if part.status()!=1:
                    continue
                if somedR < mindR: 
                    mindR = somedR
                    choseId = idx
            truthPDG = abs(genpart[choseId].pdgId())
            motherPDG = abs(genpart[choseId].mother().pdgId())
            break
    if found:
        if mindR > 0.1:
            print ("eventId = ", pico_entry, file = notruth)
            print ("mindR = ", mindR, file = notruth)
            print ("-"*13, file = notruth)
            event_id[0] = aodEventId
            npv[0] = npv_
            njet[0] = njet_
            w_lumi[0] = w_lumi_
            photon_pt[0] = picoPh.Pt()
            photon_eta[0] = picoPh.Eta()
            photon_phi[0] = picoPh.Phi()
            photon_idmva[0] = photon_idmva_
            photon_energyErr[0] = photon_energyErr_
            photon_drmin[0] = mindR_
            llphoton_m[0] = llg_m_
            llphoton_pt[0] = llg_pt_
            llphoton_costheta[0] = costheta_
            llphoton_cosTheta[0] = cosTheta_
            llphoton_psi[0] = psi_
            outtree.Fill()
            continue

        print ("eventId = ", pico_entry, file = truth)
        print ("mindR = %.3f" %mindR, ", pdgId = ", truthPDG, ", mother pdgId = ", motherPDG, file = truth)
        print ("-"*13, file = truth)
        
        if len(truthCheckList)==0:
            truthCheckList.append(truthPDG)
            motherList.append([])
        else:
            add = True
            for pdg in truthCheckList:
                if pdg == truthPDG:
                    add = False
            if add:
                truthCheckList.append(truthPDG)
                motherList.append([])

        if len(motherCheckList)==0:
            motherCheckList.append(motherPDG)
        else:
            add = True
            for pdg in motherCheckList:
                if pdg == motherPDG:
                    add = False
            if add:
                motherCheckList.append(motherPDG)
        
        localTruthIndex = 999
        localMotherIndex = 999
        for ind, pdg in enumerate(truthCheckList):
            if pdg == truthPDG:
                localTruthIndex = ind

        for ind, pdg in enumerate(motherCheckList):
            if pdg == motherPDG:
                localMotherIndex = ind

        if len(motherList[localTruthIndex]) < (localMotherIndex+1):
            motherList[localTruthIndex].extend(zero*(localMotherIndex + 1 - len(motherList[localTruthIndex])))

        motherList[localTruthIndex][localMotherIndex] += 1

truth.close()       
notruth.close() 
maxlength = 0
for entry in motherList:
    maxlength = len(entry) if len(entry)>maxlength else maxlength
for entry in motherList:
    entry.extend(zero*(maxlength-len(entry)))

#print("truth list = ", truthCheckList)
#print("mother list = ", motherCheckList)
#print("breakdown = ", motherList)
total = sum(sum(x) for x in motherList)
motherCheckList.append('Total')
for entry in motherList:
    entry.append(sum(entry))

#pd.set_option('display.max_columns', None)
#df = pd.DataFrame(motherList, columns = motherCheckList, index=truthCheckList)
format_row = "{:>10}" * (len(motherCheckList) + 1)
tablefile = open('/eos/user/f/fanx/HZg_AOD/list/truth_info_table_' + args.number + '.txt', 'w')
#print(df, file=tablefile)
print(format_row.format("", *motherCheckList), file=tablefile)
for truth, mothers in zip(truthCheckList, motherList):
    print(format_row.format(truth, *mothers), file=tablefile)
print('Total = ', total, end='\n\n', file=tablefile)
tablefile.close()
print ("n found = ", nfound)
outfile.cd()
outtree.Write()
outfile.Close()

