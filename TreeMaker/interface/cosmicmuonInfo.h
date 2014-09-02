#ifndef __COSMICMUON_INFO_H_
#define __COSMICMUON_INFO_H_

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TTree.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "RKAnalyzer/TreeMaker/interface/utils.h"
#include "RKAnalyzer/TreeMaker/interface/baseTree.h"


class cosmicmuonInfo : public baseTree{

 public:
  cosmicmuonInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~cosmicmuonInfo();
  void Fill(const edm::Event& iEvent);
  void SetBranches();
  void Clear();

 private:
  cosmicmuonInfo(){};
  edm::InputTag cosmicmuonLabel_;
  bool isAOD_; 

  //variables which would become branches
  int Cosmicmuon_n;
  TClonesArray *Cosmicmuon_4Momentum, *Cosmicmuon_Vposition;
  TClonesArray *Cosmicmuon_OuterTrack_InnerPoint_position, *Cosmicmuon_OuterTrack_InnerPoint_momentum;
  TClonesArray *Cosmicmuon_OuterTrack_OuterPoint_position, *Cosmicmuon_OuterTrack_OuterPoint_momentum;
  TClonesArray *Cosmicmuon_InnerTrack_InnerPoint_position, *Cosmicmuon_InnerTrack_InnerPoint_momentum;
  TClonesArray *Cosmicmuon_InnerTrack_OuterPoint_position, *Cosmicmuon_InnerTrack_OuterPoint_momentum;
  TClonesArray *Cosmicmuon_OuterPoint_position, *Cosmicmuon_InnerPoint_position;

  std::vector<bool> Cosmicmuon_isGlobalCosmicmuon, Cosmicmuon_isTrackerCosmicmuon, Cosmicmuon_isStandAloneCosmicmuon, Cosmicmuon_InnerTrack_isNonnull, Cosmicmuon_OuterTrack_isNonnull;
  std::vector<int>   Cosmicmuon_validHits, Cosmicmuon_tkHits, Cosmicmuon_pixHits, Cosmicmuon_numberOfMatches, Cosmicmuon_charge;
  
};

#endif

