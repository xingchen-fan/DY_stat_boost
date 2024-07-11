#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>

class TwoHist {
 public:
  TH1F *h1;
  TH1F *h2;
  TwoHist(TString name_, int nbins_, float low_, float high_){
    name = name_;
    nbins = nbins_;
    low = low_;
    high = high_;
    h1 = new TH1F(name_ + "_pico", name_ + "_pico", nbins_, low_, high_);
    h2 = new TH1F(name_ + "_mix", name_ + "_mix", nbins_, low_, high_);
    errBar.reserve(nbins);
    temp_.reserve(nbins);
    for(int i(0); i < nbins; i++) {
      errBar.push_back(0.);
      temp_.push_back(0.);
    }
  }
  void Fill1 (float input_, float w_){
    h1->Fill(input_, w_);
  }
  void easyFill (vector<float> &counts, TH1F *h_){
    //    std::cout << "easy fill" << std::endl;
    //    std::cout << "counts size = " << counts.size() << std::endl;
    for(int i(0); i < counts.size();i++){
      h_->Fill(low + (i+0.5)*(high-low)/nbins, counts[i]);
    }
  }

  void Fill2 (TTree *tree_, TString var_str){
    //h2->Fill(input_, w_);
    tree_->SetBranchAddress("z_w_lumi", &w_);
    tree_->SetBranchAddress("photon_event", &event_);
    if (var_str == "npv") tree_->SetBranchAddress(var_str, &npv_);
    else tree_->SetBranchAddress(var_str, &var_);
    /* for (int i(0); i<tree_->GetEntries(); i++){
      tree_->GetEntry(i);
      //      std::cout << var_ << std::endl;
      h2->Fill(var_, w_);
      }*/
    int rep = 0;
    //vector<float> temp_(nbins);
    for (int j(0);j < tree_->GetEntries();j++){
      tree_->GetEntry(j);
      if(var_str == "npv") var_ = (float)npv_;
      if (var_<low || var_ > high) continue;
      int temp_ind = h2->Fill(var_, w_) - 1;
      temp_[temp_ind]+=w_;
      int temp_event = event_;
      rep++;
      if (j  ==  tree_->GetEntries() -1) {
	//easyFill(temp_, h2);
	/*std::cout << "count " << rep << " " << temp_event << std::endl;
	std::cout << "weight = ";
	//for(auto entry:temp_) std::cout << entry << ", ";
	for(int k(0); k < nbins; k++) std::cout <<temp_[k] << ", ";
	std::cout << std::endl;*/
	for(int k(0); k < nbins; k++){
	  
	  errBar[k] += temp_[k]*temp_[k];
	}
	temp_.clear();
      }
      else{
	tree_->GetEntry(j+1);
	if (event_ == temp_event) continue;
	else {
	  // easyFill(temp_, h2);
	  /*	  std::cout << "count " << rep << " " <<  temp_event << std::endl;
	  std::cout << "weight = ";
	    //for(auto entry:temp_) std::cout << entry << ", ";
	  for(int k(0); k < nbins; k++){
	    std::cout <<temp_[k] << ", ";
	  }
	  std::cout << std::endl;*/

	  for(int k(0); k < nbins; k++){
	    errBar[k] += temp_[k]*temp_[k];
	  }
	  for (int k(0); k <nbins; k++){
            temp_[k]=0;
          }
	  rep = 0;
	  //for(int k(0); k < nbins; k++) temp_[k] = 0.;
	  //std::cout << "weight = ";
          //for(auto entry:temp_) std::cout << entry << ", ";
          //std::cout << std::endl;
	}
      }
    }
  } 


  
  void PlotHists(TString title_){
    std::cout << "sumW2 = ";
    for(int k(0); k < nbins; k++) std::cout << errBar[k] << ", ";
    std::cout << std::endl;
    for (int i(0); i < nbins; i++){
      h2->SetBinError(i+1, ROOT::Math::sqrt(errBar[i]));
    }

    errBar.clear();
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
    leg->AddEntry(h1,"DY No Match","L");
    leg->AddEntry(h2,"Mixing","L");
    auto ratio = new TH1F("ratio", "ratio", nbins,low, high);
    ratio->Divide(h1, h2);
    ratio->SetMarkerStyle(8);
    ratio->SetMarkerSize(1);
    ratio->SetTitle("");
    ratio->GetYaxis()->SetRangeUser(0.5, 1.5);
    ratio->GetXaxis()->SetTitle(name);
    ratio->GetXaxis()->SetTitleSize(0.1);
    ratio->GetYaxis()->SetLabelSize(0.1);
    ratio->GetXaxis()->SetLabelSize(0.15);

    auto line = new TLine( low, 1, high, 1);
    line->SetLineColor(kBlack);
    line->SetLineStyle(7);
    line->SetLineWidth(2);
    auto c1 = new TCanvas("c_"+ title_, "c_"+ title_, 500, 500);
    c1->Divide(1,2);
    c1->cd(1);
    gPad->SetPad(0.0, 0.3, 1.0, 0.99);
    gPad->SetBottomMargin(0);
    h1->Draw();
    h2->Draw("SAME");
    leg->Draw("SAME");
    c1->cd(2);
    gPad->SetPad(0.0, 0.1, 1.0, 0.3);
    gPad->SetTopMargin(0);
    ratio->Draw();
    line->Draw("SAME");
    c1->cd();
    c1->Draw();
    c1->SaveAs("/eos/user/f/fanx/event_mixing/plots/" + title_ +"_mix.pdf");

  }
 private:
  vector<float> errBar;
  vector<float> temp_;
  //  float weight_ = 0.;
  TString name;
  float w_, var_;
  int npv_;
  int event_;
  int nbins = 0;
  float high = 0.;
  float low = 0.;
};

void plot_mixing_corrErrBar(){
  gStyle->SetOptStat(0);
  auto fpico = new TFile("/eos/user/f/fanx/HZg_AOD/pico/no_match_DY_full2017.root", "READ");
  auto picotree = (TTree*)fpico->Get("tree");
  auto fmix = new TFile("/eos/user/f/fanx/event_mixing/sample/mixing_output_full_photon.root", "READ");
  auto mixtree = (TTree*)fmix->Get("tree");
  int nEntries1 = picotree->GetEntries();
  int nEntries2 = mixtree->GetEntries();

  int event, npv;
  float llg_mix_m, llg_mix_pt, ph_mix_pt, ph_mix_eta, ph_mix_phi, cosTheta_mix, costheta_mix, psi_mix, idmva_mix, mindR_mix, photon_ptErr_mix, decopT_mix, w_lumi, z_w_lumi, ph_w_lumi;
  float llg_m, llg_pt, cosTheta, costheta, psi, mindR, ph_pt, ph_eta, ph_phi, idmva, photon_energyErr;
  
  picotree->SetBranchAddress("w_lumi", &w_lumi);
  picotree->SetBranchAddress("npv", &npv);
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
  
  TwoHist photon_pt("pT", 130, 15., 80.);
  TwoHist photon_eta("eta", 50, -2.5, 2.5);
  TwoHist photon_phi("phi", 70, -3.14, 3.14);
  TwoHist llphoton_psi("llphoton_psi", 70, -3.14, 3.14);
  TwoHist llphoton_pt("llphoton_pt", 150, 0, 150);
  TwoHist llphoton_m("llphoton_m", 80, 100, 180);
  TwoHist h_ptErr("photon_ptErr", 80, 0, 0.2);
  TwoHist h_decopT("decopT", 60, 0.1, 0.4);
  TwoHist h_mindR("mindR", 60, 0.3, 1.8);
  TwoHist h_cosTheta("llg_cosTheta", 100, -1, 1);
  TwoHist h_costheta("llg_costheta", 100, -1, 1 );
  TwoHist h_idmva("idmva", 100, 0, 1);
  TwoHist h_npv("npv", 40, 10,50);
  std::cout << "Read and setup ok" << std::endl;
  int nomatch = 0;
  int mixing = 0;
  for (auto i(0); i<nEntries1; i++){
    //    if (ntest > 100) break;
    picotree->GetEntry(i);
    nomatch += 1;
    float ptErr = photon_energyErr/TMath::CosH(ph_eta)/ph_pt;
    // photon_pt.Fill1(ph_pt, w_lumi);
    //photon_eta.Fill1(ph_eta, w_lumi);
    //photon_phi.Fill1(ph_phi, w_lumi);
    //h_idmva.Fill1(idmva, w_lumi);
    //h_mindR.Fill1(mindR, w_lumi);
    //h_cosTheta.Fill1(cosTheta, w_lumi);
    //h_costheta.Fill1(costheta, w_lumi);
    //llphoton_psi.Fill1(psi, w_lumi);
    //llphoton_pt.Fill1(llg_pt, w_lumi);
    //llphoton_m.Fill1(llg_m, w_lumi);
    //h_ptErr.Fill1(ptErr, w_lumi);
    //h_decopT.Fill1(ph_pt/llg_m, w_lumi);
    h_npv.Fill1(npv, w_lumi);
  }
  std::cout << "Fill1 ok" << std::endl;
  //photon_pt.Fill2(mixtree, "photon_pt");
  //photon_eta.Fill2(mixtree, "photon_eta");
  //photon_phi.Fill2(mixtree, "photon_phi");
  //h_idmva.Fill2(mixtree, "photon_idmva");
  //h_mindR.Fill2(mixtree,"mindR");
  //h_cosTheta.Fill2(mixtree,"llphoton_cosTheta");
  //h_costheta.Fill2(mixtree,"llphoton_costheta");
  //llphoton_psi.Fill2(mixtree,"llphoton_psi");
  //llphoton_pt.Fill2(mixtree,"llphoton_pt");
  //llphoton_m.Fill2(mixtree,"llphoton_m");
  //h_ptErr.Fill2(mixtree, "photon_ptErr");
  //h_decopT.Fill2(mixtree, "decorr_photon_pt");
  h_npv.Fill2(mixtree, "npv");
  std::cout << "Fill2 ok" << std::endl;

  h_npv.PlotHists("npv (full 2017)");
  //llphoton_m.PlotHists("llphoton m (full 2017 clever)");
  //llphoton_pt.PlotHists("llphoton pT (full 2017 clever)");
  //photon_pt.PlotHists("photon pT (full 2017 clever)");
  //photon_eta.PlotHists("photon eta (full 2017 clever)");
  //photon_phi.PlotHists("photon phi (full 2017 clever)");
  //h_idmva.PlotHists("idmva (full 2017 clever)");
  //h_mindR.PlotHists("mindR (full 2017 clever)");
  //h_cosTheta.PlotHists("cosTheta  (full 2017 clever)");
  //h_costheta.PlotHists("costheta (full 2017 clever)");
  //llphoton_psi.PlotHists("Psi (full 2017 clever)");
  //h_ptErr.PlotHists("photon pT resolution (full 2017 clever)");
  //h_decopT.PlotHists("decorrelated photon pT (full 2017 clever)");
  //auto c = new TCanvas("c","c", 500,500);
  //idmva_endcap->Draw("HIST");
  //  c->Draw();
  std::cout << "n no match = " << nomatch << ", n mixing = " << mixing << std::endl; 
}






  
  
