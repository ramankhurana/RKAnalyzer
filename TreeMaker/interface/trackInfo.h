#ifndef __TRACK_INFO_H_
#define __TRACK_INFO_H_


#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TTree.h"
#include "TVector3.h"
#include "TClonesArray.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "RKAnalyzer/TreeMaker/interface/utils.h"
#include "RKAnalyzer/TreeMaker/interface/baseTree.h"


class trackInfo : public baseTree{

 public:
  trackInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~trackInfo();
  void Fill(const edm::Event& iEvent);
  void SetBranches();
  void Clear();

 private:
  trackInfo(){};
  edm::InputTag trackLabel_;

  //variables which would become branches
  int Track_n;
  TClonesArray *Track_Vposition, *Track_3Momentum; 
  bool Scraping_isScrapingEvent;  
};

#endif

