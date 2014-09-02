#ifndef __GenHiggs_INFO_H_
#define __GenHiggs_INFO_H_

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
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
class genHiggsInfo : public baseTree{

 public:
  genHiggsInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~genHiggsInfo();
  void Fill(const edm::Event& iEvent);
  void SetBranches();
  void Clear();
  
 private:
  genHiggsInfo(){};
  edm::InputTag tauLabel_;
  //variables which would become branches
  TClonesArray *H_Gen_4Momentum;
  std::vector<float> HMass;
};

#endif

