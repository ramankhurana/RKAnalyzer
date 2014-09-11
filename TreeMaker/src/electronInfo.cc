#include "RKAnalyzer/TreeMaker/interface/electronInfo.h"
#include "TrackingTools/TransientTrack/interface/GsfTransientTrack.h"
//--

// Tracker tracks 
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GeometryVector/interface/Basic3DVector.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include <DataFormats/MuonReco/interface/MuonFwd.h>

// Transient tracks
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoTracker/Record/interface/TrackerRecoGeometryRecord.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

// Vertex utilities
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

// Beam Spot
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

// Geometry
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/Common/interface/AssociationVector.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/Common/interface/RefToBase.h"
//--

electronInfo::electronInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in electron constructor"<<std::endl;
  electronLabel_               = iConfig.getUntrackedParameter<edm::InputTag> ("electronLabel_");
  Electron_4Momentum           = new TClonesArray("TLorentzVector");
  Electron_Vposition           = new TClonesArray("TVector3");
  Electron_sc_position         = new TClonesArray("TVector3");
  if(debug) std::cout<<"in electron constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

electronInfo::~electronInfo(){
  delete tree_;
}

void electronInfo::Fill(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  Clear();
  if(debug_)    std::cout<<"getting electron info"<<std::endl;

  
  GsfTrackRef eletrack;
  reco::TransientTrack eletranstrack;  
  ESHandle<TransientTrackBuilder> trackBuilder;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",trackBuilder);
  
  Handle<reco::BeamSpot> recoBeamSpotHandle;
  //iEvent.getByType(recoBeamSpotHandle);
  iEvent.getByLabel("offlineBeamSpot",recoBeamSpotHandle);


  //const BeamSpot bs = *recoBeamSpotHandle;
  //GlobalPoint BSVertex(bs.position().x(),bs.position().y(),bs.position().z());
  //Basic3DVector<double> BSVertexErr(bs.x0Error(),bs.y0Error(),bs.z0Error());
  //
  //reco::Vertex::Point BSp(bs.position().x(),bs.position().y(),bs.position().z());
  //reco::Vertex::Error BSe;
  //
  //BSe(0,0) = bs.x0Error()*bs.x0Error();
  //BSe(1,1) = bs.y0Error()*bs.y0Error();
  //BSe(2,2) = bs.z0Error()*bs.z0Error();
  //reco::Vertex BSprimVertex = reco::Vertex(BSp,BSe,1,1,1);
  //reco::Vertex primVertex;
  //GlobalPoint pVertex(primVertex.position().x(),primVertex.position().y(),primVertex.position().z());



  //edm::Handle<std::vector<pat::Electron> > electronHandle;
  edm::Handle<edm::View<pat::Electron> > electronHandle;
  iEvent.getByLabel(electronLabel_,electronHandle);
  if(not iEvent.getByLabel(electronLabel_,electronHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<electronLabel_<<std::endl; 
    exit(0);
  }  
  
  const edm::View<pat::Electron> ele = *(electronHandle.product());



  edm::Handle<edm::View<pat::Electron> > electronHandle_mva;
  iEvent.getByLabel(electronLabel_,electronHandle_mva);
  if(not iEvent.getByLabel(electronLabel_,electronHandle_mva)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<"selectedPatElectrons"<<std::endl; 
    exit(0);
  }  
  
  //const edm::View<pat::Electron> ele = *(electronHandle.product());
  const edm::View<pat::Electron> ele_mva = *(electronHandle_mva.product());
  

  int i=0;
  //pat::ElectronCollection::const_iterator electron;
  for(edm::View<pat::Electron>::const_iterator electron=electronHandle->begin(); electron!=electronHandle->end(); electron++){
    //double id = mvaelectronid.FindMVA(iEvent,iSetup,ele[i]);
    //double id_new = mvaelectronid.FindMVA(iEvent,iSetup,ele_mva[i]);
    double id = -999.0;
    double id_new = -999.0; 

    
    //    if(debug_)
      std::cout<<" id = "<<id
	       <<" id new = "<<id_new<<std::endl;
    Electron_MVAID.push_back(id);
    Electron_MVAID_new.push_back(id_new);
    
    Electron_particleIso.push_back((float)electron->particleIso());
    Electron_neutralIso.push_back((float)electron->neutralHadronIso());
    Electron_photonIso.push_back((float)electron->photonIso());
    Electron_chargedIso.push_back((float)electron->chargedHadronIso());
    Electron_dbeta.push_back((float)electron->userIso(0.0));

    
    //if(debug_)   
    std::cout<<"  pt  = "<<electron->pt()
	     <<"  energy :  "<<electron->energy()
	     <<" eta : "<<electron->eta()
	     <<" phi : "<<electron->phi()
	     <<"  electron particle iso "<<electron->particleIso()
	     <<"  neutral hadron   :  "<<electron->neutralHadronIso()
	     <<"  gamma   :  "<<electron->photonIso()
	     <<"  charged hadron  :  "<<electron->chargedHadronIso()
	     <<std::endl;
    
    ////////New dz 
    ESHandle<MagneticField> B;
    iSetup.get<IdealMagneticFieldRecord > ().get(B);
    const MagneticField* magField = B.product();

    //BeamSpot
    Handle<reco::BeamSpot> theBeamSpotHandle;
    iEvent.getByLabel(std::string("offlineBeamSpot"), theBeamSpotHandle);
    const reco::BeamSpot* beamSpot = theBeamSpotHandle.product();
    
    // Get the geometry
    ESHandle<GlobalTrackingGeometry> geomHandle;
    iSetup.get<GlobalTrackingGeometryRecord > ().get(geomHandle);
    
    
    
    if (electron->gsfTrack().isNonnull()) {
      reco::GsfTransientTrack track(electron->gsfTrack(), magField, geomHandle);
      TransverseImpactPointExtrapolator extrapolator(magField);
      TrajectoryStateOnSurface closestOnTransversePlaneState = extrapolator.extrapolate(track.impactPointState(), GlobalPoint(beamSpot->position().x(), beamSpot->position().y(), 0.0));
      NewVz.push_back(closestOnTransversePlaneState.globalPosition().z());
      //std::cout<<" closestOnTransversePlaneState.globalPosition().z() = "<<closestOnTransversePlaneState.globalPosition().z()<<std::endl;
    }


    TLorentzVector p4(electron->px(),electron->py(),electron->pz(),electron->energy());
    new( (*Electron_4Momentum)[Electron_n]) TLorentzVector(p4);
    TVector3 v3(electron->vx(),electron->vy(),electron->vz());
    new( (*Electron_Vposition)[Electron_n]) TVector3(v3);
    new( (*Electron_sc_position)[Electron_n]) TVector3(electron->superCluster()->x(),
						       electron->superCluster()->y(),
						       electron->superCluster()->z());
    
    Electron_charge.push_back((int)electron->charge());
    Electron_HoE.push_back((float)electron->hadronicOverEm());
    Electron_SigmaIetaIeta.push_back((float)electron->sigmaIetaIeta());
    Electron_trkIso.push_back((float)electron->dr03TkSumPt()) ;
    Electron_ecalIso.push_back((float)electron->dr03EcalRecHitSumEt());
    Electron_hcalIso.push_back((float)electron->dr03HcalTowerSumEt());
    Electron_dEtaIn.push_back((float)electron->deltaEtaSuperClusterTrackAtVtx());
    Electron_dPhiIn.push_back((float)electron->deltaPhiSuperClusterTrackAtVtx());
    Electron_sc_E.push_back((float)electron->superCluster()->energy());
    Electron_sc_Eta.push_back((float)electron->superCluster()->eta());
    Electron_missing_hits.push_back((float)electron->gsfTrack()->trackerExpectedHitsInner().numberOfHits());
    
    
    
    Electron_Significance3D.push_back(electron->dB(pat::Electron::PV3D)/electron->edB(pat::Electron::PV3D));  
    Electron_Value3D.push_back(electron->dB(pat::Electron::PV3D));
    Electron_Error3D.push_back(electron->edB(pat::Electron::PV3D));

    Electron_Significance2D.push_back(electron->dB(pat::Electron::PV2D)/electron->edB(pat::Electron::PV2D));  
    Electron_Value2D.push_back(electron->dB(pat::Electron::PV2D));
    Electron_Error2D.push_back(electron->edB(pat::Electron::PV2D));
    
    
    // Added on 3rd September 2014
    Electron_IsPF.push_back(electron->isPF());
    Electron_SigmaIetaIphi.push_back(electron->sigmaIetaIphi());
    Electron_IP3D.push_back(electron->ip3d());
    Electron_full5x5_sigmaEtaEta.push_back(electron->full5x5_sigmaEtaEta());
    Electron_full5x5_sigmaIetaIeta.push_back(electron->full5x5_sigmaIetaIeta());
    Electron_full5x5_sigmaIphiIphi.push_back(electron->full5x5_sigmaIphiIphi());
    Electron_full5x5_sigmaIetaIphi.push_back(electron->full5x5_sigmaIetaIphi());
    Electron_full5x5_e1x5.push_back(electron->full5x5_e1x5());
    Electron_full5x5_e2x5Max.push_back(electron->full5x5_e2x5Max());
    Electron_full5x5_e5x5.push_back(electron->full5x5_e5x5());
    Electron_full5x5_r9.push_back(electron->full5x5_r9());
    Electron_full5x5_hcalDepth1OverEcal.push_back(electron->full5x5_hcalDepth1OverEcal());
    Electron_full5x5_hcalDepth2OverEcal.push_back(electron->full5x5_hcalDepth2OverEcal());
    Electron_full5x5_hcalOverEcal.push_back(electron->full5x5_hcalOverEcal());
    Electron_full5x5_hcalDepth1OverEcalBc.push_back(electron->full5x5_hcalDepth1OverEcalBc());
    Electron_full5x5_hcalDepth2OverEcalBc.push_back(electron->full5x5_hcalDepth2OverEcalBc());
    Electron_full5x5_hcalOverEcalBc.push_back(electron->full5x5_hcalOverEcalBc());

    //std::cout<<"superCluster()->clustersSize() "<<electron->superCluster()->clustersSize()
    //	     <<"superCluster()->hitsAndFractions().size() "<<electron->superCluster()->hitsAndFractions().size()
    //	     <<"superCluster()->rawEnergy() "<<electron->superCluster()->rawEnergy()
    //	     <<"superCluster()->seed()->energy() "<<electron->superCluster()->seed()->energy()
    //	     <<"superCluster()->seed()->eta() "<<electron->superCluster()->seed()->eta()
    //	     <<"superCluster()->seed()->phi() "<<electron->superCluster()->seed()->phi()
    //	     <<"superCluster()->preshowerEnergy() "<<electron->superCluster()->preshowerEnergy()
    //	     <<"superCluster()->phiWidth() "<<electron->superCluster()->phiWidth()
    //	     <<"superCluster()->etaWidth() "<<electron->superCluster()->etaWidth()
    //	     <<"superCluster()->position().eta() "<<electron->superCluster()->position().eta()
    //	     <<"superCluster()->position().theta() "<<electron->superCluster()->position().theta()
    //	     <<"superCluster()->position().phi() "<<electron->superCluster()->position().phi()
    //         //<<" "<<electron->
    //	     //<<" "<<electron->
    //	     //<<" "<<electron->
    //	     //<<" "<<electron->
    //	     //<<" "<<electron->
    //	     //<<" "<<electron->
    //	     //<<" "<<electron->
    //	     <<std::endl;
    
    //----- FOR SIP 
    
    //eletrack = electron->get<GsfTrackRef>();
    //eletranstrack = trackBuilder->build( eletrack ) ;
    //
    //TrajectoryStateOnSurface eleTSOS;
    //bool useBeamSpot_= false;
    //if (useBeamSpot_==true){ 
    //  eleTSOS = IPTools::transverseExtrapolate(eletranstrack.impactPointState(), BSVertex, eletranstrack.field());
    //} 
    //else {
    //  eleTSOS = IPTools::transverseExtrapolate(eletranstrack.impactPointState(), pVertex, eletranstrack.field());
    //}
    //
    //
    //if (eleTSOS.isValid()){
    //  std::pair<bool,Measurement1D> eleIPpair;
    //  if (useBeamSpot_==true){ 
    //	eleIPpair = IPTools::signedImpactParameter3D(eletranstrack, eleTSOS.globalDirection(), BSprimVertex); 
    //  }
    //  else {
    //	eleIPpair = IPTools::signedImpactParameter3D(eletranstrack, eleTSOS.globalDirection(), primVertex);
    //  }
    //  //
    //  if (eleIPpair.first){
    //	std::cout<<"  eleIPpair.second.significance() = "<<eleIPpair.second.significance()
    //		 <<"  eleIPpair.second.value() = "<<eleIPpair.second.value()
    //		 <<"  eleIPpair.second.error() = "<<eleIPpair.second.error()
    //		 <<std::endl;
    //	Electron_Significance3D.push_back(eleIPpair.second.significance());
    //	Electron_Value3D.push_back(eleIPpair.second.value());
    //	Electron_Error3D.push_back(eleIPpair.second.error());
    //	
    //  } 
    //}           

    //----- FOR SIP ends here

    Electron_n++;	
    i++;
  }//end of electronloop
  
  
  if(debug_)    std::cout<<"got electron info"<<std::endl;
}

void electronInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&Electron_n  ,"Electron_n");
  
  AddBranch(&NewVz, "NewVz");
  AddBranch(&Electron_chargedIso, "Electron_chargedIso");
  AddBranch(&Electron_photonIso,"Electron_photonIso");
  AddBranch(&Electron_neutralIso, "Electron_neutralIso");
  AddBranch(&Electron_particleIso , "Electron_particleIso");
  AddBranch(&Electron_dbeta, "Electron_dbeta");
  AddBranch(&Electron_MVAID , "Electron_MVAID");
  AddBranch(&Electron_MVAID_new , "Electron_MVAID_new");
  
  AddBranch(&Electron_4Momentum   ,"Electron_4Momentum");
  AddBranch(&Electron_Vposition   ,"Electron_Vposition");
  AddBranch(&Electron_sc_position ,"Electron_sc_position");
  AddBranch(&Electron_charge ,"Electron_charge");
  AddBranch(&Electron_HoE    ,"Electron_HoE");
  AddBranch(&Electron_SigmaIetaIeta,"Electron_SigmaIetaIeta");
  AddBranch(&Electron_trkIso	 ,"Electron_trkIso");	
  AddBranch(&Electron_ecalIso	 ,"Electron_ecalIso");	
  AddBranch(&Electron_hcalIso	 ,"Electron_hcalIso");	
  AddBranch(&Electron_dEtaIn	 ,"Electron_dEtaIn");	
  AddBranch(&Electron_dPhiIn	 ,"Electron_dPhiIn");	
  AddBranch(&Electron_sc_E	 ,"Electron_sc_E");
  AddBranch(&Electron_sc_Eta	 ,"Electron_sc_Eta");
  AddBranch(&Electron_missing_hits	 ,"Electron_missing_hits");
   
  AddBranch(&Electron_Significance3D,"Electron_Significance3D");
  AddBranch(&Electron_Value3D,"Electron_Value3D");
  AddBranch(&Electron_Error3D,"Electron_Error3D");

  AddBranch(&Electron_Significance2D,"Electron_Significance2D");
  AddBranch(&Electron_Value2D,"Electron_Value2D");
  AddBranch(&Electron_Error2D,"Electron_Error2D");

  
  AddBranch(&Electron_IsPF,"Electron_IsPF");
  AddBranch(&Electron_SigmaIetaIphi,"Electron_SigmaIetaIphi");
  AddBranch(&Electron_IP3D,"Electron_IP3D");
  AddBranch(&Electron_full5x5_sigmaEtaEta,"Electron_full5x5_sigmaEtaEta");
  AddBranch(&Electron_full5x5_sigmaIetaIeta,"Electron_full5x5_sigmaIetaIeta");
  AddBranch(&Electron_full5x5_sigmaIphiIphi,"Electron_full5x5_sigmaIphiIphi");
  AddBranch(&Electron_full5x5_sigmaIetaIphi,"Electron_full5x5_sigmaIetaIphi");
  AddBranch(&Electron_full5x5_e1x5,"Electron_full5x5_e1x5");
  AddBranch(&Electron_full5x5_e2x5Max,"Electron_full5x5_e2x5Max");
  AddBranch(&Electron_full5x5_e5x5,"Electron_full5x5_e5x5");
  AddBranch(&Electron_full5x5_r9,"Electron_full5x5_r9");
  AddBranch(&Electron_full5x5_hcalDepth1OverEcal,"Electron_full5x5_hcalDepth1OverEcal");
  AddBranch(&Electron_full5x5_hcalDepth2OverEcal,"Electron_full5x5_hcalDepth2OverEcal");
  AddBranch(&Electron_full5x5_hcalOverEcal,"Electron_full5x5_hcalOverEcal");
  AddBranch(&Electron_full5x5_hcalDepth1OverEcalBc,"Electron_full5x5_hcalDepth1OverEcalBc");
  AddBranch(&Electron_full5x5_hcalDepth2OverEcalBc,"Electron_full5x5_hcalDepth2OverEcalBc");
  AddBranch(&Electron_full5x5_hcalOverEcalBc,"Electron_full5x5_hcalOverEcalBc");

  
  
  
  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void electronInfo::Clear(){
  if(debug_)std::cout<<"clearing Electron info"<<std::endl;
  Electron_n =0;
  Electron_MVAID.clear();
  Electron_MVAID_new.clear();
  NewVz.clear();
  Electron_chargedIso.clear();
  Electron_photonIso.clear();
  Electron_neutralIso.clear();
  Electron_particleIso.clear();
  Electron_dbeta.clear();

  Electron_4Momentum->Clear();
  Electron_Vposition->Clear();
  Electron_sc_position->Clear();
  Electron_charge.clear();
  Electron_HoE.clear();
  Electron_SigmaIetaIeta.clear();
  Electron_trkIso.clear();
  Electron_ecalIso.clear(); 
  Electron_hcalIso.clear(); 
  Electron_dEtaIn.clear();	   
  Electron_dPhiIn.clear(); 
  Electron_sc_E.clear();        
  Electron_sc_Eta.clear();        
  Electron_missing_hits.clear();        


  Electron_Significance3D.clear();
  Electron_Value3D.clear();
  Electron_Error3D.clear();

  Electron_Significance2D.clear();
  Electron_Value2D.clear();
  Electron_Error2D.clear();
  
  Electron_IsPF.clear();
  Electron_SigmaIetaIphi.clear();
  Electron_IP3D.clear();
  Electron_full5x5_sigmaEtaEta.clear();
  Electron_full5x5_sigmaIetaIeta.clear();
  Electron_full5x5_sigmaIphiIphi.clear();
  Electron_full5x5_sigmaIetaIphi.clear();
  Electron_full5x5_e1x5.clear();
  Electron_full5x5_e2x5Max.clear();
  Electron_full5x5_e5x5.clear();
  Electron_full5x5_r9.clear();
  Electron_full5x5_hcalDepth1OverEcal.clear();
  Electron_full5x5_hcalDepth2OverEcal.clear();
  Electron_full5x5_hcalOverEcal.clear();
  Electron_full5x5_hcalDepth1OverEcalBc.clear();
  Electron_full5x5_hcalDepth2OverEcalBc.clear();
  Electron_full5x5_hcalOverEcalBc.clear();
  
   
  if(debug_) std::cout<<"cleared"<<std::endl;
}
