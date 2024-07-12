//#include "GeneratorInterface/GenFilters/plugins/MatchDYFilter.h"
#include "GeneratorInterface/GenFilters/interface/MatchDYFilter.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
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
  bool found_lep = false;
  int el_counter = 0;
  int mu_counter = 0;
  int el_pdg = 0;
  int mu_pdg = 0;

  using namespace edm;
  Handle<HepMCProduct> evt;
  iEvent.getByToken(token_, evt);
  
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
    else if (el_counter == 0 && abs((*p)->pdg_id()) == 11 /*&& mother->pdg_id() == 23*/){
      el_counter ++;
      el_pdg = (*p)->pdg_id();
    }
    else if (el_counter == 1 && (*p)->pdg_id() == -el_pdg /*&& mother->pdg_id() == 23*/){
      el_counter ++;
    }
    else if (mu_counter == 0 && abs((*p)->pdg_id()) == 13 /*&& mother->pdg_id() == 23*/){
      mu_counter ++;
      mu_pdg = (*p)->pdg_id();
    }
    else if (mu_counter == 1 && (*p)->pdg_id() == -mu_pdg /*&& mother->pdg_id() == 23*/){
      mu_counter ++;
    }
  }
  found_lep = el_counter == 2 || mu_counter == 2;
  return found_lep&&found_ph;
}
