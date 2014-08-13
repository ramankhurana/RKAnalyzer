#include "RKAnalyzer/TreeMaker/interface/photonInfo.h"

photonInfo::photonInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in photon constructor"<<std::endl;
  photonLabel_               = iConfig.getUntrackedParameter<edm::InputTag> ("photonLabel_");
  hcalrechitLabel_           = iConfig.getUntrackedParameter<edm::InputTag> ("hcalrechitLabel_");
  rechitBLabel_              = iConfig.getUntrackedParameter<edm::InputTag> ("rechitBLabel_");
  rechitELabel_              = iConfig.getUntrackedParameter<edm::InputTag> ("rechitELabel_");
  fillgenMatchInfo_          = iConfig.getUntrackedParameter<bool> ("fillgenMatchInfo_");
  fillHErechitInfo_          = iConfig.getUntrackedParameter<bool> ("fillHErechitInfo_");
  fillrechitInfo_            = iConfig.getUntrackedParameter<bool> ("fillrechitInfo_");
  Photon_4Momentum           = new TClonesArray("TLorentzVector");
  Photon_match_4Momentum     = new TClonesArray("TLorentzVector");
  Photon_Vposition           = new TClonesArray("TVector3");
  Photon_sc_position         = new TClonesArray("TVector3");
  Photon_pairMomentum        = new TClonesArray("TVector3");
  Photon_conv_Vposition      = new TClonesArray("TVector3");
  HERecHit_subset_position   = new TClonesArray("TVector3");									    

  if(debug) std::cout<<"in photon constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

photonInfo::~photonInfo(){
  delete tree_;
}

void photonInfo::Fill(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  Clear();
  if(debug_)    std::cout<<"getting photon info"<<std::endl;

  edm::Handle<std::vector<pat::Photon> > photonHandle;
  iEvent.getByLabel(photonLabel_,photonHandle);
  if(not iEvent.getByLabel(photonLabel_,photonHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<photonLabel_<<std::endl; 
    exit(0);
  }  

  edm::ESHandle<CaloGeometry> geoHandle;
  iSetup.get<CaloGeometryRecord>().get(geoHandle);
  const CaloGeometry* caloGeom = geoHandle.product();

  edm::Handle<HBHERecHitCollection> hcalRecHitHandle;
  iEvent.getByLabel(hcalrechitLabel_, hcalRecHitHandle);
  const HBHERecHitCollection *hbhe =  hcalRecHitHandle.product();
  if(not iEvent.getByLabel(hcalrechitLabel_,hcalRecHitHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<hcalrechitLabel_<<std::endl; 
    exit(0);
  }  

  edm::Handle<EcalRecHitCollection> Brechit;//barrel
  edm::Handle<EcalRecHitCollection> Erechit;//endcap
  iEvent.getByLabel(rechitBLabel_,Brechit);
  iEvent.getByLabel(rechitELabel_,Erechit);

  const EcalRecHitCollection* barrelRecHits= Brechit.product();
  const EcalRecHitCollection* endcapRecHits= Erechit.product();
  
  if(not iEvent.getByLabel(rechitBLabel_,Brechit)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<rechitBLabel_<<std::endl; 
    exit(0);
  }    
  if(not iEvent.getByLabel(rechitELabel_,Erechit)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<rechitELabel_<<std::endl; 
    exit(0);
  }    

  //this will be needed later for swiss corss
  EcalClusterLazyTools lazyTool(iEvent, iSetup,rechitBLabel_, rechitELabel_ );
  
  edm::ESHandle<CaloTopology> theCaloTopo_;
  iSetup.get<CaloTopologyRecord>().get(theCaloTopo_);
  const CaloTopology *topology = theCaloTopo_.product();
     
  pat::PhotonCollection phColl(*(photonHandle.product()));
  std::sort(phColl.begin(),phColl.end(),PtGreater());
  
  
  pat::PhotonCollection::const_iterator photon;
  for(photon=phColl.begin(); photon!=phColl.end(); photon++){
    TLorentzVector p4(photon->px(),photon->py(),photon->pz(),photon->energy());
    new( (*Photon_4Momentum)[Photon_n]) TLorentzVector(p4);
    TVector3 v3(photon->vx(),photon->vy(),photon->vz());
    new( (*Photon_Vposition)[Photon_n]) TVector3(v3);
    Photon_ecalRecHitSumEtConeDR04.push_back((float)photon->ecalRecHitSumEtConeDR04());
    Photon_hcalTowerSumEtConeDR04.push_back((float)photon->hcalTowerSumEtConeDR04());
    Photon_trkSumPtHollowConeDR04.push_back((float)photon->trkSumPtHollowConeDR04());
    Photon_trkSumPtSolidConeDR04.push_back((float)photon->trkSumPtSolidConeDR04());
    Photon_nTrkSolidConeDR04.push_back((int)photon->nTrkSolidConeDR04());
    Photon_nTrkHollowConeDR04.push_back((int)photon->nTrkHollowConeDR04());
    Photon_hcalDepth1TowerSumEtConeDR04.push_back((float)photon->hcalDepth1TowerSumEtConeDR04());
    Photon_hcalDepth2TowerSumEtConeDR04.push_back((float)photon->hcalDepth2TowerSumEtConeDR04());

    Photon_ecalRecHitSumEtConeDR03.push_back((float)photon->ecalRecHitSumEtConeDR03());
    Photon_hcalTowerSumEtConeDR03.push_back((float)photon->hcalTowerSumEtConeDR03());
    Photon_trkSumPtHollowConeDR03.push_back((float)photon->trkSumPtHollowConeDR03());
    Photon_trkSumPtSolidConeDR03.push_back((float)photon->trkSumPtSolidConeDR03());
    Photon_nTrkSolidConeDR03.push_back((int)photon->nTrkSolidConeDR03());
    Photon_nTrkHollowConeDR03.push_back((int)photon->nTrkHollowConeDR03());
    Photon_hcalDepth1TowerSumEtConeDR03.push_back((float)photon->hcalDepth1TowerSumEtConeDR03());
    Photon_hcalDepth2TowerSumEtConeDR03.push_back((float)photon->hcalDepth2TowerSumEtConeDR03());

    
    Photon_sigmaIetaIeta.push_back((float)photon->sigmaIetaIeta());
    Photon_hadronicOverEm.push_back((float)photon->hadronicOverEm());
    Photon_hasPixelSeed.push_back((float)photon->hasPixelSeed());
    Photon_r9.push_back((float)photon->r9());
    Photon_SigmaEtaEta.push_back(photon->sigmaEtaEta());        
    Photon_e1x5.push_back((float)photon->e1x5());
    Photon_e2x5.push_back((float)photon->e2x5());
    Photon_e3x3.push_back((float)photon->e3x3());
    Photon_e5x5.push_back((float)photon->e5x5());
    Photon_r1x5.push_back((float)photon->r1x5());
    Photon_r2x5.push_back((float)photon->r2x5());
    Photon_isEB.push_back((bool)photon->isEB()); 
    Photon_isEE.push_back((bool)photon->isEE());
    Photon_isEBGap.push_back((bool)photon->isEBGap()); 
    Photon_isEEGap.push_back((bool)photon->isEEGap()); 
    Photon_isEBEEGap.push_back((bool)photon->isEBEEGap()); 
    
 
    Photon_nBCs.push_back((int)photon->superCluster()->clustersSize());
    Photon_seedE.push_back((float)photon->maxEnergyXtal());
    new ((*Photon_sc_position)[Photon_n]) TVector3(photon->superCluster()->x(),
						   photon->superCluster()->y(),
						   photon->superCluster()->z());
    Photon_sc_E.push_back((float)photon->superCluster()->energy());
    Photon_sc_etaWidth.push_back((float)photon->superCluster()->etaWidth());
    Photon_sc_phiWidth.push_back((float)photon->superCluster()->phiWidth());
    
    Photon_hasConvTrk.push_back((bool)photon->hasConversionTracks());
    Photon_mipChi2.push_back((float)photon->mipChi2());
    Photon_mipTotEnergy.push_back((float)photon->mipTotEnergy());
    Photon_mipSlope.push_back((float)photon->mipSlope());
    Photon_mipIntercept.push_back((float)photon->mipIntercept());
    Photon_mipNhitCone.push_back((int)photon->mipNhitCone());
    Photon_mipIsHalo.push_back((bool)photon->mipIsHalo());
    
    if(fillgenMatchInfo_){
      if( photon->genParticleRef().isNonnull() )       
	{
	  Photon_match_MGenpdgId.push_back((int)photon->genParticleRef()->pdgId());
	  Photon_match_numM.push_back((int)photon->genParticleRef()->numberOfMothers());    
	  std::vector<int> GrandMGenmompdgId;
	  GrandMGenmompdgId.clear();
   	  if(photon->genParticleRef()->numberOfMothers()!=0)                                     
	    {   
	      for(unsigned int imoth=0; imoth<photon->genParticleRef()->numberOfMothers() && imoth< 100; imoth++){       
		GrandMGenmompdgId.push_back((int)photon->genParticleRef()->mother(imoth)->pdgId());
	      }
	    } 
	  Photon_match_GrandMGenmompdgId.push_back(GrandMGenmompdgId);
	  new ((*Photon_match_4Momentum)[Photon_n]) TLorentzVector(photon->genPhoton()->px(),
								   photon->genPhoton()->py(),
								   photon->genPhoton()->pz(),
								   photon->genPhoton()->energy());
	}
      else{
	new ((*Photon_match_4Momentum)[Photon_n]) TLorentzVector(-99.,-99.,-99.,-99.);
	std::vector<int> dflt;
	dflt.push_back(-99);
	Photon_match_GrandMGenmompdgId.push_back(dflt);
	Photon_match_numM.push_back(-99);
	Photon_match_MGenpdgId.push_back(-99);
      }
      Photon_isgenmatched.push_back((bool)photon->genParticleRef().isNonnull());
    }
    

    Photon_nTracks.push_back(9999);
    Photon_isConverted.push_back(false);
    Photon_pairInvariantMass.push_back(-99.);
    Photon_pairCotThetaSeparation.push_back(-99.);
    Photon_EoverP.push_back(-99.);
    Photon_zOfPrimaryVertex.push_back(-99.);
    Photon_distOfMinimumApproach.push_back(-99.);
    Photon_dPhiTracksAtVtx.push_back(-99.);
    Photon_dPhiTracksAtEcal.push_back(-99.);
    Photon_dEtaTracksAtEcal.push_back(-99.);
    new ((*Photon_pairMomentum)[Photon_n]) TVector3(-99.,-99.,-99.);
    new ((*Photon_conv_Vposition)[Photon_n]) TVector3(-99.,-99.,-99.);
    
    reco::ConversionRefVector conversions  = photon->conversions();
    for (unsigned int iConv=0; iConv<conversions.size(); iConv++) {
      reco::ConversionRef aConv=conversions[iConv];
      if ( aConv->nTracks() <2 ) continue; 
      if ( aConv->conversionVertex().isValid() ){
	Photon_nTracks[Photon_n]                    = (int)  aConv->nTracks();
	Photon_isConverted[Photon_n]                = (bool) aConv->isConverted();
	Photon_pairInvariantMass[Photon_n]          = (float)aConv->pairInvariantMass();
	Photon_pairCotThetaSeparation[Photon_n]     = (float)aConv->pairCotThetaSeparation();
	Photon_EoverP[Photon_n]                     = (float)aConv->EoverP();
	Photon_zOfPrimaryVertex[Photon_n]           = (float)aConv->zOfPrimaryVertexFromTracks();
	Photon_distOfMinimumApproach[Photon_n]      = (float)aConv->distOfMinimumApproach();
	Photon_dPhiTracksAtVtx[Photon_n]            = (float)aConv->dPhiTracksAtVtx();
	Photon_dPhiTracksAtEcal[Photon_n]           = (float)aConv->dPhiTracksAtEcal();
	Photon_dEtaTracksAtEcal[Photon_n]           = (float)aConv->dEtaTracksAtEcal();
	new ((*Photon_pairMomentum)[Photon_n])   TVector3(aConv->pairMomentum().x(),
							  aConv->pairMomentum().y(),
							  aConv->pairMomentum().z());
	new ((*Photon_conv_Vposition)[Photon_n]) TVector3(aConv->conversionVertex().x(),
							  aConv->conversionVertex().y(),
							  aConv->conversionVertex().z());
      }//end of if ( aConv->conversionVertex().isValid() )
    }//end of for (unsigned int iConv=0; iConv<conversions.size(); iConv++)

    if(fillHErechitInfo_ && photon->isEB()){
      set<DetId> HERecHitSet;
      for(HBHERecHitCollection::const_iterator hh = hbhe->begin(); hh != hbhe->end() && HERecHit_subset_n<10000; hh++){
	HcalDetId id(hh->detid());
	if (id.subdet()==2){
	  const CaloCellGeometry *hbhe_cell = caloGeom->getGeometry(hh->id());
	  Global3DPoint hbhe_position = hbhe_cell->getPosition();
	  if(fabs(deltaPhi(correct_phi((float)photon->superCluster()->phi()),correct_phi(hbhe_position.phi())) ) < 0.5
	     && hh->energy()>1.){
	    //find the detid in the set
	    set<DetId>::const_iterator HERecHitChecker = HERecHitSet.find(hh->detid());
	    //if detid is not found in the set,(we reached the end), save info!
	    if(HERecHitChecker == HERecHitSet.end()){
	      HERecHitSet.insert(hh->detid());
	      //HERecHit_subset_detid.push_back(hh->detid());
	      HERecHit_subset_energy.push_back((float) hh->energy());
	      HERecHit_subset_time.push_back((float)hh->time());
	      HERecHit_subset_depth.push_back((float)id.depth());
	      new ((*HERecHit_subset_position)[HERecHit_subset_n])   TVector3(hbhe_position.x(),
									      hbhe_position.y(),
									      hbhe_position.z());
	      HERecHit_subset_n++;
	    }//check to see if hit is already saved
	  }//if delta dphi from photon is small and E>1 try to save
	}
      }
    }// fillHErechitInfo__ && pho_isEB
    
    if(fillrechitInfo_)
      {
	std::vector< std::pair<DetId, float> >  PhotonHit_DetIds ;
	PhotonHit_DetIds  = photon->superCluster()->hitsAndFractions();
	std::vector< std::pair<DetId, float> >::const_iterator detitr;
	std::vector<int> IEtas, IPhis, IXs, IYs; 
	std::vector<float> Energy, Time;
      
	IEtas.clear();   IPhis.clear(); IXs.clear();     IYs.clear();
	Energy.clear(); Time.clear(); 
	int ihit = 0;
	
	for (detitr = PhotonHit_DetIds.begin(); detitr != PhotonHit_DetIds.end(); ++detitr) {
	  IEtas.push_back(-99);	  IPhis.push_back(-99);
	  IXs.push_back(-99);     IYs.push_back(-99);
	  Energy.push_back(-99);  Time.push_back(-99);
	  if (((*detitr).first).det() == DetId::Ecal && ((*detitr).first).subdetId() == EcalBarrel) {
	    EcalRecHitCollection::const_iterator j= Brechit->find(((*detitr).first));
	    EcalRecHitCollection::const_iterator thishit;
	    if ( j!= Brechit->end())  {
	      thishit = j;
	      EBDetId detId = (EBDetId)((*detitr).first);
	      IEtas[ihit]   = detId.ieta();
	      IPhis[ihit]   = detId.iphi();
	      Energy[ihit]  = (float)thishit->energy();
	      Time[ihit]    = (float)thishit->time();
	      ihit++;
	    }
	  }
	  if (((*detitr).first).det() == DetId::Ecal && ((*detitr).first).subdetId() == EcalEndcap) {
	    EcalRecHitCollection::const_iterator j= Erechit->find(((*detitr).first));
	    EcalRecHitCollection::const_iterator thishit;
	    if ( j!= Erechit->end())  {
	      thishit = j;
	      EEDetId detId = (EEDetId)((*detitr).first);
	      IXs[ihit]     = detId.ix();
	      IYs[ihit]     = detId.iy();
	      Energy[ihit]  = (float)thishit->energy();
	      Time[ihit]    = (float)thishit->time();
	    }
	  }
	  if (((*detitr).first).det() == DetId::Ecal && ((*detitr).first).subdetId() != EcalEndcap && ((*detitr).first).subdetId() != EcalBarrel) {
	    std::cout<<" a photon in "<<((*detitr).first).subdetId()<<std::endl;
	  }
	}
	Photon_IEtas.push_back(IEtas); Photon_IPhis.push_back(IPhis);  
	Photon_IXs.push_back(IXs); Photon_IYs.push_back(IYs);  
	Photon_RecHitEnergy.push_back(Energy); Photon_RecHitTime.push_back(Time);
	
	if(photon->isEB()){
	  std::vector<float> showershapes_barrel = EcalClusterTools::roundnessBarrelSuperClusters(*(photon->superCluster()),*barrelRecHits,0);
	  Photon_roundness.push_back((float)showershapes_barrel[0]);
	  Photon_angle.push_back((float)showershapes_barrel[1]);
	  
	  const reco::CaloClusterPtr  seed = photon->superCluster()->seed();
	  DetId id = lazyTool.getMaximum(*seed).first;
	  float swissCross =-99.;
	  Photon_e6e2.push_back(-99.);
	  Photon_e4e1.push_back(-99.);    
	  
	  const EcalRecHitCollection & rechits = ( photon->isEB() ? *Brechit : *Brechit);
	  EcalRecHitCollection::const_iterator it = rechits.find( id );
	  if( it != rechits.end() )
	    swissCross = EcalTools::swissCross( id, rechits, 0.08, true);
	  Photon_swissCross.push_back(swissCross);
	  Photon_e2e9.push_back(-99.);
	  Photon_e2e9[Photon_n]    = GetE2OverE9(id,rechits); 
	  Photon_e6e2[Photon_n]    = Gete6e2( id, rechits); 
	  Photon_e4e1[Photon_n]    = e4e1(id, rechits); 
	  
	  if(debug_ && 1-Photon_swissCross[Photon_n]/Photon_seedE[Photon_n] > 0.95) 
	    std::cout<<"This photon candidate is an ECAL spike identified by Swiss Cross algorithm."<<std::endl;
	  
	  vector<float> stdCov  = EcalClusterTools::covariances(*(photon->superCluster()->seed()),&(*barrelRecHits),&(*topology),&(*caloGeom));
	  vector<float> crysCov = EcalClusterTools::localCovariances(*(photon->superCluster()->seed()),&(*barrelRecHits),&(*topology));
	  Photon_SigmaEtaPhi.push_back(sqrt(stdCov[1]));
	  Photon_SigmaIetaIphi.push_back(sqrt(crysCov[1]));   
	  Photon_SigmaPhiPhi.push_back(sqrt(stdCov[2]));
	  Photon_SigmaIphiIphi.push_back(sqrt(crysCov[2]));
	}
	else{ 
	  Photon_roundness.push_back(-99.);
	  Photon_angle.push_back(-99.);
	  
	  const reco::CaloClusterPtr  seed = photon->superCluster()->seed();
	  DetId id = lazyTool.getMaximum(*seed).first;
	  float swissCross = -99.;
	  Photon_e6e2.push_back(-99.);
	  Photon_e4e1.push_back(-99.);    
	  
	  const EcalRecHitCollection & rechits = ( photon->isEB() ? *Erechit : *Erechit);
	  EcalRecHitCollection::const_iterator it = rechits.find( id );
	  if( it != rechits.end() ) { 
	    swissCross = EcalTools::swissCross( id, rechits, 0.08, true);
	  }
	  
	  Photon_swissCross.push_back(swissCross);
	  Photon_e2e9.push_back(-99.);
	  Photon_e2e9[Photon_n]    = GetE2OverE9(id,rechits); 
	  Photon_e6e2[Photon_n]    = Gete6e2( id, rechits); 
	  Photon_e4e1[Photon_n]    = e4e1(id, rechits); 
	  
	  if(debug_ && 1-Photon_swissCross[Photon_n]/Photon_seedE[Photon_n] > 0.95) {
	    std::cout<<"This photon candidate is an ECAL spike identified by Swiss Cross algorithm." << std::endl;
	    std::cout<<"This would be weird since there aren't spikes in the endcap of ECAL"<< std::endl; 
	  }
	  
	  vector<float> stdCov  = EcalClusterTools::covariances(*(photon->superCluster()->seed()),&(*endcapRecHits),&(*topology),&(*caloGeom));
	  vector<float> crysCov = EcalClusterTools::localCovariances(*(photon->superCluster()->seed()),&(*endcapRecHits),&(*topology));
	  Photon_SigmaEtaPhi.push_back(sqrt(stdCov[1]));
	  Photon_SigmaIetaIphi.push_back(sqrt(crysCov[1]));   
	  Photon_SigmaPhiPhi.push_back(sqrt(stdCov[2]));
	  Photon_SigmaIphiIphi.push_back(sqrt(crysCov[2]));
   
	}//end of else (if !EB)
      }

    Photon_n++;	
  }//end of photonloop
  
  
  if(debug_)    std::cout<<"got photon info"<<std::endl;
}

void photonInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&Photon_n  ,"Photon_n");
  AddBranch(&Photon_ecalRecHitSumEtConeDR04 ,"Photon_ecalRecHitSumEtConeDR04");
  AddBranch(&Photon_hcalTowerSumEtConeDR04,  "Photon_hcalTowerSumEtConeDR04");
  AddBranch(&Photon_trkSumPtHollowConeDR04,  "Photon_trkSumPtHollowConeDR04");
  AddBranch(&Photon_trkSumPtSolidConeDR04,   "Photon_trkSumPtSolidConeDR04");
  AddBranch(&Photon_nTrkSolidConeDR04		,"Photon_nTrkSolidConeDR04");
  AddBranch(&Photon_nTrkHollowConeDR04		,"Photon_nTrkHollowConeDR04");
  AddBranch(&Photon_hcalDepth1TowerSumEtConeDR04,"Photon_hcalDepth1TowerSumEtConeDR04");
  AddBranch(&Photon_hcalDepth2TowerSumEtConeDR04,"Photon_hcalDepth2TowerSumEtConeDR04");
  
  AddBranch(&Photon_ecalRecHitSumEtConeDR03	,"Photon_ecalRecHitSumEtConeDR03");
  AddBranch(&Photon_hcalTowerSumEtConeDR03	,"Photon_hcalTowerSumEtConeDR03");
  AddBranch(&Photon_trkSumPtHollowConeDR03	,"Photon_trkSumPtHollowConeDR03");
  AddBranch(&Photon_trkSumPtSolidConeDR03	,"Photon_trkSumPtSolidConeDR03");
  AddBranch(&Photon_nTrkSolidConeDR03		,"Photon_nTrkSolidConeDR03");
  AddBranch(&Photon_nTrkHollowConeDR03		,"Photon_nTrkHollowConeDR03");
  AddBranch(&Photon_hcalDepth1TowerSumEtConeDR03,"Photon_hcalDepth1TowerSumEtConeDR03");
  AddBranch(&Photon_hcalDepth2TowerSumEtConeDR03,"Photon_hcalDepth2TowerSumEtConeDR03");
  
  AddBranch(&Photon_SigmaEtaEta			,"Photon_SigmaEtaEta");
  AddBranch(&Photon_e1x5			,"Photon_e1x5");
  AddBranch(&Photon_e2x5			,"Photon_e2x5");
  AddBranch(&Photon_e3x3                        ,"Photon_e3x3");
  AddBranch(&Photon_e5x5			,"Photon_e5x5");
  AddBranch(&Photon_r1x5			,"Photon_r1x5");
  AddBranch(&Photon_r2x5			,"Photon_r2x5");
  AddBranch(&Photon_isEB			,"Photon_isEB");
  AddBranch(&Photon_isEE			,"Photon_isEE");
  AddBranch(&Photon_isEBGap			,"Photon_isEBGap");
  AddBranch(&Photon_isEEGap   			,"Photon_isEEGap");
  AddBranch(&Photon_isEBEEGap			,"Photon_isEBEEGap");
  
  AddBranch(&Photon_nBCs			,"Photon_nBCs");
  AddBranch(&Photon_seedE			,"Photon_seedE");
  AddBranch(&Photon_sc_E			,"Photon_sc_E");
  AddBranch(&Photon_sc_etaWidth			,"Photon_sc_etaWidth");
  AddBranch(&Photon_sc_phiWidth			,"Photon_sc_phiWidth");
  AddBranch(&Photon_hasConvTrk			,"Photon_hasConvTrk");
  AddBranch(&Photon_mipChi2			,"Photon_mipChi2");
  AddBranch(&Photon_mipTotEnergy		,"Photon_mipTotEnergy");
  AddBranch(&Photon_mipSlope			,"Photon_mipSlope");
  AddBranch(&Photon_mipIntercept		,"Photon_mipIntercept");
  AddBranch(&Photon_mipNhitCone			,"Photon_mipNhitCone");
  AddBranch(&Photon_mipIsHalo			,"Photon_mipIsHalo");
  if(fillgenMatchInfo_){
    AddBranch(&Photon_match_MGenpdgId		,"Photon_match_MGenpdgId");
    AddBranch(&Photon_match_numM			,"Photon_match_numM");
    AddBranch(&Photon_match_GrandMGenmompdgId	,"Photon_match_GrandMGenmompdgId");
    AddBranch(&Photon_isgenmatched         	,"Photon_isgenmatched");
    AddBranch(&Photon_match_4Momentum  ,"Photon_match_4Momentum");
    
  }
  AddBranch(&Photon_nTracks			,"Photon_nTracks");
  AddBranch(&Photon_isConverted			,"Photon_isConverted");
  AddBranch(&Photon_pairInvariantMass		,"Photon_pairInvariantMass");
  AddBranch(&Photon_pairCotThetaSeparation	,"Photon_pairCotThetaSeparation");
  AddBranch(&Photon_EoverP			,"Photon_EoverP");
  AddBranch(&Photon_zOfPrimaryVertex		,"Photon_zOfPrimaryVertex");
  AddBranch(&Photon_distOfMinimumApproach	,"Photon_distOfMinimumApproach");
  AddBranch(&Photon_dPhiTracksAtVtx		,"Photon_dPhiTracksAtVtx");
  AddBranch(&Photon_dPhiTracksAtEcal		,"Photon_dPhiTracksAtEcal");
  AddBranch(&Photon_dEtaTracksAtEcal		,"Photon_dEtaTracksAtEcal");
  if(fillHErechitInfo_){
    AddBranch(&HERecHit_subset_n,"HERecHit_subset_n");
    AddBranch(&HERecHit_subset_position,"HERecHit_subset_position");
    AddBranch(&HERecHit_subset_energy		,"HERecHit_subset_energy");
    AddBranch(&HERecHit_subset_time	   	,"HERecHit_subset_time");
    AddBranch(&HERecHit_subset_depth		,"HERecHit_subset_depth");
  }

  if(fillrechitInfo_)
    {
      AddBranch(&Photon_IEtas		      	,"Photon_IEtas");
      AddBranch(&Photon_IPhis		      	,"Photon_IPhis");
      AddBranch(&Photon_IXs				,"Photon_IXs");
      AddBranch(&Photon_IYs				,"Photon_IYs");
      AddBranch(&Photon_RecHitEnergy	      	,"Photon_RecHitEnergy");
      AddBranch(&Photon_RecHitTime			,"Photon_RecHitTime");
      AddBranch(&Photon_roundness			,"Photon_roundness");
      AddBranch(&Photon_angle		     	,"Photon_angle");
      AddBranch(&Photon_e6e2		      	,"Photon_e6e2");
      AddBranch(&Photon_e4e1		      	,"Photon_e4e1");
      AddBranch(&Photon_e2e9		      	,"Photon_e2e9");
      AddBranch(&Photon_swissCross			,"Photon_swissCross");
      
      AddBranch(&Photon_SigmaEtaPhi			,"Photon_SigmaEtaPhi");
      AddBranch(&Photon_SigmaIetaIphi	      	,"Photon_SigmaIetaIphi");
      AddBranch(&Photon_SigmaPhiPhi			,"Photon_SigmaPhiPhi");
      AddBranch(&Photon_SigmaIphiIphi               ,"Photon_SigmaIphiIphi");   
      AddBranch(&Photon_hadronicOverEm,          "Photon_hadronicOverEm");
      AddBranch(&Photon_hasPixelSeed,            "Photon_hasPixelSeed");
      AddBranch(&Photon_sigmaIetaIeta,           "Photon_sigmaIetaIeta");
      AddBranch(&Photon_r9,                      "Photon_r9");
    }
  AddBranch(&Photon_4Momentum ,"Photon_4Momentum");
  AddBranch(&Photon_Vposition ,"Photon_Vposition");
  AddBranch(&Photon_sc_position ,"Photon_sc_position");
  AddBranch(&Photon_pairMomentum,"Photon_pairMomentum");
  AddBranch(&Photon_conv_Vposition,"Photon_conv_Vposition");
  
  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void photonInfo::Clear(){
  if(debug_)std::cout<<"clearing Photon info"<<std::endl;
  Photon_n =0;
  Photon_4Momentum->Clear();
  Photon_Vposition->Clear();
  Photon_sc_position->Clear();
  Photon_pairMomentum->Clear();
  Photon_conv_Vposition->Clear();

  Photon_ecalRecHitSumEtConeDR04.clear();
  Photon_hcalTowerSumEtConeDR04.clear();
  Photon_trkSumPtHollowConeDR04.clear();
  Photon_trkSumPtSolidConeDR04.clear();
  Photon_nTrkSolidConeDR04.clear();
  Photon_nTrkHollowConeDR04.clear();
  Photon_hcalDepth1TowerSumEtConeDR04.clear();
  Photon_hcalDepth2TowerSumEtConeDR04.clear();

  Photon_ecalRecHitSumEtConeDR03.clear();
  Photon_hcalTowerSumEtConeDR03.clear();
  Photon_trkSumPtHollowConeDR03.clear();
  Photon_trkSumPtSolidConeDR03.clear();
  Photon_nTrkSolidConeDR03.clear();
  Photon_nTrkHollowConeDR03.clear();
  Photon_hcalDepth1TowerSumEtConeDR03.clear();
  Photon_hcalDepth2TowerSumEtConeDR03.clear();

  Photon_SigmaEtaEta.clear();
  Photon_e1x5.clear();
  Photon_e2x5.clear();
  Photon_e3x3.clear();
  Photon_e5x5.clear();
  Photon_r1x5.clear();
  Photon_r2x5.clear();
  Photon_isEB.clear();
  Photon_isEE.clear();
  Photon_isEBGap.clear();
  Photon_isEEGap.clear();
  Photon_isEBEEGap.clear();

  Photon_nBCs.clear();
  Photon_seedE.clear();
  Photon_sc_E.clear();
  Photon_sc_etaWidth.clear();
  Photon_sc_phiWidth.clear();
  Photon_hasConvTrk.clear();
  Photon_mipChi2.clear();
  Photon_mipTotEnergy.clear();
  Photon_mipSlope.clear();
  Photon_mipIntercept.clear();
  Photon_mipNhitCone.clear();
  Photon_mipIsHalo.clear();

  if(fillgenMatchInfo_){
    Photon_match_4Momentum->Clear();
    Photon_match_MGenpdgId.clear();
    Photon_match_numM.clear();
    Photon_match_GrandMGenmompdgId.clear();
    Photon_isgenmatched.clear();
  }
  
  Photon_nTracks.clear();
  Photon_isConverted.clear();
  Photon_pairInvariantMass.clear();
  Photon_pairCotThetaSeparation.clear();
  Photon_EoverP.clear();
  Photon_zOfPrimaryVertex.clear();
  Photon_distOfMinimumApproach.clear();
  Photon_dPhiTracksAtVtx.clear();
  Photon_dPhiTracksAtEcal.clear();
  Photon_dEtaTracksAtEcal.clear();
  
  if(fillHErechitInfo_){
  HERecHit_subset_energy.clear();
  HERecHit_subset_time.clear();
  HERecHit_subset_depth.clear();
  HERecHit_subset_position->Clear();
  HERecHit_subset_n = 0;
  }
  if(fillrechitInfo_){
    Photon_IEtas.clear();
    Photon_IPhis.clear();
    Photon_IXs.clear();
    Photon_IYs.clear();
    Photon_RecHitEnergy.clear();
    Photon_RecHitTime.clear();
    Photon_roundness.clear();
    Photon_angle.clear();
    Photon_e6e2.clear();
    Photon_e4e1.clear();
    Photon_e2e9.clear();
    Photon_swissCross.clear();
    Photon_SigmaEtaPhi.clear();
    Photon_SigmaIetaIphi.clear();
    Photon_SigmaPhiPhi.clear();
    Photon_SigmaIphiIphi.clear();
    Photon_sigmaIetaIeta.clear();
    Photon_hadronicOverEm.clear();
    Photon_hasPixelSeed.clear();
    Photon_r9.clear();
  }
    

  if(debug_) std::cout<<"cleared"<<std::endl;
}

float photonInfo::GetE2OverE9( const DetId id, const EcalRecHitCollection & recHits)
{ 
  ///////////start calculating e2/e9
  ////http://cmslxr.fnal.gov/lxr/source/RecoLocalCalo/EcalRecAlgos/src/EcalSeverityLevelAlgo.cc#240
  // compute e2overe9
  //   | | | |
  //   +-+-+-+
  //   | |1|2|
  //   +-+-+-+
  //   | | | |
  //   1 - input hit,  2 - highest energy hit in a 3x3 around 1
  //   rechit 1 must have E_t > recHitEtThreshold
  //   rechit 2 must have E_t > recHitEtThreshold2
  //   function returns value of E2/E9 centered around 1 (E2=energy of hits 1+2) if energy of 1>2
  //   if energy of 2>1 and KillSecondHit is set to true, function returns value of E2/E9 centered around 2
  //   *provided* that 1 is the highest energy hit in a 3x3 centered around 2, otherwise, function returns 0
  
  
  float recHitEtThreshold = 10.0; 
  float recHitEtThreshold2 = 1.0;
  bool avoidIeta85=false;
  bool KillSecondHit=true;
  
  if ( id.subdetId() == EcalBarrel ) {
    EBDetId ebId( id );
    // avoid recHits at |eta|=85 where one side of the neighbours is missing
    if ( abs(ebId.ieta())==85 && avoidIeta85) return 0;
    // select recHits with Et above recHitEtThreshold
    float e1 = recHitE( id, recHits );
    float ete1=recHitApproxEt( id, recHits );
    // check that rechit E_t is above threshold
    if (ete1 < std::min(recHitEtThreshold,recHitEtThreshold2) ) return 0;
    if (ete1 < recHitEtThreshold && !KillSecondHit ) return 0;
    
    float e2=-1;
    float ete2=0;
    float s9 = 0;
    // coordinates of 2nd hit relative to central hit
    int e2eta=0;
    int e2phi=0;
    
    // LOOP OVER 3x3 ARRAY CENTERED AROUND HIT 1
    for ( int deta = -1; deta <= +1; ++deta ) {
    for ( int dphi = -1; dphi <= +1; ++dphi ) {
      // compute 3x3 energy 
      float etmp=recHitE( id, recHits, deta, dphi );
      s9 += etmp;
      EBDetId idtmp=EBDetId::offsetBy(id,deta,dphi);
      float eapproxet=recHitApproxEt( idtmp, recHits );
      // remember 2nd highest energy deposit (above threshold) in 3x3 array
      if (etmp>e2 && eapproxet>recHitEtThreshold2 && !(deta==0 && dphi==0)) {
	e2=etmp;
	ete2=eapproxet;
	e2eta=deta;
	e2phi=dphi;
      }
    }
    }
    
    if ( e1 == 0 )  return 0;
    // return 0 if 2nd hit is below threshold
    if ( e2 == -1 ) return 0;
    // compute e2/e9 centered around 1st hit
    float e2nd=e1+e2;
    float e2e9=0;
    
    if (s9!=0) e2e9=e2nd/s9;
    // if central hit has higher energy than 2nd hit
    //  return e2/e9 if 1st hit is above E_t threshold
    if (e1 > e2 && ete1>recHitEtThreshold) return e2e9;
    // if second hit has higher energy than 1st hit
    if ( e2 > e1 ) {
      // return 0 if user does not want to flag 2nd hit, or
      // hits are below E_t thresholds - note here we
      // now assume the 2nd hit to be the leading hit.
    
      if (!KillSecondHit || ete2<recHitEtThreshold || ete1<recHitEtThreshold2) {
      return 0;
      }
      else {
	// LOOP OVER 3x3 ARRAY CENTERED AROUND HIT 2 
      float s92nd=0;
      float e2nd_prime=0;
      int e2prime_eta=0;
      int e2prime_phi=0;
      
      EBDetId secondid=EBDetId::offsetBy(id,e2eta,e2phi);
      
      for ( int deta = -1; deta <= +1; ++deta ) {
	for ( int dphi = -1; dphi <= +1; ++dphi ) {
	  
	  // compute 3x3 energy
	  float etmp=recHitE( secondid, recHits, deta, dphi );
	  s92nd += etmp;
	  
	  if (etmp>e2nd_prime && !(deta==0 && dphi==0)) {
	    e2nd_prime=etmp;
	    e2prime_eta=deta;
	    e2prime_phi=dphi;
	  }
	}
      }
      // if highest energy hit around E2 is not the same as the input hit, return 0;
      if (!(e2prime_eta==-e2eta && e2prime_phi==-e2phi))
	{
	  return 0;
	}
      // compute E2/E9 around second hit 
      float e2e9_2=0;
      if (s92nd!=0) e2e9_2=e2nd/s92nd;
      //   return the value of E2/E9 calculated around 2nd hit
      return e2e9_2;
      }
    }
  } else if ( id.subdetId() == EcalEndcap ) {
    // only used for EB at the moment 
    return 0;
  }
  return 0;
}


//to remove double spikes: IN RECO double splikes are if e6e2< 0.04
float photonInfo::Gete6e2(const DetId& id, 
			   const EcalRecHitCollection& rhs){
  float s4_1 = 0;
  float s4_2 = 0;
  float e1 = recHitE( id, rhs , false );
  
  float maxene=0;
  DetId maxid;
  
  if ( e1 == 0 ) return 0;
  
  const std::vector<DetId>& neighs =  neighbours(id);
  
  // find the most energetic neighbour ignoring time info
  for (size_t i=0; i<neighs.size(); ++i){
    float ene = recHitE(neighs[i],rhs,false);
    if (ene>maxene)  {
      maxene=ene;
      maxid = neighs[i];
    }
  }
  
  float e2=maxene;
  
  s4_1 = e4e1(id,rhs)* e1;
  s4_2 = e4e1(maxid,rhs)* e2;
  
  return (s4_1 + s4_2) / (e1+e2) -1. ;
}

float photonInfo::correct_phi(float phi){
	return (phi >= 0 ? phi : (2*TMath::Pi() + phi));
}


float photonInfo::e4e1(const DetId& id, 
		       const EcalRecHitCollection& rhs)
{
  float s4 = 0;
  float e1 = recHitE( id, rhs, false );
  if ( e1 == 0 ) return 0;
  const std::vector<DetId>& neighs =  neighbours(id);
  for (size_t i=0; i<neighs.size(); ++i)
    // avoid hits out of time when making s4
    s4+=recHitE(neighs[i],rhs, true);
  
  return s4 / e1;
  
}

float photonInfo::recHitE( const  DetId id,  const EcalRecHitCollection &recHits )
{
  if ( id == DetId(0) ) {
    return 0;
  } else {
    EcalRecHitCollection::const_iterator it = recHits.find( id );
    if ( it != recHits.end() ) return (*it).energy();
  }
  return 0;
}


float photonInfo::recHitE( const DetId id, const EcalRecHitCollection & recHits,int di, int dj )
{
  // in the barrel:   di = dEta   dj = dPhi
  // in the endcap:   di = dX     dj = dY
  DetId nid;
  if( id.subdetId() == EcalBarrel) nid = EBDetId::offsetBy( id, di, dj );
  else if( id.subdetId() == EcalEndcap) nid = EEDetId::offsetBy( id, di, dj );
  return ( nid == DetId(0) ? 0 : recHitE( nid, recHits ) );
}



float photonInfo::recHitApproxEt(const DetId id,  const EcalRecHitCollection &recHits )
{
  // for the time being works only for the barrel
  if ( id.subdetId() == EcalBarrel ) {
    return recHitE(id,recHits ) / cosh( EBDetId::approxEta( id ) );
  }
  return 0;
}




float photonInfo::recHitE( const DetId id, const EcalRecHitCollection &recHits, bool useTimingInfo ){
  if ( id.rawId() == 0 ) return 0;

 //These values are taken from:RecoLocalCalo/EcalRecAlgos/python/ecalCleaningAlgo.py
 double e4e1Threshold_barrel_  = 0.080;
 double e4e1Threshold_endcap_  = 0.300;
 double ignoreOutOfTimeThresh_ = 2.0;
 
 
   float threshold = e4e1Threshold_barrel_;

   if ( id.subdetId() == EcalEndcap) threshold = e4e1Threshold_endcap_; 
 
   EcalRecHitCollection::const_iterator it = recHits.find( id );
   if ( it != recHits.end() ){
     float ene= (*it).energy();
 
     // ignore out of time in EB when making e4e1 if so configured
     if (useTimingInfo){
        if (id.subdetId()==EcalBarrel &&
          it->checkFlag(EcalRecHit::kOutOfTime) 
           && ene>ignoreOutOfTimeThresh_) return 0;
     }
 
     // ignore hits below threshold
     if (ene < threshold) return 0;
 
     // else return the energy of this hit
     return ene;
   }
   return 0;
}

/// four neighbours in the swiss cross around id
const std::vector<DetId> photonInfo::neighbours(const DetId& id){                                                                                                                              
        std::vector<DetId> ret;
    
   if ( id.subdetId() == EcalBarrel) {
    
     ret.push_back( EBDetId::offsetBy( id,  1, 0 ));
     ret.push_back( EBDetId::offsetBy( id, -1, 0 ));
     ret.push_back( EBDetId::offsetBy( id,  0, 1 ));
     ret.push_back( EBDetId::offsetBy( id,  0,-1 ));
   }
   // nobody understands what polymorphism is for, sgrunt !
   else  if (id.subdetId() == EcalEndcap) {
     ret.push_back( EEDetId::offsetBy( id,  1, 0 ));
     ret.push_back( EEDetId::offsetBy( id, -1, 0 ));
     ret.push_back( EEDetId::offsetBy( id,  0, 1 ));
     ret.push_back( EEDetId::offsetBy( id,  0,-1 ));
    }
    
    
   return ret;
    
 }  

