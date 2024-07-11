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
#include "MVAfunc.cxx"

void producer(string year, string lepton) {
//Open files
  string proctype;
  int nyear = 0;
  //  cout << "number = " << dec << endl;
  TChain *picotree = new TChain("tree");
  //if (strcmp(argv.c_str(), "GGF")) {
  TFile *fout1;

  if (year == "2016"){
    nyear = 2016;
    if (lepton == "el")fout1 = new TFile("pico/DY_output_kingscanyon_v1_2016_el.root", "RECREATE");
    else if (lepton == "mu") fout1 = new TFile("pico/DY_output_kingscanyon_v1_2016_mu.root", "RECREATE");
    else throw std::runtime_error("Wrong lepton name.");
    picotree -> Add("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_kingscanyon_v1/2016/mc/merged_zgmc_llg/merged_pico_llg_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_llg_nfiles_41.root");}
  else if (year == "2016APV"){
    nyear = 2016;
    if (lepton == "el")    fout1 = new TFile("pico/DY_output_kingscanyon_v1_2016APV_el.root", "RECREATE");
    else if (lepton == "mu")    fout1 = new TFile("pico/DY_output_kingscanyon_v1_2016APV_mu.root", "RECREATE");
    else throw std::runtime_error("Wrong lepton name.");
    picotree -> Add("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_kingscanyon_v1/2016APV/mc/merged_zgmc_llg/merged_pico_llg_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_llg_nfiles_62.root");}
  else if (year == "2017"){
    nyear = 2017;
    if (lepton == "el") fout1 = new TFile("DY_output_kingscanyon_v1_2017_el.root", "RECREATE");
    //    if (lepton == "el") fout1 = new TFile("SM_output_kingscanyon_v1_2017_el.root", "RECREATE");
    //    if (lepton == "el") fout1 = new TFile("FullSig_output_kingscanyon_v1_2017_el.root", "RECREATE");
    else if (lepton == "mu")    fout1 = new TFile("DY_output_kingscanyon_v1_2017_mu.root", "RECREATE");
    //    else if (lepton == "mu")    fout1 = new TFile("SM_output_kingscanyon_v1_2017_mu.root", "RECREATE");
    //    else if (lepton == "mu")    fout1 = new TFile("FullSig_output_kingscanyon_v1_2017_mu.root", "RECREATE");
    else throw std::runtime_error("Wrong lepton name.");
    picotree -> Add("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_kingscanyon_v1/2017/mc/merged_zgmc_llg/merged_pico_llg_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_llg_nfiles_153.root");
    //picotree -> Add("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_kingscanyon_v1/2017/mc/merged_zgmc_llg/merged_pico_llg_ZGToLLG_01J_5f_lowMLL_lowGPt_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_llg_nfiles_51.root");
    //Signal samples down here
    /*TString direct = "/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_kingscanyon_v1/2017/mc/merged_zgmc_llg/";
    picotree -> Add(direct+"merged_pico_llg_GluGluHToZG_ZToLL_M-125_TuneCP5_13TeV-powheg-pythia8_zgmc_llg_nfiles_5.root");
    picotree -> Add(direct+"merged_pico_llg_VBFHToZG_ZToLL_M-125_TuneCP5_13TeV-powheg-pythia8_zgmc_llg_nfiles_14.root");
    picotree -> Add(direct+"merged_pico_llg_WminusH_HToZG_WToAll_M-125_TuneCP5_13TeV-powheg-pythia8_zgmc_llg_nfiles_11.root");
    picotree -> Add(direct+"merged_pico_llg_WplusH_HToZG_WToAll_M-125_TuneCP5_13TeV-powheg-pythia8_zgmc_llg_nfiles_2.root");
    picotree -> Add(direct+"merged_pico_llg_ZH_HToZG_ZToAll_M-125_TuneCP5_13TeV-powheg-pythia8_zgmc_llg_nfiles_26.root");
    picotree -> Add(direct+"merged_pico_llg_ttHToZG_ZToLL_M-125_TuneCP5_13TeV-powheg-pythia8_zgmc_llg_nfiles_31.root");*/
  }
  else if (year == "2018"){
    nyear = 2018;
    if (lepton == "el")    fout1 = new TFile("pico/DY_output_kingscanyon_v1_2018_el.root", "RECREATE");
    else if (lepton == "mu") fout1 = new TFile("pico/DY_output_kingscanyon_v1_2018_mu.root", "RECREATE");
    else throw std::runtime_error("Wrong lepton name.");
    picotree -> Add("/net/cms11/cms11r0/pico/NanoAODv9/htozgamma_kingscanyon_v1/2018/mc/merged_zgmc_llg/merged_pico_llg_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_zgmc_llg_nfiles_204.root");}
  
  else throw std::runtime_error("Wrong year.");
      
  TTree *outtree = new TTree("tree", "Just a little tree");

    //float llphoton_m, ll_m, photon_mva, pt_mass, photon_res, photon_rapidity, l1_rapidity, l2_rapidity, cosTheta,\
    //costheta, phi, min_dR, max_dR, weight;

//Read old branches from input file
    int nll, nphoton, nel, nmu, njet, nlep;
    int lep;
    Long64_t event; 
    int npv;
    float weight, w_lumi;
    float jj_m, jj_pt, jj_dR, jj_dphi;
    bool el_trig1 = false;
    bool el_trig2 = false;
    bool mu_trig1 = false;
    bool mu_trig2 = false;
    bool mu_trig3 = false;
    bool mu_trig4 = false;
    bool stitch_dy = false;
    bool mu_single1 = false;
    bool el_single1 = false;
    bool mu_single2 = false;
    bool el_single2 = false;
    std::vector<float> *mu_dz = new std::vector<float>();
    std::vector<float> *mu_dxy = new std::vector<float>();
    std::vector<float> *el_dz = new std::vector<float>();
    std::vector<float> *el_dxy = new std::vector<float>();
    std::vector<float> *photon_drmin = new std::vector<float>();
    std::vector<bool> *el_sig = new std::vector<bool>();
    std::vector<bool> *mu_sig = new std::vector<bool>();
    std::vector<bool> *ph_sig = new std::vector<bool>();
    std::vector<bool> *ph_id80 = new std::vector<bool>();
    //std::vector<int> *ll_lepid, *ll_i1, *ll_i2, *el_charge, *mu_charge;
    //std::vector<float> *llphoton_eta, *llphoton_phi, *llphoton_pt, *ll_pt, *ll_eta, *ll_phi, *photon_eta, *photon_phi, *el_pt, *el_eta, *el_phi, *mu_pt, *mu_eta, *mu_phi;
    //std::vector<float> *ll_m, *llphoton_m, *photon_idmva, *photon_pt, *photon_pterr;

    std::vector<int> *ll_lepid = new std::vector<int>();
    std::vector<int> *ll_i1 = new std::vector<int>();
    std::vector<int> *ll_i2 = new std::vector<int>();
    std::vector<int> *el_charge = new std::vector<int>();
    std::vector<int> *mu_charge = new std::vector<int>();
    std::vector<int> *ll_charge = new std::vector<int>();

    std::vector<float> *llphoton_psi = new std::vector<float>();
    std::vector<float> *llphoton_costheta = new std::vector<float>();
    std::vector<float> *llphoton_cosTheta = new std::vector<float>();

    std::vector<float> *llphoton_eta = new std::vector<float>();
    std::vector<float> *llphoton_phi = new std::vector<float>();
    std::vector<float> *llphoton_pt = new std::vector<float>();
    std::vector<float> *ll_pt = new std::vector<float>();
    std::vector<float> *ll_eta = new std::vector<float>();
    std::vector<float> *ll_phi = new std::vector<float>();
    std::vector<float> *photon_eta = new std::vector<float>();
    std::vector<float> *photon_phi = new std::vector<float>();
    std::vector<float> *el_pt = new std::vector<float>();
    std::vector<float> *el_eta = new std::vector<float>();
    std::vector<float> *el_phi = new std::vector<float>();
    std::vector<float> *mu_pt = new std::vector<float>();
    std::vector<float> *mu_eta = new std::vector<float>();
    std::vector<float> *mu_phi = new std::vector<float>();
    std::vector<float> *ll_m = new std::vector<float>();
    std::vector<float> *llphoton_m = new std::vector<float>();
    std::vector<float> *photon_idmva = new std::vector<float>();
    std::vector<float> *photon_pt = new std::vector<float>();
    //    std::vector<float> *photon_pterr = new std::vector<float>();
    std::vector<float> *photon_reliso = new std::vector<float>();
    std::vector<float> *photon_energyErr = new std::vector<float>();

    std::vector<float> *jet_m = new std::vector<float>();
    std::vector<float> *jet_pt = new std::vector<float>();
    std::vector<float> *jet_eta = new std::vector<float>();
    std::vector<float> *jet_phi = new std::vector<float>();
    std::vector<bool> *jet_isgood = new std::vector<bool>();

//Branches for baseline selection
    picotree->SetBranchAddress("w_lumi", &w_lumi);
    picotree->SetBranchAddress("npv", &npv);
    picotree->SetBranchAddress("nll", &nll);
    picotree->SetBranchAddress("nphoton", &nphoton);
    picotree->SetBranchAddress("weight", &weight);
    picotree->SetBranchAddress("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL", &el_trig1);
    picotree->SetBranchAddress("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", &el_trig2);
    picotree->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8", &mu_trig1);
    picotree->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8", &mu_trig2);
    picotree->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ", &mu_trig3);
    picotree->SetBranchAddress("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ", &mu_trig4);
    picotree->SetBranchAddress("HLT_Ele32_WPTight_Gsf_L1DoubleEG", &el_single1);
    picotree->SetBranchAddress("HLT_IsoMu27", &mu_single1); 
    picotree->SetBranchAddress("HLT_Ele27_WPTight_Gsf", &el_single2);
    picotree->SetBranchAddress("HLT_IsoMu24", &mu_single2);
    picotree->SetBranchAddress("use_event", &stitch_dy);
    picotree->SetBranchAddress("nel", &nel);
    picotree->SetBranchAddress("nmu", &nmu);
    picotree->SetBranchAddress("el_dz", &el_dz);
    picotree->SetBranchAddress("el_dxy", &el_dxy);
    picotree->SetBranchAddress("mu_dz", &mu_dz);
    picotree->SetBranchAddress("mu_dxy", &mu_dxy);
    picotree->SetBranchAddress("photon_drmin", &photon_drmin);
    picotree->SetBranchAddress("el_sig", &el_sig);
    picotree->SetBranchAddress("mu_sig", &mu_sig);
    picotree->SetBranchAddress("photon_sig", &ph_sig);
    picotree->SetBranchAddress("nlep", &nlep);
    picotree->SetBranchAddress("njet", &njet);
    picotree->SetBranchAddress("photon_id80", &ph_id80);
    picotree->SetBranchAddress("event", &event);
    picotree->SetBranchAddress("jet_pt", &jet_pt);
    picotree->SetBranchAddress("jet_m", &jet_m);
    picotree->SetBranchAddress("jet_eta", &jet_eta);
    picotree->SetBranchAddress("jet_phi", &jet_phi);
    picotree->SetBranchAddress("jet_isgood", &jet_isgood);

//Branches for MVA variables
    picotree->SetBranchAddress("ll_lepid", &ll_lepid);
    picotree->SetBranchAddress("ll_m", &ll_m);
    picotree->SetBranchAddress("ll_pt", &ll_pt);
    picotree->SetBranchAddress("ll_eta", &ll_eta);
    picotree->SetBranchAddress("ll_phi", &ll_phi);
    picotree->SetBranchAddress("ll_i1", &ll_i1);
    picotree->SetBranchAddress("ll_i2", &ll_i2);
    picotree->SetBranchAddress("ll_charge", &ll_charge);

    picotree->SetBranchAddress("llphoton_m", &llphoton_m);
    picotree->SetBranchAddress("llphoton_pt", &llphoton_pt);
    picotree->SetBranchAddress("llphoton_eta", &llphoton_eta);
    picotree->SetBranchAddress("llphoton_phi", &llphoton_phi);
    picotree->SetBranchAddress("photon_pt", &photon_pt);
    picotree->SetBranchAddress("photon_eta", &photon_eta);
    picotree->SetBranchAddress("photon_phi", &photon_phi);
    picotree->SetBranchAddress("photon_idmva", &photon_idmva);
    picotree->SetBranchAddress("photon_reliso", &photon_reliso);
    picotree->SetBranchAddress("photon_energyErr", &photon_energyErr);
    picotree->SetBranchAddress("llphoton_psi", &llphoton_psi);
    picotree->SetBranchAddress("llphoton_costheta", &llphoton_costheta);
    picotree->SetBranchAddress("llphoton_cosTheta", &llphoton_cosTheta);

    picotree->SetBranchAddress("el_charge", &el_charge);
    picotree->SetBranchAddress("el_pt", &el_pt);
    picotree->SetBranchAddress("el_eta", &el_eta);
    picotree->SetBranchAddress("el_phi", &el_phi);
    picotree->SetBranchAddress("mu_charge", &mu_charge);
    picotree->SetBranchAddress("mu_pt", &mu_pt);
    picotree->SetBranchAddress("mu_eta", &mu_eta);
    picotree->SetBranchAddress("mu_phi", &mu_phi);

//BDT SetUp

    TMVA::Tools::Instance();
    TMVA::Reader *reader = new TMVA::Reader( "V:Color:!Silent" );
    Float_t var[12];
    reader->AddVariable("costheta", &var[0]);
    reader->AddVariable("cosTheta", &var[1]);
    reader->AddVariable("pt_mass", &var[2]);
    reader->AddVariable("l1_rapidity", &var[3]);
    reader->AddVariable("l2_rapidity", &var[4]);
    reader->AddVariable("photon_rapidity", &var[5]);
    reader->AddVariable("phi", &var[6]);
    reader->AddVariable("photon_mva", &var[7]);
    reader->AddVariable("photon_res", &var[8]);
    reader->AddVariable("min_dR", &var[9]);
    reader->AddVariable("max_dR", &var[10]);
    reader->AddVariable("photon_pt_ratio", &var[11]);
    reader->BookMVA("BDT method", "/net/cms37/data1/rui/Training/dataset_input_G_narrow_strict_photon_pt_ratio/weights/TMVAClassification_BDTG.weights.xml");

    float xc_cosTheta, xc_costheta, xc_psi;
//Create new branches for output file
    outtree->Branch("year", &nyear, "year/I");
    outtree->Branch("npv", &npv, "npv/I");
    
    outtree->Branch("photon_pt", "std::vector<float>", &photon_pt);
    outtree->Branch("photon_eta", "std::vector<float>", &photon_eta);
    outtree->Branch("photon_phi", "std::vector<float>", &photon_phi);
    outtree->Branch("photon_idmva", "std::vector<float>", &photon_idmva);
    outtree->Branch("photon_reliso", "std::vector<float>", &photon_reliso);
    outtree->Branch("el_pt", "std::vector<float>", &el_pt);
    outtree->Branch("el_eta", "std::vector<float>", &el_eta);
    outtree->Branch("el_phi", "std::vector<float>", &el_phi);
    outtree->Branch("mu_pt", "std::vector<float>", &mu_pt);
    outtree->Branch("mu_eta", "std::vector<float>", &mu_eta);
    outtree->Branch("mu_phi", "std::vector<float>", &mu_phi);
    outtree->Branch("ll_m", "std::vector<float>", &ll_m);
    outtree->Branch("llphoton_m", "std::vector<float>", &llphoton_m);
    outtree->Branch("llphoton_pt", "std::vector<float>", &llphoton_pt);
    outtree->Branch("llphoton_cosTheta", "std::vector<float>", &llphoton_cosTheta);
    outtree->Branch("llphoton_costheta", "std::vector<float>", &llphoton_costheta);
    outtree->Branch("llphoton_psi", "std::vector<float>", &llphoton_psi);
    outtree->Branch("ll_lepid", "std::vector<int>", &ll_lepid);
    outtree->Branch("photon_drmin", "std::vector<float>", &photon_drmin);
    outtree->Branch("photon_energyErr", "std::vector<float>", &photon_energyErr);
    outtree->Branch("el_dz", "std::vector<float>", &el_dz);
    outtree->Branch("el_dxy", "std::vector<float>", &el_dxy);
    outtree->Branch("mu_dz", "std::vector<float>", &mu_dz);
    outtree->Branch("mu_dxy", "std::vector<float>", &mu_dxy);
    outtree->Branch("el_sig", "std::vector<bool>", &el_sig);
    outtree->Branch("mu_sig", "std::vector<bool>", &mu_sig);
    outtree->Branch("photon_sig", "std::vector<bool>", &ph_sig);

    outtree->Branch("w_lumi", &w_lumi, "w_lumi/F");
    outtree->Branch("weight", &weight, "weight/F");
    outtree->Branch("nphoton", &nphoton, "nphoton/I");
    outtree->Branch("nel", &nel, "nel/I");
    outtree->Branch("nmu", &nmu, "nmu/I");
    outtree->Branch("nll", &nll, "nll/I");
    outtree->Branch("stitch_dy", &stitch_dy, "stitch_dy/O");
    outtree->Branch("ll_i1", "std::vector<int>", &ll_i1);
    outtree->Branch("ll_i2", "std::vector<int>", &ll_i2);
    // outtree->Branch("el_trigger", &output_el_trigger, "el_trigger/O");
    //outtree->Branch("mu_trigger", &output_mu_trigger, "mu_trigger/O");
    outtree->Branch("nlep", &nlep, "nlep/I");
    outtree->Branch("njet", &njet, "njet/I");
    outtree->Branch("event", &event, "event/L");
    outtree->Branch("leptag", &lep, "leptag/I");
    outtree->Branch("xc_cosTheta", &xc_cosTheta);
    outtree->Branch("xc_costheta", &xc_costheta);
    outtree->Branch("xc_psi", &xc_psi);
    outtree->Branch("jj_m", &jj_m);
    outtree->Branch("jj_pt", &jj_pt);
    outtree->Branch("jj_dR", &jj_dR);
    outtree->Branch("jj_dphi", &jj_dphi);

    float bdt_score;
    outtree->Branch("BDT", &bdt_score);
    //outtree->Branch("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL", &el_trig, "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL/O");
    //outtree->Branch("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8", &mu_trig1, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8/O");
    //outtree->Branch("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8", &mu_trig2, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8/O");

 //Run over events
    int nevents_output = 0;
    const Long64_t nevents = picotree->GetEntries();
    for (Long64_t i = 0; i < nevents; i++){
      if (i%100000 == 0) std::cout << "--- ... Processing event: " << i << std::endl;
      mu_dz->clear();
      mu_dxy->clear();
      el_dz->clear();
      el_dxy->clear();
      photon_drmin->clear();
      ll_lepid->clear();
      ll_i1->clear();
      ll_i2->clear();
      ll_charge->clear();
      el_charge->clear();
      mu_charge->clear();
      jet_m->clear();
      jet_pt->clear();
      jet_eta->clear();
      jet_phi->clear();
      jet_isgood->clear();

      llphoton_psi->clear();
      llphoton_costheta->clear();
      llphoton_cosTheta->clear();
      llphoton_eta->clear();
      llphoton_phi->clear();
      llphoton_pt->clear();
      ll_pt->clear();
      ll_eta->clear();
      ll_phi->clear();
      photon_eta->clear();
      photon_phi->clear();
      el_pt->clear();
      el_eta->clear();
      el_phi->clear();
      mu_pt->clear();
      mu_eta->clear();
      mu_phi->clear();
      ll_m->clear();
      llphoton_m->clear();
      photon_idmva->clear();
      photon_pt->clear();
      photon_reliso->clear();
      el_sig->clear();
      mu_sig->clear();
      ph_sig->clear();
      photon_energyErr->clear();

      jj_m = -1.;
      jj_pt = -1.;
      jj_dR = -1.;
      jj_dphi = -1.;
      picotree->GetEntry(i);
      bool el_d_trigs;
      bool mu_d_trigs;
      if (year == "2016") mu_d_trigs = mu_trig3 || mu_trig4;
      else if (year == "2016APV") mu_d_trigs = mu_trig3 || mu_trig4;
      else if (year == "2017") mu_d_trigs = mu_trig1 || mu_trig2;
      else if (year == "2018") mu_d_trigs = mu_trig1 || mu_trig2;
	
      if (year == "2016") el_d_trigs = el_trig2;
      else if (year == "2016APV") el_d_trigs = el_trig2;
      else if (year == "2017") el_d_trigs = el_trig1;
      else if (year == "2018") el_d_trigs = el_trig1;
	
      bool el_s_trigs;
      bool mu_s_trigs;
      
      if (year == "2016" || year == "2016APV") {
	mu_s_trigs = mu_single2;
	el_s_trigs = el_single2;
      }
      else if (year == "2017") {
	mu_s_trigs = mu_single1;
	el_s_trigs = el_single1;
      }
      else if (year == "2018") {
	mu_s_trigs = mu_single2;
        el_s_trigs = el_single1;
      }

      if (nphoton>0 && nll>0 && (el_d_trigs || mu_d_trigs || el_s_trigs || mu_s_trigs) && (*ll_charge)[0] == 0) {
	
	if (year == "2016") {w_lumi *= 19.51; weight *=  19.51;}
	else if (year == "2016APV") {w_lumi *= 16.8; weight *=  16.8;}//36.32264
	//else if (year == "2017") {w_lumi *= 41.52756; weight *= 41.52756;}
	else if (year == "2018") {w_lumi *= 59.67377; weight *= 59.67377;}
	
	double  highx = 180.;
	double lowx = 100.;
	TLorentzVector l1 = AssignL1(*ll_lepid, *ll_i1, *ll_i2, *el_charge, *mu_charge, *el_pt, *el_eta, *el_phi, *mu_pt, *mu_eta, *mu_phi);
	TLorentzVector l2 = AssignL2(*ll_lepid, *ll_i1, *ll_i2, *el_charge, *mu_charge, *el_pt, *el_eta, *el_phi, *mu_pt, *mu_eta, *mu_phi);
	TLorentzVector g = AssignGamma(*photon_pt, *photon_eta, *photon_phi);
	TLorentzVector z = AssignZ(*ll_pt, *ll_eta, *ll_phi, *ll_m);
	TLorentzVector h = AssignH(*llphoton_pt, *llphoton_eta, *llphoton_phi, *llphoton_m);
	TLorentzVector q1 = AssignQ1(h);
	TLorentzVector q2 = AssignQ2(h);

	if ((el_d_trigs && (*ll_lepid)[0] == 11 && (*el_pt)[0] > 25. && (*el_pt)[1] > 15.) || (el_s_trigs && (*ll_lepid)[0] == 11 && (*el_pt)[0] > 35.)){
	  if ((*el_sig)[(*ll_i1)[0]] && (*el_sig)[(*ll_i2)[0]] && (*ph_sig)[0] &&  (*llphoton_m)[0]>lowx && (*llphoton_m)[0] < highx && stitch_dy && ((*llphoton_m)[0]+(*ll_m)[0])>185.0 && (*photon_pt)[0]/(*llphoton_m)[0] > 15./110 && (*ll_m)[0] > 81. && (*ll_m)[0] < 101. && (*ph_id80)[0]){ 
	    lep = 1;
	    if (njet > 1) {
	      int chose_jet = 0;
	      TLorentzVector j1, j2;
	      for (int k(0); k < jet_m->size(); k++){
		if ((*jet_isgood)[k] && chose_jet == 0){
		  j1.SetPtEtaPhiM((*jet_pt)[k], (*jet_eta)[k], (*jet_phi)[k],(*jet_m)[k]);
		  chose_jet ++;
		}
		else if ((*jet_isgood)[k] && chose_jet == 1){
		  j2.SetPtEtaPhiM((*jet_pt)[k], (*jet_eta)[k], (*jet_phi)[k],(*jet_m)[k]);
		  break;
		}
	      }
	      TLorentzVector jj = j1 + j2;
	      jj_m = jj.M();
	      jj_pt = jj.Pt();
	      jj_dR = j1.DeltaR(j2);
	      jj_dphi = j1.DeltaPhi(j2);
	    }

	    if (lepton == "el") {
	      var[0] = (*llphoton_costheta)[0];
	      var[1] = (*llphoton_cosTheta)[0];
	      var[2] = (*llphoton_pt)[0]/ (*llphoton_m)[0];
	      var[3] = l1.Eta();
	      var[4] = l2.Eta();
	      var[5] = (*photon_eta)[0];
	      var[6] = (*llphoton_psi)[0];
	      var[7] = (*photon_idmva)[0];
	      var[8] = (*photon_energyErr)[0]/TMath::CosH((*photon_eta)[0])/(*photon_pt)[0];
	      var[9] = GetmindR(l1, l2, g);
	      var[10] = GetmaxdR(l1, l2, g);
	      var[11] = (*photon_pt)[0]/(*llphoton_m)[0];
	      bdt_score = (float)reader -> EvaluateMVA("BDT method");
	      xc_costheta = cos_theta(h, z, l1, l2);
              xc_cosTheta = cos_Theta(h, z, q1, q2);
	      xc_psi = Getphi(q1, q2, z, l1, l2, h);
	      outtree->Fill(); nevents_output++;}
	  }}
	if ((mu_d_trigs && (*ll_lepid)[0] == 13 && (*mu_pt)[0] > 20. && (*mu_pt)[1] > 10.) || (mu_s_trigs && (*ll_lepid)[0] == 13 && (*mu_pt)[0] > 28.)){
	  if ((*mu_sig)[(*ll_i1)[0]] && (*mu_sig)[(*ll_i2)[0]] && (*ph_sig)[0] && (*llphoton_m)[0]>lowx && (*llphoton_m)[0] < highx && stitch_dy && ((*llphoton_m)[0]+(*ll_m)[0])>185.0 && (*photon_pt)[0]/(*llphoton_m)[0] > 15./110 && (*ll_m)[0] > 81. && (*ll_m)[0] < 101. && (*ph_id80)[0]){ 
	    lep = 2;
	    if (njet > 1) {
	      int chose_jet = 0;
              TLorentzVector j1, j2;
              for (int k(0); k < jet_m->size(); k++){
                if ((*jet_isgood)[k] && chose_jet == 0){
                  j1.SetPtEtaPhiM((*jet_pt)[k], (*jet_eta)[k], (*jet_phi)[k],(*jet_m)[k]);
                  chose_jet ++;
                }
                else if ((*jet_isgood)[k] && chose_jet == 1){
                  j2.SetPtEtaPhiM((*jet_pt)[k], (*jet_eta)[k], (*jet_phi)[k],(*jet_m)[k]);
                  break;
                }
              }
              TLorentzVector jj = j1 + j2;
              jj_m = jj.M();
              jj_pt = jj.Pt();
              jj_dR = j1.DeltaR(j2);
              jj_dphi = j1.DeltaPhi(j2);
            }

	    if (lepton == "mu") {
	      var[0] = (*llphoton_costheta)[0];
              var[1] = (*llphoton_cosTheta)[0];
              var[2] = (*llphoton_pt)[0]/ (*llphoton_m)[0];
              var[3] = l1.Eta();
              var[4] = l2.Eta();
              var[5] = (*photon_eta)[0];
              var[6] = (*llphoton_psi)[0];
              var[7] = (*photon_idmva)[0];
              var[8] = (*photon_energyErr)[0]/TMath::CosH((*photon_eta)[0])/(*photon_pt)[0];
              var[9] = GetmindR(l1, l2, g);
              var[10] = GetmaxdR(l1, l2, g);
              var[11] = (*photon_pt)[0]/(*llphoton_m)[0];
              bdt_score = (float)reader -> EvaluateMVA("BDT method");
	      xc_costheta = cos_theta(h, z, l1, l2);
              xc_cosTheta = cos_Theta(h, z, q1, q2);
              xc_psi = Getphi(q1, q2, z, l1, l2, h);
	      outtree->Fill(); nevents_output++;}
	    
	  }
	}
      }
      
    }

    std::cout << std::endl;
    std::cout <<"N events = " << nevents_output << std::endl;
    fout1->Write();
    fout1->Close();

}
