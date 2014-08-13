#ifndef __EVENT_INFO_H_
#define __EVENT_INFO_H_


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
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "DataFormats/Common/interface/ConditionsInEdm.h"
#include "DataFormats/Luminosity/interface/LumiSummary.h"

using namespace std;
using namespace edm;


class eventInfo : public baseTree{

 public:
  eventInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~eventInfo();
  void Fill(const edm::Event& iEvent);
  void SetBranches();


 private:
  eventInfo(){};
  edm::InputTag lumiLabel_;
  edm::InputTag lumiProducerLabel_;

  //variables which would become branches
  unsigned int RunNumber, EventNumber, LumiNumber, BXNumber;
  unsigned int totalIntensityBeam1, totalIntensityBeam2;
  int Event_n;
  float avgInsDelLumi, avgInsDelLumiErr, avgInsRecLumi, avgInsRecLumiErr;
  
};

#endif

