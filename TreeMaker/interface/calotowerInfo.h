#ifndef __CALOTOWER_INFO_H_
#define __CALOTOWER_INFO_H_

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
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"

class calotowerInfo : public baseTree{

 public:
  calotowerInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~calotowerInfo();
  void Fill(const edm::Event& iEvent);
  void SetBranches();
  void Clear();

 private:
  calotowerInfo(){};
  edm::InputTag calotowerLabel_;

  //variables which would become branches
  int CaloTower_n;
  TClonesArray *CaloTower_HadPosition;
  std::vector<float> CaloTower_eta;        
  std::vector<float> CaloTower_phi;        
  std::vector<float> CaloTower_E;              
  std::vector<float> CaloTower_Et;	    
  std::vector<float> CaloTower_emEnergy;   
  std::vector<float> CaloTower_hadEnergy;  
  std::vector<float> CaloTower_p;	    
  std::vector<float> CaloTower_EMEt;	    
  std::vector<float> CaloTower_HadEt;	    
  std::vector<float> CaloTower_EMPhi;	    
  std::vector<float> CaloTower_EMEta;	    
  std::vector<float> CaloTower_HE_E;	    
  std::vector<float> CaloTower_HB_E;	    
  std::vector<float> CaloTower_EMTime;	    
  std::vector<float> CaloTower_HadTime;    
};
#endif
