#ifndef MVAfunc
#define MVAfunc

TLorentzVector AssignL1(vector<int> ll_lepid, vector<int> ll_i1, vector<int> ll_i2, vector<int> el_charge, vector<int> mu_charge, vector<float> el_pt, vector<float> el_eta, vector<float> el_phi,vector<float> mu_pt, vector<float> mu_eta, vector<float> mu_phi);

TLorentzVector AssignL2(vector<int> ll_lepid, vector<int> ll_i1, vector<int> ll_i2, vector<int> el_charge, vector<int> mu_charge, vector<float> el_pt, vector<float> el_eta, vector<float> el_phi,vector<float> mu_pt, vector<float> mu_eta, vector<float> mu_phi);

TLorentzVector AssignZ(vector<float> ll_pt, vector<float> ll_eta, vector<float> ll_phi, vector<float> ll_m);
TLorentzVector AssignGamma(vector<float> photon_pt, vector<float> photon_eta, vector<float> photon_phi);
TLorentzVector AssignH(vector<float> llphoton_pt, vector<float> llphoton_eta, vector<float> llphoton_phi, vector<float> llphoton_m);
TLorentzVector AssignHtweak(TLorentzVector l1, TLorentzVector l2, TLorentzVector g);
TLorentzVector AssignQ1(TLorentzVector h);
TLorentzVector AssignQ2(TLorentzVector h);
double lambdaZ(TLorentzVector h, TLorentzVector z);
double cos_theta(TLorentzVector h, TLorentzVector z, TLorentzVector l1, TLorentzVector l2);
double cos_Theta(TLorentzVector h, TLorentzVector z, TLorentzVector q1, TLorentzVector q2);
double Getphi(TLorentzVector q11,TLorentzVector q12, TLorentzVector z1, TLorentzVector l11, TLorentzVector l21, TLorentzVector h);
double GetmindR(TLorentzVector l1, TLorentzVector l2, TLorentzVector g);
double GetmaxdR (TLorentzVector l1, TLorentzVector l2, TLorentzVector g);

#endif
