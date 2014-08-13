#ifndef __RHO_INFO_H_
#define __RHO_INFO_H_

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


class rhoInfo : public baseTree{

 public:
  rhoInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~rhoInfo();
  void Fill(const edm::Event& iEvent);
  void SetBranches();


 private:
  rhoInfo(){};
  edm::InputTag rhoLabel_, sigmaLabel_,sigma44Label_,rho44Label_;
  //variables which would become branches
  float rho,rho44,sigma,sigma44;
    
};

#endif

