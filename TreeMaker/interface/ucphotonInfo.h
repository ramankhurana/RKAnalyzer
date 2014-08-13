#ifndef __UCPHOTON_INFO_H_
#define __UCPHOTON_INFO_H_

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
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "RKAnalyzer/TreeMaker/interface/utils.h"
#include "RKAnalyzer/TreeMaker/interface/baseTree.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h" 
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloTopology/interface/EcalBarrelTopology.h"
#include "Geometry/CaloTopology/interface/EcalEndcapTopology.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

using namespace std;

class ucphotonInfo : public baseTree{

 public:
  ucphotonInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~ucphotonInfo();
  void Fill(const edm::Event& iEvent, const edm::EventSetup& iSetup);
  void SetBranches();
  void Clear();
  float correct_phi(float phi);
  float recHitE( const DetId id, const EcalRecHitCollection &recHits );
  float recHitE( const DetId id, const EcalRecHitCollection &recHits, int dEta, int dPhi );
  float recHitE( const DetId id, const EcalRecHitCollection &recHits , bool useTimingInfo);
  float recHitApproxEt( const DetId id, const EcalRecHitCollection &recHits );
  const std::vector<DetId> neighbours(const DetId& id);
  
  float GetE2OverE9(const DetId id, const EcalRecHitCollection& recHits); 
  float  e4e1(const DetId& id, const EcalRecHitCollection& rhs); 
  float  Gete6e2(const DetId& id, const EcalRecHitCollection& rhs); 
  
 private:
  ucphotonInfo(){};
  edm::InputTag ucphotonLabel_, hcalrechitLabel_, rechitBLabel_, rechitELabel_;
  bool fillgenMatchInfo_, fillHErechitInfo_, fillrechitInfo_;

  //variables which would become branches
  int Ucphoton_n,UcHERecHit_subset_n;
  TClonesArray *Ucphoton_4Momentum, *Ucphoton_Vposition, *Ucphoton_sc_position, *Ucphoton_match_4Momentum;
  TClonesArray *Ucphoton_pairMomentum, *Ucphoton_conv_Vposition, *UcHERecHit_subset_position;

  std::vector<float> Ucphoton_ecalRecHitSumEtConeDR04;
  std::vector<float> Ucphoton_hcalTowerSumEtConeDR04;
  std::vector<float> Ucphoton_trkSumPtHollowConeDR04;
  std::vector<float> Ucphoton_trkSumPtSolidConeDR04;
  std::vector<float> Ucphoton_hadronicOverEm;
  std::vector<bool>  Ucphoton_hasPixelSeed;
  std::vector<float> Ucphoton_sigmaIetaIeta;
  std::vector<float> Ucphoton_r9;
  std::vector<float>  Ucphoton_nTrkSolidConeDR04;
  std::vector<float>  Ucphoton_nTrkHollowConeDR04;
  std::vector<float>  Ucphoton_hcalDepth1TowerSumEtConeDR04;
  std::vector<float>  Ucphoton_hcalDepth2TowerSumEtConeDR04;

  std::vector<float>  Ucphoton_ecalRecHitSumEtConeDR03;
  std::vector<float>  Ucphoton_hcalTowerSumEtConeDR03;
  std::vector<float>  Ucphoton_trkSumPtHollowConeDR03;
  std::vector<float>  Ucphoton_trkSumPtSolidConeDR03;
  std::vector<float>  Ucphoton_nTrkSolidConeDR03;
  std::vector<float>  Ucphoton_nTrkHollowConeDR03;
  std::vector<float>  Ucphoton_hcalDepth1TowerSumEtConeDR03;
  std::vector<float>  Ucphoton_hcalDepth2TowerSumEtConeDR03;

  std::vector<float>  Ucphoton_SigmaEtaEta;
  std::vector<float> Ucphoton_e1x5;
  std::vector<float> Ucphoton_e2x5;
  std::vector<float> Ucphoton_e3x3;
  std::vector<float> Ucphoton_e5x5;
  std::vector<float> Ucphoton_r1x5;
  std::vector<float> Ucphoton_r2x5;
  std::vector<bool> Ucphoton_isEB;
  std::vector<bool> Ucphoton_isEE;
  std::vector<bool> Ucphoton_isEBGap;
  std::vector<bool> Ucphoton_isEEGap;
  std::vector<bool> Ucphoton_isEBEEGap;

  std::vector<int> Ucphoton_nBCs;
  std::vector<float> Ucphoton_seedE;
  std::vector<float> Ucphoton_sc_E;
  std::vector<float> Ucphoton_sc_etaWidth;
  std::vector<float> Ucphoton_sc_phiWidth;
  std::vector<bool> Ucphoton_hasConvTrk;
  std::vector<float> Ucphoton_mipChi2;
  std::vector<float> Ucphoton_mipTotEnergy;
  std::vector<float> Ucphoton_mipSlope;
  std::vector<float> Ucphoton_mipIntercept;
  std::vector<int> Ucphoton_mipNhitCone;
  std::vector<bool> Ucphoton_mipIsHalo;
  std::vector<int> Ucphoton_match_MGenpdgId;
  std::vector<int> Ucphoton_match_numM;
  std::vector<std::vector<int> > Ucphoton_match_GrandMGenmompdgId;
  std::vector<bool> Ucphoton_isgenmatched;

  std::vector<int> Ucphoton_nTracks;
  std::vector<bool> Ucphoton_isConverted;
  std::vector<float> Ucphoton_pairInvariantMass;
  std::vector<float> Ucphoton_pairCotThetaSeparation;
  std::vector<float> Ucphoton_EoverP;
  std::vector<float> Ucphoton_zOfPrimaryVertex;
  std::vector<float> Ucphoton_distOfMinimumApproach;
  std::vector<float> Ucphoton_dPhiTracksAtVtx;
  std::vector<float> Ucphoton_dPhiTracksAtEcal;
  std::vector<float> Ucphoton_dEtaTracksAtEcal;
		     
  std::vector<float> UcHERecHit_subset_energy;
  std::vector<float> UcHERecHit_subset_time;
  std::vector<float> UcHERecHit_subset_depth;
		     
  std::vector<std::vector<int> > Ucphoton_IEtas;
  std::vector<std::vector<int> > Ucphoton_IPhis;
  std::vector<std::vector<int> > Ucphoton_IXs;
  std::vector<std::vector<int> > Ucphoton_IYs;
  std::vector<std::vector<float> > Ucphoton_RecHitEnergy;
  std::vector<std::vector<float> > Ucphoton_RecHitTime;
  std::vector<float> Ucphoton_roundness;
  std::vector<float> Ucphoton_angle;
  std::vector<float> Ucphoton_e6e2;
  std::vector<float> Ucphoton_e4e1;
  std::vector<float> Ucphoton_e2e9;
  std::vector<float> Ucphoton_swissCross;
		     
  std::vector<float> Ucphoton_SigmaEtaPhi;
  std::vector<float> Ucphoton_SigmaIetaIphi;
  std::vector<float> Ucphoton_SigmaPhiPhi;
  std::vector<float> Ucphoton_SigmaIphiIphi;


  
};

#endif

