import ROOT
import math
import sys
import argparse
from plot_utility import *

#parser = argparse.ArgumentParser()
#parser.add_argument("year")
#args = parser.parse_args()

tree1 = ROOT.TChain("tree")
tree1.Add('/afs/cern.ch/user/f/fanx/EOS_space/HZg_AOD/pico/full_class_DY_redwood.root')

tree2 = ROOT.TChain("tree")

tree2.Add('sample/mixing_output_2016_redwood_refit.root')
tree2.Add('sample/mixing_output_2016APV_redwood_refit.root')
tree2.Add('sample/mixing_output_2017_redwood_refit.root')
tree2.Add('sample/mixing_output_2018_redwood_refit.root')

sample1 = "Drell-Yan Pile-up"
sample2 = "Mixing Events"

nEntries1 = tree1.GetEntries()
nEntries2 = tree2.GetEntries()

#pt = twoHist("pT", 130, 15, 80)
njet = twoHist("njet", 2, 0, 2, sample1, sample2)
j1_pt = twoHist("pT(j1)", 50, 20, 200, sample1, sample2)
j1_m = twoHist("m(j1)", 30, 0, 30, sample1, sample2)
j1_eta = twoHist("#eta(j1)", 50, -2.5, 2.5, sample1, sample2)
#j2_pt = twoHist("j2_pt", 50, 20, 200, sample1, sample2)
llphoton_dijet_balance =  twoHist("llphoton_dijet_balance", 100, 0, 1, sample1, sample2)
llphoton_dijet_dphi =  twoHist("llphoton_dijet_dphi", 70, -3.14, 3.14, sample1, sample2)
llphoton_psi = twoHist("llphoton_psi", 100, -5, 5, sample1, sample2)
min_dR = twoHist("min_dR", 70, 0.3, 3.5, sample1, sample2)
max_dR = twoHist("max_dR", 60, 1, 4, sample1, sample2)
costheta = twoHist("llphoton_cos(#theta)", 100, -1, 1, sample1, sample2)
cosTheta = twoHist("llphoton_cos(#Theta)", 100, -1, 1, sample1, sample2)
l1_eta = twoHist("#eta(l1)", 50, -2.5, 2.5, sample1, sample2)
l2_eta = twoHist("#eta(l2)", 50, -2.5, 2.5, sample1, sample2)
photon_eta = twoHist("#eta(#gamma)", 50, -2.5, 2.5, sample1, sample2)
photon_mva = twoHist("photon_idmva", 90, 0.1, 1, sample1, sample2)
photon_res = twoHist("photon_res", 80, 0, 0.2, sample1, sample2)
llphoton_pTt = twoHist("llphoton_pTt", 100, 10, 200, sample1, sample2)
photon_zeppenfeld =  twoHist("photon_zeppenfeld", 100, 0, 5, sample1, sample2)
photon_jet1_dr =  twoHist("photon_jet1_dr", 60, 0, 6, sample1, sample2)
dphi_llg_j1 = twoHist("#Delta#phi(Z#gamma,j1)", 35, 0, 3.5, sample1, sample2)
dphi_hmiss_photon = twoHist("#Delta#phi(H_{miss},#gamma)", 35, 0, 3.5, sample1, sample2)
#photon_jet2_dr =  twoHist("photon_jet2_dr", 100, 0, 6, sample1, sample2)
pt_mass = twoHist("pt_mass", 50, 0, 1, sample1, sample2)
llgpsi =  twoHist("llg_psi", 70, -3.14,3.14 , sample1, sample2)
for i in range(nEntries1):
    tree1.GetEntry(i)
    if  not (tree1.njet < 2 and tree1.met < 90 and tree1.nlep == 2) or tree1.DY_class != 1: continue
    z = ROOT.TLorentzVector()
    g = ROOT.TLorentzVector()
    z.SetPtEtaPhiM(tree1.ll_pt, tree1.ll_eta, tree1.ll_phi, tree1.ll_m)
    g.SetPtEtaPhiM(tree1.photon_pt, tree1.photon_eta, tree1.photon_phi, 0)
    h = z + g
    h_pt = h.Vect()
    z_pt = z.Vect()
    g_pt = g.Vect()
    if tree1.njet > 0:
        j = ROOT.TLorentzVector()
        j.SetPtEtaPhiM(tree1.j1_pt, tree1.j1_eta, tree1.j1_phi, tree1.j1_m)
        j1_m.Fill1(tree1.j1_m, tree1.weight)
        j1_pt.Fill1(tree1.j1_pt, tree1.weight)
        j1_eta.Fill1(tree1.j1_eta, tree1.weight)
        photon_jet1_dr.Fill1(j.DeltaR(g), tree1.weight)
        photon_zeppenfeld.Fill1(abs(tree1.j1_eta - tree1.photon_eta), tree1.weight)
        dphi_llg_j1.Fill1(h.DeltaPhi(j), tree1.weight)
        llphoton_dijet_balance.Fill1((h+j).Pt()/(z.Pt() + g.Pt() + j.Pt()), tree1.weight)
        dphi_hmiss_photon.Fill1(abs(tree1.photon_phi - (h + j).Phi()), tree1.weight)
    photon_eta.Fill1(tree1.photon_eta, tree1.weight)
    l1_eta.Fill1(tree1.l1_eta, tree1.weight)
    l2_eta.Fill1(tree1.l2_eta, tree1.weight)
    llphoton_pTt.Fill1(h_pt.Cross((z_pt-g_pt).Unit()).Mag(), tree1.weight)
    photon_mva.Fill1(tree1.photon_idmva, tree1.weight)
    costheta.Fill1(tree1.llphoton_costheta, tree1.weight)
    cosTheta.Fill1(tree1.llphoton_cosTheta, tree1.weight)
    min_dR.Fill1(tree1.photon_drmin, tree1.weight)
    max_dR.Fill1(tree1.photon_drmax, tree1.weight)
    llgpsi.Fill1(tree1.llphoton_psi, tree1.weight)
    photon_res.Fill1(tree1.photon_energyErr/ROOT.TMath.CosH(tree1.photon_eta)/tree1.photon_pt, tree1.weight)
    pt_mass.Fill1(tree1.llphoton_pt/tree1.llphoton_m, tree1.weight)
    njet.Fill1(tree1.njet, tree1.weight)
    if tree1.njet == 0:
        dphi_hmiss_photon.Fill1(abs(tree1.photon_phi - h.Phi()), tree1.weight)
for j in range(nEntries2):
    tree2.GetEntry(j)
    if not (tree2.njet < 2 and tree2.met < 90 and tree2.nel + tree2.nmu == 2 and tree2.llphoton_m[0] > 100 and tree2.llphoton_m[0] < 180): continue
    photon_eta.Fill2(tree2.photon_eta[0], tree2.weight * tree2.yr_weight)
    llphoton_pTt.Fill2(tree2.llphoton_pTt[0], tree2.weight * tree2.yr_weight)
    costheta.Fill2(tree2.llphoton_costheta[0], tree2.weight * tree2.yr_weight)
    cosTheta.Fill2(tree2.llphoton_cosTheta[0], tree2.weight * tree2.yr_weight)
    photon_mva.Fill2(tree2.photon_idmva[0], tree2.weight * tree2.yr_weight)
    min_dR.Fill2(tree2.photon_drmin[0], tree2.weight * tree2.yr_weight)
    max_dR.Fill2(tree2.photon_drmax[0], tree2.weight * tree2.yr_weight)
    if tree2.nel >= 2 and tree2.ll_lepid[0] == 11:
        l1_eta.Fill2(tree2.el_eta[tree2.ll_i1[0]], tree2.weight * tree2.yr_weight)
        l2_eta.Fill2(tree2.el_eta[tree2.ll_i2[0]], tree2.weight * tree2.yr_weight)
    elif tree2.nmu >= 2 and tree2.ll_lepid[0] == 13:
        l1_eta.Fill2(tree2.mu_eta[tree2.ll_i1[0]], tree2.weight * tree2.yr_weight)
        l2_eta.Fill2(tree2.mu_eta[tree2.ll_i2[0]], tree2.weight * tree2.yr_weight)
    llgpsi.Fill2(tree2.llphoton_psi[0], tree2.weight * tree2.yr_weight)
    photon_res.Fill2(tree2.photon_energyErr[0]/ROOT.TMath.CosH(tree2.photon_eta[0])/tree2.photon_pt[0], tree2.weight * tree2.yr_weight)
    pt_mass.Fill2(tree2.llphoton_pt[0]/tree2.llphoton_m[0], tree2.weight * tree2.yr_weight)
    njet.Fill2(tree2.njet, tree2.weight * tree2.yr_weight)
    h = ROOT.TLorentzVector()
    g = ROOT.TLorentzVector()
    g.SetPtEtaPhiM(tree2.photon_pt[0], tree2.photon_eta[0], tree2.photon_phi[0], 0)
    h.SetPtEtaPhiM(tree2.llphoton_pt[0], tree2.llphoton_eta[0], tree2.llphoton_phi[0], tree2.llphoton_m[0])
    if tree2.njet == 0:
        dphi_hmiss_photon.Fill2(abs(tree2.photon_phi[0] - h.Phi()), tree2.weight * tree2.yr_weight)
    if tree2.njet > 0:
        j = ROOT.TLorentzVector()
        for i, is_good in enumerate(tree2.jet_isgood):
            if is_good:
                j1_m.Fill2(tree2.jet_m[i], tree2.weight * tree2.yr_weight)
                j1_pt.Fill2(tree2.jet_pt[i],  tree2.weight * tree2.yr_weight)
                j1_eta.Fill2(tree2.jet_eta[i],  tree2.weight * tree2.yr_weight)
                j.SetPtEtaPhiM(tree2.jet_pt[i], tree2.jet_eta[i], tree2.jet_phi[i], tree2.jet_m[i])
                photon_zeppenfeld.Fill2(abs(tree2.jet_eta[i] - tree2.photon_eta[0]), tree2.weight * tree2.yr_weight)
                dphi_hmiss_photon.Fill2(abs(tree2.photon_phi[0] - (h+j).Phi()), tree2.weight * tree2.yr_weight)
                break
        llphoton_dijet_balance.Fill2((h+j).Pt()/(z.Pt() + g.Pt() + j.Pt()), tree2.weight * tree2.yr_weight)
        photon_jet1_dr.Fill2(tree2.photon_jet1_dr[0], tree2.weight * tree2.yr_weight)
        dphi_llg_j1.Fill2(h.DeltaPhi(j), tree2.weight * tree2.yr_weight)

llphoton_pTt.makePlot('ggF_mix_run2')
costheta.makePlot('ggF_mix_run2')
cosTheta.makePlot('ggF_mix_run2')
photon_mva.makePlot('ggF_mix_run2')
min_dR.makePlot('ggF_mix_run2')
max_dR.makePlot('ggF_mix_run2')
l1_eta.makePlot('ggF_mix_run2')
l2_eta.makePlot('ggF_mix_run2')
llgpsi.makePlot('ggF_mix_run2')
photon_res.makePlot('ggF_mix_run2')
pt_mass.makePlot('ggF_mix_run2')
j1_m.makePlot('ggF_mix_run2')
j1_pt.makePlot('ggF_mix_run2')
j1_eta.makePlot('ggF_mix_run2')
photon_eta.makePlot('ggF_mix_run2')
photon_jet1_dr.makePlot('ggF_mix_run2')
njet.makePlot('ggF_mix_run2')
photon_zeppenfeld.makePlot('ggF_mix_run2')
llphoton_dijet_balance.makePlot('ggF_mix_run2')
photon_jet1_dr.makePlot('ggF_mix_run2')
dphi_llg_j1.makePlot('ggF_mix_run2')
dphi_hmiss_photon.makePlot('ggF_mix_run2')

