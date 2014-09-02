#ifndef __BEAMHALO_INFO_H_
#define __BEAMHALO_INFO_H_


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
#include "DataFormats/METReco/interface/BeamHaloSummary.h"

using namespace std;
using namespace edm;


class beamhaloInfo : public baseTree{

 public:
  beamhaloInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~beamhaloInfo();
  void Fill(const edm::Event& iEvent);
  void SetBranches();


 private:
  beamhaloInfo(){};
  edm::InputTag beamhaloLabel_;

  //variables which would become branches
  bool isBeamHaloIDTightPass;
  bool isBeamHaloIDLoosePass;  

  bool isBeamHaloEcalLoosePass;
  bool isBeamHaloHcalLoosePass;
  bool isBeamHaloCSCLoosePass;
  bool isBeamHaloGlobalLoosePass; 

  bool isBeamHaloEcalTightPass;
  bool isBeamHaloHcalTightPass;
  bool isBeamHaloCSCTightPass;
  bool isBeamHaloGlobalTightPass; 

  bool isSmellsLikeHalo_Tag;
  bool isLooseHalo_Tag;   
  bool isTightHalo_Tag;           
  bool isExtremeTightHalo_Tag;


  };
#endif
