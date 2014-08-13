#include "RKAnalyzer/TreeMaker/interface/muonInfo.h"
#include "TrackingTools/TransientTrack/interface/GsfTransientTrack.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"

// Geometry
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/Common/interface/AssociationVector.h"

#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoTracker/Record/interface/TrackerRecoGeometryRecord.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"


muonInfo::muonInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in muon constructor"<<std::endl;
  muonLabel_     = iConfig.getUntrackedParameter<edm::InputTag> ("muonLabel_");
  isAOD_         = iConfig.getUntrackedParameter<bool> ("isAOD_");
  Muon_4Momentum = new TClonesArray("TLorentzVector");
  Muon_Vposition = new TClonesArray("TVector3");
  Muon_OuterTrack_InnerPoint_position = new TClonesArray("TVector3");
  Muon_OuterTrack_InnerPoint_momentum = new TClonesArray("TVector3");
  Muon_OuterTrack_OuterPoint_position = new TClonesArray("TVector3");
  Muon_OuterTrack_OuterPoint_momentum = new TClonesArray("TVector3");
  Muon_InnerTrack_InnerPoint_position = new TClonesArray("TVector3");
  Muon_InnerTrack_InnerPoint_momentum = new TClonesArray("TVector3");
  Muon_InnerTrack_OuterPoint_position = new TClonesArray("TVector3");
  Muon_InnerTrack_OuterPoint_momentum = new TClonesArray("TVector3");
  if(debug) std::cout<<"in muon constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

muonInfo::~muonInfo(){
  delete tree_;
}

void muonInfo::Fill(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  Clear();
  if(debug_)    std::cout<<"getting muon info"<<std::endl;

  edm::Handle<edm::View<pat::Muon> > muonHandle;
  iEvent.getByLabel(muonLabel_,muonHandle);
  const edm::View<pat::Muon> & muons = *muonHandle;   // const ... &, we don't make a copy of it!
  
  
  ESHandle<TransientTrackBuilder> trackBuilder;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",trackBuilder);
  
  
  if(not iEvent.getByLabel(muonLabel_,muonHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<muonLabel_<<std::endl; 
    exit(0);
  }  
  edm::View<pat::Muon>::const_iterator muon;
  for(muon = muons.begin(); muon!=muons.end(); muon++){
    
    Muon_PFMuID.push_back((bool)muon->isPFMuon());
    TLorentzVector p4(muon->px(),muon->py(),muon->pz(),muon->energy());
    new( (*Muon_4Momentum)[Muon_n]) TLorentzVector(p4);
    TVector3 v3(muon->vx(),muon->vy(),muon->vz());
    new( (*Muon_Vposition)[Muon_n]) TVector3(v3);
    Muon_charge.push_back((int)muon->charge());
    Muon_trackIso.push_back((float) muon->trackIso());
    Muon_ecalIso.push_back ((float) muon->ecalIso());
    Muon_hcalIso.push_back ((float) muon->hcalIso());
    
    Muon_particleIso.push_back((float)muon->particleIso());
    Muon_neutralIso.push_back((float)muon->neutralHadronIso());
    Muon_photonIso.push_back((float)muon->photonIso());
    Muon_chargedIso.push_back((float)muon->chargedHadronIso());
    Muon_dbeta.push_back((float)muon->userIso(0.0));
    if(debug_) std::cout<<" Muon_chargedIso = "<<muon->particleIso()<<std::endl;

    
    
    
    if(debug_) std::cout<<" muon iso "<<muon->particleIso()
			<<"  :  "<<muon->neutralHadronIso()
			<<"  :  "<<muon->photonIso()
			<<"  :  "<<muon->chargedHadronIso()
			<<std::endl;
    

    ESHandle<MagneticField> B;
    iSetup.get<IdealMagneticFieldRecord > ().get(B);
    const MagneticField* magField = B.product();
    Handle<reco::BeamSpot> theBeamSpotHandle;
    iEvent.getByLabel(std::string("offlineBeamSpot"), theBeamSpotHandle);
    const reco::BeamSpot* beamSpot = theBeamSpotHandle.product();
    ESHandle<GlobalTrackingGeometry> geomHandle;
    iSetup.get<GlobalTrackingGeometryRecord > ().get(geomHandle);
    // Beamspot 
    Handle<reco::BeamSpot> recoBeamSpotHandle;
    //iEvent.getByType(recoBeamSpotHandle);
    iEvent.getByLabel("offlineBeamSpot",recoBeamSpotHandle);
    //const BeamSpot bs = *recoBeamSpotHandle;
    
    
    Muon_Significance3D.push_back(muon->dB(pat::Muon::PV3D)/muon->edB(pat::Muon::PV3D));
    Muon_Value3D.push_back(muon->dB(pat::Muon::PV3D));
    Muon_Error3D.push_back(muon->edB(pat::Muon::PV3D));  
    
    Muon_Significance2D.push_back(muon->dB(pat::Muon::PV2D)/muon->edB(pat::Muon::PV2D));
    Muon_Value2D.push_back(muon->dB(pat::Muon::PV2D));
    Muon_Error2D.push_back(muon->edB(pat::Muon::PV2D));  
    
    
    //---- FOR SIP
    //TrackRef mutrack;
    //reco::TransientTrack mutranstrack;
    //mutrack = muon->get<TrackRef>();
    //if (mutrack.isNull()){
    //  std::cout <<"tracker trackref is null since muon is STA" << std::endl;
    //  mutrack=muon->get<TrackRef,reco::StandAloneMuonTag>();
    //} 
    //
    //if(!mutrack.isNull()){
    //  std::cout<<" tracker trackref is NON-null since muon is STA"<<std::endl;
    //}
    //
    //mutranstrack = trackBuilder->build( mutrack ) ;
    //TrajectoryStateOnSurface muTSOS;
    //
    //std::pair<bool,Measurement1D> muIPpair;
    //bool useBeamSpot_=false;
    //reco::Vertex primVertex;
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
    //
    //GlobalPoint pVertex(primVertex.position().x(),primVertex.position().y(),primVertex.position().z());
    //if (useBeamSpot_==true){ 
    //  muTSOS = IPTools::transverseExtrapolate(mutranstrack.impactPointState(), BSVertex, mutranstrack.field());
    //} 
    //else {
    //  muTSOS = IPTools::transverseExtrapolate(mutranstrack.impactPointState(), pVertex, mutranstrack.field());
    //}
    //if (muTSOS.isValid()){
    //  std::pair<bool,Measurement1D> muIPpair;
    //  
    //  if (useBeamSpot_==true){   
    //	muIPpair = IPTools::absoluteImpactParameter3D(mutranstrack, BSprimVertex);
    //  } 
    //  else { 
    //	muIPpair = IPTools::absoluteImpactParameter3D(mutranstrack, primVertex);
    //  }
    //  
    //  if (muIPpair.first){
    //	Muon_Significance3D.push_back(muIPpair.second.significance());
    //	Muon_Value3D.push_back(muIPpair.second.value());
    //	Muon_Error3D.push_back(muIPpair.second.error());
    //	std::cout<<" Significance3D = "<<muIPpair.second.significance()
    //		 <<" Value3D = "<<muIPpair.second.value()
    //		 <<" Error3D = "<<muIPpair.second.error()
    //		 <<std::endl;
    //  }      
    //  //---- FOR SIP ends here
    //  }
    if (muon->innerTrack().isNonnull()) {
      reco::TransientTrack track(muon->innerTrack(), magField, geomHandle);
      TransverseImpactPointExtrapolator extrapolator(magField);
      TrajectoryStateOnSurface closestOnTransversePlaneState = extrapolator.extrapolate(track.impactPointState(), GlobalPoint(beamSpot->position().x(), beamSpot->position().y(), 0.0));
      Muon_NewVz.push_back(closestOnTransversePlaneState.globalPosition().z());
      
    }
    
    
    
    
    

    if(muon->globalTrack().isNonnull() ){
      Muon_normChi2.push_back((float)muon->normChi2());
      //Muon_normChi2.push_back((float)muon->normalizedChi2());
      Muon_validHits.push_back((int)muon->globalTrack()->hitPattern().numberOfValidMuonHits());
      Muon_trackerLayersWithMeasurement.push_back((int)muon->globalTrack()->hitPattern().trackerLayersWithMeasurement());
    }
    else {
      Muon_normChi2.push_back(-99.);
      Muon_validHits.push_back(-99);
    }
    //Muon_numberOfMatches.push_back((int)muon->numberOfMatches());
    Muon_numberOfMatches.push_back((int)muon->numberOfMatchedStations());
    if(muon->track().isNonnull() ){
      Muon_tkHits.push_back((int)muon->track()->numberOfValidHits());
      Muon_pixHits.push_back((int)muon->innerTrack()->hitPattern().numberOfValidPixelHits());
    }
    else {
      Muon_tkHits.push_back(-99);
      Muon_pixHits.push_back(-99);
    }

    Muon_isGlobalMuon.push_back((bool)muon->isGlobalMuon());
    Muon_isTrackerMuon.push_back((bool)muon->isTrackerMuon());
    Muon_isStandAloneMuon.push_back((bool)muon->isStandAloneMuon());
    
    new( (*Muon_OuterTrack_InnerPoint_position)[Muon_n]) TVector3(0,0,0);
    new( (*Muon_OuterTrack_InnerPoint_momentum)[Muon_n]) TVector3(0,0,0);
    new( (*Muon_OuterTrack_OuterPoint_position)[Muon_n]) TVector3(0,0,0);
    new( (*Muon_OuterTrack_OuterPoint_momentum)[Muon_n]) TVector3(0,0,0);
    new( (*Muon_InnerTrack_InnerPoint_position)[Muon_n]) TVector3(0,0,0);
    new( (*Muon_InnerTrack_InnerPoint_momentum)[Muon_n]) TVector3(0,0,0);
    new( (*Muon_InnerTrack_OuterPoint_position)[Muon_n]) TVector3(0,0,0);
    new( (*Muon_InnerTrack_OuterPoint_momentum)[Muon_n]) TVector3(0,0,0);

    if(!isAOD_){
      Muon_InnerTrack_isNonnull.push_back(muon->innerTrack().isNonnull());
      Muon_OuterTrack_isNonnull.push_back(muon->outerTrack().isNonnull());
      if(muon->innerTrack().isNonnull()){
	new( (*Muon_InnerTrack_InnerPoint_position)[Muon_n]) TVector3(muon->innerTrack()->innerPosition().x(),
								      muon->innerTrack()->innerPosition().y(),
								      muon->innerTrack()->innerPosition().z());
	new( (*Muon_InnerTrack_InnerPoint_momentum)[Muon_n]) TVector3(muon->innerTrack()->innerMomentum().x(), 
								      muon->innerTrack()->innerMomentum().y(), 
								      muon->innerTrack()->innerMomentum().z());
	new( (*Muon_InnerTrack_OuterPoint_position)[Muon_n]) TVector3(muon->innerTrack()->outerPosition().x(), 
								      muon->innerTrack()->outerPosition().y(), 
								      muon->innerTrack()->outerPosition().z());
	new( (*Muon_InnerTrack_OuterPoint_momentum)[Muon_n]) TVector3(muon->innerTrack()->outerMomentum().x(),
								      muon->innerTrack()->outerMomentum().y(), 
								      muon->innerTrack()->outerMomentum().z());
      }
      if(muon->outerTrack().isNonnull()){
	new( (*Muon_OuterTrack_InnerPoint_position)[Muon_n]) TVector3(muon->outerTrack()->innerPosition().x(),
								      muon->outerTrack()->innerPosition().y(),
								      muon->outerTrack()->innerPosition().z());
	new( (*Muon_OuterTrack_InnerPoint_momentum)[Muon_n]) TVector3(muon->outerTrack()->innerMomentum().x(), 
								      muon->outerTrack()->innerMomentum().y(), 
								      muon->outerTrack()->innerMomentum().z());
	new( (*Muon_OuterTrack_OuterPoint_position)[Muon_n]) TVector3(muon->outerTrack()->outerPosition().x(), 
								      muon->outerTrack()->outerPosition().y(), 
								      muon->outerTrack()->outerPosition().z());
	new( (*Muon_OuterTrack_OuterPoint_momentum)[Muon_n]) TVector3(muon->outerTrack()->outerMomentum().x(),
								      muon->outerTrack()->outerMomentum().y(), 
								      muon->outerTrack()->outerMomentum().z());
      }
    }//if(!isAOD_)

    if(!isAOD_){
      //FIXME: Seems needed  top and bottom referene point, but not sure how to do that !!!
      reco::TrackRef MuTrkref;
      Muon_InnerTrack_isNonnull.push_back(false);
      Muon_OuterTrack_isNonnull.push_back(false);
      if((muon->isGlobalMuon()) || (muon->isTrackerMuon()) ){
	MuTrkref = muon->innerTrack();
	Muon_InnerTrack_isNonnull[Muon_n] = (muon->innerTrack().isNonnull());
      }
      else{   
	MuTrkref = muon->outerTrack();
	Muon_OuterTrack_isNonnull[Muon_n] = (muon->outerTrack().isNonnull());
      }
      //For StandAlone
      new( (*Muon_OuterPoint_position)[Muon_n]) TVector3(0.,0.,0.);
      //For Global,Tracker
      new( (*Muon_InnerPoint_position)[Muon_n]) TVector3(0.,0.,0.);
      
      if(muon->outerTrack().isNonnull()){
	//stand-alone
	new( (*Muon_OuterPoint_position)[Muon_n]) TVector3(MuTrkref->referencePoint().x(),
							   MuTrkref->referencePoint().y(),
							   MuTrkref->referencePoint().z());
      }
      if(muon->innerTrack().isNonnull()){
	//global,tracker
	new( (*Muon_InnerPoint_position)[Muon_n]) TVector3(MuTrkref->referencePoint().x(),
							   MuTrkref->referencePoint().y(),
							   MuTrkref->referencePoint().z());
      }
    }
      
    Muon_n++;
}  
  if(debug_)    std::cout<<"got muon info"<<std::endl;
}

void muonInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&Muon_n  ,"Muon_n");
  AddBranch(&Muon_chargedIso, "Muon_chargedIso");
  AddBranch(&Muon_photonIso,"Muon_photonIso");
  AddBranch(&Muon_neutralIso, "Muon_neutralIso");
  AddBranch(&Muon_particleIso , "Muon_particleIso");
  AddBranch(&Muon_dbeta, "Muon_dbeta");
  AddBranch(&Muon_trackerLayersWithMeasurement , "Muon_trackerLayersWithMeasurement");
  AddBranch(&Muon_PFMuID , "Muon_PFMuID");

  
  AddBranch(&Muon_Significance3D, "Muon_Significance3D");
  AddBranch(&Muon_Value3D , "Muon_Value3D");
  AddBranch(&Muon_Error3D , "Muon_Error3D");

  AddBranch(&Muon_Significance2D, "Muon_Significance2D");
  AddBranch(&Muon_Value2D , "Muon_Value2D");
  AddBranch(&Muon_Error2D , "Muon_Error2D");

  
  
  AddBranch(&Muon_4Momentum ,"Muon_4Momentum");
  AddBranch(&Muon_Vposition ,"Muon_Vposition");
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree 1"<<std::endl;
  AddBranch(&Muon_OuterTrack_InnerPoint_position,"Muon_OuterTrack_InnerPoint_position");
  AddBranch(&Muon_OuterTrack_InnerPoint_momentum,"Muon_OuterTrack_InnerPoint_momentum");
  AddBranch(&Muon_OuterTrack_OuterPoint_position,"Muon_OuterTrack_OuterPoint_position");
  AddBranch(&Muon_OuterTrack_OuterPoint_momentum,"Muon_OuterTrack_OuterPoint_momentum");
  AddBranch(&Muon_InnerTrack_InnerPoint_position,"Muon_Innertrack_InnerPoint_position");
  AddBranch(&Muon_InnerTrack_InnerPoint_momentum,"Muon_Innertrack_InnerPoint_momentum");
  AddBranch(&Muon_InnerTrack_OuterPoint_position,"Muon_Innertrack_OuterPoint_position");
  AddBranch(&Muon_InnerTrack_OuterPoint_momentum,"Muon_Innertrack_OuterPoint_momentum");
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree 2"<<std::endl;
  AddBranch(&Muon_charge              ,"Muon_charge");
  AddBranch(&Muon_isGlobalMuon        ,"Muon_isGlobalMuon");
  AddBranch(&Muon_isTrackerMuon       ,"Muon_isTrackerMuon");
  AddBranch(&Muon_isStandAloneMuon    ,"Muon_isStandAloneMuon");
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree 3"<<std::endl;
  AddBranch(&Muon_InnerTrack_isNonnull,"Muon_InnerTrack_isNonnull");
  AddBranch(&Muon_OuterTrack_isNonnull,"Muon_OuterTrack_isNonnull");

  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree 4"<<std::endl;
  AddBranch(&Muon_trackIso            ,"Muon_trackIso");
  AddBranch(&Muon_ecalIso             ,"Muon_ecalIso");
  AddBranch(&Muon_hcalIso             ,"Muon_hcalIso");
  AddBranch(&Muon_NewVz            ,"Muon_NewVz");
  AddBranch(&Muon_normChi2            ,"Muon_normChi2");
  AddBranch(&Muon_validHits           ,"Muon_validHits");
  AddBranch(&Muon_tkHits              ,"Muon_tkHits");
  AddBranch(&Muon_pixHits             ,"Muon_pixHits");
  AddBranch(&Muon_numberOfMatches     ,"Muon_numberOfMatches");
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree 5"<<std::endl;
  if(!isAOD_){
    AddBranch(&Muon_OuterPoint_position,"muon_OuterPoint_position");
    AddBranch(&Muon_InnerPoint_position,"muon_InnerPoint_position");
    }

  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void muonInfo::Clear(){
  if(debug_)std::cout<<"clearing Muon info"<<std::endl;
  Muon_n =0;
  Muon_PFMuID.clear();
  Muon_chargedIso.clear();
  Muon_photonIso.clear();
  Muon_neutralIso.clear();
  Muon_particleIso.clear();
  Muon_dbeta.clear();
  Muon_trackerLayersWithMeasurement.clear();
  Muon_4Momentum->Clear();
  Muon_Vposition->Clear();
  Muon_OuterTrack_InnerPoint_position->Clear();
  Muon_OuterTrack_InnerPoint_momentum->Clear();
  Muon_OuterTrack_OuterPoint_position->Clear();
  Muon_OuterTrack_OuterPoint_momentum->Clear();
  Muon_InnerTrack_InnerPoint_position->Clear();
  Muon_InnerTrack_InnerPoint_momentum->Clear();
  Muon_InnerTrack_OuterPoint_position->Clear();
  Muon_InnerTrack_OuterPoint_momentum->Clear();
  Muon_charge.clear(); 
  Muon_isGlobalMuon.clear(); 
  Muon_isTrackerMuon.clear();   
  Muon_isStandAloneMuon.clear();   
  Muon_InnerTrack_isNonnull.clear();
  Muon_OuterTrack_isNonnull.clear();
  Muon_trackIso.clear();
  Muon_ecalIso.clear();             
  Muon_hcalIso.clear(); 
  Muon_NewVz.clear();   
  Muon_normChi2.clear();   
  Muon_validHits.clear();           
  Muon_tkHits.clear();              
  Muon_pixHits.clear();             
  Muon_numberOfMatches.clear(); 
  
  Muon_Significance3D.clear();
  Muon_Value3D.clear();
  Muon_Error3D.clear();


  Muon_Significance2D.clear();
  Muon_Value2D.clear();
  Muon_Error2D.clear();

  if(!isAOD_)
    {
      Muon_OuterPoint_position->Clear();
      Muon_InnerPoint_position->Clear();
    }

  if(debug_) std::cout<<"cleared"<<std::endl;
}




