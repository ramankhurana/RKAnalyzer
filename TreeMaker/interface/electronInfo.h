#ifndef __ELECTRON_INFO_H_
#define __ELECTRON_INFO_H_

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include "TTree.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "RKAnalyzer/TreeMaker/interface/utils.h"
#include "RKAnalyzer/TreeMaker/interface/baseTree.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "MVAElectronID.h"

using namespace std;

class electronInfo : public baseTree{

 public:
  MVAElectronID  mvaelectronid;
  electronInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~electronInfo();
  void Fill(const edm::Event& iEvent, const edm::EventSetup& iSetup);
  void SetBranches();
  void Clear();
  
 private:
  electronInfo(){};
  edm::InputTag electronLabel_;


  //variables which would become branches
  int Electron_n;
  TClonesArray *Electron_4Momentum, *Electron_Vposition, *Electron_sc_position;

  std::vector<int>   Electron_charge       ;
  std::vector<float> Electron_SigmaIetaIeta;
  std::vector<float> Electron_HoE          ;
  std::vector<float> Electron_trkIso	   ;
  std::vector<float> Electron_ecalIso	   ;
  std::vector<float> Electron_hcalIso	   ;
  std::vector<float> Electron_dEtaIn	   ;
  std::vector<float> Electron_dPhiIn	   ;
  std::vector<float> Electron_sc_E         ;
  std::vector<float> Electron_sc_Eta       ;
  std::vector<float> Electron_missing_hits       ;
  std::vector<float> Electron_MVAID        ;
  std::vector<float> Electron_MVAID_new        ;
  std::vector<float> Electron_chargedIso   ;
  std::vector<float> Electron_photonIso    ;
  std::vector<float> Electron_neutralIso   ;
  std::vector<float> Electron_particleIso  ;
  std::vector<float> Electron_dbeta        ;
  
  std::vector<float> NewVz;
  
  std::vector<float> Electron_Significance3D ;
  std::vector<float> Electron_Value3D ;
  std::vector<float> Electron_Error3D ;


  std::vector<float> Electron_Significance2D ;
  std::vector<float> Electron_Value2D ;
  std::vector<float> Electron_Error2D ;
  
  std::vector<float> Electron_IsPF;
  std::vector<float> Electron_SigmaIetaIphi;
  std::vector<float>  Electron_IP3D;
  std::vector<float>  Electron_full5x5_sigmaEtaEta;
  std::vector<float>  Electron_full5x5_sigmaIetaIeta;
  std::vector<float>  Electron_full5x5_sigmaIphiIphi;
  std::vector<float>  Electron_full5x5_sigmaIetaIphi;
  std::vector<float>  Electron_full5x5_e1x5;
  std::vector<float>  Electron_full5x5_e2x5Max;
  std::vector<float>  Electron_full5x5_e5x5;
  std::vector<float>  Electron_full5x5_r9;
  std::vector<float>  Electron_full5x5_hcalDepth1OverEcal;
  std::vector<float>  Electron_full5x5_hcalDepth2OverEcal;
  std::vector<float>  Electron_full5x5_hcalOverEcal;
  std::vector<float>  Electron_full5x5_hcalDepth1OverEcalBc;
  std::vector<float>  Electron_full5x5_hcalDepth2OverEcalBc;
  std::vector<float>  Electron_full5x5_hcalOverEcalBc;
  
};

#endif

