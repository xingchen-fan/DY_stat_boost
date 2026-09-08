#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#include "TVector3.h"
#include "../BDT/MVAfunc.cxx"

void DY_photon_producer(string year, TString cat){
  TFile* dyfile;
  if (year == "2016"){
    dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_redwood_v1/2016/mc/merged_zgmc_llg/merged_pico_llg_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_llg_nfiles_41.root", "READ");
  }
  else if (year == "2016APV"){
    dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_redwood_v1/2016APV/mc/merged_zgmc_llg/merged_pico_llg_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_llg_nfiles_62.root", "READ");
  }
  else if (year == "2017"){
    dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_redwood_v1/2017/mc/merged_zgmc_llg/merged_pico_llg_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_llg_nfiles_153.root", "READ");
  }
  else if (year == "2018") {
    dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_redwood_v1/2018/mc/merged_zgmc_llg/merged_pico_llg_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_llg_nfiles_204.root", "READ");
  }
  else if (year == "2022"){
    dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv12/htozgamma_redwood_v1/2022/mc/merged_zgmc_llg/merged_pico_llg_DYto2L-2Jets_MLL-50_TuneCP5_13p6TeV_amcatnloFXFX-pythia8_zgmc_llg_nfiles_374.root", "READ");
  }
  else if (year == "2022EE"){
    dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv12/htozgamma_redwood_v1/2022EE/mc/merged_zgmc_llg/merged_pico_llg_DYto2L-2Jets_MLL-50_TuneCP5_13p6TeV_amcatnloFXFX-pythia8_zgmc_llg_nfiles_693.root", "READ");
  }
  else if (year == "2023"){
    dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv12/htozgamma_redwood_v1/2023/mc/merged_zgmc_llg/merged_pico_llg_DYto2L-2Jets_MLL-50_TuneCP5_13p6TeV_amcatnloFXFX-pythia8_zgmc_llg_nfiles_181.root", "READ");
  }
  else if (year == "2023BPix"){
    dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv12/htozgamma_redwood_v1/2023BPix/mc/merged_zgmc_llg/merged_pico_llg_DYto2L-2Jets_MLL-50_TuneCP5_13p6TeV_amcatnloFXFX-pythia8_zgmc_llg_nfiles_221.root", "READ");
  }

  auto picotree = (TTree*)dyfile->Get("tree");
  auto fout = new TFile("DY_photon_output_redwood_v1_"+year+cat+".root", "RECREATE");
  Long64_t event;
  float w_lumi;
  int npv;
  int nll;
  int njet, nbdfm;
  bool use;
  float mllg;
  float met, met_phi, met_calo, met_tru;
  float w_photon, w_phshape, w_fakephoton, w_pu;
  float j1_pt, j1_eta, j1_phi, j1_m, j2_pt, j2_eta, j2_phi, j2_m;
  std::vector<bool> *ph_ishard = new std::vector<bool>();
  std::vector<bool> *ph_id80 = new std::vector<bool>();
  std::vector<bool> *ph_elveto = new std::vector<bool>();
  std::vector<bool> *ph_isScEtaEB = new std::vector<bool>();
  std::vector<bool> *ph_isScEtaEE = new std::vector<bool>();
  std::vector<float> *ph_pt = new std::vector<float>();
  std::vector<float> *ph_eta = new std::vector<float>();
  std::vector<float> *ph_phi = new std::vector<float>();
  std::vector<float> *ph_energyErr = new std::vector<float>();
  std::vector<float> *ph_idmva = new std::vector<float>();
  std::vector<float> *ll_m = new std::vector<float>();
  std::vector<float> *ll_pt = new std::vector<float>();
  std::vector<float> *ll_eta = new std::vector<float>();
  std::vector<float> *ll_phi = new std::vector<float>();
  std::vector<float> *jet_eta = new std::vector<float>();
  std::vector<float> *jet_pt = new std::vector<float>();
  std::vector<float> *jet_phi = new std::vector<float>();
  std::vector<float> *jet_m = new std::vector<float>();
  std::vector<bool> *jet_isgood = new std::vector<bool>();

  picotree->SetBranchAddress("event", &event);
  picotree->SetBranchAddress("npv", &npv);
  picotree->SetBranchAddress("nll", &nll);
  picotree->SetBranchAddress("photon_hardprocess", &ph_ishard);
  picotree->SetBranchAddress("met", &met);
  picotree->SetBranchAddress("met_phi", &met_phi);
  picotree->SetBranchAddress("met_calo", &met_calo);
  picotree->SetBranchAddress("met_tru", &met_tru);
  picotree->SetBranchAddress("use_event", &use);
  picotree->SetBranchAddress("w_lumi", &w_lumi);
  picotree->SetBranchAddress("w_photon", &w_photon);
  picotree->SetBranchAddress("w_phshape", &w_phshape);
  picotree->SetBranchAddress("w_fakephoton", &w_fakephoton);
  picotree->SetBranchAddress("w_pu", &w_pu);
  picotree->SetBranchAddress("photon_pt", &ph_pt);
  picotree->SetBranchAddress("photon_eta", &ph_eta);
  picotree->SetBranchAddress("photon_phi", &ph_phi);
  picotree->SetBranchAddress("photon_id80", &ph_id80);
  picotree->SetBranchAddress("photon_elveto", &ph_elveto);
  picotree->SetBranchAddress("photon_isScEtaEE", &ph_isScEtaEE);
  picotree->SetBranchAddress("photon_isScEtaEB", &ph_isScEtaEB);
  picotree->SetBranchAddress("photon_energyErr", &ph_energyErr);
  picotree->SetBranchAddress("photon_idmva", &ph_idmva);
  picotree->SetBranchAddress("ll_m", &ll_m);
  picotree->SetBranchAddress("ll_pt", &ll_pt);
  picotree->SetBranchAddress("ll_eta", &ll_eta);
  picotree->SetBranchAddress("ll_phi", &ll_phi);
  picotree->SetBranchAddress("njet", &njet);
  picotree->SetBranchAddress("nbdfm", &nbdfm);
  picotree->SetBranchAddress("jet_isgood", &jet_isgood);
  picotree->SetBranchAddress("jet_pt", &jet_pt);
  picotree->SetBranchAddress("jet_eta", &jet_eta);
  picotree->SetBranchAddress("jet_phi", &jet_phi);
  picotree->SetBranchAddress("jet_m", &jet_m);

  auto outtree = new TTree("tree", "tree");
  int index;
  float ph_pt_, ph_eta_, ph_phi_, idmva_, err_;
  outtree->Branch("event", &event);
  outtree->Branch("met", &met);
  outtree->Branch("met_phi", &met_phi);
  outtree->Branch("met_calo", &met_calo);
  outtree->Branch("met_tru", &met_tru);
  outtree->Branch("npv", &npv);
  outtree->Branch("nll", &nll);
  outtree->Branch("njet", &njet);
  outtree->Branch("use_event", &use);
  outtree->Branch("w_lumi", &w_lumi);
  outtree->Branch("w_photon", &w_photon);
  outtree->Branch("w_phshape", &w_phshape);
  outtree->Branch("w_fakephoton", &w_fakephoton);
  outtree->Branch("w_pu", &w_pu);
  outtree->Branch("photon_index", &index);
  outtree->Branch("photon_pt", &ph_pt_);
  outtree->Branch("photon_eta", &ph_eta_);
  outtree->Branch("photon_phi", &ph_phi_);
  outtree->Branch("photon_energyErr", &err_);
  outtree->Branch("photon_idmva", &idmva_);
  outtree->Branch("mllg", &mllg);
  outtree->Branch("j1_m", &j1_m);
  outtree->Branch("j2_m", &j2_m);
  outtree->Branch("j1_pt", &j1_pt);
  outtree->Branch("j2_pt", &j2_pt);
  outtree->Branch("j1_phi", &j1_phi);
  outtree->Branch("j2_phi", &j2_phi);
  outtree->Branch("j1_eta", &j1_eta);
  outtree->Branch("j2_eta", &j2_eta);
  outtree->Branch("ph_nbdfm", &nbdfm);

  int nproc = 0;
  Long64_t nevent = picotree->GetEntries();
  for (Long64_t i(0); i < nevent; i++){
    ph_id80->clear();
    ph_elveto->clear();
    ph_isScEtaEB->clear();
    ph_isScEtaEE->clear();
    ph_pt->clear();
    ph_eta->clear();
    ph_phi->clear();
    ph_idmva->clear();
    ph_energyErr->clear();
    ph_ishard->clear();
    ll_m->clear();
    ll_pt->clear();
    ll_eta->clear();
    ll_phi->clear();
    jet_isgood->clear();
    jet_pt->clear();
    jet_m->clear();
    jet_phi->clear();
    jet_eta->clear();

    mllg = 0;
    picotree->GetEntry(i);
    
    if (ph_pt->size() < 1) continue;
    if (cat == "vbf" && !(njet >= 2 && nbdfm == 0)) continue;
    int nphoton = 0;
    TLorentzVector j1, j2;
    if (cat == "vbf" && njet >= 2 && nbdfm == 0) {
      int chose_jet = 0;
      for (int k(0); k < jet_m->size(); k++){
	if ((*jet_isgood)[k] && chose_jet == 0){
	  j1.SetPtEtaPhiM((*jet_pt)[k], (*jet_eta)[k], (*jet_phi)[k],(*jet_m)[k]);
	  j1_m = j1.M();
	  j1_pt = j1.Pt();
	  j1_eta = j1.Eta();
	  j1_phi = j1.Phi();
	  chose_jet ++;
	}
	else if ((*jet_isgood)[k] && chose_jet == 1){
	  j2.SetPtEtaPhiM((*jet_pt)[k], (*jet_eta)[k], (*jet_phi)[k],(*jet_m)[k]);
	  j2_m = j2.M();
          j2_pt = j2.Pt();
          j2_eta = j2.Eta();
          j2_phi = j2.Phi();
	  break;
	}
      }
    }
    bool vbf_pass = false;
    for (int j(0); j < ph_pt->size(); j++){
      if ((*ph_pt)[j] > 15. && ((*ph_isScEtaEB)[j] || (*ph_isScEtaEE)[j]) && (*ph_elveto)[j] && (*ph_id80)[j]){
	index = j;
	TLorentzVector g, ll;
	ph_pt_ = (*ph_pt)[j];
	ph_eta_= (*ph_eta)[j]; 
	ph_phi_= (*ph_phi)[j];
	idmva_ = (*ph_idmva)[j];
	err_ = (*ph_energyErr)[j];
	if (nll>0){
	  g.SetPtEtaPhiM((*ph_pt)[j], (*ph_eta)[j], (*ph_phi)[j], 0);
	  ll.SetPtEtaPhiM((*ll_pt)[0], (*ll_eta)[0], (*ll_phi)[0], (*ll_m)[0]);
	  mllg = (g+ll).M();
	}
	if (cat == "vbf" && nll>0) {
	  vbf_pass = true;//j1.DeltaR(g) > 0.4 && j2.DeltaR(g) > 0.4;
	  if (vbf_pass) {
	    outtree->Fill();
	    nphoton++;
	  }
	}
	else if (cat != "vbf" && nll>0) {
	  //if (!(*ph_ishard)[0]) outtree->Fill();
	  outtree->Fill();
	  nphoton++;
	}
      }
    }
    if (nphoton>0) nproc++;
  }
  std::cout << "n proc = " << nproc << std::endl;
  fout->cd();
  outtree->Write();
}
