#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
//#include "Math.h"
void buildEventList(vector<int> &list_, vector<float> &count_, int event_){
  //  std::cout << "event id = " << event_<< std::endl;
  if (list_.size() == 0){
    list_.push_back(event_);
    count_.push_back(0.);
  }
  else{
    bool match = false;
    for (int ent : list_){
      if (event_== ent){
	match = true;
	break;
      }
    }
    if (!match) {
      list_.push_back(event_);
      count_.push_back(0.);
    }
  }
  //  std::cout << "list size = " << list_.size()<< std::endl;
}

void addPhCount(vector<int> &list_, vector<float> &count_, int event_, float w_){
  int e_ind = -1;
  //  std::cout << "list size = " << list_.size()<< std::endl;
  for (int i(0); i < list_.size(); i++){
    if ( event_ == list_[i]){
      e_ind = i;
      break;
    }
  }
  //  std::cout << "event ind = " << e_ind << std::endl;
  count_[e_ind] += w_;
}

class TwoHist {
 public:
  //TString name;
  //int nbins;
  //float low, high;
  TH1F *h1;
  TH1F *h2;
  TwoHist(TString name_, int nbins_, float low_, float high_){
    //name = name_;
    nbins = nbins_;
    low = low_;
    high = high_;
    h1 = new TH1F(name_ + "_pico", name_ + "_pico", nbins_, low_, high_);
    h2 = new TH1F(name_ + "_mix", name_ + "_mix", nbins_, low_, high_);
    vector<vector<float>> phCount_(nbins_);
    vector<vector<int>> phEvent_(nbins_);
    /*for (int j (0); j < nbins; j++){
      vector<int> dum1;
      vector<float> dum2;
      phCount.push_back(dum2);
      phEvent.push_back(dum1);
      }*/
    phCount = phCount_;
    phEvent = phEvent_;
    if (phCount_.size() !=0) phCount_.clear();
    if (phEvent_.size() !=0) phEvent_.clear();
  }
  void Fill1 (float input_, float w_){
    h1->Fill(input_, w_);
  }

  void Fill2 (int event_, float input_, float w_){
    h2->Fill(input_, w_);
    //std::cout << "Fill2 fill ok" << std::endl;
    int nbin = h2->FindBin(input_);
    if (nbin > 0 && nbin <= nbins){
      //std::cout << "n bin = "<< nbin << ", event = " <<  event_ << std::endl;
      buildEventList(phEvent[nbin-1], phCount[nbin-1], event_);
      /*std::cout << "ph id = (";
      for (int i(0); i < phEvent[nbin-1].size(); i++){
	std::cout << phEvent[nbin-1][i] << ", ";
      }
      std::cout << ")" << std::endl;*/
      addPhCount(phEvent[nbin-1], phCount[nbin-1], event_, w_);
      /*std::cout << "counts = (";
      for (int i(0); i < phCount[nbin-1].size(); i++){
        std::cout << phCount[nbin-1][i] << ", ";
      }
      std::cout << ")" << std::endl;*/
    }
  }
  void Fill2Simp (int event_, float input_, float w_){
    h2->Fill(input_, w_);
  }
  void PlotHists(TString title_){
    /*for (int i(0); i < nbins; i++){
      float sumW2 = 0.;
      for (float entry : phCount[i]){
	sumW2 += entry * entry;
      }
      h2->SetBinError(i+1, ROOT::Math::sqrt(sumW2));
      }*/
    h1->Scale(1./h1->Integral());
    h2->Scale(1./h2->Integral());
    h1->SetLineWidth(2);
    h2->SetLineWidth(2);
    h1->SetLineColor(2);
    h1->SetMarkerColor(2);
    h1->SetMarkerStyle(8);
    h2->SetLineColor(3);
    h2->SetMarkerColor(3);
    h2->SetMarkerStyle(8);
    
    float max1 = h1->GetBinContent(h1->GetMaximumBin());
    float max2 = h2->GetBinContent(h2->GetMaximumBin());
    float max = (max1 > max2)? max1:max2;
    h1->SetTitle(title_);
    h1->GetYaxis()->SetRangeUser(0, 1.05 * max);
    auto leg = new TLegend(.6,.7,.9,.9);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.035);
    leg->AddEntry(h1,"DY No Match (dijet)","L");
    leg->AddEntry(h2,"Mixing","L");
    auto c1 = new TCanvas("c_"+ title_, "c_"+ title_, 500, 500);
    c1->cd();
    h1->Draw();
    h2->Draw("SAME");
    leg->Draw("SAME");
    c1->Draw();
    c1->SaveAs("/eos/user/f/fanx/event_mixing/plots/" + title_ +"_mix.pdf");
    phEvent.clear();
    phCount.clear();
  }
 private:
  vector<vector<int>> phEvent;
  vector<vector<float>> phCount;
  //vector<float> errBar;
  //  float weight_ = 0.;
  int nbins = 0;
  float high = 0.;
  float low = 0.;
};

void plot_mixing_simple(){
  gStyle->SetOptStat(0);
  auto fpico = TFile::Open("sample/DY_output_kingscanyon_v1_2017_merge.root", "READ");
  auto picotree = (TTree*)fpico->Get("tree");
  auto fmix = new TFile("sample/mixing_output_full_photon_dijet.root", "READ");
  auto mixtree = (TTree*)fmix->Get("tree");
  int nEntries1 = picotree->GetEntries();
  int nEntries2 = mixtree->GetEntries();

  int event;
  float llg_mix_m, llg_mix_pt, ph_mix_pt, ph_mix_eta, ph_mix_phi, cosTheta_mix, costheta_mix, psi_mix, idmva_mix, mindR_mix, photon_ptErr_mix, decopT_mix, w_lumi, z_w_lumi, ph_w_lumi;
  float llg_m, llg_pt, cosTheta, costheta, psi, mindR, ph_pt, ph_eta, ph_phi, idmva, photon_energyErr;

  float jj_m, jj_pt, jj_dphi, jj_dR;
  float jj_m_mix, jj_pt_mix, jj_dphi_mix, jj_dR_mix;
  picotree->SetBranchAddress("event", &event);
  picotree->SetBranchAddress("w_lumi", &w_lumi);
  picotree->SetBranchAddress("llphoton_m", &llg_m);
  picotree->SetBranchAddress("llphoton_pt", &llg_pt);
  picotree->SetBranchAddress("llphoton_cosTheta", &cosTheta);
  picotree->SetBranchAddress("llphoton_costheta", &costheta);
  picotree->SetBranchAddress("llphoton_psi", &psi);
  picotree->SetBranchAddress("photon_drmin", &mindR);
  picotree->SetBranchAddress("photon_pt", &ph_pt);
  picotree->SetBranchAddress("photon_eta", &ph_eta);
  picotree->SetBranchAddress("photon_phi", &ph_phi);
  picotree->SetBranchAddress("photon_idmva", &idmva);
  picotree->SetBranchAddress("photon_energyErr", &photon_energyErr);
  picotree->SetBranchAddress("jj_m", &jj_m);
  picotree->SetBranchAddress("jj_pt", &jj_pt);
  picotree->SetBranchAddress("jj_dphi", &jj_dphi);
  picotree->SetBranchAddress("jj_dR", &jj_dR);


  mixtree->SetBranchAddress("photon_event", &event);
  mixtree->SetBranchAddress("mindR", &mindR_mix);
  mixtree->SetBranchAddress("photon_pt", &ph_mix_pt);
  mixtree->SetBranchAddress("photon_eta", &ph_mix_eta);
  mixtree->SetBranchAddress("photon_phi", &ph_mix_phi);
  mixtree->SetBranchAddress("photon_idmva", &idmva_mix);
  mixtree->SetBranchAddress("llphoton_pt", &llg_mix_pt);
  mixtree->SetBranchAddress("llphoton_m", &llg_mix_m);
  mixtree->SetBranchAddress("llphoton_cosTheta", &cosTheta_mix);
  mixtree->SetBranchAddress("llphoton_costheta", &costheta_mix);
  mixtree->SetBranchAddress("llphoton_psi", &psi_mix);
  mixtree->SetBranchAddress("photon_ptErr", &photon_ptErr_mix);
  mixtree->SetBranchAddress("decorr_photon_pt", &decopT_mix);
  mixtree->SetBranchAddress("z_w_lumi", &z_w_lumi);
  mixtree->SetBranchAddress("ph_w_lumi", &ph_w_lumi);
  mixtree->SetBranchAddress("jj_m", &jj_m_mix);
  mixtree->SetBranchAddress("jj_pt", &jj_pt_mix);
  mixtree->SetBranchAddress("jj_dphi", &jj_dphi_mix);
  mixtree->SetBranchAddress("jj_dR", &jj_dR_mix);

  TwoHist photon_pt("pT", 130, 15., 80.);
  TwoHist photon_eta("eta", 50, -2.5, 2.5);
  TwoHist photon_phi("phi", 70, -3.14, 3.14);
  TwoHist llphoton_psi("llphoton_psi", 70, -3.14, 3.14);
  TwoHist llphoton_pt("llphoton_pt", 150, 0, 150);
  TwoHist llphoton_m("llphoton_m", 80, 100, 180);
  TwoHist h_ptErr("photon_ptErr", 60, 0, 0.3);
  TwoHist h_decopT("decopT", 60, 0.1, 0.4);
  TwoHist h_mindR("mindR", 60, 0.3, 1.8);
  TwoHist h_cosTheta("llg_cosTheta", 100, -1, 1);
  TwoHist h_costheta("llg_costheta", 100, -1, 1 );
  TwoHist h_idmva("idmva", 100, 0, 1);
  TwoHist h_jjm("jj_m", 100, 0, 1000);
  TwoHist h_jjpt("jj_pt", 100, 0, 200);
  TwoHist h_jjdphi("jj_dphi", 50, 0, 3.1415);
  TwoHist h_jjdR("jj_dR", 50, 0, 10);

  vector<Long64_t> nomatch_list;
  string line;
  ifstream ReadFile("vbf.txt");
  while (getline(ReadFile, line)){
    if (line[0] == 'e') {
      line = line.substr(11,  line.size()-11);
      Long64_t numList = static_cast<Long64_t>(stoi(line));
      nomatch_list.push_back(numList);
    }
  }
  ReadFile.close();

  std::cout << "Read and setup ok" << std::endl;
  int nomatch = 0;
  int mixing = 0;
  for (auto i(0); i<nEntries1; i++){
    //    if (ntest > 100) break;
    picotree->GetEntry(i);
    bool chose = false;
    for (auto num:nomatch_list){
      if(num == event) chose = true;
    }
    if (!chose) continue;
    nomatch += 1;
    float ptErr = photon_energyErr/TMath::CosH(ph_eta)/ph_pt; 
    photon_pt.Fill1(ph_pt, w_lumi);
    photon_eta.Fill1(ph_eta, w_lumi);
    photon_phi.Fill1(ph_phi, w_lumi);
    h_idmva.Fill1(idmva, w_lumi);
    h_mindR.Fill1(mindR, w_lumi);
    h_cosTheta.Fill1(cosTheta, w_lumi);
    h_costheta.Fill1(costheta, w_lumi);
    llphoton_psi.Fill1(psi, w_lumi);
    llphoton_pt.Fill1(llg_pt, w_lumi);
    llphoton_m.Fill1(llg_m, w_lumi);
    h_ptErr.Fill1(ptErr, w_lumi);
    h_decopT.Fill1(ph_pt/llg_m, w_lumi);
    h_jjm.Fill1(jj_m, w_lumi);
    h_jjpt.Fill1(jj_pt, w_lumi);
    h_jjdphi.Fill1(jj_dphi, w_lumi);
    h_jjdR.Fill1(jj_dR, w_lumi);
	
  }
  std::cout << "Fill1 ok" << std::endl;

  for (auto j(0); j < nEntries2; j++){
    mixtree->GetEntry(j);

    //bool sel = (ph_mix_eta > 1.479 || ph_mix_eta < -1.479) && idmva_mix < 0.4 && ph_mix_pt < 17.;
    bool sel = false;
    //if (ph_mix_eta > 1.479 || ph_mix_eta < -1.479){
    //  idmva_endcap->Fill(idmva_mix, z_w_lumi);
    //}
    if (!sel){
      mixing +=1;
      photon_pt.Fill2Simp(event, ph_mix_pt, z_w_lumi);
      photon_eta.Fill2Simp(event, ph_mix_eta, z_w_lumi);
      photon_phi.Fill2Simp(event, ph_mix_phi, z_w_lumi);
      h_idmva.Fill2Simp(event, idmva_mix, z_w_lumi);
      h_mindR.Fill2Simp(event, mindR_mix, z_w_lumi);
      h_cosTheta.Fill2Simp(event, cosTheta_mix, z_w_lumi);
      h_costheta.Fill2Simp(event, costheta_mix, z_w_lumi);
      llphoton_psi.Fill2Simp(event, psi_mix, z_w_lumi);
      llphoton_pt.Fill2Simp(event, llg_mix_pt, z_w_lumi);
      llphoton_m.Fill2Simp(event, llg_mix_m, z_w_lumi);
      h_ptErr.Fill2Simp(event, photon_ptErr_mix, z_w_lumi);
      h_decopT.Fill2Simp(event, decopT_mix, z_w_lumi);
      h_jjm.Fill2Simp(event, jj_m_mix, z_w_lumi);
      h_jjpt.Fill2Simp(event, jj_pt_mix, z_w_lumi);
      h_jjdphi.Fill2Simp(event, jj_dphi_mix, z_w_lumi);
      h_jjdR.Fill2Simp(event, jj_dR_mix, z_w_lumi);
    }
  }
  std::cout << "Fill2 ok" << std::endl;

  llphoton_m.PlotHists("llphoton m (full 2017 dijet)");
  llphoton_pt.PlotHists("llphoton pT (full 2017 dijet)");
  photon_pt.PlotHists("photon pT (full 2017 dijet)");
  photon_eta.PlotHists("photon eta (full 2017 dijet)");
  photon_phi.PlotHists("photon phi (full 2017 dijet)");
  h_idmva.PlotHists("idmva (full 2017 dijet)");
  h_mindR.PlotHists("mindR (full 2017 dijet)");
  h_cosTheta.PlotHists("cosTheta  (full 2017 dijet)");
  h_costheta.PlotHists("costheta (full 2017 dijet)");
  llphoton_psi.PlotHists("Psi (full 2017 dijet)");
  h_ptErr.PlotHists("photon pT resolution (full 2017 dijet)");
  h_decopT.PlotHists("decorrelated photon pT (full 2017 dijet)");
  h_jjm.PlotHists("dijet mass (full 2017 dijet)");
  h_jjpt.PlotHists("dijet pT (full 2017 dijet)");
  h_jjdphi.PlotHists("dijet dPhi (full 2017 dijet)");
  h_jjdR.PlotHists("dijet dR (full 2017 dijet)");

  //auto c = new TCanvas("c","c", 500,500);
  //idmva_endcap->Draw("HIST");
  //  c->Draw();
  std::cout << "n no match = " << nomatch << ", n mixing = " << mixing << std::endl; 
}






  
  
