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

void DY_photon_producer(){

  //auto dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_kingscanyon_v1/2017/mc/merged_zgmc_llg/merged_pico_llg_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_llg_nfiles_153.root", "READ");
  //auto dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_kingscanyon_v1/2018/mc/merged_zgmc_llg/merged_pico_llg_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_llg_nfiles_204.root", "READ");
  //auto dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_kingscanyon_v1/2016/mc/merged_zgmc_llg/merged_pico_llg_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_llg_nfiles_41.root", "READ");
  auto dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_kingscanyon_v1/2016APV/mc/merged_zgmc_llg/merged_pico_llg_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_llg_nfiles_62.root", "READ");

  auto picotree = (TTree*)dyfile->Get("tree");
  auto fout = new TFile("DY_photon_output_kingscanyon_v1_2016APV.root", "RECREATE");
  Long64_t event;
  float w_lumi;
  int npv;
  std::vector<bool> *ph_id80 = new std::vector<bool>();
  std::vector<bool> *ph_elveto = new std::vector<bool>();
  std::vector<bool> *ph_isScEtaEB = new std::vector<bool>();
  std::vector<bool> *ph_isScEtaEE = new std::vector<bool>();
  std::vector<float> *ph_pt = new std::vector<float>();
  std::vector<float> *ph_eta = new std::vector<float>();
  std::vector<float> *ph_phi = new std::vector<float>();
  std::vector<float> *ph_energyErr = new std::vector<float>();
  std::vector<float> *ph_idmva = new std::vector<float>();

  picotree->SetBranchAddress("event", &event);
  picotree->SetBranchAddress("npv", &npv);
  picotree->SetBranchAddress("w_lumi", &w_lumi);
  picotree->SetBranchAddress("photon_pt", &ph_pt);
  picotree->SetBranchAddress("photon_eta", &ph_eta);
  picotree->SetBranchAddress("photon_phi", &ph_phi);
  picotree->SetBranchAddress("photon_id80", &ph_id80);
  picotree->SetBranchAddress("photon_elveto", &ph_elveto);
  picotree->SetBranchAddress("photon_isScEtaEE", &ph_isScEtaEE);
  picotree->SetBranchAddress("photon_isScEtaEB", &ph_isScEtaEB);
  picotree->SetBranchAddress("photon_energyErr", &ph_energyErr);
  picotree->SetBranchAddress("photon_idmva", &ph_idmva);

  auto outtree = new TTree("tree", "tree");
  int index;
  float ph_pt_, ph_eta_, ph_phi_, idmva_, err_;
  outtree->Branch("event", &event);
  outtree->Branch("npv", &npv);
  outtree->Branch("w_lumi", &w_lumi);
  outtree->Branch("photon_index", &index);
  outtree->Branch("photon_pt", &ph_pt_);
  outtree->Branch("photon_eta", &ph_eta_);
  outtree->Branch("photon_phi", &ph_phi_);
  outtree->Branch("photon_energyErr", &err_);
  outtree->Branch("photon_idmva", &idmva_);

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
    picotree->GetEntry(i);
    
    if (ph_pt->size() < 1) continue;
    int nphoton = 0;
    for (int j(0); j < ph_pt->size(); j++){
      if ((*ph_pt)[j] > 15. && ((*ph_isScEtaEB)[j] || (*ph_isScEtaEE)[j]) && (*ph_elveto)[j] && (*ph_id80)[j]){
	nphoton++;
	index = j;
	ph_pt_ = (*ph_pt)[j];
	ph_eta_= (*ph_eta)[j]; 
	ph_phi_= (*ph_phi)[j];
	idmva_ = (*ph_idmva)[j];
	err_ = (*ph_energyErr)[j];
	outtree->Fill();
      }
    }
    if (nphoton>0) nproc++;
  }
  std::cout << "n proc = " << nproc << std::endl;
  fout->cd();
  outtree->Write();
}
