// -*- C++ -*-
//
// Package:    MyFilters/EventIDFilter22
// Class:      EventIDFilter22
//
/**\class EventIDFilter22 EventIDFilter22.cc MyFilters/EventIDFilter22/plugins/EventIDFilter22.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Xingchen Fan
//         Created:  Sun, 09 Aug 2026 22:10:47 GMT
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
using namespace std;
//
// class declaration
//

class EventIDFilter22 : public edm::stream::EDFilter<> {
public:
  explicit EventIDFilter22(const edm::ParameterSet&);
  ~EventIDFilter22() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginStream(edm::StreamID) override;
  bool filter(edm::Event&, const edm::EventSetup&) override;
  void endStream() override;
  vector<Long64_t> id_list;
  vector<int> lumi_list;
  vector<int> run_list;
  //void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //void endRun(edm::Run const&, edm::EventSetup const&) override;
  //void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

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
EventIDFilter22::EventIDFilter22(const edm::ParameterSet& iConfig) {
  //now do what ever initialization is needed
#ifdef THIS_IS_AN_EVENT_EXAMPLE
  exampleToken_ = consumes<ExampleData>(iConfig.getParameter<edm::InputTag>("examples"));
#endif
#ifdef THIS_IS_EN_EVENTSETUP_EXAMPLE
  setupToken_ = esConsumes<SetupData, SetupRecord>();
#endif
}

EventIDFilter22::~EventIDFilter22() {
  // do anything here that needs to be done at destruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
}

//
// member functions
//

// ------------ method called on each new Event  ------------
bool EventIDFilter22::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  Long64_t event_aod = iEvent.id().event();
  int lumi_aod = iEvent.id().luminosityBlock();
  int run_aod = iEvent.id().run();
  bool match = false;
  for (long unsigned int i(0); i < id_list.size(); i++){
    if (event_aod == id_list[i] && lumi_aod == lumi_list[i] && run_aod == run_list[i]) {
      match = true;
      break;
    }
  }
  return match;
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void EventIDFilter22::beginStream(edm::StreamID) {
  // please remove this method if not needed
  auto picofile = TFile::Open("root://eosuser.cern.ch//eos/user/f/fanx/DY_output_redwood_v1_2022.root", "READ");
  Long64_t event_pico = 0;
  int lumi_pico = 0;
  int run_pico = 0;
  auto tree = (TTree*)picofile->Get("tree");
  tree->SetBranchAddress("event", &event_pico);
  tree->SetBranchAddress("lumiblock", &lumi_pico);
  tree->SetBranchAddress("run", &run_pico);
  for (auto i(0); i < tree->GetEntries(); i++){
    tree->GetEntry(i);
    id_list.push_back(event_pico);
    lumi_list.push_back(lumi_pico);
    run_list.push_back(run_pico);
  }
  picofile->Close();
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void EventIDFilter22::endStream() {
  // please remove this method if not needed
  id_list.clear();
  lumi_list.clear();
  run_list.clear();
}

// ------------ method called when starting to processes a run  ------------
/*
void
EventIDFilter22::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
EventIDFilter22::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
EventIDFilter22::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
EventIDFilter22::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void EventIDFilter22::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(EventIDFilter22);
