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

void DY_Z_producer_V2(string year_){
  TFile *dyfile;
  TFile *fout;
  if (year_ == "2017"){
    dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_redwood_v1/2017/mc/merged_zgmc_ll/merged_pico_ll_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_ll_nfiles_153.root", "READ");
    fout = new TFile("pico/DY_Z_output_redwood_v1_2017.root", "RECREATE");
  }
  else if(year_ == "2018"){
    dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_redwood_v1/2018/mc/merged_zgmc_ll/merged_pico_ll_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_ll_nfiles_204.root", "READ");
    fout = new TFile("pico/DY_Z_output_redwood_v1_2018.root", "RECREATE");
  }
  else if (year_ == "2016"){
    dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_redwood_v1/2016/mc/merged_zgmc_ll/merged_pico_ll_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_ll_nfiles_41.root", "READ");
    fout = new TFile("pico/DY_Z_output_redwood_v1_2016.root", "RECREATE");
  }
  else if (year_ == "2016APV"){
    dyfile = new TFile("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_redwood_v1/2016APV/mc/merged_zgmc_ll/merged_pico_ll_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_ll_nfiles_62.root", "READ");
    fout = new TFile("pico/DY_Z_output_redwood_v1_2016APV.root", "RECREATE");
  }
  else throw std::runtime_error("Wrong year.");
  auto picotree = (TTree*)dyfile->Get("tree");
  Long64_t event;
  //  bool mu_trig1, mu_trig2, el_trig, mu_single, el_single;

  bool mu_trig = false;
  bool el_trig = false;
  bool pass;
  int nbdfm;
  float met;
  int nll, njet, nel, nmu, nlep;
  float w_lumi, weight;
  float w_lep, w_fs_lep, w_btag_df, w_trig, w_prefire;
  float dijet_pt, dijet_m, dijet_eta, dijet_phi, dijet_dr, dijet_deta, dijet_dphi;
  int bitmap = 0;
  float ll_refit_pt, ll_refit_eta, ll_refit_phi, ll_refit_m, ll_refit_l1_pt, ll_refit_l2_pt;
  std::vector<int> *ll_lepid = new std::vector<int>();
  std::vector<float> *jet_qgl = new std::vector<float>();
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
  std::vector<float> *mu_dz = new std::vector<float>();
  std::vector<float> *el_dz = new std::vector<float>();
  std::vector<float> *mu_dxy = new std::vector<float>();
  std::vector<float> *el_dxy = new std::vector<float>();
  //  std::vector<int> *mu_charge = new std::vector<int>();
  //  std::vector<int> *el_charge = new std::vector<int>();
  picotree->SetBranchAddress("w_lep", &w_lep);
  picotree->SetBranchAddress("w_fs_lep", &w_fs_lep);
  picotree->SetBranchAddress("w_btag_df", &w_btag_df);
  picotree->SetBranchAddress("w_trig", &w_trig);
  picotree->SetBranchAddress("w_prefire", &w_prefire);
  picotree->SetBranchAddress("ll_refit_pt", &ll_refit_pt);
  picotree->SetBranchAddress("ll_refit_eta", &ll_refit_eta);
  picotree->SetBranchAddress("ll_refit_phi", &ll_refit_phi);
  picotree->SetBranchAddress("ll_refit_m", &ll_refit_m);
  picotree->SetBranchAddress("ll_refit_l1_pt", &ll_refit_l1_pt);
  picotree->SetBranchAddress("ll_refit_l2_pt", &ll_refit_l2_pt);
  picotree->SetBranchAddress("zg_cutBitMap", &bitmap);
  picotree->SetBranchAddress("trig_mu_pt", &mu_trig);
  picotree->SetBranchAddress("trig_el_pt", &el_trig);
  picotree->SetBranchAddress("pass", &pass);
  picotree->SetBranchAddress("nbdfm", &nbdfm);
  picotree->SetBranchAddress("met", &met);
  picotree->SetBranchAddress("nlep", &nlep);
  picotree->SetBranchAddress("jet_qgl", &jet_qgl);
  picotree->SetBranchAddress("event", &event);
  picotree->SetBranchAddress("njet", &njet);
  picotree->SetBranchAddress("nll", &nll);
  picotree->SetBranchAddress("nel", &nel);
  picotree->SetBranchAddress("nmu", &nmu);
  picotree->SetBranchAddress("w_lumi", &w_lumi);
  picotree->SetBranchAddress("weight", &weight);
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

  /*
  picotree->SetBranchAddress("mu_dz", &mu_dz);
  picotree->SetBranchAddress("el_dz", &el_dz);
  picotree->SetBranchAddress("mu_dxy", &mu_dxy);
  picotree->SetBranchAddress("el_dxy", &el_dxy);*/
  picotree->SetBranchAddress("mu_charge", &mu_charge);
  picotree->SetBranchAddress("el_charge", &el_charge);
  picotree->SetBranchAddress("dijet_pt", &dijet_pt);
  picotree->SetBranchAddress("dijet_eta", &dijet_eta);
  picotree->SetBranchAddress("dijet_phi", &dijet_phi);
  picotree->SetBranchAddress("dijet_m", &dijet_m);
  picotree->SetBranchAddress("dijet_dr", &dijet_dr);
  picotree->SetBranchAddress("dijet_deta", &dijet_deta);
  picotree->SetBranchAddress("dijet_dphi", &dijet_dphi);

  auto outtree = new TTree("tree", "tree");
  float ll_m_, ll_pt_, ll_eta_, ll_phi_, l1_pt_, l1_eta_, l1_phi_, l1_m_, l2_eta_, l2_phi_, l2_m_, l2_pt_;
  int l1_charge_, l2_charge_;
  outtree->Branch("weight", &weight, "weight/F");
  outtree->Branch("w_lumi", &w_lumi, "w_lumi/F");
  outtree->Branch("w_lep", &w_lep);
  outtree->Branch("w_fs_lep", &w_fs_lep);
  outtree->Branch("w_btag_df", &w_btag_df);
  outtree->Branch("w_trig", &w_trig);
  outtree->Branch("w_prefire", &w_prefire);
  outtree->Branch("z_weight", &weight);
  outtree->Branch("event", &event);
  outtree->Branch("zg_cutBitMap", &bitmap);
  outtree->Branch("njet", &njet);
  outtree->Branch("nll", &nll);
  outtree->Branch("nel", &nel);
  outtree->Branch("nmu", &nmu);
  outtree->Branch("nlep", &nlep);
  outtree->Branch("ll_m", &ll_m);
  outtree->Branch("ll_pt", &ll_pt);
  outtree->Branch("ll_eta", &ll_eta);
  outtree->Branch("ll_phi", &ll_phi);
  outtree->Branch("ll_lepid", &ll_lepid);
  outtree->Branch("ll_i1", &ll_i1);
  outtree->Branch("ll_i2", &ll_i2);
  outtree->Branch("el_charge", &el_charge);
  outtree->Branch("el_pt", &el_pt);
  outtree->Branch("el_eta", &el_eta);
  outtree->Branch("el_phi", &el_phi);
  outtree->Branch("mu_charge", &mu_charge);
  outtree->Branch("mu_pt", &mu_pt);
  outtree->Branch("mu_eta", &mu_eta);
  outtree->Branch("mu_phi", &mu_phi);
  outtree->Branch("lead_ll_m", &ll_m_);
  outtree->Branch("lead_ll_pt", &ll_pt_);
  outtree->Branch("lead_ll_eta", &ll_eta_);
  outtree->Branch("lead_ll_phi", &ll_phi_);
  outtree->Branch("l1_m", &l1_m_);
  outtree->Branch("l1_pt", &l1_pt_);
  outtree->Branch("l1_eta", &l1_eta_);
  outtree->Branch("l1_phi", &l1_phi_);
  outtree->Branch("l1_charge", &l1_charge_);
  outtree->Branch("l2_m", &l2_m_);
  outtree->Branch("l2_pt", &l2_pt_);
  outtree->Branch("l2_eta", &l2_eta_);
  outtree->Branch("l2_phi", &l2_phi_);
  outtree->Branch("l2_charge", &l2_charge_);
  outtree->Branch("jet_m", &jet_m);
  outtree->Branch("jet_pt", &jet_pt);
  outtree->Branch("jet_eta", &jet_eta);
  outtree->Branch("jet_phi", &jet_phi);
  outtree->Branch("jet_isgood", &jet_isgood);
  outtree->Branch("dijet_pt", &dijet_pt);
  outtree->Branch("dijet_eta", &dijet_eta);
  outtree->Branch("dijet_phi", &dijet_phi);
  outtree->Branch("dijet_m", &dijet_m);
  outtree->Branch("dijet_dr", &dijet_dr);
  outtree->Branch("dijet_deta", &dijet_deta);
  outtree->Branch("dijet_dphi", &dijet_dphi);
  outtree->Branch("pass", &pass);
  outtree->Branch("nbdfm", &nbdfm);
  outtree->Branch("met", &met);
  outtree->Branch("nlep", &nlep);
  outtree->Branch("jet_qgl", &jet_qgl);
  outtree->Branch("ll_refit_pt", &ll_refit_pt);
  outtree->Branch("ll_refit_eta", &ll_refit_eta);
  outtree->Branch("ll_refit_phi", &ll_refit_phi);
  outtree->Branch("ll_refit_m", &ll_refit_m);
  outtree->Branch("ll_refit_l1_pt", &ll_refit_l1_pt);
  outtree->Branch("ll_refit_l2_pt", &ll_refit_l2_pt);

  bool mu_d_trigs, el_d_trigs, el_s_trigs, mu_s_trigs;
  int nproc = 0;
  Long64_t nevent = picotree->GetEntries();
  for (Long64_t i(0); i < nevent; i++){
    if (i%10000 == 0) std::cout << "Processing " << i << std::endl;
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
    jet_qgl->clear();
    picotree->GetEntry(i);

    if ((bitmap&(1<<11)) && (bitmap&(1<<8)) && (bitmap&(1<<7))){
      TLorentzVector l1 = AssignL1(*ll_lepid, *ll_i1, *ll_i2, *el_charge, *mu_charge, *el_pt, *el_eta, *el_phi, *mu_pt, *mu_eta, *mu_phi);
      TLorentzVector l2 = AssignL2(*ll_lepid, *ll_i1, *ll_i2, *el_charge, *mu_charge, *el_pt, *el_eta, *el_phi, *mu_pt, *mu_eta, *mu_phi);
      if ((*ll_lepid)[0] == 11){
	if ((*ll_m)[0] > 80. && (*ll_m)[0] < 100. ){
	  ll_m_ = (*ll_m)[0];
	  ll_pt_ = (*ll_pt)[0];
	  ll_eta_ = (*ll_eta)[0];
	  ll_phi_ = (*ll_phi)[0];
	  l1_m_ = 0.00511;
	  l1_pt_ = l1.Pt();//(*el_pt)[(*ll_i1)[0]];
	  l1_eta_ = l1.Eta();//(*el_eta)[(*ll_i1)[0]];
	  l1_phi_ = l1.Phi();//(*el_phi)[(*ll_i1)[0]];
	  l1_charge_ = -1;//(*el_charge)[(*ll_i1)[0]];
	  l2_m_ = 0.00511;
          l2_pt_ = l2.Pt();//(*el_pt)[(*ll_i2)[0]];
          l2_eta_ = l2.Eta();(*el_eta)[(*ll_i2)[0]];
          l2_phi_ = l2.Phi();//(*el_phi)[(*ll_i2)[0]];
	  l2_charge_ = 1; //(*el_charge)[(*ll_i2)[0]];
	  outtree->Fill();
	}
      }
      if ((*ll_lepid)[0] == 13) {
        if ((*ll_m)[0] > 80. && (*ll_m)[0] < 100. ){
          ll_m_ = (*ll_m)[0];
          ll_pt_ = (*ll_pt)[0];
          ll_eta_ = (*ll_eta)[0];
          ll_phi_ = (*ll_phi)[0];
          l1_m_ = 0.105;
          l1_pt_ = l1.Pt();//(*el_pt)[(*ll_i1)[0]];
          l1_eta_ = l1.Eta();//(*el_eta)[(*ll_i1)[0]];
          l1_phi_ = l1.Phi();//(*el_phi)[(*ll_i1)[0]];
          l1_charge_ = -1;//(*el_charge)[(*ll_i1)[0]];
          l2_m_ = 0.105;

          l2_pt_ = l2.Pt();//(*el_pt)[(*ll_i2)[0]];
          l2_eta_ = l2.Eta();(*el_eta)[(*ll_i2)[0]];
          l2_phi_ = l2.Phi();//(*el_phi)[(*ll_i2)[0]];
          l2_charge_ = 1; //(*el_charge)[(*ll_i2)[0]];
	  outtree->Fill();
        }
      }
    }
    //if (i>100000) break;
  }
  fout->cd();
  outtree->Write();
}
