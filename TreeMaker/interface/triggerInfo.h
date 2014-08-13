#ifndef __TRIGGER_INFO_H_
#define __TRIGGER_INFO_H_


#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TTree.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "RKAnalyzer/TreeMaker/interface/utils.h"
#include "RKAnalyzer/TreeMaker/interface/baseTree.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Common/interface/TriggerNames.h"
using namespace std;
using namespace edm;


class triggerInfo : public baseTree{

 public:
  triggerInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~triggerInfo();
  void Fill(const edm::Event& iEvent, const edm::EventSetup& iSetup, 
	    std::vector<std::string>& all_triggers, 
	    std::vector<std::string>& electron_triggers,
	    std::vector<std::string>& muon_triggers,
	    std::vector<std::string>& emu_triggers,
	    HLTConfigProvider& hltConfig_, 
	    std::string& hltlabel_, const size_t& MaxN );
  void SetBranches();
  void Clear();

 private:
  triggerInfo(){};
  edm::InputTag HLTriggerResults_;
  edm::InputTag triggerEventTag_; 

  
  TString module_type[50];
  float trobjpt[100][100][10];
  float trobjeta[100][100][10];
  float trobjphi[100][100][10];
  int   lastFilterIndex[100];
  int   lastFilterIndexHLT135[100];

  int ntriggers;
  std::vector<int>  all_triggerprescales;
  std::vector<bool> all_ifTriggerpassed;

  std::vector<bool> doubleElectron_ifTriggerpassed;
  std::vector<bool> doubleMuon_ifTriggerpassed;
  std::vector<bool> emu_ifTriggerpassed;
  
  std::vector<std::string>  hlNames_;

  
};

#endif

