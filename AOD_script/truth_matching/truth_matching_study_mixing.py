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
file0 = ROOT.TFile.Open('/eos/user/f/fanx/event_mixing/sample/DY_photon_output_redwood_v1_'+args.year+'.root', "READ")
tree0 = file0.Get("tree")
if args.year == '2017':
    aodDir = '/afs/cern.ch/user/f/fanx/EOS_space/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/HtoZg_DY_fakePhoton_photonObjectSel_redo/240525_225334/0000/match_ph_'+args.number+'.root'
elif args.year == '2018':
    aodDir = '/afs/cern.ch/user/f/fanx/EOS_space/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/HtoZg_DY_fakePhoton_merge_2018_photonObjectSel/240709_145118/0000/eventmatch_DY_18photon_'+args.number+'.root'
elif args.year == '2016':
    aodDir = '/afs/cern.ch/user/f/fanx/EOS_space/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/HtoZg_DY_fakePhoton_merge_2016_photonObjectSel/240710_101537/0000/eventmatch_DY_16photon_'+args.number+'.root'
elif args.year == '2016APV':
    aodDir = '/afs/cern.ch/user/f/fanx/EOS_space/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/HtoZg_DY_fakePhoton_merge_2016APV_photonObjectSel/240710_101632/0000/eventmatch_DY_16APVphoton_'+args.number+'.root'

nEntries = tree0.GetEntries()
#truth = open('/eos/user/f/fanx/event_mixing/list/match_list_' + args.number + '.txt', 'w')
#notruth = open('/eos/user/f/fanx/event_mixing/list/nomatch_list_' + args.number + '.txt', 'w')
truthCheckList = []
motherCheckList = []
motherList = []
zero = [0]

aod = Events(aodDir)
handle  = Handle('std::vector<reco::GenParticle>')
label = ("genParticles")
nfound = 0

outfile = ROOT.TFile('/eos/user/f/fanx/event_mixing/condor_sample/no_match_ph_'+args.year+'_' + args.number + '.root', 'RECREATE')
outtree = ROOT.TTree("tree", "tree")
event_id = array('i', [0])
npv = array('i', [0])
w_lumi = array('f', [0])
photon_pt = array('f', [0])
photon_eta = array('f', [0])
photon_phi = array('f', [0])
photon_energyErr = array('f', [0])
photon_idmva = array('f', [0])
mllg = array('f', [0])
ph_index = array('i', [0])
met = array('f', [0])
met_phi = array('f', [0])
met_calo = array('f', [0])
met_tru = array('f', [0])
w_photon = array('f', [0])
w_fakephoton = array('f', [0])
w_phshape = array('f', [0])
w_pu = array('f', [0])
njet = array('i', [0])
nbdfm = array('i', [0])

outtree.Branch('photon_event', event_id, 'photon_event/I')
outtree.Branch('npv', npv, 'npv/I')
outtree.Branch('njet', njet, 'njet/I')
outtree.Branch('ph_index', ph_index, 'ph_index/I')
outtree.Branch('w_lumi', w_lumi, 'w_lumi/F')
outtree.Branch('w_photon', w_photon, 'w_photon/F')
outtree.Branch('w_phshape', w_phshape, 'w_phshape/F')
outtree.Branch('w_fakephoton', w_fakephoton, 'w_fakephoton/F')
outtree.Branch('w_pu', w_pu, 'w_pu/F')
outtree.Branch('photon_pt', photon_pt, 'photon_pt/F')
outtree.Branch('photon_eta', photon_eta, 'photon_eta/F')
outtree.Branch('photon_phi', photon_phi, 'photon_phi/F')
outtree.Branch('photon_energyErr', photon_energyErr, 'photon_energyErr/F')
outtree.Branch('photon_idmva', photon_idmva, 'photon_idmva/F')
outtree.Branch('photon_index', ph_index, 'photon_index/I')
outtree.Branch('llphoton_m', mllg, 'llphoton_m/F')
outtree.Branch('met', met, 'met/F')
outtree.Branch('met_phi', met_phi, 'met_phi/F')
outtree.Branch('met_calo', met_calo, 'met_calo/F')
outtree.Branch('met_tru', met_tru, 'met_tru/F')
outtree.Branch('ph_nbdfm', nbdfm, 'ph_nbdfm/I')

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
    w_photon_ = 0.
    w_fakephoton_=0.
    w_phshape_=0.
    w_pu_=0.
    photon_energyErr_ = 0.
    photon_idmva_ = -2.
    npv_ = -1
    mllg_ = 0
    ph_index_ = -1
    met_ = -1
    met_phi_ = -1
    met_calo_ = -1
    met_tru_ = -1
    gen_pt_ = -1
    nbdfm_ = -1
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
            picoPh.SetPtEtaPhiM(tree0.photon_pt, tree0.photon_eta, tree0.photon_phi, 0.)
            w_lumi_ = tree0.w_lumi
            w_photon_ = tree0.w_photon
            w_fakephoton_=tree0.w_fakephoton
            w_phshape_=tree0.w_phshape
            w_pu_ = tree0.w_pu
            npv_ = tree0.npv
            photon_idmva_ = tree0.photon_idmva
            photon_energyErr_ = tree0.photon_energyErr
            mllg_ = tree0.mllg
            ph_index_ = tree0.photon_index
            met_phi_ = tree0.met_phi
            met_ = tree0.met
            met_calo_ = tree0.met_calo
            met_tru_ = tree0.met_tru
            nbdfm_ = tree0.ph_nbdfm
            
            gen_pt_list = []
            gen_index_list = []
            gen_ph_pt_list = []
            gen_ph_index_list = []
            gen_mother_pt_list = []
            gen_mother_id_list = []

            for idx, part in enumerate(genpart):
                aodPh = ROOT.TLorentzVector()
                aodPh.SetPtEtaPhiM(part.pt(), part.eta(), part.phi(), part.mass())
                somedR = aodPh.DeltaR(picoPh)
                #if part.statusFlags().isPrompt()<1 or part.statusFlags().isHardProcess()<1:
                #    continue
                if part.status()!=1:
                    continue
                if somedR < 0.1 and part.pdgId() == 22:
                    gen_ph_pt_list.append(part.pt())
                    gen_ph_index_list.append(idx)
                    gen_mother_pt_list.append(part.mother().pt())
                    gen_mother_id_list.append(part.mother().pdgId())
                if somedR < 0.1:
                    gen_pt_list.append(part.pt())
                    gen_index_list.append(idx)

                if somedR < mindR: 
                    mindR = somedR
                    choseId = idx
            truthPDG = abs(genpart[choseId].pdgId())
            motherPDG = abs(genpart[choseId].mother().pdgId())
            if truthPDG == 22 and mindR < 0.1:
                max_ind = gen_ph_pt_list.index(max(gen_ph_pt_list))
                maxPtInd = gen_ph_index_list[max_ind]
                motherPDG = gen_mother_id_list[max_ind]#abs(genpart[choseId].mother().pdgId())
                gen_pt_ = gen_ph_pt_list[max_ind]#genpart[choseId].pt()
                mother_pt_ = gen_mother_pt_list[max_ind]#genpart[choseId].mother().pt()
            elif truthPDG != 22 and mindR < 0.1:
                max_ind = gen_pt_list.index(max(gen_pt_list))
                maxPtInd = gen_index_list[max_ind]
                gen_pt_ = gen_pt_list[max_ind]
                truthPDG = abs(genpart[maxPtInd].pdgId())
            break
    if found:
        if mindR > 0.1 or (truthPDG == 22 and gen_pt_ < 5.):
            event_id[0] = aodEventId
            npv[0] = npv_
            w_lumi[0] = w_lumi_
            photon_pt[0] = picoPh.Pt()
            photon_eta[0] = picoPh.Eta()
            photon_phi[0] = picoPh.Phi()
            photon_idmva[0] = photon_idmva_
            photon_energyErr[0] = photon_energyErr_
            mllg[0] = mllg_
            ph_index[0] = ph_index_
            met[0] = met_
            met_calo[0] = met_calo_
            met_phi[0] = met_phi_
            met_tru[0] = met_tru_
            w_photon[0] = w_photon_
            w_fakephoton[0] = w_fakephoton_
            w_phshape[0] = w_phshape_
            w_pu[0] = w_pu_
            njet[0] = tree0.njet
            nbdfm[0] = nbdfm_
            outtree.Fill()
            #continue

'''
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
tablefile = open('/eos/user/f/fanx/event_mixing/list/truth_info_table_' + args.number + '.txt', 'w')
#print(df, file=tablefile)
print(format_row.format("", *motherCheckList), file=tablefile)
for truth, mothers in zip(truthCheckList, motherList):
    print(format_row.format(truth, *mothers), file=tablefile)
print('Total = ', total, end='\n\n', file=tablefile)
tablefile.close()
'''
print ("n found = ", nfound)
outfile.cd()
outtree.Write()
outfile.Close()

