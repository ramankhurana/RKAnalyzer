#ifndef __TCMET_INFO_H_
#define __TCMET_INFO_H_

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


class tcmetInfo : public baseTree{

 public:
  tcmetInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~tcmetInfo();
  void Fill(const edm::Event& iEvent);
  void SetBranches();
  void Clear();
  float correct_phi(float phi);

 private:
  tcmetInfo(){};
  edm::InputTag tcmetLabel_;

  //variables which would become branches
  std::vector<float> TcmetPt,TcmetPx,TcmetPy,TcmetPhi,TcmetSumEt;
};

#endif

