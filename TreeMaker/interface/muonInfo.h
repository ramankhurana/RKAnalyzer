#ifndef __MUON_INFO_H_
#define __MUON_INFO_H_
#include "MVAElectronID.h"

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include "TTree.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "RKAnalyzer/TreeMaker/interface/utils.h"
#include "RKAnalyzer/TreeMaker/interface/baseTree.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"

                                                                                                                   
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"


class muonInfo : public baseTree{

 public:
  muonInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~muonInfo();
  void Fill(const edm::Event& iEvent,  const edm::EventSetup& iSetup);
  void SetBranches();
  void Clear();

 private:
  muonInfo(){};
  edm::InputTag muonLabel_;
  bool isAOD_; 

  //variables which would become branches
  int Muon_n;
  TClonesArray *Muon_4Momentum, *Muon_Vposition;
  TClonesArray *Muon_OuterTrack_InnerPoint_position, *Muon_OuterTrack_InnerPoint_momentum;
  TClonesArray *Muon_OuterTrack_OuterPoint_position, *Muon_OuterTrack_OuterPoint_momentum;
  TClonesArray *Muon_InnerTrack_InnerPoint_position, *Muon_InnerTrack_InnerPoint_momentum;
  TClonesArray *Muon_InnerTrack_OuterPoint_position, *Muon_InnerTrack_OuterPoint_momentum;
  TClonesArray *Muon_OuterPoint_position, *Muon_InnerPoint_position;
  
  std::vector<bool> Muon_isGlobalMuon, Muon_isTrackerMuon, Muon_isStandAloneMuon, Muon_InnerTrack_isNonnull, Muon_OuterTrack_isNonnull, Muon_PFMuID;
  std::vector<float> Muon_trackIso, Muon_ecalIso, Muon_hcalIso, Muon_normChi2,Muon_NewVz;
  std::vector<int>   Muon_validHits, Muon_tkHits, Muon_pixHits, Muon_numberOfMatches, Muon_charge, Muon_trackerLayersWithMeasurement;
  
  std::vector<float>  Muon_Significance3D,  Muon_Value3D , Muon_Error3D;
  std::vector<float>  Muon_Significance2D,  Muon_Value2D , Muon_Error2D;
  
  std::vector<float> Muon_chargedIso , Muon_photonIso , Muon_neutralIso , Muon_particleIso , Muon_dbeta ;
  std::vector<float> Muon_DXY, Muon_DZ;
};

#endif

