#ifndef __PFMET_INFO_H_
#define __PFMET_INFO_H_

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TTree.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "RKAnalyzer/TreeMaker/interface/utils.h"
#include "RKAnalyzer/TreeMaker/interface/baseTree.h"


class pfmetInfo : public baseTree{

 public:
  pfmetInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~pfmetInfo();
  void Fill(const edm::Event& iEvent);
  void SetBranches();
  void Clear();
  float correct_phi(float phi);

 private:
  pfmetInfo(){};
  edm::InputTag pfmetLabel_;

  //variables which would become branches
  std::vector<float> PFMetPt,PFMetPx,PFMetPy,PFMetPhi,PFMetSumEt;
};

#endif

