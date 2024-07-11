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

void DY_Z_producer(){

  auto dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_kingscanyon_v1/2017/mc/merged_zgmc_ll/merged_pico_ll_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_ll_nfiles_153.root", "READ");
  auto picotree = (TTree*)dyfile->Get("tree");
  auto fout = new TFile("DY_Z_output_kingscanyon_v1_2017.root", "RECREATE");
  Long64_t event;
  bool mu_trig1, mu_trig2, el_trig, mu_single, el_single;
  int nll, njet;
  float w_lumi;
  std::vector<int> *ll_lepid = new std::vector<int>();
  std::vector<int> *ll_i1 = new std::vector<int>();
  std::vector<int> *ll_i2 = new std::vector<int>();
  std::vector<float> *ll_m = new std::vector<float>();
  std::vector<float> *ll_pt = new std::vector<float>();
  std::vector<float> *ll_eta = new std::vector<float>();
  std::vector<float> *ll_phi = new std::vector<float>();
  std::vector<int> *ll_charge = new std::vector<int>();
  std::vector<float> *el_pt = new std::vector<float>();
  std::vector<float> *el_eta = new std::vector<float>();
  std::vector<float> *el_phi = new std::vector<float>();
  std::vector<float> *mu_pt = new std::vector<float>();
  std::vector<float> *mu_eta = new std::vector<float>();
  std::vector<float> *mu_phi = new std::vector<float>();
  std::vector<int> *el_charge = new std::vector<int>();
  std::vector<int> *mu_charge = new std::vector<int>();
  std::vector<float> *jet_pt = new std::vector<float>();
  std::vector<float> *jet_m = new std::vector<float>();
  std::vector<float> *jet_eta = new std::vector<float>();
  std::vector<float> *jet_phi = new std::vector<float>();
  std::vector<bool> *jet_isgood = new std::vector<bool>();

  picotree->SetBranchAddress("event", &event);
  picotree->SetBranchAddress("njet", &njet);
  picotree->SetBranchAddress("w_lumi", &w_lumi);
  picotree->SetBranchAddress("el_charge", &el_charge);
  picotree->SetBranchAddress("el_pt", &el_pt);
  picotree->SetBranchAddress("el_eta", &el_eta);
  picotree->SetBranchAddress("el_phi", &el_phi);
  picotree->SetBranchAddress("mu_charge", &mu_charge);
  picotree->SetBranchAddress("mu_pt", &mu_pt);
  picotree->SetBranchAddress("mu_eta", &mu_eta);
  picotree->SetBranchAddress("mu_phi", &mu_phi);
  picotree->SetBranchAddress("jet_m", &jet_m);
  picotree->SetBranchAddress("jet_pt", &jet_pt);
  picotree->SetBranchAddress("jet_eta", &jet_eta);
  picotree->SetBranchAddress("jet_phi", &jet_phi);
  picotree->SetBranchAddress("nll", &nll);
  picotree->SetBranchAddress("ll_i1", &ll_i1);
  picotree->SetBranchAddress("ll_i2", &ll_i2);
  picotree->SetBranchAddress("ll_charge", &ll_charge);
  picotree->SetBranchAddress("ll_lepid", &ll_lepid);
  picotree->SetBranchAddress("ll_m", &ll_m);
  picotree->SetBranchAddress("ll_pt", &ll_pt);
  picotree->SetBranchAddress("ll_eta", &ll_eta);
  picotree->SetBranchAddress("ll_phi", &ll_phi);
  picotree->SetBranchAddress("jet_isgood", &jet_isgood);
  //  picotree->SetBranchAddress("use_event", &use);

  picotree->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8", &mu_trig1);
  picotree->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8", &mu_trig2);
  picotree->SetBranchAddress("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL", &el_trig);
  picotree->SetBranchAddress("HLT_Ele32_WPTight_Gsf_L1DoubleEG", &el_single);
  picotree->SetBranchAddress("HLT_IsoMu27", &mu_single);

  auto outtree = new TTree("tree", "tree");
  float ll_m_, ll_pt_, ll_eta_, ll_phi_, l1_pt_, l1_eta_, l1_phi_, l1_m_, l2_eta_, l2_phi_, l2_m_, l2_pt_;
  outtree->Branch("w_lumi", &w_lumi, "w_lumi/F");
  outtree->Branch("event", &event);
  outtree->Branch("njet", &njet);
  outtree->Branch("ll_m", &ll_m_);
  outtree->Branch("ll_pt", &ll_pt_);
  outtree->Branch("ll_eta", &ll_eta_);
  outtree->Branch("ll_phi", &ll_phi_);
  outtree->Branch("l1_m", &l1_m_);
  outtree->Branch("l1_pt", &l1_pt_);
  outtree->Branch("l1_eta", &l1_eta_);
  outtree->Branch("l1_phi", &l1_phi_);
  outtree->Branch("l2_m", &l2_m_);
  outtree->Branch("l2_pt", &l2_pt_);
  outtree->Branch("l2_eta", &l2_eta_);
  outtree->Branch("l2_phi", &l2_phi_);
  outtree->Branch("jet_m", &jet_m);
  outtree->Branch("jet_pt", &jet_pt);
  outtree->Branch("jet_eta", &jet_eta);
  outtree->Branch("jet_phi", &jet_phi);
  outtree->Branch("jet_isgood", &jet_isgood);

  bool mu_d_trigs, el_d_trigs;
  int nproc = 0;
  Long64_t nevent = picotree->GetEntries();
  for (Long64_t i(0); i < nevent; i++){
    ll_m->clear();
    ll_pt->clear();
    ll_eta->clear();
    ll_phi->clear();
    ll_i1->clear();
    ll_i2->clear();
    el_pt->clear();
    mu_pt->clear();
    el_eta->clear();
    mu_eta->clear();
    el_phi->clear();
    mu_phi->clear();
    el_charge->clear();
    mu_charge->clear();
    ll_charge->clear();
    ll_lepid->clear();
    jet_m->clear();
    jet_pt->clear();
    jet_eta->clear();
    jet_phi->clear();
    jet_isgood->clear();

    picotree->GetEntry(i);
    
    el_d_trigs = el_trig;
    mu_d_trigs = mu_trig1 || mu_trig2;
    if (nll>0 && (el_d_trigs || mu_d_trigs || el_single || mu_single) && (*ll_charge)[0] == 0){
      TLorentzVector l1 = AssignL1(*ll_lepid, *ll_i1, *ll_i2, *el_charge, *mu_charge, *el_pt, *el_eta, *el_phi, *mu_pt, *mu_eta, *mu_phi);
      TLorentzVector l2 = AssignL2(*ll_lepid, *ll_i1, *ll_i2, *el_charge, *mu_charge, *el_pt, *el_eta, *el_phi, *mu_pt, *mu_eta, *mu_phi);
      if ((el_d_trigs && (*ll_lepid)[0] == 11 && (*el_pt)[0] > 25. && (*el_pt)[1] > 15.) || (el_single && (*ll_lepid)[0] == 11 && (*el_pt)[0] > 35.)){
	if ((*ll_m)[0] > 80. && (*ll_m)[0] < 100. ){
	  ll_m_ = (*ll_m)[0];
	  ll_pt_ = (*ll_pt)[0];
	  ll_eta_ = (*ll_eta)[0];
	  ll_phi_ = (*ll_phi)[0];
	  l1_m_ = l1.M();
	  l1_pt_ = l1.Pt();
	  l1_eta_ = l1.Eta();
	  l1_phi_ = l1.Phi();
	  l2_m_ = l2.M();
          l2_pt_ = l2.Pt();
          l2_eta_ = l2.Eta();
          l2_phi_ = l2.Phi();
	  outtree->Fill();
	}
      }
      if ((mu_d_trigs && (*ll_lepid)[0] == 13 && (*mu_pt)[0] > 20. && (*mu_pt)[1] > 10.) || (mu_single && (*ll_lepid)[0] == 13 && (*mu_pt)[0] > 28.)){
        if ((*ll_m)[0] > 80. && (*ll_m)[0] < 100. ){
          ll_m_ = (*ll_m)[0];
          ll_pt_ = (*ll_pt)[0];
          ll_eta_ = (*ll_eta)[0];
          ll_phi_ = (*ll_phi)[0];
          l1_m_ = l1.M();
          l1_pt_ = l1.Pt();
          l1_eta_ = l1.Eta();
          l1_phi_ = l1.Phi();
          l2_m_ = l2.M();
          l2_pt_ = l2.Pt();
          l2_eta_ = l2.Eta();
          l2_phi_ = l2.Phi();
          outtree->Fill();
        }
      }
    }
  }
  fout->cd();
  outtree->Write();
}
