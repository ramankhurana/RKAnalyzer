#include "RKAnalyzer/TreeMaker/interface/ucphotonInfo.h"

ucphotonInfo::ucphotonInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in ucphoton constructor"<<std::endl;
  ucphotonLabel_               = iConfig.getUntrackedParameter<edm::InputTag> ("ucphotonLabel_");
  hcalrechitLabel_           = iConfig.getUntrackedParameter<edm::InputTag> ("hcalrechitLabel_");
  rechitBLabel_              = iConfig.getUntrackedParameter<edm::InputTag> ("rechitBLabel_");
  rechitELabel_              = iConfig.getUntrackedParameter<edm::InputTag> ("rechitELabel_");
  fillgenMatchInfo_          = iConfig.getUntrackedParameter<bool> ("fillgenMatchInfo_");
  fillHErechitInfo_          = iConfig.getUntrackedParameter<bool> ("fillHErechitInfo_");
  fillrechitInfo_            = iConfig.getUntrackedParameter<bool> ("fillrechitInfo_");
  Ucphoton_4Momentum           = new TClonesArray("TLorentzVector");
  Ucphoton_match_4Momentum     = new TClonesArray("TLorentzVector");
  Ucphoton_Vposition           = new TClonesArray("TVector3");
  Ucphoton_sc_position         = new TClonesArray("TVector3");
  Ucphoton_pairMomentum        = new TClonesArray("TVector3");
  Ucphoton_conv_Vposition      = new TClonesArray("TVector3");
  UcHERecHit_subset_position   = new TClonesArray("TVector3");									    

  if(debug) std::cout<<"in ucphoton constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

ucphotonInfo::~ucphotonInfo(){
  delete tree_;
}

void ucphotonInfo::Fill(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  Clear();
  if(debug_)    std::cout<<"getting ucphoton info"<<std::endl;

  edm::Handle<std::vector<pat::Photon> > ucphotonHandle;
  iEvent.getByLabel(ucphotonLabel_,ucphotonHandle);
  if(not iEvent.getByLabel(ucphotonLabel_,ucphotonHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<ucphotonLabel_<<std::endl; 
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
     
  pat::PhotonCollection phColl(*(ucphotonHandle.product()));
  std::sort(phColl.begin(),phColl.end(),PtGreater());
  
  
  pat::PhotonCollection::const_iterator ucphoton;
  for(ucphoton=phColl.begin(); ucphoton!=phColl.end(); ucphoton++){
    TLorentzVector p4(ucphoton->px(),ucphoton->py(),ucphoton->pz(),ucphoton->energy());
    new( (*Ucphoton_4Momentum)[Ucphoton_n]) TLorentzVector(p4);
    TVector3 v3(ucphoton->vx(),ucphoton->vy(),ucphoton->vz());
    new( (*Ucphoton_Vposition)[Ucphoton_n]) TVector3(v3);
    Ucphoton_ecalRecHitSumEtConeDR04.push_back((float)ucphoton->ecalRecHitSumEtConeDR04());
    Ucphoton_hcalTowerSumEtConeDR04.push_back((float)ucphoton->hcalTowerSumEtConeDR04());
    Ucphoton_trkSumPtHollowConeDR04.push_back((float)ucphoton->trkSumPtHollowConeDR04());
    Ucphoton_trkSumPtSolidConeDR04.push_back((float)ucphoton->trkSumPtSolidConeDR04());
    Ucphoton_nTrkSolidConeDR04.push_back((int)ucphoton->nTrkSolidConeDR04());
    Ucphoton_nTrkHollowConeDR04.push_back((int)ucphoton->nTrkHollowConeDR04());
    Ucphoton_hcalDepth1TowerSumEtConeDR04.push_back((float)ucphoton->hcalDepth1TowerSumEtConeDR04());
    Ucphoton_hcalDepth2TowerSumEtConeDR04.push_back((float)ucphoton->hcalDepth2TowerSumEtConeDR04());

    Ucphoton_ecalRecHitSumEtConeDR03.push_back((float)ucphoton->ecalRecHitSumEtConeDR03());
    Ucphoton_hcalTowerSumEtConeDR03.push_back((float)ucphoton->hcalTowerSumEtConeDR03());
    Ucphoton_trkSumPtHollowConeDR03.push_back((float)ucphoton->trkSumPtHollowConeDR03());
    Ucphoton_trkSumPtSolidConeDR03.push_back((float)ucphoton->trkSumPtSolidConeDR03());
    Ucphoton_nTrkSolidConeDR03.push_back((int)ucphoton->nTrkSolidConeDR03());
    Ucphoton_nTrkHollowConeDR03.push_back((int)ucphoton->nTrkHollowConeDR03());
    Ucphoton_hcalDepth1TowerSumEtConeDR03.push_back((float)ucphoton->hcalDepth1TowerSumEtConeDR03());
    Ucphoton_hcalDepth2TowerSumEtConeDR03.push_back((float)ucphoton->hcalDepth2TowerSumEtConeDR03());

    
    Ucphoton_sigmaIetaIeta.push_back((float)ucphoton->sigmaIetaIeta());
    Ucphoton_hadronicOverEm.push_back((float)ucphoton->hadronicOverEm());
    Ucphoton_hasPixelSeed.push_back((float)ucphoton->hasPixelSeed());
    Ucphoton_r9.push_back((float)ucphoton->r9());
    Ucphoton_SigmaEtaEta.push_back(ucphoton->sigmaEtaEta());        
    Ucphoton_e1x5.push_back((float)ucphoton->e1x5());
    Ucphoton_e2x5.push_back((float)ucphoton->e2x5());
    Ucphoton_e3x3.push_back((float)ucphoton->e3x3());
    Ucphoton_e5x5.push_back((float)ucphoton->e5x5());
    Ucphoton_r1x5.push_back((float)ucphoton->r1x5());
    Ucphoton_r2x5.push_back((float)ucphoton->r2x5());
    Ucphoton_isEB.push_back((bool)ucphoton->isEB()); 
    Ucphoton_isEE.push_back((bool)ucphoton->isEE());
    Ucphoton_isEBGap.push_back((bool)ucphoton->isEBGap()); 
    Ucphoton_isEEGap.push_back((bool)ucphoton->isEEGap()); 
    Ucphoton_isEBEEGap.push_back((bool)ucphoton->isEBEEGap()); 
    
 
    Ucphoton_nBCs.push_back((int)ucphoton->superCluster()->clustersSize());
    Ucphoton_seedE.push_back((float)ucphoton->maxEnergyXtal());
    new ((*Ucphoton_sc_position)[Ucphoton_n]) TVector3(ucphoton->superCluster()->x(),
						   ucphoton->superCluster()->y(),
						   ucphoton->superCluster()->z());
    Ucphoton_sc_E.push_back((float)ucphoton->superCluster()->energy());
    Ucphoton_sc_etaWidth.push_back((float)ucphoton->superCluster()->etaWidth());
    Ucphoton_sc_phiWidth.push_back((float)ucphoton->superCluster()->phiWidth());
    
    Ucphoton_hasConvTrk.push_back((bool)ucphoton->hasConversionTracks());
    Ucphoton_mipChi2.push_back((float)ucphoton->mipChi2());
    Ucphoton_mipTotEnergy.push_back((float)ucphoton->mipTotEnergy());
    Ucphoton_mipSlope.push_back((float)ucphoton->mipSlope());
    Ucphoton_mipIntercept.push_back((float)ucphoton->mipIntercept());
    Ucphoton_mipNhitCone.push_back((int)ucphoton->mipNhitCone());
    Ucphoton_mipIsHalo.push_back((bool)ucphoton->mipIsHalo());
    
    if(fillgenMatchInfo_){
      if( ucphoton->genParticleRef().isNonnull() )       
	{
	  Ucphoton_match_MGenpdgId.push_back((int)ucphoton->genParticleRef()->pdgId());
	  Ucphoton_match_numM.push_back((int)ucphoton->genParticleRef()->numberOfMothers());    
	  std::vector<int> GrandMGenmompdgId;
	  GrandMGenmompdgId.clear();
   	  if(ucphoton->genParticleRef()->numberOfMothers()!=0)                                     
	    {   
	      for(unsigned int imoth=0; imoth<ucphoton->genParticleRef()->numberOfMothers() && imoth< 100; imoth++){       
		GrandMGenmompdgId.push_back((int)ucphoton->genParticleRef()->mother(imoth)->pdgId());
	      }
	    } 
	  Ucphoton_match_GrandMGenmompdgId.push_back(GrandMGenmompdgId);
	  new ((*Ucphoton_match_4Momentum)[Ucphoton_n]) TLorentzVector(ucphoton->genPhoton()->px(),
								   ucphoton->genPhoton()->py(),
								   ucphoton->genPhoton()->pz(),
								   ucphoton->genPhoton()->energy());
	}
      else{
	new ((*Ucphoton_match_4Momentum)[Ucphoton_n]) TLorentzVector(-99.,-99.,-99.,-99.);
	std::vector<int> dflt;
	dflt.push_back(-99);
	Ucphoton_match_GrandMGenmompdgId.push_back(dflt);
	Ucphoton_match_numM.push_back(-99);
	Ucphoton_match_MGenpdgId.push_back(-99);
      }
      Ucphoton_isgenmatched.push_back((bool)ucphoton->genParticleRef().isNonnull());
    }
    

    Ucphoton_nTracks.push_back(9999);
    Ucphoton_isConverted.push_back(false);
    Ucphoton_pairInvariantMass.push_back(-99.);
    Ucphoton_pairCotThetaSeparation.push_back(-99.);
    Ucphoton_EoverP.push_back(-99.);
    Ucphoton_zOfPrimaryVertex.push_back(-99.);
    Ucphoton_distOfMinimumApproach.push_back(-99.);
    Ucphoton_dPhiTracksAtVtx.push_back(-99.);
    Ucphoton_dPhiTracksAtEcal.push_back(-99.);
    Ucphoton_dEtaTracksAtEcal.push_back(-99.);
    new ((*Ucphoton_pairMomentum)[Ucphoton_n]) TVector3(-99.,-99.,-99.);
    new ((*Ucphoton_conv_Vposition)[Ucphoton_n]) TVector3(-99.,-99.,-99.);
    
    reco::ConversionRefVector conversions  = ucphoton->conversions();
    for (unsigned int iConv=0; iConv<conversions.size(); iConv++) {
      reco::ConversionRef aConv=conversions[iConv];
      if ( aConv->nTracks() <2 ) continue; 
      if ( aConv->conversionVertex().isValid() ){
	Ucphoton_nTracks[Ucphoton_n]                    = (int)  aConv->nTracks();
	Ucphoton_isConverted[Ucphoton_n]                = (bool) aConv->isConverted();
	Ucphoton_pairInvariantMass[Ucphoton_n]          = (float)aConv->pairInvariantMass();
	Ucphoton_pairCotThetaSeparation[Ucphoton_n]     = (float)aConv->pairCotThetaSeparation();
	Ucphoton_EoverP[Ucphoton_n]                     = (float)aConv->EoverP();
	Ucphoton_zOfPrimaryVertex[Ucphoton_n]           = (float)aConv->zOfPrimaryVertexFromTracks();
	Ucphoton_distOfMinimumApproach[Ucphoton_n]      = (float)aConv->distOfMinimumApproach();
	Ucphoton_dPhiTracksAtVtx[Ucphoton_n]            = (float)aConv->dPhiTracksAtVtx();
	Ucphoton_dPhiTracksAtEcal[Ucphoton_n]           = (float)aConv->dPhiTracksAtEcal();
	Ucphoton_dEtaTracksAtEcal[Ucphoton_n]           = (float)aConv->dEtaTracksAtEcal();
	new ((*Ucphoton_pairMomentum)[Ucphoton_n])   TVector3(aConv->pairMomentum().x(),
							  aConv->pairMomentum().y(),
							  aConv->pairMomentum().z());
	new ((*Ucphoton_conv_Vposition)[Ucphoton_n]) TVector3(aConv->conversionVertex().x(),
							  aConv->conversionVertex().y(),
							  aConv->conversionVertex().z());
      }//end of if ( aConv->conversionVertex().isValid() )
    }//end of for (unsigned int iConv=0; iConv<conversions.size(); iConv++)

    if(fillHErechitInfo_ && ucphoton->isEB()){
      set<DetId> UcHERecHitSet;
      for(HBHERecHitCollection::const_iterator hh = hbhe->begin(); hh != hbhe->end() && UcHERecHit_subset_n<10000; hh++){
	HcalDetId id(hh->detid());
	if (id.subdet()==2){
	  const CaloCellGeometry *hbhe_cell = caloGeom->getGeometry(hh->id());
	  Global3DPoint hbhe_position = hbhe_cell->getPosition();
	  if(fabs(deltaPhi(correct_phi((float)ucphoton->superCluster()->phi()),correct_phi(hbhe_position.phi())) ) < 0.5
	     && hh->energy()>1.){
	    //find the detid in the set
	    set<DetId>::const_iterator UcHERecHitChecker = UcHERecHitSet.find(hh->detid());
	    //if detid is not found in the set,(we reached the end), save info!
	    if(UcHERecHitChecker == UcHERecHitSet.end()){
	      UcHERecHitSet.insert(hh->detid());
	      //UcHERecHit_subset_detid.push_back(hh->detid());
	      UcHERecHit_subset_energy.push_back((float) hh->energy());
	      UcHERecHit_subset_time.push_back((float)hh->time());
	      UcHERecHit_subset_depth.push_back((float)id.depth());
	      new ((*UcHERecHit_subset_position)[UcHERecHit_subset_n])   TVector3(hbhe_position.x(),
										  hbhe_position.y(),
										  hbhe_position.z());
	      UcHERecHit_subset_n++;
	    }//check to see if hit is already saved
	  }//if delta dphi from ucphoton is small and E>1 try to save
	}
      }
    }// fillHErechitInfo__ && pho_isEB    
    if(fillrechitInfo_)
      {
	std::vector< std::pair<DetId, float> >  UcphotonHit_DetIds ;
	UcphotonHit_DetIds  = ucphoton->superCluster()->hitsAndFractions();
	std::vector< std::pair<DetId, float> >::const_iterator detitr;
	std::vector<int> IEtas, IPhis, IXs, IYs; 
	std::vector<float> Energy, Time;
      
	IEtas.clear();   IPhis.clear(); IXs.clear();     IYs.clear();
	Energy.clear(); Time.clear(); 
	int ihit = 0;
	
	for (detitr = UcphotonHit_DetIds.begin(); detitr != UcphotonHit_DetIds.end(); ++detitr) {
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
	      Energy[ihit]  = thishit->energy();
	      Time[ihit]    = thishit->time();
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
	      Energy[ihit]  = thishit->energy();
	      Time[ihit]    = thishit->time();
	    }
	  }
	  if (((*detitr).first).det() == DetId::Ecal && ((*detitr).first).subdetId() != EcalEndcap && ((*detitr).first).subdetId() != EcalBarrel) {
	    std::cout<<" a ucphoton in "<<((*detitr).first).subdetId()<<std::endl;
	  }
	}
	Ucphoton_IEtas.push_back(IEtas); Ucphoton_IPhis.push_back(IPhis);  
	Ucphoton_IXs.push_back(IXs); Ucphoton_IYs.push_back(IYs);  
	Ucphoton_RecHitEnergy.push_back(Energy); Ucphoton_RecHitTime.push_back(Time);
	
	if(ucphoton->isEB()){
	  std::vector<float> showershapes_barrel = EcalClusterTools::roundnessBarrelSuperClusters(*(ucphoton->superCluster()),*barrelRecHits,0);
	  Ucphoton_roundness.push_back((float)showershapes_barrel[0]);
	  Ucphoton_angle.push_back((float)showershapes_barrel[1]);
	  
	  const reco::CaloClusterPtr  seed = ucphoton->superCluster()->seed();
	  DetId id = lazyTool.getMaximum(*seed).first;
	  float swissCross =-99.;
	  Ucphoton_e6e2.push_back(-99.);
	  Ucphoton_e4e1.push_back(-99.);    
	  
	  const EcalRecHitCollection & rechits = ( ucphoton->isEB() ? *Brechit : *Brechit);
	  EcalRecHitCollection::const_iterator it = rechits.find( id );
	  if( it != rechits.end() )
	    swissCross = EcalTools::swissCross( id, rechits, 0.08, true);
	  Ucphoton_swissCross.push_back(swissCross);
	  Ucphoton_e2e9.push_back(-99.);
	  Ucphoton_e2e9[Ucphoton_n]    = GetE2OverE9(id,rechits); 
	  Ucphoton_e6e2[Ucphoton_n]    = Gete6e2( id, rechits); 
	  Ucphoton_e4e1[Ucphoton_n]    = e4e1(id, rechits); 
	  
	  if(debug_ && 1-Ucphoton_swissCross[Ucphoton_n]/Ucphoton_seedE[Ucphoton_n] > 0.95) 
	    std::cout<<"This ucphoton candidate is an ECAL spike identified by Swiss Cross algorithm."<<std::endl;
	  
	  vector<float> stdCov  = EcalClusterTools::covariances(*(ucphoton->superCluster()->seed()),&(*barrelRecHits),&(*topology),&(*caloGeom));
	  vector<float> crysCov = EcalClusterTools::localCovariances(*(ucphoton->superCluster()->seed()),&(*barrelRecHits),&(*topology));
	  Ucphoton_SigmaEtaPhi.push_back(sqrt(stdCov[1]));
	  Ucphoton_SigmaIetaIphi.push_back(sqrt(crysCov[1]));   
	  Ucphoton_SigmaPhiPhi.push_back(sqrt(stdCov[2]));
	  Ucphoton_SigmaIphiIphi.push_back(sqrt(crysCov[2]));
	}
	else{ 
	  Ucphoton_roundness.push_back(-99.);
	  Ucphoton_angle.push_back(-99.);
	  
	  const reco::CaloClusterPtr  seed = ucphoton->superCluster()->seed();
	  DetId id = lazyTool.getMaximum(*seed).first;
	  float swissCross = -99.;
	  Ucphoton_e6e2.push_back(-99.);
	  Ucphoton_e4e1.push_back(-99.);    
	  
	  const EcalRecHitCollection & rechits = ( ucphoton->isEB() ? *Erechit : *Erechit);
	  EcalRecHitCollection::const_iterator it = rechits.find( id );
	  if( it != rechits.end() ) { 
	    swissCross = EcalTools::swissCross( id, rechits, 0.08, true);
	  }
	  
	  Ucphoton_swissCross.push_back(swissCross);
	  Ucphoton_e2e9.push_back(-99.);
	  Ucphoton_e2e9[Ucphoton_n]    = GetE2OverE9(id,rechits); 
	  Ucphoton_e6e2[Ucphoton_n]    = Gete6e2( id, rechits); 
	  Ucphoton_e4e1[Ucphoton_n]    = e4e1(id, rechits); 
	  
	  if(debug_ && 1-Ucphoton_swissCross[Ucphoton_n]/Ucphoton_seedE[Ucphoton_n] > 0.95) {
	    std::cout<<"This ucphoton candidate is an ECAL spike identified by Swiss Cross algorithm." << std::endl;
	    std::cout<<"This would be weird since there aren't spikes in the endcap of ECAL"<< std::endl; 
	  }
	  
	  vector<float> stdCov  = EcalClusterTools::covariances(*(ucphoton->superCluster()->seed()),&(*endcapRecHits),&(*topology),&(*caloGeom));
	  vector<float> crysCov = EcalClusterTools::localCovariances(*(ucphoton->superCluster()->seed()),&(*endcapRecHits),&(*topology));
	  Ucphoton_SigmaEtaPhi.push_back(sqrt(stdCov[1]));
	  Ucphoton_SigmaIetaIphi.push_back(sqrt(crysCov[1]));   
	  Ucphoton_SigmaPhiPhi.push_back(sqrt(stdCov[2]));
	  Ucphoton_SigmaIphiIphi.push_back(sqrt(crysCov[2]));
   
	}//end of else (if !EB)
      }

    Ucphoton_n++;	
  }//end of ucphotonloop
  
  
  if(debug_)    std::cout<<"got ucphoton info"<<std::endl;
}

void ucphotonInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&Ucphoton_n  ,"Ucphoton_n");
  AddBranch(&Ucphoton_ecalRecHitSumEtConeDR04 ,"Ucphoton_ecalRecHitSumEtConeDR04");
  AddBranch(&Ucphoton_hcalTowerSumEtConeDR04,  "Ucphoton_hcalTowerSumEtConeDR04");
  AddBranch(&Ucphoton_trkSumPtHollowConeDR04,  "Ucphoton_trkSumPtHollowConeDR04");
  AddBranch(&Ucphoton_trkSumPtSolidConeDR04,   "Ucphoton_trkSumPtSolidConeDR04");
  AddBranch(&Ucphoton_nTrkSolidConeDR04		,"Ucphoton_nTrkSolidConeDR04");
  AddBranch(&Ucphoton_nTrkHollowConeDR04		,"Ucphoton_nTrkHollowConeDR04");
  AddBranch(&Ucphoton_hcalDepth1TowerSumEtConeDR04,"Ucphoton_hcalDepth1TowerSumEtConeDR04");
  AddBranch(&Ucphoton_hcalDepth2TowerSumEtConeDR04,"Ucphoton_hcalDepth2TowerSumEtConeDR04");
  
  AddBranch(&Ucphoton_ecalRecHitSumEtConeDR03	,"Ucphoton_ecalRecHitSumEtConeDR03");
  AddBranch(&Ucphoton_hcalTowerSumEtConeDR03	,"Ucphoton_hcalTowerSumEtConeDR03");
  AddBranch(&Ucphoton_trkSumPtHollowConeDR03	,"Ucphoton_trkSumPtHollowConeDR03");
  AddBranch(&Ucphoton_trkSumPtSolidConeDR03	,"Ucphoton_trkSumPtSolidConeDR03");
  AddBranch(&Ucphoton_nTrkSolidConeDR03		,"Ucphoton_nTrkSolidConeDR03");
  AddBranch(&Ucphoton_nTrkHollowConeDR03		,"Ucphoton_nTrkHollowConeDR03");
  AddBranch(&Ucphoton_hcalDepth1TowerSumEtConeDR03,"Ucphoton_hcalDepth1TowerSumEtConeDR03");
  AddBranch(&Ucphoton_hcalDepth2TowerSumEtConeDR03,"Ucphoton_hcalDepth2TowerSumEtConeDR03");
  
  AddBranch(&Ucphoton_SigmaEtaEta			,"Ucphoton_SigmaEtaEta");
  AddBranch(&Ucphoton_e1x5			,"Ucphoton_e1x5");
  AddBranch(&Ucphoton_e2x5			,"Ucphoton_e2x5");
  AddBranch(&Ucphoton_e3x3                        ,"Ucphoton_e3x3");
  AddBranch(&Ucphoton_e5x5			,"Ucphoton_e5x5");
  AddBranch(&Ucphoton_r1x5			,"Ucphoton_r1x5");
  AddBranch(&Ucphoton_r2x5			,"Ucphoton_r2x5");
  AddBranch(&Ucphoton_isEB			,"Ucphoton_isEB");
  AddBranch(&Ucphoton_isEE			,"Ucphoton_isEE");
  AddBranch(&Ucphoton_isEBGap			,"Ucphoton_isEBGap");
  AddBranch(&Ucphoton_isEEGap   			,"Ucphoton_isEEGap");
  AddBranch(&Ucphoton_isEBEEGap			,"Ucphoton_isEBEEGap");
  
  AddBranch(&Ucphoton_nBCs			,"Ucphoton_nBCs");
  AddBranch(&Ucphoton_seedE			,"Ucphoton_seedE");
  AddBranch(&Ucphoton_sc_E			,"Ucphoton_sc_E");
  AddBranch(&Ucphoton_sc_etaWidth			,"Ucphoton_sc_etaWidth");
  AddBranch(&Ucphoton_sc_phiWidth			,"Ucphoton_sc_phiWidth");
  AddBranch(&Ucphoton_hasConvTrk			,"Ucphoton_hasConvTrk");
  AddBranch(&Ucphoton_mipChi2			,"Ucphoton_mipChi2");
  AddBranch(&Ucphoton_mipTotEnergy		,"Ucphoton_mipTotEnergy");
  AddBranch(&Ucphoton_mipSlope			,"Ucphoton_mipSlope");
  AddBranch(&Ucphoton_mipIntercept		,"Ucphoton_mipIntercept");
  AddBranch(&Ucphoton_mipNhitCone			,"Ucphoton_mipNhitCone");
  AddBranch(&Ucphoton_mipIsHalo			,"Ucphoton_mipIsHalo");
  if(fillgenMatchInfo_){
    AddBranch(&Ucphoton_match_MGenpdgId		,"Ucphoton_match_MGenpdgId");
    AddBranch(&Ucphoton_match_numM			,"Ucphoton_match_numM");
    AddBranch(&Ucphoton_match_GrandMGenmompdgId	,"Ucphoton_match_GrandMGenmompdgId");
    AddBranch(&Ucphoton_isgenmatched         	,"Ucphoton_isgenmatched");
    AddBranch(&Ucphoton_match_4Momentum  ,"Ucphoton_match_4Momentum");
  }

  AddBranch(&Ucphoton_nTracks			,"Ucphoton_nTracks");
  AddBranch(&Ucphoton_isConverted			,"Ucphoton_isConverted");
  AddBranch(&Ucphoton_pairInvariantMass		,"Ucphoton_pairInvariantMass");
  AddBranch(&Ucphoton_pairCotThetaSeparation	,"Ucphoton_pairCotThetaSeparation");
  AddBranch(&Ucphoton_EoverP			,"Ucphoton_EoverP");
  AddBranch(&Ucphoton_zOfPrimaryVertex		,"Ucphoton_zOfPrimaryVertex");
  AddBranch(&Ucphoton_distOfMinimumApproach	,"Ucphoton_distOfMinimumApproach");
  AddBranch(&Ucphoton_dPhiTracksAtVtx		,"Ucphoton_dPhiTracksAtVtx");
  AddBranch(&Ucphoton_dPhiTracksAtEcal		,"Ucphoton_dPhiTracksAtEcal");
  AddBranch(&Ucphoton_dEtaTracksAtEcal		,"Ucphoton_dEtaTracksAtEcal");
  if(fillHErechitInfo_ ){
    AddBranch(&UcHERecHit_subset_energy		,"UcHERecHit_subset_energy");
    AddBranch(&UcHERecHit_subset_time	   	,"UcHERecHit_subset_time");
    AddBranch(&UcHERecHit_subset_depth		,"UcHERecHit_subset_depth");
    AddBranch(&UcHERecHit_subset_position,"UcHERecHit_subset_position");
    AddBranch(&UcHERecHit_subset_n,"UcHERecHit_subset_n");
  }
  if(fillrechitInfo_){
    AddBranch(&Ucphoton_IEtas		      	,"Ucphoton_IEtas");
    AddBranch(&Ucphoton_IPhis		      	,"Ucphoton_IPhis");
    AddBranch(&Ucphoton_IXs				,"Ucphoton_IXs");
    AddBranch(&Ucphoton_IYs				,"Ucphoton_IYs");
    AddBranch(&Ucphoton_RecHitEnergy	      	,"Ucphoton_RecHitEnergy");
    AddBranch(&Ucphoton_RecHitTime			,"Ucphoton_RecHitTime");
    AddBranch(&Ucphoton_roundness			,"Ucphoton_roundness");
    AddBranch(&Ucphoton_angle		     	,"Ucphoton_angle");
    AddBranch(&Ucphoton_e6e2		      	,"Ucphoton_e6e2");
    AddBranch(&Ucphoton_e4e1		      	,"Ucphoton_e4e1");
    AddBranch(&Ucphoton_e2e9		      	,"Ucphoton_e2e9");
    AddBranch(&Ucphoton_swissCross			,"Ucphoton_swissCross");
    
    AddBranch(&Ucphoton_SigmaEtaPhi			,"Ucphoton_SigmaEtaPhi");
    AddBranch(&Ucphoton_SigmaIetaIphi	      	,"Ucphoton_SigmaIetaIphi");
    AddBranch(&Ucphoton_SigmaPhiPhi			,"Ucphoton_SigmaPhiPhi");
    AddBranch(&Ucphoton_SigmaIphiIphi               ,"Ucphoton_SigmaIphiIphi");   
  
    AddBranch(&Ucphoton_hadronicOverEm,          "Ucphoton_hadronicOverEm");
    AddBranch(&Ucphoton_hasPixelSeed,            "Ucphoton_hasPixelSeed");
    AddBranch(&Ucphoton_sigmaIetaIeta,           "Ucphoton_sigmaIetaIeta");
    AddBranch(&Ucphoton_r9,                      "Ucphoton_r9");
  }
  AddBranch(&Ucphoton_4Momentum ,"Ucphoton_4Momentum");
  AddBranch(&Ucphoton_Vposition ,"Ucphoton_Vposition");
  AddBranch(&Ucphoton_sc_position ,"Ucphoton_sc_position");
  
  AddBranch(&Ucphoton_pairMomentum,"Ucphoton_pairMomentum");
  AddBranch(&Ucphoton_conv_Vposition,"Ucphoton_conv_Vposition");

  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void ucphotonInfo::Clear(){
  if(debug_)std::cout<<"clearing Ucphoton info"<<std::endl;
  Ucphoton_n =0;
  
  Ucphoton_4Momentum->Clear();
  Ucphoton_Vposition->Clear();
  Ucphoton_sc_position->Clear();
  Ucphoton_pairMomentum->Clear();
  Ucphoton_conv_Vposition->Clear();

  Ucphoton_ecalRecHitSumEtConeDR04.clear();
  Ucphoton_hcalTowerSumEtConeDR04.clear();
  Ucphoton_trkSumPtHollowConeDR04.clear();
  Ucphoton_trkSumPtSolidConeDR04.clear();
  Ucphoton_nTrkSolidConeDR04.clear();
  Ucphoton_nTrkHollowConeDR04.clear();
  Ucphoton_hcalDepth1TowerSumEtConeDR04.clear();
  Ucphoton_hcalDepth2TowerSumEtConeDR04.clear();

  Ucphoton_ecalRecHitSumEtConeDR03.clear();
  Ucphoton_hcalTowerSumEtConeDR03.clear();
  Ucphoton_trkSumPtHollowConeDR03.clear();
  Ucphoton_trkSumPtSolidConeDR03.clear();
  Ucphoton_nTrkSolidConeDR03.clear();
  Ucphoton_nTrkHollowConeDR03.clear();
  Ucphoton_hcalDepth1TowerSumEtConeDR03.clear();
  Ucphoton_hcalDepth2TowerSumEtConeDR03.clear();

  Ucphoton_SigmaEtaEta.clear();
  Ucphoton_e1x5.clear();
  Ucphoton_e2x5.clear();
  Ucphoton_e3x3.clear();
  Ucphoton_e5x5.clear();
  Ucphoton_r1x5.clear();
  Ucphoton_r2x5.clear();
  Ucphoton_isEB.clear();
  Ucphoton_isEE.clear();
  Ucphoton_isEBGap.clear();
  Ucphoton_isEEGap.clear();
  Ucphoton_isEBEEGap.clear();

  Ucphoton_nBCs.clear();
  Ucphoton_seedE.clear();
  Ucphoton_sc_E.clear();
  Ucphoton_sc_etaWidth.clear();
  Ucphoton_sc_phiWidth.clear();
  Ucphoton_hasConvTrk.clear();
  Ucphoton_mipChi2.clear();
  Ucphoton_mipTotEnergy.clear();
  Ucphoton_mipSlope.clear();
  Ucphoton_mipIntercept.clear();
  Ucphoton_mipNhitCone.clear();
  Ucphoton_mipIsHalo.clear();

  if(fillgenMatchInfo_){
    Ucphoton_match_MGenpdgId.clear();
    Ucphoton_match_numM.clear();
    Ucphoton_match_GrandMGenmompdgId.clear();
    Ucphoton_isgenmatched.clear();
    Ucphoton_match_4Momentum->Clear();
  }
  Ucphoton_nTracks.clear();
  Ucphoton_isConverted.clear();
  Ucphoton_pairInvariantMass.clear();
  Ucphoton_pairCotThetaSeparation.clear();
  Ucphoton_EoverP.clear();
  Ucphoton_zOfPrimaryVertex.clear();
  Ucphoton_distOfMinimumApproach.clear();
  Ucphoton_dPhiTracksAtVtx.clear();
  Ucphoton_dPhiTracksAtEcal.clear();
  Ucphoton_dEtaTracksAtEcal.clear();
  if(fillHErechitInfo_ ){
    UcHERecHit_subset_n = 0;
    UcHERecHit_subset_energy.clear();
    UcHERecHit_subset_time.clear();
    UcHERecHit_subset_depth.clear();
    UcHERecHit_subset_position->Clear();
  }
  if(fillrechitInfo_){
    Ucphoton_IEtas.clear();
    Ucphoton_IPhis.clear();
    Ucphoton_IXs.clear();
    Ucphoton_IYs.clear();
    Ucphoton_RecHitEnergy.clear();
    Ucphoton_RecHitTime.clear();
    Ucphoton_roundness.clear();
    Ucphoton_angle.clear();
    Ucphoton_e6e2.clear();
    Ucphoton_e4e1.clear();
    Ucphoton_e2e9.clear();
    Ucphoton_swissCross.clear();
    
    Ucphoton_SigmaEtaPhi.clear();
    Ucphoton_SigmaIetaIphi.clear();
    Ucphoton_SigmaPhiPhi.clear();
    Ucphoton_SigmaIphiIphi.clear();
    Ucphoton_sigmaIetaIeta.clear();
    Ucphoton_hadronicOverEm.clear();
    Ucphoton_hasPixelSeed.clear();
    Ucphoton_r9.clear();
  }


  if(debug_) std::cout<<"cleared"<<std::endl;
}

float ucphotonInfo::GetE2OverE9( const DetId id, const EcalRecHitCollection & recHits)
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
float ucphotonInfo::Gete6e2(const DetId& id, 
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

float ucphotonInfo::correct_phi(float phi){
	return (phi >= 0 ? phi : (2*TMath::Pi() + phi));
}


float ucphotonInfo::e4e1(const DetId& id, 
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

float ucphotonInfo::recHitE( const  DetId id,  const EcalRecHitCollection &recHits )
{
  if ( id == DetId(0) ) {
    return 0;
  } else {
    EcalRecHitCollection::const_iterator it = recHits.find( id );
    if ( it != recHits.end() ) return (*it).energy();
  }
  return 0;
}


float ucphotonInfo::recHitE( const DetId id, const EcalRecHitCollection & recHits,int di, int dj )
{
  // in the barrel:   di = dEta   dj = dPhi
  // in the endcap:   di = dX     dj = dY
  DetId nid;
  if( id.subdetId() == EcalBarrel) nid = EBDetId::offsetBy( id, di, dj );
  else if( id.subdetId() == EcalEndcap) nid = EEDetId::offsetBy( id, di, dj );
  return ( nid == DetId(0) ? 0 : recHitE( nid, recHits ) );
}



float ucphotonInfo::recHitApproxEt(const DetId id,  const EcalRecHitCollection &recHits )
{
  // for the time being works only for the barrel
  if ( id.subdetId() == EcalBarrel ) {
    return recHitE(id,recHits ) / cosh( EBDetId::approxEta( id ) );
  }
  return 0;
}




float ucphotonInfo::recHitE( const DetId id, const EcalRecHitCollection &recHits, bool useTimingInfo ){
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
const std::vector<DetId> ucphotonInfo::neighbours(const DetId& id){                                                                                                                              
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

