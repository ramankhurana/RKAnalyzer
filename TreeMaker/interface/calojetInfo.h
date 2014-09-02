#ifndef __CALOJET_INFO_H_
#define __CALOJET_INFO_H_

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
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"


class calojetInfo : public baseTree{

 public:
  calojetInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~calojetInfo();
  void Fill(const edm::Event& iEvent, const edm::EventSetup& iSetup);
  void SetBranches();
  void Clear();

 private:
  calojetInfo(){};
  edm::InputTag calojetLabel_;

  //variables which would become branches
  int Calojet_n;
  TClonesArray *Calojet_4Momentum, *Calojet_Vposition, *UcCalojet_4Momentum;

  std::vector<float> Calojet_emEnergyFraction  		  ;
  std::vector<float> Calojet_energyFractionHadronic	  ;	       
  std::vector<float> Calojet_fHPD       		  ;
  std::vector<float> Calojet_fRBX       		  ;
  std::vector<float> Calojet_RHF       			  ;
  std::vector<int>   Calojet_nTowers		       	  ;
  std::vector<int>   Calojet_hitsInN90     		  ;
  std::vector<int>   Calojet_n90Hits                	  ;
  std::vector<float> Calojet_jecUncer  		       	  ;
  std::vector<float> Calojet_jecCorr                      ;
    
};

#endif

