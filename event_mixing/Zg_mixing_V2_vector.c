#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#include "MVAfunc.cxx"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TVector3.h"

void Zg_mixing_V2_vector(TString inphoton_, TString inZ_, TString outfile_, TString cat){
  auto ph_file = new TFile("sample/" + inphoton_,"READ");
  auto z_file = new TFile("sample/" + inZ_,"READ");
  auto outfile = new TFile("sample/"+ outfile_, "RECREATE");
  auto phtree = (TTree*)ph_file->Get("tree");
  auto ztree = (TTree*)z_file->Get("tree");
  auto outtree = new TTree("tree", "tree");

  int event;
  Long64_t zevent;
  int npv, njet, nll, nel, nmu, nlep;
  float z_w_lumi, ph_w_lumi,weight, yr_weight, z_weight;
  float l1_pt, l1_eta, l1_phi, l1_m, l2_pt, l2_eta, l2_phi, l2_m;
  float photon_pt_, photon_eta_, photon_phi_, photon_idmva_, photon_energyErr_;
  //float photon_jet1_dr, photon_jet2_dr, photon_zeppenfeld; //Rui new vars
  float decorr_photon_pt, photon_ptErr;
  float jj_m, jj_dphi, jj_dR, jj_pt, jj_phi, jj_eta, jj_deta;
  float ph_mllg;
  int l1_charge, l2_charge;
  bool pass;
  int nbdfm;
  float met, met_phi, met_calo, met_tru;
  float w_lep, w_fs_lep, w_btag_df, w_trig, w_pu, w_prefire, w_photon, w_phshape, w_fakephoton;
  float ll_refit_m, ll_refit_pt, ll_refit_eta, ll_refit_phi, ll_refit_l1_pt, ll_refit_l2_pt;
  float llphoton_refit_m, llphoton_refit_pt, llphoton_refit_eta, llphoton_refit_phi;
  if(outfile_.Contains("2017")){
    yr_weight = 41.52756;
  }
  else if(outfile_.Contains("2016") && !outfile_.Contains("APV")){
    yr_weight = 16.8;
  }
  else if (outfile_.Contains("2016APV")) {
    yr_weight =  19.51;
  }
  else if (outfile_.Contains("2018")){
    yr_weight = 59.67377;
  }
  
  std::vector<float> *jet_pt = new std::vector<float>();
  std::vector<float> *jet_m = new std::vector<float>();
  std::vector<float> *jet_eta = new std::vector<float>();
  std::vector<float> *jet_phi = new std::vector<float>();
  std::vector<float> *jet_qgl = new std::vector<float>();
  std::vector<bool> *jet_isgood = new std::vector<bool>();

  std::vector<int> *ll_lepid = new std::vector<int>();
  std::vector<int> *ll_i1 = new std::vector<int>();
  std::vector<int> *ll_i2 = new std::vector<int>();
  std::vector<float> *ll_m = new std::vector<float>();
  std::vector<float> *ll_pt = new std::vector<float>();
  std::vector<float> *ll_eta = new std::vector<float>();
  std::vector<float> *ll_phi = new std::vector<float>();
  std::vector<float> *el_pt = new std::vector<float>();
  std::vector<float> *el_eta = new std::vector<float>();
  std::vector<float> *el_phi = new std::vector<float>();
  std::vector<float> *mu_pt = new std::vector<float>();
  std::vector<float> *mu_eta = new std::vector<float>();
  std::vector<float> *mu_phi = new std::vector<float>();
  std::vector<int> *el_charge = new std::vector<int>();
  std::vector<int> *mu_charge = new std::vector<int>();

  std::vector<float> *photon_pt = new std::vector<float>();
  std::vector<float> *photon_eta = new std::vector<float>();
  std::vector<float> *photon_phi = new std::vector<float>();
  std::vector<float> *photon_idmva = new std::vector<float>();
  std::vector<float> *photon_drmin = new std::vector<float>();
  std::vector<float> *photon_drmax = new std::vector<float>();
  std::vector<float> *photon_energyErr = new std::vector<float>();
  std::vector<float> *photon_jet1_dr = new std::vector<float>();
  std::vector<float> *photon_jet2_dr = new std::vector<float>();
  std::vector<float> *photon_zeppenfeld = new std::vector<float>();

  std::vector<float> *llphoton_pt = new std::vector<float>();
  std::vector<float> *llphoton_eta = new std::vector<float>();
  std::vector<float> *llphoton_phi = new std::vector<float>();
  std::vector<float> *llphoton_m = new std::vector<float>();
  std::vector<int> *llphoton_iph = new std::vector<int>();
  std::vector<float> *llphoton_cosTheta = new std::vector<float>();
  std::vector<float> *llphoton_costheta = new std::vector<float>();
  std::vector<float> *llphoton_psi = new std::vector<float>();
  std::vector<float> *llphoton_pTt = new std::vector<float>();
  std::vector<float> *llphoton_pTt_an_hig019014 = new std::vector<float>();

  std::vector<float> *llphoton_dijet_dphi = new std::vector<float>();
  std::vector<float> *llphoton_dijet_balance = new std::vector<float>();
  std::vector<float> *llphoton_dijet_dr = new std::vector<float>();


  
  phtree->SetBranchAddress("photon_event", &event);
  phtree->SetBranchAddress("w_lumi", &ph_w_lumi);
  phtree->SetBranchAddress("w_photon", &w_photon);
  phtree->SetBranchAddress("w_fakephoton", &w_fakephoton);
  phtree->SetBranchAddress("w_phshape", &w_phshape);
  phtree->SetBranchAddress("w_pu", &w_pu);
  phtree->SetBranchAddress("photon_pt", &photon_pt_);
  phtree->SetBranchAddress("photon_idmva", &photon_idmva_);
  phtree->SetBranchAddress("photon_energyErr", &photon_energyErr_);
  phtree->SetBranchAddress("photon_phi", &photon_phi_);
  phtree->SetBranchAddress("photon_eta", &photon_eta_);
  phtree->SetBranchAddress("npv", &npv);
  phtree->SetBranchAddress("met", &met);
  phtree->SetBranchAddress("met_phi", &met_phi);
  phtree->SetBranchAddress("met_calo", &met_calo);
  phtree->SetBranchAddress("met_tru", &met_tru);
  //phtree->SetBranchAddress("llphoton_m", &ph_mllg);
  ztree->SetBranchAddress("pass", &pass);
  ztree->SetBranchAddress("nlep", &nlep);
  ztree->SetBranchAddress("nbdfm", &nbdfm);
  ztree->SetBranchAddress("jet_qgl", &jet_qgl);
  ztree->SetBranchAddress("w_lumi", &z_w_lumi);
  ztree->SetBranchAddress("weight", &z_weight);
  //ztree->SetBranchAddress("weight", &weight);
  ztree->SetBranchAddress("w_lep", &w_lep);
  ztree->SetBranchAddress("w_fs_lep", &w_fs_lep);
  ztree->SetBranchAddress("w_btag_df", &w_btag_df);
  ztree->SetBranchAddress("w_trig", &w_trig);
  ztree->SetBranchAddress("w_prefire", &w_prefire);
  ztree->SetBranchAddress("event", &zevent);
  ztree->SetBranchAddress("njet", &njet);
  ztree->SetBranchAddress("nll", &nll);
  ztree->SetBranchAddress("nmu", &nmu);
  ztree->SetBranchAddress("nel", &nel);
  ztree->SetBranchAddress("ll_m", &ll_m);
  ztree->SetBranchAddress("ll_pt", &ll_pt);
  ztree->SetBranchAddress("ll_phi", &ll_phi);
  ztree->SetBranchAddress("ll_eta", &ll_eta);
  
  ztree->SetBranchAddress("ll_refit_m", &ll_refit_m);
  ztree->SetBranchAddress("ll_refit_pt", &ll_refit_pt);
  ztree->SetBranchAddress("ll_refit_phi", &ll_refit_phi);
  ztree->SetBranchAddress("ll_refit_eta", &ll_refit_eta);
  ztree->SetBranchAddress("ll_refit_l1_pt", &ll_refit_l1_pt);
  ztree->SetBranchAddress("ll_refit_l2_pt", &ll_refit_l2_pt);
  
  ztree->SetBranchAddress("ll_i1", &ll_i1);
  ztree->SetBranchAddress("ll_i2", &ll_i2);
  ztree->SetBranchAddress("ll_lepid", &ll_lepid);
  ztree->SetBranchAddress("jet_pt", &jet_pt);
  ztree->SetBranchAddress("jet_m", &jet_m);
  ztree->SetBranchAddress("jet_eta", &jet_eta);
  ztree->SetBranchAddress("jet_phi", &jet_phi);
  ztree->SetBranchAddress("jet_isgood", &jet_isgood);
  ztree->SetBranchAddress("l1_m", &l1_m);
  ztree->SetBranchAddress("l1_pt", &l1_pt);
  ztree->SetBranchAddress("l1_eta", &l1_eta);
  ztree->SetBranchAddress("l1_phi", &l1_phi);
  ztree->SetBranchAddress("l1_charge", &l1_charge);
  ztree->SetBranchAddress("l2_m", &l2_m);
  ztree->SetBranchAddress("l2_pt", &l2_pt);
  ztree->SetBranchAddress("l2_eta", &l2_eta);
  ztree->SetBranchAddress("l2_phi", &l2_phi);
  ztree->SetBranchAddress("l2_charge", &l2_charge);
  ztree->SetBranchAddress("dijet_m", &jj_m);
  ztree->SetBranchAddress("dijet_pt", &jj_pt);
  ztree->SetBranchAddress("dijet_dr", &jj_dR);
  ztree->SetBranchAddress("dijet_eta", &jj_eta);
  ztree->SetBranchAddress("dijet_phi", &jj_phi);
  ztree->SetBranchAddress("dijet_deta", &jj_deta);
  ztree->SetBranchAddress("dijet_dphi", &jj_dphi);
  ztree->SetBranchAddress("el_pt", &el_pt);
  ztree->SetBranchAddress("el_eta", &el_eta);
  ztree->SetBranchAddress("el_phi", &el_phi);
  ztree->SetBranchAddress("el_charge", &el_charge);
  ztree->SetBranchAddress("mu_pt", &mu_pt);
  ztree->SetBranchAddress("mu_eta", &mu_eta);
  ztree->SetBranchAddress("mu_phi", &mu_phi);
  ztree->SetBranchAddress("mu_charge", &mu_charge);
  /*
  TMVA::Tools::Instance();
  TMVA::Reader *reader = new TMVA::Reader( "V:Color:!Silent" );
  Float_t var[11];
  reader->AddVariable("photon_mva", &var[0]);
  reader->AddVariable("min_dR", &var[1]);
  reader->AddVariable("max_dR", &var[2]);
  reader->AddVariable("pt_mass", &var[3]);
  reader->AddVariable("cosTheta", &var[4]);
  reader->AddVariable("costheta", &var[5]);
  reader->AddVariable("phi", &var[6]);
  reader->AddVariable("photon_res", &var[7]);
  reader->AddVariable("photon_rapidity", &var[8]);
  reader->AddVariable("l1_rapidity", &var[9]);
  reader->AddVariable("l2_rapidity", &var[10]);
   
  reader->BookMVA("BDT method","../TMVAClassification_BDTG_1.weights.xml");
  */
  float bdt1 = -2.; 
		    
  outtree->Branch("photon_event", &event);
  outtree->Branch("z_event", &zevent);
  //outtree->Branch("bdt1", &bdt1);
  outtree->Branch("njet", &njet);
  outtree->Branch("nlep", &nlep);
  outtree->Branch("nel", &nel);
  outtree->Branch("nmu", &nmu);
  outtree->Branch("w_lumi", &z_w_lumi);
  outtree->Branch("z_weight", &z_weight);
  outtree->Branch("weight", &weight);
  outtree->Branch("yr_weight", &yr_weight);
  outtree->Branch("ph_w_lumi", &ph_w_lumi);
  outtree->Branch("llphoton_m", &llphoton_m);
  outtree->Branch("llphoton_pt", &llphoton_pt);
  outtree->Branch("llphoton_costheta", &llphoton_costheta);
  outtree->Branch("llphoton_cosTheta", &llphoton_cosTheta);
  outtree->Branch("llphoton_psi", &llphoton_psi);
  outtree->Branch("llphoton_phi", &llphoton_phi);
  outtree->Branch("llphoton_eta", &llphoton_eta);
  outtree->Branch("el_pt", &el_pt);
  outtree->Branch("el_eta", &el_eta);
  outtree->Branch("el_phi", &el_phi);
  outtree->Branch("el_charge", &el_charge);
  outtree->Branch("mu_pt", &mu_pt);
  outtree->Branch("mu_eta", &mu_eta);
  outtree->Branch("mu_phi", &mu_phi);
  outtree->Branch("mu_charge", &mu_charge);
  outtree->Branch("l1_eta", &l1_eta);
  outtree->Branch("l2_eta", &l2_eta);
  outtree->Branch("l1_pt", &l1_pt);
  outtree->Branch("l2_pt", &l2_pt);
  outtree->Branch("l1_phi", &l1_phi);
  outtree->Branch("l2_phi", &l2_phi);
  outtree->Branch("l1_m", &l1_m);
  outtree->Branch("l2_m", &l2_m);
  outtree->Branch("l1_charge", &l1_charge);
  outtree->Branch("l2_charge", &l2_charge);
  outtree->Branch("ll_refit_pt", &ll_refit_pt);
  outtree->Branch("ll_refit_m", &ll_refit_m);
  outtree->Branch("ll_refit_eta", &ll_refit_eta);
  outtree->Branch("ll_refit_phi", &ll_refit_phi);
  outtree->Branch("llphoton_refit_m", &llphoton_refit_m);
  outtree->Branch("llphoton_refit_pt", &llphoton_refit_pt);
  outtree->Branch("llphoton_refit_phi", &llphoton_refit_phi);
  outtree->Branch("llphoton_refit_eta", &llphoton_refit_eta);
  outtree->Branch("ll_refit_l2_pt", &ll_refit_l2_pt);
  outtree->Branch("ll_refit_l1_pt", &ll_refit_l1_pt);
  outtree->Branch("ll_refit_pt", &ll_refit_pt);
  outtree->Branch("ll_refit_m", &ll_refit_m);
  outtree->Branch("ll_refit_eta", &ll_refit_eta);
  outtree->Branch("ll_refit_phi", &ll_refit_phi);
  outtree->Branch("ll_pt", &ll_pt);
  outtree->Branch("ll_m", &ll_m);
  outtree->Branch("ll_eta", &ll_eta);
  outtree->Branch("ll_phi", &ll_phi);
  outtree->Branch("ll_i1", &ll_i1);
  outtree->Branch("ll_i2", &ll_i2);
  outtree->Branch("ll_lepid", &ll_lepid);
  outtree->Branch("photon_idmva", &photon_idmva);
  outtree->Branch("photon_eta", &photon_eta);
  outtree->Branch("photon_pt", &photon_pt);
  outtree->Branch("photon_phi", &photon_phi);
  outtree->Branch("photon_ptErr", &photon_ptErr);
  outtree->Branch("photon_energyErr", &photon_energyErr);
  outtree->Branch("photon_drmin", &photon_drmin);
  outtree->Branch("photon_drmax", &photon_drmax);
  outtree->Branch("decorr_photon_pt", &decorr_photon_pt);
  outtree->Branch("photon_jet1_dr", &photon_jet1_dr);
  outtree->Branch("photon_jet2_dr", &photon_jet2_dr);
  outtree->Branch("photon_zeppenfeld", &photon_zeppenfeld);
  outtree->Branch("jet_pt", &jet_pt);
  outtree->Branch("jet_m", &jet_m);
  outtree->Branch("jet_eta", &jet_eta);
  outtree->Branch("jet_phi", &jet_phi);
  outtree->Branch("jet_isgood", &jet_isgood);
  outtree->Branch("dijet_m", &jj_m);
  outtree->Branch("dijet_pt", &jj_pt);
  outtree->Branch("dijet_dr", &jj_dR);
  outtree->Branch("dijet_dphi", &jj_dphi);
  outtree->Branch("dijet_deta", &jj_deta);
  outtree->Branch("dijet_phi", &jj_phi);
  outtree->Branch("dijet_eta", &jj_eta);
  outtree->Branch("npv", &npv);
  //outtree->Branch("photon_mllg", &ph_mllg);
  outtree->Branch("llphoton_dijet_dphi", &llphoton_dijet_dphi);
  outtree->Branch("llphoton_dijet_balance", &llphoton_dijet_balance);
  outtree->Branch("llphoton_dijet_dr", &llphoton_dijet_dr);
  outtree->Branch("llphoton_pTt", &llphoton_pTt);
  outtree->Branch("llphoton_pTt_an_hig019014", &llphoton_pTt_an_hig019014);
  outtree->Branch("llphoton_iph", &llphoton_iph);
  outtree->Branch("pass", &pass);
  outtree->Branch("nbdfm", &nbdfm);
  outtree->Branch("jet_qgl", &jet_qgl);
  outtree->Branch("met", &met);
  outtree->Branch("met_phi", &met_phi);
  outtree->Branch("met_calo", &met_calo);
  outtree->Branch("met_tru", &met_tru);


  Long64_t nPh = phtree->GetEntries();
  Long64_t nZ = ztree->GetEntries();
  std::cout << "nZ = " << nZ << std::endl;
  std::cout << "nPh = " << nPh << std::endl;
  for (auto j(0); j < nZ; j++){
    if (j%100000 == 0) std::cout << "Processing " << j << std::endl;
    TLorentzVector g;
    ll_lepid->clear();
    ll_i1->clear();
    ll_i2->clear();
    ll_m->clear();
    ll_pt->clear();
    ll_eta->clear();
    ll_phi->clear();
    el_pt->clear();
    el_eta->clear();
    el_phi->clear();
    mu_pt->clear();
    mu_eta->clear();
    mu_phi->clear();
    el_charge->clear();
    mu_charge->clear();
    jet_qgl->clear();
    
    photon_pt->clear();
    photon_eta->clear();
    photon_phi->clear();
    photon_idmva->clear();
    photon_drmin->clear();
    photon_drmax->clear();
    photon_energyErr->clear();
    photon_jet1_dr->clear();
    photon_jet2_dr->clear();
    photon_zeppenfeld->clear();
    
    llphoton_pt->clear();
    llphoton_eta->clear();
    llphoton_phi->clear();
    llphoton_m->clear();
    llphoton_iph->clear();
    llphoton_cosTheta->clear();
    llphoton_costheta->clear();
    llphoton_psi->clear();
    llphoton_pTt->clear();
    llphoton_pTt_an_hig019014->clear();
    
    llphoton_dijet_dphi->clear();
    llphoton_dijet_balance->clear();
    llphoton_dijet_dr->clear();
  
    phtree->GetEntry(j%nPh);
    g.SetPtEtaPhiM(photon_pt_, photon_eta_, photon_phi_, 0);
    jet_m->clear();
    jet_pt->clear();
    jet_eta->clear();
    jet_phi->clear();
    jet_isgood->clear();
    ztree->GetEntry(j);
    if (cat == "vbf" && njet < 2) continue;
    //std::cout << "Z id = " << j << std::endl;
    //      if (njet <= 1) continue;
    TLorentzVector l1, l2, z, h, j1, j2, h_refit, z_refit;
    int chose_jet = 0;
    l1.SetPtEtaPhiM(l1_pt, l1_eta, l1_phi, l1_m);
    l2.SetPtEtaPhiM(l2_pt, l2_eta, l2_phi, l2_m);
    z_refit.SetPtEtaPhiM(ll_refit_pt, ll_refit_eta, ll_refit_phi, ll_refit_m);
    h_refit = z_refit + g;
    //l1.Print();
    //l2.Print();
    h = l1 + l2 + g;
    z = l1 + l2;
    //l1_refit.SetPtEtaPhiM(ll_refit_l1_pt, l1_eta, l1_phi, l1_m);
    //l2_refit.SetPtEtaPhiM(ll_refit_l2_pt, l2_eta, l2_phi, l2_m);
    //ll_pt = z.Pt();
    //ll_m = z.M();
    //ll_eta = z.Eta();
    //ll_phi = z.Phi();
    //h.Print();
    //z.Print();
    //std::cout << h.M() << std::endl;
    double mindR = GetmindR(l1, l2, g);
    weight = z_w_lumi * w_lep * w_fs_lep * w_btag_df * w_trig * w_pu * w_prefire * w_phshape * w_photon * w_fakephoton;
    /*cout << "l1 mass = " << l1_m <<endl;
    cout << "l2 mass = " << l2_m <<endl;
    cout << "l1 eta = " << l1_eta <<endl;
    cout << "l2 eta = " << l2_eta <<endl;
    cout << "Z mass = " << z.M() <<endl;
    cout << "z mass (pico) = " << ll_m << endl;
    cout << "H mass = " << h.M() <<endl;
    cout << "mindR = " << mindR <<endl;
    cout << endl;*/
    if (h.M() + z.M() > 185. && photon_pt_/h.M() > 15./110 && z.M() > 80. && z.M() < 100. && mindR > 0.3){
      TLorentzVector q1 = AssignQ1(h);
      TLorentzVector q2 = AssignQ2(h);
      if (jet_m->size() > 0){
	for (int k(0); k < jet_m->size(); k++){
	  if ((*jet_isgood)[k] && chose_jet == 0){
	    j1.SetPtEtaPhiM((*jet_pt)[k], (*jet_eta)[k], (*jet_phi)[k],(*jet_m)[k]);
	    chose_jet ++;
	  }
	  else if ((*jet_isgood)[k] && chose_jet == 1){
	    j2.SetPtEtaPhiM((*jet_pt)[k], (*jet_eta)[k], (*jet_phi)[k],(*jet_m)[k]);
	    chose_jet ++;
	    break;
	  }
	}
	TLorentzVector jj = j1 + j2;
	if (chose_jet == 2){
	  /*jj_m = jj.M();
	  jj_pt = jj.Pt();
	  jj_dR = j1.DeltaR(j2);
	  jj_dphi = j1.DeltaPhi(j2);*/
	  photon_zeppenfeld->push_back(abs(g.Eta() - (j1.Eta() + j2.Eta())/2));
	  llphoton_dijet_balance->push_back((h+jj).Pt()/(z.Pt() + g.Pt() + j1.Pt() + j2.Pt()));
	  llphoton_dijet_dr->push_back(h.DeltaR(jj));
	  llphoton_dijet_dphi->push_back(h.DeltaPhi(jj));
	  photon_jet1_dr->push_back(g.DeltaR(j1));
	  photon_jet2_dr->push_back(g.DeltaR(j2));
	  
	}
	else if (chose_jet == 1){
	  llphoton_dijet_dphi->push_back(-999.);
          photon_zeppenfeld->push_back(-999.);
          llphoton_dijet_balance->push_back((h+j1).Pt()/(z.Pt() + g.Pt() + j1.Pt()));
          llphoton_dijet_dr->push_back(-999.);
	  photon_jet1_dr->push_back(g.DeltaR(j1));
	}
	else {
	  /*jj_m = -5.;
	  jj_pt = -5.;
	  jj_dR = -5.;
	  jj_dphi = -5.;*/
	  llphoton_dijet_dphi->push_back(-999.);
	  photon_zeppenfeld->push_back(-999.);
          llphoton_dijet_balance->push_back(-999.);
          llphoton_dijet_dr->push_back(-999.);
          photon_jet1_dr->push_back(-999.);
          photon_jet2_dr->push_back(-999.);

	}
	
      }
      else {
	/*jj_m = -5.;
	jj_pt = -5.;
	jj_dR = -5.;
	jj_dphi = -5.;*/
	llphoton_dijet_dphi->push_back(-999.);
	photon_zeppenfeld->push_back(-999.);
	llphoton_dijet_balance->push_back(-999.);
	llphoton_dijet_dr->push_back(-999.);
	photon_jet1_dr->push_back(-999.);
	photon_jet2_dr->push_back(-999.);
      }
      TVector3 g_pT = g.Vect();
      TVector3 h_pT = h.Vect();
      TVector3 z_pT = z.Vect();
      photon_pt->push_back(photon_pt_);
      photon_eta->push_back(photon_eta_);
      photon_phi->push_back(photon_phi_);
      photon_idmva->push_back(photon_idmva_);
      photon_energyErr->push_back(photon_energyErr_);
      llphoton_pTt->push_back(h_pT.Cross((z_pT-g_pT).Unit()).Mag());
      llphoton_pTt_an_hig019014->push_back(h_pT.Unit().Cross(z_pT-g_pT).Mag());
      if (l1_charge < 0) llphoton_costheta->push_back(cos_theta(h, z, l1, l2));
      else llphoton_costheta->push_back(cos_theta(h, z, l2, l1));
      llphoton_cosTheta->push_back(cos_Theta(h, z, q1, q2));
      llphoton_psi->push_back(Getphi(q1, q2, z, l1, l2, h));
      photon_drmax->push_back(GetmaxdR(l1, l2, g));
      photon_drmin->push_back(mindR);
      decorr_photon_pt = photon_pt_/h.M();
      photon_ptErr = photon_energyErr_/TMath::CosH(photon_eta_)/photon_pt_;
      llphoton_m->push_back(h.M());
      llphoton_iph->push_back(0);
      llphoton_pt->push_back(h.Pt());
      llphoton_phi->push_back(h.Phi());
      llphoton_eta->push_back(h.Eta());
      llphoton_refit_m = h_refit.M();
      llphoton_refit_pt = h_refit.Pt();
      llphoton_refit_eta = h_refit.Eta();
      llphoton_refit_phi = h_refit.Phi();
	
      /*
      var[0] = (*photon_idmva)[0];
      var[1] = mindR;
      var[2] = (*photon_drmax)[0];
      var[3] = (*llphoton_pt)[0]/(*llphoton_m)[0];
      var[4] = (*llphoton_cosTheta)[0];
      var[5] = (*llphoton_costheta)[0];
      var[6] = (*llphoton_phi)[0];
      var[7] = photon_ptErr;
      var[8] = (*photon_eta)[0];
      var[9] = l1.Eta();
      var[10] = l2.Eta();
      bdt1 = (float)reader -> EvaluateMVA("BDT method");*/
      outtree->Fill();
    }
    
  }
  
  outfile->cd();
  outtree->Write();
  outfile->Close();
  
}
