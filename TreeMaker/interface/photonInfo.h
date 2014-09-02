#ifndef __PHOTON_INFO_H_
#define __PHOTON_INFO_H_

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

class photonInfo : public baseTree{

 public:
  photonInfo(std::string name, TTree* tree, bool debug, const edm::ParameterSet& cfg);
  ~photonInfo();
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
  photonInfo(){};
  edm::InputTag photonLabel_, hcalrechitLabel_, rechitBLabel_, rechitELabel_;
  bool fillgenMatchInfo_, fillHErechitInfo_, fillrechitInfo_;

  //variables which would become branches
  int Photon_n,HERecHit_subset_n;
  TClonesArray *Photon_4Momentum, *Photon_Vposition, *Photon_sc_position, *Photon_match_4Momentum;
  TClonesArray *Photon_pairMomentum, *Photon_conv_Vposition, *HERecHit_subset_position;

  std::vector<float> Photon_ecalRecHitSumEtConeDR04;
  std::vector<float> Photon_hcalTowerSumEtConeDR04;
  std::vector<float> Photon_trkSumPtHollowConeDR04;
  std::vector<float> Photon_trkSumPtSolidConeDR04;
  std::vector<float> Photon_hadronicOverEm;
  std::vector<bool>  Photon_hasPixelSeed;
  std::vector<float> Photon_sigmaIetaIeta;
  std::vector<float> Photon_r9;
  std::vector<float>  Photon_nTrkSolidConeDR04;
  std::vector<float>  Photon_nTrkHollowConeDR04;
  std::vector<float>  Photon_hcalDepth1TowerSumEtConeDR04;
  std::vector<float>  Photon_hcalDepth2TowerSumEtConeDR04;

  std::vector<float>  Photon_ecalRecHitSumEtConeDR03;
  std::vector<float>  Photon_hcalTowerSumEtConeDR03;
  std::vector<float>  Photon_trkSumPtHollowConeDR03;
  std::vector<float>  Photon_trkSumPtSolidConeDR03;
  std::vector<float>  Photon_nTrkSolidConeDR03;
  std::vector<float>  Photon_nTrkHollowConeDR03;
  std::vector<float>  Photon_hcalDepth1TowerSumEtConeDR03;
  std::vector<float>  Photon_hcalDepth2TowerSumEtConeDR03;

  std::vector<float>  Photon_SigmaEtaEta;
  std::vector<float> Photon_e1x5;
  std::vector<float> Photon_e2x5;
  std::vector<float> Photon_e3x3;
  std::vector<float> Photon_e5x5;
  std::vector<float> Photon_r1x5;
  std::vector<float> Photon_r2x5;
  std::vector<bool> Photon_isEB;
  std::vector<bool> Photon_isEE;
  std::vector<bool> Photon_isEBGap;
  std::vector<bool> Photon_isEEGap;
  std::vector<bool> Photon_isEBEEGap;

  std::vector<int> Photon_nBCs;
  std::vector<float> Photon_seedE;
  std::vector<float> Photon_sc_E;
  std::vector<float> Photon_sc_etaWidth;
  std::vector<float> Photon_sc_phiWidth;
  std::vector<bool> Photon_hasConvTrk;
  std::vector<float> Photon_mipChi2;
  std::vector<float> Photon_mipTotEnergy;
  std::vector<float> Photon_mipSlope;
  std::vector<float> Photon_mipIntercept;
  std::vector<int> Photon_mipNhitCone;
  std::vector<bool> Photon_mipIsHalo;
  std::vector<int> Photon_match_MGenpdgId;
  std::vector<int> Photon_match_numM;
  std::vector<std::vector<int> > Photon_match_GrandMGenmompdgId;
  std::vector<bool> Photon_isgenmatched;

  std::vector<int> Photon_nTracks;
  std::vector<bool> Photon_isConverted;
  std::vector<float> Photon_pairInvariantMass;
  std::vector<float> Photon_pairCotThetaSeparation;
  std::vector<float> Photon_EoverP;
  std::vector<float> Photon_zOfPrimaryVertex;
  std::vector<float> Photon_distOfMinimumApproach;
  std::vector<float> Photon_dPhiTracksAtVtx;
  std::vector<float> Photon_dPhiTracksAtEcal;
  std::vector<float> Photon_dEtaTracksAtEcal;
		     
  std::vector<float> HERecHit_subset_energy;
  std::vector<float> HERecHit_subset_time;
  std::vector<float> HERecHit_subset_depth;
		     
  std::vector<std::vector<int> > Photon_IEtas;
  std::vector<std::vector<int> > Photon_IPhis;
  std::vector<std::vector<int> > Photon_IXs;
  std::vector<std::vector<int> > Photon_IYs;
  std::vector<std::vector<float> > Photon_RecHitEnergy;
  std::vector<std::vector<float> > Photon_RecHitTime;
  std::vector<float> Photon_roundness;
  std::vector<float> Photon_angle;
  std::vector<float> Photon_e6e2;
  std::vector<float> Photon_e4e1;
  std::vector<float> Photon_e2e9;
  std::vector<float> Photon_swissCross;
		     
  std::vector<float> Photon_SigmaEtaPhi;
  std::vector<float> Photon_SigmaIetaIphi;
  std::vector<float> Photon_SigmaPhiPhi;
  std::vector<float> Photon_SigmaIphiIphi;


  
};

#endif

