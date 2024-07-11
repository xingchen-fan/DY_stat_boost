#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include "MVAfunc.h"
#include "TVector3.h"
#include "TROOT.h"

TLorentzVector AssignL1(vector<int> ll_lepid, vector<int> ll_i1, vector<int> ll_i2, vector<int> el_charge, vector<int> mu_charge, vector<float> el_pt, vector<float> el_eta, vector<float> el_phi,vector<float> mu_pt, vector<float> mu_eta, vector<float> mu_phi){
  TLorentzVector l1;
  int i1 = -1;
  if (ll_lepid[0] == 11){
    if(el_charge[ll_i1[0]] < 0) i1 = ll_i1[0];
    else i1 = ll_i2[0];
    l1.SetPtEtaPhiM(el_pt[i1], el_eta[i1], el_phi[i1], 0.00511);
    
	/*if(el_charge[0] < 0) i1 = 0;
    else i1 = 1;
    l1.SetPtEtaPhiM(el_pt[i1], el_eta[i1], el_phi[i1], 0.00511);*/
  }
  else if (ll_lepid[0] == 13){
    if(mu_charge[ll_i1[0]] < 0) i1 = ll_i1[0]; 
    else i1 = ll_i2[0];
    l1.SetPtEtaPhiM(mu_pt[i1], mu_eta[i1], mu_phi[i1], 0.105);
	  /*if(mu_charge[0] < 0) i1 = 0; 
    else i1 = 1;
    l1.SetPtEtaPhiM(mu_pt[i1], mu_eta[i1], mu_phi[i1], 0.105);*/

  }
  //cout<< "l1_id = "<<i1<< endl;
  return l1;
}

TLorentzVector AssignL2(vector<int> ll_lepid, vector<int> ll_i1, vector<int> ll_i2, vector<int> el_charge, vector<int> mu_charge, vector<float> el_pt, vector<float> el_eta, vector<float> el_phi,vector<float> mu_pt, vector<float> mu_eta, vector<float> mu_phi){
  TLorentzVector l2;
  int i1 = -1;
  if (ll_lepid[0] == 11){
    if(el_charge[ll_i1[0]] < 0) i1 = ll_i2[0];
    else i1 = ll_i1[0];
    l2.SetPtEtaPhiM(el_pt[i1], el_eta[i1], el_phi[i1], 0.00511);
	  /*if(el_charge[0] < 0) i1 = 1;
    else i1 = 0;
    l2.SetPtEtaPhiM(el_pt[i1], el_eta[i1], el_phi[i1], 0.00511);*/
  }
  else if (ll_lepid[0] == 13){
    if(mu_charge[ll_i1[0]] < 0) i1 = ll_i2[0];
    else i1 = ll_i1[0];
    l2.SetPtEtaPhiM(mu_pt[i1], mu_eta[i1], mu_phi[i1], 0.105);
	  /*if(mu_charge[0] < 0) i1 = 1;
    else i1 = 0;
    l2.SetPtEtaPhiM(mu_pt[i1], mu_eta[i1], mu_phi[i1], 0.105);*/
  }
  //cout<< "l2_id = "<<i1<<endl;
  return l2;
}

TLorentzVector AssignZ(vector<float> ll_pt, vector<float> ll_eta, vector<float> ll_phi, vector<float> ll_m){
  TLorentzVector Z;
  Z.SetPtEtaPhiM(ll_pt[0], ll_eta[0], ll_phi[0], ll_m[0]);
  return Z;
}

TLorentzVector AssignGamma(vector<float> photon_pt, vector<float> photon_eta, vector<float> photon_phi){
  TLorentzVector g;
  g.SetPtEtaPhiM(photon_pt[0],photon_eta[0], photon_phi[0],0);
  return g;
}

TLorentzVector AssignH(vector<float> llphoton_pt, vector<float> llphoton_eta, vector<float> llphoton_phi, vector<float> llphoton_m){
  TLorentzVector h;
  h.SetPtEtaPhiM(llphoton_pt[0],llphoton_eta[0], llphoton_phi[0],llphoton_m[0]);
  return h;
}

TLorentzVector AssignHtweak(TLorentzVector l1, TLorentzVector l2, TLorentzVector g){
  TLorentzVector h;
  h = l1 + l2 + g;
  return h;
}

TLorentzVector AssignQ1(TLorentzVector h){
  TVector3 htran = h.BoostVector();
  htran.SetZ(0);
  TLorentzVector hH = h;
  hH.Boost(-1*htran);
  TLorentzVector k1;
  double pz, E;
  pz = hH.Pz() + hH.E();
  E  = hH.E()  + hH.Pz();
  k1.SetPxPyPzE(0,0,pz/2,E/2);
  k1.Boost(htran);
  return k1;
}

TLorentzVector AssignQ2(TLorentzVector h) {
  TLorentzVector k2;
  TVector3 htran = h.BoostVector();
  TLorentzVector hH = h;
  htran.SetZ(0);
  hH.Boost(-1*htran);
  double pz, E;
  pz = hH.Pz() - hH.E();
  E  = hH.E()  - hH.Pz();
  k2.SetPxPyPzE(0,0,pz/2,E/2);
  k2.Boost(htran);
  return k2;
}

double lambdaZ(TLorentzVector h, TLorentzVector z){
  double M = h.M();
  double mll = z.M();
  return sqrt(pow(h.Dot(z)/M, 2) - pow(mll,2));
}

double cos_theta(TLorentzVector h, TLorentzVector z, TLorentzVector l1, TLorentzVector l2){
  double M = h.M();
  double lz = lambdaZ(h, z);
  double ctheta = h.Dot(l1-l2)/(M*lz);
  if (ctheta>1) ctheta = 0.999;
  if (ctheta<-1) ctheta = -0.999;
  //cout  << "h = ";
  //h.Print();
  //cout  << "l1 = ";
  //l1.Print();
  //cout  << "l2 = ";
  //l2.Print();
  //cout << endl;
  return ctheta;
}

double cos_Theta(TLorentzVector h, TLorentzVector z, TLorentzVector q1, TLorentzVector q2){
  double M = h.M();
  double lz = lambdaZ(h,z);
  double cosTheta = z.Dot(q2-q1)/(M*lz);
  if(abs(cosTheta) > 1.01) cout << "ERROR: cTheta = " << cosTheta <<  endl;
  return cosTheta;
}

double Getphi(TLorentzVector q11, TLorentzVector q12, TLorentzVector z1, TLorentzVector l11, TLorentzVector l21, TLorentzVector h){
  TVector3 htran = h.BoostVector();
  TLorentzVector q1b = q11;
  TLorentzVector zb = z1;
  TLorentzVector l1b = l11;
  TLorentzVector l2b = l21;
  q1b.Boost(-1*htran);
  l1b.Boost(-1*htran);
  l2b.Boost(-1*htran);
  zb.Boost(-1*htran);

  TVector3 q1 = q1b.Vect();
  TVector3 z = zb.Vect();
  TVector3 l1 = l1b.Vect();
  TVector3 l2 = l2b.Vect();
  /*cout << "l1 = ";
    l1.Print();
    cout << "l2 = ";
    l2.Print();
    cout << "z = ";
    z.Print();
    cout << "q1 = ";
    q1.Print();*/

  double cosphi, sinphi, sinTheta;
  sinTheta = sqrt(1 - pow(cos_Theta(h, z1, q11, q12), 2));
  cosphi = -1*l1.Cross(l2).Dot(q1.Cross(z))/l1.Cross(l2).Mag()/q1.Cross(z).Mag();
  sinphi = -1*l1.Cross(l2).Dot(q1)/l1.Cross(l2).Mag()/q1.Mag()/sinTheta;
  
  double phi(0);
  if(abs(cosphi) > 1.01) cout << "ERROR: cphi = " << cosphi <<  endl;
  if(cosphi > 1) cosphi = 1;
  if(cosphi < -1) cosphi = -1;
  if(sinphi < 0) phi = -1*acos(cosphi);
  else           phi = acos(cosphi);
  //cout << "cosphi = " << cosphi << endl << "sinphi = " << sinphi << endl; 
  return phi;
}

double GetmindR (TLorentzVector l1, TLorentzVector l2, TLorentzVector g){
  double dR1 = l1.DeltaR(g);
  double dR2 = l2.DeltaR(g);
  if (dR1>dR2) return dR2;
  else return dR1;
}

double GetmaxdR (TLorentzVector l1, TLorentzVector l2, TLorentzVector g){
  double dR1 = l1.DeltaR(g);
  double dR2 = l2.DeltaR(g);
  if (dR1>dR2) return dR1;
  else return dR2;
}
