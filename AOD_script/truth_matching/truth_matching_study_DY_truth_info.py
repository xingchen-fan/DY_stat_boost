#!/usr/bin/env python3
import ROOT
import sys
import argparse
from array import array
#import pandas as pd
#from IPython.display import display
from DataFormats.FWLite import Handle, Events
parser = argparse.ArgumentParser(description = "AOD file to run over")
parser.add_argument('-n', '--number')
parser.add_argument('-y', '--year')
args = parser.parse_args()
print("It begins")
file0 = ROOT.TFile.Open("/eos/user/f/fanx/DY_output_redwood_v1_" + args.year + ".root", "READ")
tree0 = file0.Get("tree")
if args.year == "2017":
    aodDir = '/afs/cern.ch/user/f/fanx/EOS_space/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/HtoZg_DY_fakePhoton_merge_2017/241127_164421/0000/eventmatch_DY_17_'+args.number+'.root'
elif args.year == "2018":
    aodDir = '/afs/cern.ch/user/f/fanx/EOS_space/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/HtoZg_DY_fakePhoton_merge_2018/240708_234706/0000/eventmatch_DY_18_'+args.number+'.root'
elif args.year == "2016":
    aodDir = '/afs/cern.ch/user/f/fanx/EOS_space/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/HtoZg_DY_fakePhoton_merge_2016/240709_112013/0000/eventmatch_DY_16_'+args.number+'.root'
elif args.year == "2016APV":
    aodDir = '/afs/cern.ch/user/f/fanx/EOS_space/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/HtoZg_DY_fakePhoton_merge_2016APV/240709_112108/0000/eventmatch_DY_16APV_'+args.number+'.root'
elif args.year == "2022":
    aodDir = '/eos/user/f/fanx/DYto2L-2Jets_MLL-50_TuneCP5_13p6TeV_amcatnloFXFX-pythia8/HtoZg_DY_fakePhoton_2022/260818_205411/0000/eventmatch_DY_22_'+args.number+'.root'
else:
    raise ValueError("Wrong year.")
nEntries = tree0.GetEntries()
#truth = open('/eos/user/f/fanx/HZg_AOD/list/match_list_'+ args.year + '_' + args.number + '.txt', 'w')
#notruth = open('/eos/user/f/fanx/HZg_AOD/list/nomatch_list_'+ args.year + '_' + args.number + '.txt', 'w')
truthCheckList = []
motherCheckList = []
motherList = []
zero = [0]

aod = Events(aodDir)
handle  = Handle('std::vector<reco::GenParticle>')
label = ("genParticles")
j_handle = Handle('std::vector<reco::GenJet>')
j_label = ("ak4GenJets")
metHandle = Handle("std::vector<reco::PFMET>")
metLabel = ("pfMet", "", "RECO")

nfound = 0
njetph = 0
nother = 0
nhardph_tot = 0
npion_tot = 0
neta_tot = 0
njet_match = 0
njet_match_photon =0
njet_match_other = 0

outfile = ROOT.TFile('/eos/user/f/fanx/HZg_AOD/condor_sample/match_DY_'+ args.year + '_' + args.number + '.root', 'RECREATE')
outtree = ROOT.TTree("tree", "tree")

event_id = array('i', [0])
npv = array('i', [0])
njet = array('i', [0]) 
w_lumi = array('f', [0])
weight = array('f', [0])
photon_pt = array('f', [0])
photon_eta = array('f', [0])
photon_phi = array('f', [0])
photon_energyErr = array('f', [0])
photon_idmva = array('f', [0])
llphoton_m = array('f', [0])
llphoton_pt = array('f', [0])
llphoton_eta = array('f', [0])
llphoton_phi = array('f', [0])
llphoton_refit_m = array('f', [0])
llphoton_refit_pt = array('f', [0])
llphoton_refit_eta = array('f', [0])
llphoton_refit_phi = array('f', [0])
llphoton_costheta = array('f', [0])
llphoton_cosTheta = array('f', [0])
llphoton_psi = array('f', [0])
photon_drmin = array('f', [0])
photon_drmax = array('f', [0])
nlep = array('i', [0])
nbdfm = array('i', [0])
met = array('f', [0])

j1_m = array('f', [0])
j1_pt = array('f', [0])
j1_eta = array('f', [0])
j1_phi = array('f', [0])
j2_m = array('f', [0])
j2_pt = array('f', [0])
j2_eta = array('f', [0])
j2_phi = array('f', [0])
jj_m = array('f', [0])
jj_pt = array('f', [0])
jj_dR = array('f', [0])
jj_dphi = array('f', [0])
ll_m = array('f', [0])
ll_pt = array('f', [0])
ll_eta = array('f', [0])
ll_phi = array('f', [0])
l1_pt = array('f', [0])
l1_eta = array('f', [0])
l1_phi = array('f', [0])
l2_pt = array('f', [0])
l2_eta = array('f', [0])
l2_phi = array('f', [0])
lep_id = array('i', [0])

DY_class = array('i', [0])
DY_class_jet = array('i', [0])
njet_gen =  array('i', [0])

photon_gen_pt = array('f', [0])
photon_gen_eta = array('f', [0])
photon_gen_phi = array('f', [0])
l1_gen_pt = array('f', [0])
l1_gen_eta = array('f', [0])
l1_gen_phi = array('f', [0])
l2_gen_pt = array('f', [0])
l2_gen_eta = array('f', [0])
l2_gen_phi = array('f', [0])
jet_gen_m = array('f', [0])
jet_gen_pt = array('f', [0])
jet_gen_eta = array('f', [0])
jet_gen_phi = array('f', [0])
mindR_gen_jet_ph = array('f', [0])
mindR_gen_jet_ph_pT = array('f', [0])
ph_cluster_pt = array('f', [0])
pion_mother_pt = array('f', [0])
ll_m_maxpT = array('f', [0])
    
outtree.Branch('event', event_id, 'event/I')
outtree.Branch('DY_class', DY_class, 'DY_class/I')
outtree.Branch('DY_class_jet', DY_class_jet, 'DY_class_jet/I')
outtree.Branch('npv', npv, 'npv/I')
outtree.Branch('njet', njet, 'njet/I')
outtree.Branch('njet_gen', njet_gen, 'njet_gen/I')
outtree.Branch('nlep', nlep, 'nlep/I')
outtree.Branch('nbdfm', nbdfm, 'nbdfm/I')
outtree.Branch('met', met, 'met/F')               
outtree.Branch('w_lumi', w_lumi, 'w_lumi/F')
outtree.Branch('weight', weight, 'weight/F')
outtree.Branch('lepid', lep_id, 'lepid/I')
outtree.Branch('photon_pt', photon_pt, 'photon_pt/F')
outtree.Branch('photon_eta', photon_eta, 'photon_eta/F')
outtree.Branch('photon_phi', photon_phi, 'photon_phi/F')
outtree.Branch('photon_energyErr', photon_energyErr, 'photon_energyErr/F')
outtree.Branch('photon_idmva', photon_idmva, 'photon_idmva/F')
outtree.Branch('photon_drmin', photon_drmin, 'photon_drmin/F')
outtree.Branch('photon_drmax', photon_drmax, 'photon_drmax/F')
outtree.Branch('photon_gen_pt', photon_gen_pt, 'photon_gen_pt/F')
outtree.Branch('photon_gen_eta', photon_gen_eta, 'photon_gen_eta/F')
outtree.Branch('photon_gen_phi', photon_gen_phi, 'photon_gen_phi/F')
outtree.Branch('photon_gen_cluster_pT', ph_cluster_pt, 'photon_gen_cluster_pT')
outtree.Branch('pion_mother_pt', pion_mother_pt, 'pion_mother_pt/F')
outtree.Branch('llphoton_m', llphoton_m, 'llphoton_m/F')
outtree.Branch('llphoton_pt', llphoton_pt, 'llphoton_pt/F')
outtree.Branch('llphoton_eta', llphoton_eta, 'llphoton_eta/F')
outtree.Branch('llphoton_phi', llphoton_phi, 'llphoton_phi/F')
outtree.Branch('llphoton_refit_m', llphoton_refit_m, 'llphoton_refit_m/F')
outtree.Branch('llphoton_refit_pt', llphoton_refit_pt, 'llphoton_refit_pt/F')
outtree.Branch('llphoton_refit_eta', llphoton_refit_eta, 'llphoton_refit_eta/F')
outtree.Branch('llphoton_refit_phi', llphoton_refit_phi, 'llphoton_refit_phi/F')
outtree.Branch('llphoton_costheta', llphoton_costheta, 'llphoton_costheta/F')
outtree.Branch('llphoton_cosTheta', llphoton_cosTheta, 'llphoton_cosTheta/F')
outtree.Branch('llphoton_psi', llphoton_psi, 'llphoton_psi/F')
outtree.Branch('j1_m', j1_m, 'j1_m/F')
outtree.Branch('j1_pt', j1_pt, 'j1_pt/F')
outtree.Branch('j1_eta', j1_eta, 'j1_eta/F')
outtree.Branch('j1_phi', j1_phi, 'j1_phi/F')
outtree.Branch('j2_m', j2_m, 'j2_m/F')
outtree.Branch('j2_pt', j2_pt, 'j2_pt/F')
outtree.Branch('j2_eta', j2_eta, 'j2_eta/F')
outtree.Branch('j2_phi', j2_phi, 'j2_phi/F')
outtree.Branch('jj_m', jj_m, 'jj_m/F')
outtree.Branch('jj_pt', jj_pt, 'jj_pt/F')
outtree.Branch('jj_dR', jj_dR, 'jj_dR/F')
outtree.Branch('jj_dphi', jj_dphi, 'jj_dphi/F')
outtree.Branch('ll_m', ll_m, 'll_m/F')
outtree.Branch('ll_pt', ll_pt, 'll_pt/F')
outtree.Branch('ll_eta', ll_eta, 'll_eta/F')
outtree.Branch('ll_phi', ll_phi, 'll_phi/F')
outtree.Branch('l1_pt', l1_pt, 'l1_pt/F')
outtree.Branch('l1_eta', l1_eta, 'l1_eta/F')
outtree.Branch('l1_phi', l1_phi, 'l1_phi/F')
outtree.Branch('l2_pt', l2_pt, 'l2_pt/F')
outtree.Branch('l2_eta', l2_eta, 'l2_eta/F')
outtree.Branch('l2_phi', l2_phi, 'l2_phi/F')
outtree.Branch('11_gen_pt', l1_gen_pt, 'l1_gen_pt/F')
outtree.Branch('l1_gen_eta', l1_gen_eta, 'l1_gen_eta/F')
outtree.Branch('l1_gen_phi', l1_gen_phi, 'l1_gen_phi/F')
outtree.Branch('l2_gen_pt', l2_gen_pt, 'l2_gen_pt/F')
outtree.Branch('l2_gen_eta', l2_gen_eta, 'l2_gen_eta/F')
outtree.Branch('l2_gen_phi', l2_gen_phi, 'l2_gen_phi/F')
outtree.Branch('ll_m_maxpT', ll_m_maxpT, 'll_m_maxpT/F')
outtree.Branch('jet_gen_m', jet_gen_m, 'jet_gen_m/F')
outtree.Branch('jet_gen_pt', jet_gen_pt, 'jet_gen_pt/F')
outtree.Branch('jet_gen_eta', jet_gen_eta, 'jet_gen_eta/F')
outtree.Branch('jet_gen_phi', jet_gen_phi, 'jet_gen_phi/F')
outtree.Branch('mindR_gen_jet_ph', mindR_gen_jet_ph, 'mindR_gen_jet_ph/F')
outtree.Branch('mindR_gen_jet_ph_pT', mindR_gen_jet_ph_pT, 'mindR_gen_jet_ph_pT/F')

for inx, event in enumerate(aod):
    if inx > 10000: break
    if inx%100 == 0: print("Processing ", inx)
    aodEventId = event.eventAuxiliary().event()
    event.getByLabel(label, handle)
    event.getByLabel(j_label, j_handle)
    event.getByLabel(metLabel, metHandle)
    AODmet = metHandle.product()[0]
    
    genpart = handle.product()
    genJets = j_handle.product()
    choseId = 9999
    truthPDG = 0
    motherPDG = 0
    picoPh = ROOT.TLorentzVector()
    choseAODPh = ROOT.TLorentzVector()
    w_lumi_ = 0.
    weight_ = 0.
    nlep_ = -1
    met_= -2.
    nbdfm_ = -10
    photon_energyErr_ = 0.
    photon_idmva_ = -2.
    llg_m_ = 0.
    llg_pt_ = 0.
    llg_eta_ = 0.
    llg_phi_ = 0.
    llg_refit_m_ = 0.
    llg_refit_pt_ = 0.
    llg_refit_eta_ = 0.
    llg_refit_phi_ = 0.
    costheta_ = -2.
    cosTheta_ = -2.
    psi_ = -5.
    mindR_ = 0.
    maxdR_ = 0.
    npv_ = -1
    njet_ = -1
    j1_m_ = -1
    j1_eta_ = -1
    j1_pt_ = -1
    j1_phi_ = -1
    j2_m_ = -1
    j2_eta_ = -1
    j2_pt_ = -1
    j2_phi_ = -1
    jj_m_= 0.
    jj_pt_=0.
    jj_dR_=-1.
    jj_dphi_=-5.
    ll_m_=0.
    ll_pt_=0.
    ll_eta_ = 0.
    ll_phi_ = 0.
    l1_pt_ = 0.
    l1_eta_ = 0.
    l1_phi_ = 0.
    l2_pt_ = 0.
    l2_eta_ = 0.
    l2_phi_ = 0.
    lep_id_ = -1
    gen_pt_ = -1
    gen_eta_ = -1
    gen_phi_ = -1
    mother_pt_ = -1
    njet_gen_ = -1
    found = False
    
    # Jet matching quantities
    found_jet = False
    nearJetdR = 999.
    nearJetPt = -1.
    maxJetPt = -1.
    maxJetDR = 999.
    maxJetIndex = -1
    actual_ph_mindR = 999.
    actual_ph_mindR_pt = -1.
    for i in range(nEntries):
        tree0.GetEntry(i)
        nhardph = 0
        pico_entry = tree0.event
        mindR = 999.
        if aodEventId == pico_entry: 
            nfound +=1
            found = True
            mindR = 999.
            picoPh = ROOT.TLorentzVector()
            picoPh.SetPtEtaPhiM(tree0.photon_pt[0], tree0.photon_eta[0], tree0.photon_phi[0], 0.)
            w_lumi_ = tree0.w_lumi
            weight_ = tree0.weight
            nlep_ = tree0.nlep
            met_ = tree0.met
            nbdfm_=tree0.nbdfm
            photon_idmva_ = tree0.photon_idmva[0]
            photon_energyErr_ = tree0.photon_energyErr[0]
            llg_m_ = tree0.llphoton_m[0]
            llg_pt_ = tree0.llphoton_pt[0]
            llg_eta_ = tree0.llphoton_eta[0]
            llg_phi_ = tree0.llphoton_phi[0]
            llg_refit_m_ = tree0.llphoton_refit_m
            llg_refit_pt_ = tree0.llphoton_refit_pt
            llg_refit_eta_ = tree0.llphoton_refit_eta
            llg_refit_phi_ = tree0.llphoton_refit_phi
            costheta_ = tree0.llphoton_costheta[0]
            cosTheta_ = tree0.llphoton_cosTheta[0]
            psi_ = tree0.llphoton_psi[0]
            mindR_ = tree0.photon_drmin[0]
            maxdR_ = tree0.photon_drmax[0]
            npv_ = tree0.npv
            njet_ =  tree0.njet
            j1_m_ = tree0.j1_m
            j1_pt_ = tree0.j1_pt
            j1_eta_ = tree0.j1_eta
            j1_phi_ = tree0.j1_phi
            j2_m_ = tree0.j2_m
            j2_pt_ = tree0.j2_pt
            j2_eta_ = tree0.j2_eta
            j2_phi_ = tree0.j2_phi
            
            jj_m_= tree0.jj_m
            jj_pt_= tree0.jj_pt
            jj_dR_= tree0.jj_dR
            jj_dphi_= tree0.jj_dphi
            ll_m_ = tree0.ll_m[0]
            ll_pt_ = tree0.ll_pt[0]
            ll_eta_ = tree0.ll_eta[0]
            ll_phi_ = tree0.ll_phi[0]
            if tree0.leptag == 1:
                lep_id_ = 11
                l1_pt_ = tree0.el_pt[tree0.ll_i1[0]]
                l1_eta_ = tree0.el_eta[tree0.ll_i1[0]]
                l1_phi_ = tree0.el_phi[tree0.ll_i1[0]]
                l2_pt_ = tree0.el_pt[tree0.ll_i2[0]]
                l2_eta_ = tree0.el_eta[tree0.ll_i2[0]]
                l2_phi_ = tree0.el_phi[tree0.ll_i2[0]]
            elif tree0.leptag == 2:
                lep_id_ = 13
                l1_pt_ = tree0.mu_pt[tree0.ll_i1[0]]
                l1_eta_ = tree0.mu_eta[tree0.ll_i1[0]]
                l1_phi_ = tree0.mu_phi[tree0.ll_i1[0]]
                l2_pt_ = tree0.mu_pt[tree0.ll_i2[0]]
                l2_eta_ = tree0.mu_eta[tree0.ll_i2[0]]
                l2_phi_ = tree0.mu_phi[tree0.ll_i2[0]]

            gen_pt_list = []
            gen_index_list = []
            gen_ph_pt_list = []
            gen_ph_index_list = []
            gen_mother_pt_list = []
            gen_mother_id_list = []
            ph_cluster = ROOT.TLorentzVector(0, 0, 0, 0)
            
            njet_gen_ = len(genJets)
            for idx, jet in enumerate(genJets):
                jetLV = ROOT.TLorentzVector()
                jetLV.SetPtEtaPhiM(jet.pt(), jet.eta(), jet.phi(), jet.mass())
                dR = jetLV.DeltaR(picoPh)
                if dR < nearJetdR:
                    nearJetdR = dR
                    nearJetPt = jet.pt()
                if dR < 0.1 and jet.pt() > maxJetPt:
                    maxJetPt = jet.pt()
                    maxJetDR = dR
                    maxJetIndex = idx
            
            if maxJetIndex != -1:
                if genJets[maxJetIndex].pt() > 5.: found_jet = True

            for idx, part in enumerate(genpart):
                aodPh = ROOT.TLorentzVector()
                aodPh.SetPtEtaPhiM(part.pt(), part.eta(), part.phi(), part.mass())
                somedR = aodPh.DeltaR(picoPh)
                #if part.statusFlags().isPrompt()<1 or part.statusFlags().isHardProcess()<1:
                #    continue
                if part.pdgId() == 111 and part.pt() > 5.: npion_tot += 1
                if part.pdgId() == 221 and part.pt() > 5.: neta_tot += 1
                    
                if part.status()!=1:
                    continue
                if part.pdgId() == 22 and part.pt() > 5.:
                    if somedR < actual_ph_mindR:
                        actual_ph_mindR = somedR
                        actual_ph_mindR_pt = part.pt()
                if somedR < 0.1:
                    gen_pt_list.append(part.pt())
                    gen_index_list.append(idx)
                    if part.pdgId() == 22:
                        gen_ph_pt_list.append(part.pt())
                        gen_ph_index_list.append(idx)
                        gen_mother_pt_list.append(part.mother().pt())
                        gen_mother_id_list.append(part.mother().pdgId())
                        ph_cluster = ph_cluster + aodPh
                if part.pdgId() == 22 and part.pt() > 5.:
                    nhardph += 1
                    nhardph_tot += 1
                if somedR < mindR:
                    mindR = somedR
                    choseId = idx
            truthPDG = abs(genpart[choseId].pdgId())
            
            # Find truth leptons
            picoL1 = ROOT.TLorentzVector()
            mLep = 0.00051 if tree0.leptag == 1 else 0.1057
            picoL1.SetPtEtaPhiM(l1_pt_, l1_eta_, l1_phi_, mLep)
            picoL2 = ROOT.TLorentzVector()
            picoL2.SetPtEtaPhiM(l2_pt_, l2_eta_, l2_phi_, mLep)

            if truthPDG == 22 and mindR < 0.1:
                max_ind = gen_ph_pt_list.index(max(gen_ph_pt_list))
                maxPtInd = gen_ph_index_list[max_ind]
                motherPDG = gen_mother_id_list[max_ind]#abs(genpart[choseId].mother().pdgId())
                gen_pt_ = gen_ph_pt_list[max_ind]#genpart[choseId].pt()
                gen_eta_ = genpart[maxPtInd].eta()
                gen_phi_ = genpart[maxPtInd].phi()
                mother_pt_ = gen_mother_pt_list[max_ind]#genpart[choseId].mother().pt()
                choseAODPh.SetPtEtaPhiM(genpart[maxPtInd].pt(), genpart[maxPtInd].eta(), genpart[maxPtInd].phi(), 0)

            elif truthPDG != 22 and mindR < 0.1:
                max_ind = gen_pt_list.index(max(gen_pt_list))
                maxPtInd = gen_index_list[max_ind]
                gen_pt_ = gen_pt_list[max_ind]
                choseAODPh.SetPtEtaPhiM(genpart[maxPtInd].pt(), genpart[maxPtInd].eta(), genpart[maxPtInd].phi(), genpart[maxPtInd].mass())
                mindR = choseAODPh.DeltaR(picoPh)
                truthPDG = abs(genpart[maxPtInd].pdgId())                

            #if mindR > 0.1: gen_pt_ = -1
            break

    if found:
        ph_cluster_pt[0] = ph_cluster.Pt()
        event_id[0] = aodEventId
        photon_gen_pt[0] = gen_pt_
        photon_gen_eta[0] = gen_eta_
        photon_gen_phi[0] = gen_phi_
        pion_mother_pt[0] = mother_pt_
        npv[0] = npv_
        njet_gen[0] = njet_gen_
        lep_id[0] = lep_id_
        nlep[0] = nlep_
        met[0] = met_
        nbdfm[0] = nbdfm_
        njet[0] = njet_
        w_lumi[0] = w_lumi_
        weight[0] = weight_
        photon_pt[0] = picoPh.Pt()
        photon_eta[0] = picoPh.Eta()
        photon_phi[0] = picoPh.Phi()
        photon_idmva[0] = photon_idmva_
        photon_energyErr[0] = photon_energyErr_
        photon_drmin[0] = mindR_
        photon_drmax[0] = maxdR_
        llphoton_m[0] = llg_m_
        llphoton_pt[0] = llg_pt_
        llphoton_costheta[0] = costheta_
        llphoton_cosTheta[0] = cosTheta_
        llphoton_psi[0] = psi_
        j1_m[0] = j1_m_
        j1_pt[0] = j1_pt_
        j1_eta[0] = j1_eta_
        j1_phi[0] = j1_phi_
        j2_m[0] = j2_m_
        j2_pt[0] = j2_pt_
        j2_eta[0] = j2_eta_
        j2_phi[0] = j2_phi_
        jj_m[0]= jj_m_
        jj_pt[0]=jj_pt_
        jj_dR[0]=jj_dR_
        jj_dphi[0]=jj_dphi_
        ll_m[0] = ll_m_
        ll_pt[0] = ll_pt_
        ll_eta[0] = ll_eta_
        ll_phi[0] = ll_phi_
        l1_pt[0] = l1_pt_
        l1_eta[0] = l1_eta_
        l1_phi[0] = l1_phi_
        l2_pt[0] = l2_pt_
        l2_eta[0] = l2_eta_
        l2_phi[0] = l2_phi_
        mindR_gen_jet_ph[0] = nearJetdR
        mindR_gen_jet_ph_pT[0] = nearJetPt
        if maxJetIndex != -1:
            jet_gen_m[0] = genJets[maxJetIndex].mass()
            jet_gen_pt[0] = genJets[maxJetIndex].pt()
            jet_gen_eta[0] = genJets[maxJetIndex].eta()
            jet_gen_phi[0] = genJets[maxJetIndex].phi()
        l1_index = 0
        l1_mindR = 999
        l2_mindR = 999

        lead_p = ROOT.TLorentzVector()
        lead_m = ROOT.TLorentzVector()
        maxpT_p = -1
        maxpT_m = -1
        
        for idx, part in enumerate(genpart):
            if not (part.status()==1 and abs(part.pdgId()) == lep_id_): continue
            aodlep = ROOT.TLorentzVector()
            aodlep.SetPtEtaPhiM(part.pt(), part.eta(), part.phi(), part.mass())
            somedR = aodlep.DeltaR(picoL1)
            if somedR < l1_mindR:
                l1_index = idx
                l1_mindR = somedR
                l1_gen_pt[0] = part.pt()
                l1_gen_phi[0] = part.phi()
                l1_gen_eta[0] = part.eta()

        for idx, part in enumerate(genpart):
            if not (part.status()==1 and abs(part.pdgId()) == lep_id_ and idx != l1_index): continue
            aodlep = ROOT.TLorentzVector()
            aodlep.SetPtEtaPhiM(part.pt(), part.eta(), part.phi(), part.mass())
            somedR = aodlep.DeltaR(picoL2)
            if somedR < l2_mindR:
                l2_mindR = somedR
                l2_gen_pt[0] = part.pt()
                l2_gen_phi[0] = part.phi()
                l2_gen_eta[0] = part.eta()
                
        for idx, part in enumerate(genpart):
            if part.status()==1 and part.pdgId() == lep_id_:
                if part.pt() > maxpT_m:
                    maxpT_m = part.pt()
                    lead_m.SetPtEtaPhiM(part.pt(), part.eta(), part.phi(), part.mass())
            elif part.status()==1 and part.pdgId() == -lep_id_:
                if part.pt() > maxpT_p:
                    maxpT_p = part.pt()
                    lead_p.SetPtEtaPhiM(part.pt(), part.eta(), part.phi(), part.mass())
        ll_m_maxpT[0] = (lead_p + lead_m).M()
        
        if mindR > 0.1 or (gen_pt_ < 5. and truthPDG == 22):
            DY_class[0] = 1
        elif mindR < 0.1 and truthPDG == 22 and (motherPDG == 111 or motherPDG == 221) and gen_pt_ > 5.:
            DY_class[0] = 2
            njetph += 1

        else:
            DY_class[0] = 3
            nother += 1
            
        if found_jet:
            if mindR < 0.1 and truthPDG != 22 and gen_pt_ > 5.:
                DY_class_jet[0] = 3
                njet_match_other +=1
            else:
                DY_class_jet[0] = 2
                njet_match += 1
                if DY_class[0] == 2:
                    njet_match_photon += 1
        else:
            DY_class_jet[0] = 1
        outtree.Fill()
        #print("DY class = ", DY_class[0], ", n hard photon = ", nhardph)
        #print()

print ("n found = ", nfound)
print ("n jet photon = ", njetph)
print ("n other = ", nother)

outfile.cd()
outtree.Write()
