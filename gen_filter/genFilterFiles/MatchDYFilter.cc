//#include "GeneratorInterface/GenFilters/plugins/MatchDYFilter.h"
#include "GeneratorInterface/GenFilters/interface/MatchDYFilter.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include <iostream>

using namespace edm;
using namespace std;

MatchDYFilter::MatchDYFilter(const edm::ParameterSet& iConfig)
  :token_(consumes<edm::HepMCProduct>(
				      edm::InputTag(iConfig.getUntrackedParameter("moduleLabel", std::string("generator")), "unsmeared"))),
   pdgID(iConfig.getUntrackedParameter<int>("pdgID")),
   motherPdgID(iConfig.getUntrackedParameter<std::vector<int>>("motherPdgID")),
   minPt((float)iConfig.getUntrackedParameter<double>("minPt")){
  }

bool MatchDYFilter::filter(edm::StreamID, edm::Event& iEvent, const edm::EventSetup&) const{
  bool found_ph = false;
  int mother_pdgid = 0;
    
  using namespace edm;
  Handle<HepMCProduct> evt;
  iEvent.getByToken(token_, evt);
  vector<math::XYZTLorentzVector> photons;
  vector<math::XYZTLorentzVector> el_m;
  vector<math::XYZTLorentzVector> el_p;
  vector<math::XYZTLorentzVector> mu_m;
  vector<math::XYZTLorentzVector> mu_p;
  math::XYZTLorentzVector max_photon;
  math::XYZTLorentzVector photon_cluster;
  unsigned int max_photon_ind = 0;
  int index = 0;
  float maxpT_ph = -1.0;
  float maxpT_el_p = -1.0, maxpT_el_m = -1.0, maxpT_mu_p = -1.0, maxpT_mu_m = -1.0;
  int max_el_p_index = -1, max_el_m_index = -1, max_mu_p_index = -1, max_mu_m_index = -1;
  
  const HepMC::GenEvent* myGenEvent = evt->GetEvent();
  for (HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin(); p != myGenEvent->particles_end(); ++p) {
    HepMC::GenParticle* mother = (*((*p)->production_vertex()->particles_in_const_begin()));
    if (abs((*p)->pdg_id()) == pdgID && (*p)->status() == 1 && (*p)->momentum().perp() > minPt){
      for (unsigned int i = 0; i < motherPdgID.size(); i++){
	if (abs(mother->pdg_id()) == motherPdgID[i]){
	  found_ph = true;
	}
      }
    }
    if (abs((*p)->pdg_id()) == pdgID && (*p)->status() == 1){
      math::XYZTLorentzVector ph_((*p)->momentum().px(), (*p)->momentum().py(), (*p)->momentum().pz(), (*p)->momentum().e());
      mother_pdgid = abs(mother->pdg_id());
      bool main_ph = false;
      for (unsigned int i = 0; i < motherPdgID.size(); i++){
	if (mother_pdgid == motherPdgID[i]) main_ph = true;
      }
      if (main_ph) {
	if ((*p)->momentum().perp() > maxpT_ph){
	  maxpT_ph = (*p)->momentum().perp();
	  max_photon =  ph_;
	  max_photon_ind = index;
	}
      }
      photons.push_back(ph_);
      index += 1;
    }
    else if ((*p)->pdg_id() == 11){
      math::XYZTLorentzVector el_ ((*p)->momentum().px(), (*p)->momentum().py(), (*p)->momentum().pz(), (*p)->momentum().e());
      if ((*p)->momentum().perp() > maxpT_el_m){
	maxpT_el_m = (*p)->momentum().perp();
	max_el_m_index = el_m.size();
      }
      el_m.push_back(el_);
    }
    else if ((*p)->pdg_id() == -11){
      math::XYZTLorentzVector el_ ((*p)->momentum().px(), (*p)->momentum().py(), (*p)->momentum().pz(), (*p)->momentum().e());
      if ((*p)->momentum().perp() > maxpT_el_p){
        maxpT_el_p = (*p)->momentum().perp();
        max_el_p_index = el_p.size();
      }
      el_p.push_back(el_);
    }
    else if ((*p)->pdg_id() == 13){
      math::XYZTLorentzVector mu_ ((*p)->momentum().px(), (*p)->momentum().py(), (*p)->momentum().pz(), (*p)->momentum().e());
      if ((*p)->momentum().perp() > maxpT_mu_m){
        maxpT_mu_m = (*p)->momentum().perp();
        max_mu_m_index = mu_m.size();
      }
      mu_m.push_back(mu_);
    }
    else if ((*p)->pdg_id() == -13){
      math::XYZTLorentzVector mu_ ((*p)->momentum().px(), (*p)->momentum().py(), (*p)->momentum().pz(), (*p)->momentum().e());
      if ((*p)->momentum().perp() > maxpT_mu_p){
        maxpT_mu_p = (*p)->momentum().perp();
        max_mu_p_index = mu_p.size();
      }
      mu_p.push_back(mu_);
    }

    /*
    else if (el_counter == 0 && abs((*p)->pdg_id()) == 11 ){
      el_counter ++;
      el_pdg = (*p)->pdg_id();
    }
    else if (el_counter == 1 && (*p)->pdg_id() == -el_pdg ){
      el_counter ++;
    }
    else if (mu_counter == 0 && abs((*p)->pdg_id()) == 13 ){
      mu_counter ++;
      mu_pdg = (*p)->pdg_id();
    }
    else if (mu_counter == 1 && (*p)->pdg_id() == -mu_pdg ){
      mu_counter ++;
    }
  */
    
      
  }
  bool z_mass_good = false;
  if (photons.size() > 0){
    for (unsigned int j = 0; j < photons.size(); j++) {
      if ( j != max_photon_ind && reco::deltaR(max_photon, photons[j]) < 0.1) max_photon += photons[j];
    }
  }
  if (el_m.size() > 0 && el_p.size() > 0){
    float z_mass = (el_m[max_el_m_index] + el_p[max_el_p_index]).mass();
    if (z_mass > 75 && z_mass < 105) z_mass_good = true;
  }
  if (mu_m.size() > 0 && mu_p.size() > 0){
    float z_mass = (mu_m[max_mu_m_index] + mu_p[max_mu_p_index]).mass();
    if (z_mass > 75 && z_mass < 105) z_mass_good = true;
  }
  bool cluster_pt = max_photon.pt() > 10;
  return cluster_pt&&found_ph&&z_mass_good;
}
