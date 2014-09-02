#ifndef __PFJET_INFO_H_
#define __PFJET_INFO_H_

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
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"


class pfjetInfo : public baseTree{

 public:
  pfjetInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~pfjetInfo();
  void Fill(const edm::Event& iEvent, const edm::EventSetup& iSetup);
  void SetBranches();
  void Clear();

 private:
  pfjetInfo(){};
  edm::InputTag pfjetLabel_;

  //variables which would become branches
  int PFJet_n;
  TClonesArray *PFJet_4Momentum, *PFJet_Vposition, *UcPFJet_4Momentum;

  std::vector<float> PFJet_CEF;    		       
  std::vector<float> PFJet_NEF;  		       
  std::vector<float> PFJet_CHF;  		       
  std::vector<float> PFJet_NHF;  		       
  std::vector<float> PFJet_HFHAE;		       
  std::vector<float> PFJet_HFEME;		       
  std::vector<int> PFJet_NCH;  		       
  std::vector<int> PFJet_NConstituents;            
  std::vector<float> PFJet_TrackCountHiEffBJetTags  ;
  std::vector<float> PFJet_TrackCountHiPurBJetTags  ;
  std::vector<float> PFJet_SimpleSVHiEffBJetTags    ;
  std::vector<float> PFJet_SimpleSVHiPurBJetTags    ;
  std::vector<float> PFJet_CombinedSecondaryVertexBJetTags    ;
  std::vector<float> PFJet_JetProbabilityBJetTags   ;
  std::vector<float> PFJet_jecUncer		  ;     
  std::vector<float> PFJet_jecCorr                  ;
    
};

#endif

