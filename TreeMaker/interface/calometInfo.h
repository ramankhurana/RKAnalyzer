#ifndef __CALOMET_INFO_H_
#define __CALOMET_INFO_H_

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


class calometInfo : public baseTree{

 public:
  calometInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~calometInfo();
  void Fill(const edm::Event& iEvent);
  void SetBranches();
  void Clear();
  float correct_phi(float phi);

 private:
  calometInfo(){};
  edm::InputTag calometLabel_;
  bool fillgenmetInfo_;

  //variables which would become branches
  std::vector<float> CalometPt,CalometPx,CalometPy,CalometPhi,CalometSumEt;
  float genMetPt   ;
  float genMetPhi  ;
  float genMetSumEt;
  float genMetPx   ;
  float genMetPy   ;
};

#endif

