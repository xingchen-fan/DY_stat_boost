// -*- C++ -*-
//
// Package:    AODmacthing/EventIDFilter
// Class:      EventIDFilter
//
/**\class EventIDFilter EventIDFilter.cc AODmacthing/EventIDFilter/plugins/EventIDFilterDY2018photon.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Xingchen Fan
//         Created:  Thu, 07 Mar 2024 16:10:43 GMT
//
//

// system include files
#include <memory>
#include <iostream>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TFile.h"
#include "TTree.h"

//
// class declaration
//

class EventIDFilterDY2018photon : public edm::stream::EDFilter<> {
public:
  explicit EventIDFilterDY2018photon(const edm::ParameterSet&);
  ~EventIDFilterDY2018photon();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginStream(edm::StreamID) override;
  virtual bool filter(edm::Event&, const edm::EventSetup&) override;
  virtual void endStream() override;
  std::vector<Long64_t> id_list;
  //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  // ----------member data ---------------------------
#ifdef THIS_IS_AN_EVENT_EXAMPLE
  edm::EDGetTokenT<ExampleData> exampleToken_;
#endif
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  edm::ESGetToken<SetupData, SetupRecord> setupToken_;
#endif
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
EventIDFilterDY2018photon::EventIDFilterDY2018photon(const edm::ParameterSet& iConfig) {
  //now do what ever initialization is needed
#ifdef THIS_IS_AN_EVENT_EXAMPLE
  exampleToken_ = consumes<ExampleData>(iConfig.getParameter<edm::InputTag>("examples"));
#endif
#ifdef THIS_IS_EN_EVENTSETUP_EXAMPLE
  setupToken_ = esConsumes<SetupData, SetupRecord>();
#endif
}

EventIDFilterDY2018photon::~EventIDFilterDY2018photon() {
  // do anything here that needs to be done at destruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
}

//
// member functions
//

// ------------ method called on each new Event  ------------
bool EventIDFilterDY2018photon::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  Long64_t event_aod = iEvent.id().event();
  bool match = false;
  for (unsigned int i(0); i < id_list.size(); i++){
    if (event_aod == id_list[i]) {
      match = true;
      break;
    }
  }
  return match;
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void EventIDFilterDY2018photon::beginStream(edm::StreamID) {
  auto picofile = TFile::Open("root://eosuser.cern.ch//eos/user/f/fanx/event_mixing/sample/DY_photon_output_kingscanyon_v1_2018.root", "READ");
  Long64_t event_pico = 0;
  auto tree = (TTree*)picofile->Get("tree");
  tree->SetBranchAddress("event", &event_pico);
  for (auto i(0); i < tree->GetEntries(); i++){
    tree->GetEntry(i);
    id_list.push_back(event_pico);
  }
  picofile->Close();
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void EventIDFilterDY2018photon::endStream() {
  id_list.clear();
}

// ------------ method called when starting to processes a run  ------------
/*
void
EventIDFilterDY2018photon::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
EventIDFilterDY2018photon::endRun(edm::Run const&, edm::EventSetup const&)
{
  
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
EventIDFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
EventIDFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void EventIDFilterDY2018photon::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(EventIDFilterDY2018photon);
