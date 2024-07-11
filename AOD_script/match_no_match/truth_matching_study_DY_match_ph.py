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
file0 = ROOT.TFile.Open("/eos/user/f/fanx/DY_output_kingscanyon_v1_2016_merge.root", "READ")
tree0 = file0.Get("tree")
#aodDir = '/afs/cern.ch/user/f/fanx/EOS_space/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/HtoZg_DY_fakePhoton_merge/240528_161419/0000/match_DY_'+args.number+'.root'
#aodDir = '/afs/cern.ch/user/f/fanx/EOS_space/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/HtoZg_DY_fakePhoton_merge_2018/240708_234706/0000/eventmatch_DY_18_'+args.number+'.root'
aodDir = '/afs/cern.ch/user/f/fanx/EOS_space/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/HtoZg_DY_fakePhoton_merge_2018/240708_234706/0000/eventmatch_DY_16_'+args.number+'.root'

nEntries = tree0.GetEntries()
#truth = open('/eos/user/f/fanx/HZg_AOD/list/match_list_' + args.number + '.txt', 'w')
#notruth = open('/eos/user/f/fanx/HZg_AOD/list/nomatch_list_' + args.number + '.txt', 'w')
truthCheckList = []
motherCheckList = []
motherList = []
zero = [0]

aod = Events(aodDir)
handle  = Handle('std::vector<reco::GenParticle>')
label = ("genParticles")
nfound = 0

outfile = ROOT.TFile('/eos/user/f/fanx/HZg_AOD/condor_sample/ph_match_pion_mom__DY_18_' + args.number + '.root', 'RECREATE')
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
photon_gen_pt = array('f', [0])
pion_mother_pt = array('f', [0])
photon_gen_drmin = array('f', [0])

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
outtree.Branch('photon_gen_pt', photon_gen_pt, 'photon_gen_pt/F')
outtree.Branch('pion_mother_pt', pion_mother_pt, 'pion_mother_pt/F')
outtree.Branch('photon_gen_drmin', photon_gen_drmin, 'photon_gen_drmin/F')

def findTruth(reco, gen):
    minLep = 999.
    choseLep = ROOT.TLorentzVector()
    for idx_, part_ in enumerate(gen):
        aodLep = ROOT.TLorentzVector()
        aodLep.SetPtEtaPhiM(part_.pt(), part_.eta(), part_.phi(), part_.mass())
        somedR_ = aodLep.DeltaR(reco)
        #if part.statusFlags().isPrompt()<1 or part.statusFlags().isHardProcess()<1:
        #    continue
        if part_.status()!=1:
            continue
        if somedR_ < minLep:
            minLep = somedR_
            choseId = idx
            choseLep.SetPtEtaPhiM(aodLep.Pt(), aodLep.Eta(), aodLep.Phi(), aodLep.M())
    return choseLep

for inx, event in enumerate(aod):
    if inx%100 == 0: print("Processing ", inx)
    aodEventId = event.eventAuxiliary().event()
    event.getByLabel(label, handle)
    genpart = handle.product()
    choseId = 9999
    truthPDG = 0
    motherPDG = 0
    picoPh = ROOT.TLorentzVector()
    choseAODPh = ROOT.TLorentzVector()
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
    gen_pt_ = -1
    mother_pt_ = -1
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
            picoLep1 = ROOT.TLorentzVector()
            picoLep2 = ROOT.TLorentzVector()
            picoPh.SetPtEtaPhiM(tree0.photon_pt[0], tree0.photon_eta[0], tree0.photon_phi[0], 0.)
            if tree0.leptag == 1:
                picoLep1.SetPtEtaPhiM(tree0.el_pt[tree0.ll_i1[0]], tree0.el_eta[tree0.ll_i1[0]], tree0.el_phi[tree0.ll_i1[0]], 0.00511)
                picoLep2.SetPtEtaPhiM(tree0.el_pt[tree0.ll_i2[0]], tree0.el_eta[tree0.ll_i2[0]], tree0.el_phi[tree0.ll_i2[0]], 0.00511)
            elif tree0.leptag == 2:
                picoLep1.SetPtEtaPhiM(tree0.mu_pt[tree0.ll_i1[0]], tree0.mu_eta[tree0.ll_i1[0]], tree0.mu_phi[tree0.ll_i1[0]], 0.105)
                picoLep2.SetPtEtaPhiM(tree0.mu_pt[tree0.ll_i2[0]], tree0.mu_eta[tree0.ll_i2[0]], tree0.mu_phi[tree0.ll_i2[0]], 0.105)

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
            gen_pt_ = genpart[choseId].pt()
            mother_pt_ = genpart[choseId].mother().pt()
            choseAODPh.SetPtEtaPhiM(genpart[choseId].pt(), genpart[choseId].eta(), genpart[choseId].phi(), 0)
            break
    if found:
        if mindR < 0.1 and truthPDG == 22 and (motherPDG == 111):
            lep1 = findTruth(picoLep1, genpart)
            lep2 = findTruth(picoLep2, genpart)
            photon_gen_drmin[0] = min(lep1.DeltaR(choseAODPh), lep2.DeltaR(choseAODPh))
            #print ("eventId = ", pico_entry, file = notruth)
            #print ("mindR = ", mindR, file = notruth)
            #print ("-"*13, file = notruth)
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
            photon_gen_pt[0] = gen_pt_
            pion_mother_pt[0] = mother_pt_
            outtree.Fill()
            continue

      

outfile.cd()
outtree.Write()
outfile.Close()

