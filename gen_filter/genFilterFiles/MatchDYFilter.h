#ifndef MatchDYFilter_h
#define MatchDYFilter_h

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/global/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//-----------------------------------------------------------------------
//Only accept positive pdg ID
//Require a certain particle who has a certain mother and has a pT > minpT
//-----------------------------------------------------------------------

namespace edm {
  class HepMCProduct;
}

class MatchDYFilter : public edm::global::EDFilter<> {
 public:
  explicit MatchDYFilter(const edm::ParameterSet&);
  ~MatchDYFilter() override{};

  bool filter(edm::StreamID, edm::Event&, const edm::EventSetup&) const override;

 private:
  const edm::EDGetTokenT<edm::HepMCProduct> token_;
  const int pdgID;
  const std::vector<int> motherPdgID;
  const float minPt;
};
#endif
