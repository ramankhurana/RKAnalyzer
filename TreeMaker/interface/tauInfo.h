#ifndef __TAU_INFO_H_
#define __TAU_INFO_H_



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
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "PhysicsTools/JetMCUtils/interface/JetMCTag.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

class tauInfo : public baseTree{

 public:
  tauInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~tauInfo();
  void Fill(const edm::Event& iEvent);
  void SetBranches();
  void Clear();

 private:
  tauInfo(){};
  edm::InputTag tauLabel_;
  bool fillDetailTauInfo_;

  int Tau_n;
  TClonesArray *Tau_4Momentum, *Tau_Vposition;
  std::vector<int>                  Tau_charge       ;
  std::vector<int>                  Tau_nPions       ;
  std::vector<int>                  Tau_nPi0	     ;
  std::vector<int>                  Tau_nPhotons     ;
  std::vector<int>                  oneProng0Pi0     ;
  std::vector<int>                  oneProng1Pi0     ;
  std::vector<int>                  oneProng2Pi0     ;
  int                               nthreeProng0Pi0  ;
  std::vector<int>                  threeProng0Pi0   ;
  int                               nthreeProng1Pi0  ;
  std::vector<int>                  threeProng1Pi0   ;
  int                               ntauelectron     ;
  std::vector<int>                  tauelectron	     ;
  int                               ntaumuon	     ;
  std::vector<int>                  taumuon	     ;

  std::vector<float>                oneProng0Pi0Pt   ;
  std::vector<float>                oneProng0Pi0Eta  ;
  std::vector<float>                oneProng0Pi0Phi  ;
  std::vector<float>                genHadTauPt	     ;
  std::vector<float>                genHadTauEta     ;
  std::vector<float>                genHadTauPhi     ;
  
  std::vector<std::vector<int> >    Tau_PionPdgId    ;
  std::vector<std::vector<float> >  Tau_PionPt	     ;
  std::vector<std::vector<float> >  Tau_PionEta	     ;
  std::vector<std::vector<float> >  Tau_PionPhi	     ;
  std::vector<std::vector<int> >    Tau_Pi0PdgId     ;
  std::vector<std::vector<float> >  Tau_Pi0Pt	     ;
  std::vector<std::vector<float> >  Tau_Pi0Eta	     ;
  std::vector<std::vector<float> >  Tau_Pi0Phi	     ;
  std::vector<std::vector<int> >    Tau_PhotonPdgId  ;
  std::vector<std::vector<float> >  Tau_PhotonPt     ;
  std::vector<std::vector<float> >  Tau_PhotonEta    ;
  std::vector<std::vector<float> >  Tau_PhotonPhi    ; 

  std::vector<std::string> genTauDecayMode1 ;
  std::string genTauDecayMode;

  
};

#endif

