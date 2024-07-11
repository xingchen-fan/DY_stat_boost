#include "../pico_script/MVAfunc.cxx"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TVector3.h"

void Zg_mixing(){
  auto ph_file = new TFile("sample/no_match_ph_npv.root","READ");
  auto z_file = new TFile("sample/DY_Z_output_kingscanyon_v1_2017.root","READ");
  auto outfile = new TFile("sample/mixing_output_full_photon_dijet.root", "RECREATE");
  auto phtree = (TTree*)ph_file->Get("tree");
  auto ztree = (TTree*)z_file->Get("tree");
  auto outtree = new TTree("tree", "tree");

  int event;
  int npv, njet;
  float z_weight, ph_weight;
  float l1_pt, l1_eta, l1_phi, l1_m, l2_pt, l2_eta, l2_phi, l2_m;
  float photon_pt, photon_eta, photon_phi, photon_idmva, photon_energyErr;
  float llphoton_m, llphoton_costheta, llphoton_cosTheta, llphoton_pt, llphoton_psi, mindR, maxdR, photon_ptErr, decorr_photon_pt;
  float jj_m, jj_dphi, jj_dR, jj_pt;
  std::vector<float> *jet_pt = new std::vector<float>();
  std::vector<float> *jet_m = new std::vector<float>();
  std::vector<float> *jet_eta = new std::vector<float>();
  std::vector<float> *jet_phi = new std::vector<float>();
  std::vector<bool> *jet_isgood = new std::vector<bool>();

  phtree->SetBranchAddress("photon_event", &event);
  phtree->SetBranchAddress("w_lumi", &ph_weight);
  phtree->SetBranchAddress("photon_pt", &photon_pt);
  phtree->SetBranchAddress("photon_idmva", &photon_idmva);
  phtree->SetBranchAddress("photon_energyErr", &photon_energyErr);
  phtree->SetBranchAddress("photon_phi", &photon_phi);
  phtree->SetBranchAddress("photon_eta", &photon_eta);
  phtree->SetBranchAddress("npv", &npv);

  ztree->SetBranchAddress("w_lumi", &z_weight);
  ztree->SetBranchAddress("njet", &njet);
  ztree->SetBranchAddress("jet_pt", &jet_pt);
  ztree->SetBranchAddress("jet_m", &jet_m);
  ztree->SetBranchAddress("jet_eta", &jet_eta);
  ztree->SetBranchAddress("jet_phi", &jet_phi);
  ztree->SetBranchAddress("jet_isgood", &jet_isgood);
  ztree->SetBranchAddress("l1_m", &l1_m);
  ztree->SetBranchAddress("l1_pt", &l1_pt);
  ztree->SetBranchAddress("l1_eta", &l1_eta);
  ztree->SetBranchAddress("l1_phi", &l1_phi);
  ztree->SetBranchAddress("l2_m", &l2_m);
  ztree->SetBranchAddress("l2_pt", &l2_pt);
  ztree->SetBranchAddress("l2_eta", &l2_eta);
  ztree->SetBranchAddress("l2_phi", &l2_phi);

  outtree->Branch("photon_event", &event);
  outtree->Branch("z_w_lumi", &z_weight);
  outtree->Branch("ph_w_lumi", &ph_weight);
  outtree->Branch("llphoton_m", &llphoton_m);
  outtree->Branch("llphoton_pt", &llphoton_pt);
  outtree->Branch("llphoton_costheta", &llphoton_costheta);
  outtree->Branch("llphoton_cosTheta", &llphoton_cosTheta);
  outtree->Branch("llphoton_psi", &llphoton_psi);
  outtree->Branch("l1_eta", &l1_eta);
  outtree->Branch("l2_eta", &l2_eta);
  outtree->Branch("photon_idmva", &photon_idmva);
  outtree->Branch("photon_eta", &photon_eta);
  outtree->Branch("photon_pt", &photon_pt);
  outtree->Branch("photon_phi", &photon_phi);
  outtree->Branch("photon_ptErr", &photon_ptErr);
  outtree->Branch("mindR", &mindR);
  outtree->Branch("maxdR", &maxdR);
  outtree->Branch("decorr_photon_pt", &decorr_photon_pt);
  outtree->Branch("jj_m", &jj_m);
  outtree->Branch("jj_pt", &jj_pt);
  outtree->Branch("jj_dR", &jj_dR);
  outtree->Branch("jj_dphi", &jj_dphi);
  outtree->Branch("npv", &npv);

  Long64_t nPh = phtree->GetEntries();
  Long64_t nZ = ztree->GetEntries();
  int choseZ = 0;

  for (auto i(0); i < nPh; i++){
    if (i%1000 == 0) std::cout << "Processing " << i << std::endl;
    phtree->GetEntry(i);
    choseZ = 0;
    TLorentzVector g;
    g.SetPtEtaPhiM(photon_pt, photon_eta, photon_phi, 0);
    int start = (i*10000)%nZ;
    for ( auto j(start); j < start + 10000; j++){
      jet_m->clear();
      jet_pt->clear();
      jet_eta->clear();
      jet_phi->clear();
      jet_isgood->clear();
      ztree->GetEntry(j);
      if (njet <= 1) continue;
      TLorentzVector l1, l2, z, h, j1, j2;
      int chose_jet = 0;
      l1.SetPtEtaPhiM(l1_pt, l1_eta, l1_phi, l1_m);
      l2.SetPtEtaPhiM(l2_pt, l2_eta, l2_phi, l2_m);
      //l1.Print();
      //l2.Print();
      h = l1 + l2 + g;
      z = l1 + l2;
      //h.Print();
      //z.Print();
      //std::cout << h.M() << std::endl;
      mindR = GetmindR(l1, l2, g);
      if (h.M() > 100. && h.M() < 180. && h.M() + z.M() > 185. && photon_pt/h.M() > 15./110 && z.M() > 81. && z.M() < 101. && mindR > 0.4){
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
	TLorentzVector q1 = AssignQ1(h);
        TLorentzVector q2 = AssignQ2(h);
	TLorentzVector jj = j1 + j2;
	jj_m = jj.M();
	jj_pt = jj.Pt();
	jj_dR = j1.DeltaR(j2);
	jj_dphi = j1.DeltaPhi(j2);
	llphoton_costheta = cos_theta(h, z, l1, l2);
	llphoton_cosTheta = cos_Theta(h, z, q1, q2);
	llphoton_psi = Getphi(q1, q2, z, l1, l2, h);
	maxdR = GetmaxdR(l1, l2, g);
	decorr_photon_pt = photon_pt/h.M();
	photon_ptErr = photon_energyErr/TMath::CosH(photon_eta)/photon_pt;
	llphoton_m = h.M();
	llphoton_pt = h.Pt();
	outtree->Fill();
      }
	
    }
  }
    outfile->cd();
    outtree->Write();
    outfile->Close();

}
