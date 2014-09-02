#ifndef __GENJET_INFO_H_
#define __GENJET_INFO_H_

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TTree.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "RKAnalyzer/TreeMaker/interface/utils.h"
#include "RKAnalyzer/TreeMaker/interface/baseTree.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

class genjetInfo : public baseTree{

 public:
  genjetInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~genjetInfo();
  void Fill(const edm::Event& iEvent);
  void SetBranches();
  void Clear();

 private:
  genjetInfo(){};
  edm::InputTag genjetLabel_;

  //variables which would become branches
  int GENJet_n;
  TClonesArray *GENJet_4Momentum;
    
};

#endif

