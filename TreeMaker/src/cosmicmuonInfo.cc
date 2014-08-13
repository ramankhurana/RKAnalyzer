#include "RKAnalyzer/TreeMaker/interface/cosmicmuonInfo.h"

cosmicmuonInfo::cosmicmuonInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in cosmicmuon constructor"<<std::endl;
  cosmicmuonLabel_     = iConfig.getUntrackedParameter<edm::InputTag> ("cosmicmuonLabel_");
  isAOD_               = iConfig.getUntrackedParameter<bool> ("isAOD_");
  Cosmicmuon_4Momentum = new TClonesArray("TLorentzVector");
  Cosmicmuon_Vposition = new TClonesArray("TVector3");
  Cosmicmuon_OuterTrack_InnerPoint_position = new TClonesArray("TVector3");
  Cosmicmuon_OuterTrack_InnerPoint_momentum = new TClonesArray("TVector3");
  Cosmicmuon_OuterTrack_OuterPoint_position = new TClonesArray("TVector3");
  Cosmicmuon_OuterTrack_OuterPoint_momentum = new TClonesArray("TVector3");
  Cosmicmuon_InnerTrack_InnerPoint_position = new TClonesArray("TVector3");
  Cosmicmuon_InnerTrack_InnerPoint_momentum = new TClonesArray("TVector3");
  Cosmicmuon_InnerTrack_OuterPoint_position = new TClonesArray("TVector3");
  Cosmicmuon_InnerTrack_OuterPoint_momentum = new TClonesArray("TVector3");
  if(debug) std::cout<<"in cosmicmuon constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

cosmicmuonInfo::~cosmicmuonInfo(){
  delete tree_;
}

void cosmicmuonInfo::Fill(const edm::Event& iEvent){
  Clear();
  if(debug_)    std::cout<<"getting cosmicmuon info"<<std::endl;
  edm::Handle<reco::MuonCollection> cosmicmuonHandle;
  iEvent.getByLabel(cosmicmuonLabel_,cosmicmuonHandle);
  
  reco::MuonCollection cosmicmuons(*(cosmicmuonHandle.product()));
  std::sort(cosmicmuons.begin(),cosmicmuons.end(),PtGreater());
  if(not iEvent.getByLabel(cosmicmuonLabel_,cosmicmuonHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<cosmicmuonLabel_<<std::endl; 
    exit(0);
  }  
  
  reco::MuonCollection::const_iterator cosmicmuon;
  for(cosmicmuon = cosmicmuons.begin(); cosmicmuon!=cosmicmuons.end(); cosmicmuon++){
    TLorentzVector p4(cosmicmuon->px(),cosmicmuon->py(),cosmicmuon->pz(),cosmicmuon->energy());
    new( (*Cosmicmuon_4Momentum)[Cosmicmuon_n]) TLorentzVector(p4);
    TVector3 v3(cosmicmuon->vx(),cosmicmuon->vy(),cosmicmuon->vz());
    new( (*Cosmicmuon_Vposition)[Cosmicmuon_n]) TVector3(v3);
    Cosmicmuon_charge.push_back((int)cosmicmuon->charge());
    if(cosmicmuon->globalTrack().isNonnull() ){
      Cosmicmuon_validHits.push_back((int)cosmicmuon->globalTrack()->hitPattern().numberOfValidMuonHits());
    }
    else {
      Cosmicmuon_validHits.push_back(-99);
    }
    Cosmicmuon_numberOfMatches.push_back((int)cosmicmuon->numberOfMatches());
    if(cosmicmuon->track().isNonnull() ){
      Cosmicmuon_tkHits.push_back((int)cosmicmuon->track()->numberOfValidHits());
      Cosmicmuon_pixHits.push_back((int)cosmicmuon->track()->hitPattern().numberOfValidPixelHits());
    }
    else {
      Cosmicmuon_tkHits.push_back(-99);
      Cosmicmuon_pixHits.push_back(-99);
    }

    Cosmicmuon_isGlobalCosmicmuon.push_back((bool)cosmicmuon->isGlobalMuon());
    Cosmicmuon_isTrackerCosmicmuon.push_back((bool)cosmicmuon->isTrackerMuon());
    Cosmicmuon_isStandAloneCosmicmuon.push_back((bool)cosmicmuon->isStandAloneMuon());
    
    new( (*Cosmicmuon_OuterTrack_InnerPoint_position)[Cosmicmuon_n]) TVector3(0,0,0);
    new( (*Cosmicmuon_OuterTrack_InnerPoint_momentum)[Cosmicmuon_n]) TVector3(0,0,0);
    new( (*Cosmicmuon_OuterTrack_OuterPoint_position)[Cosmicmuon_n]) TVector3(0,0,0);
    new( (*Cosmicmuon_OuterTrack_OuterPoint_momentum)[Cosmicmuon_n]) TVector3(0,0,0);
    new( (*Cosmicmuon_InnerTrack_InnerPoint_position)[Cosmicmuon_n]) TVector3(0,0,0);
    new( (*Cosmicmuon_InnerTrack_InnerPoint_momentum)[Cosmicmuon_n]) TVector3(0,0,0);
    new( (*Cosmicmuon_InnerTrack_OuterPoint_position)[Cosmicmuon_n]) TVector3(0,0,0);
    new( (*Cosmicmuon_InnerTrack_OuterPoint_momentum)[Cosmicmuon_n]) TVector3(0,0,0);

    if(!isAOD_){
      Cosmicmuon_InnerTrack_isNonnull.push_back(cosmicmuon->innerTrack().isNonnull());
      Cosmicmuon_OuterTrack_isNonnull.push_back(cosmicmuon->outerTrack().isNonnull());
      if(cosmicmuon->innerTrack().isNonnull()){
	new( (*Cosmicmuon_InnerTrack_InnerPoint_position)[Cosmicmuon_n]) TVector3(cosmicmuon->innerTrack()->innerPosition().x(),
								      cosmicmuon->innerTrack()->innerPosition().y(),
								      cosmicmuon->innerTrack()->innerPosition().z());
	new( (*Cosmicmuon_InnerTrack_InnerPoint_momentum)[Cosmicmuon_n]) TVector3(cosmicmuon->innerTrack()->innerMomentum().x(), 
								      cosmicmuon->innerTrack()->innerMomentum().y(), 
								      cosmicmuon->innerTrack()->innerMomentum().z());
	new( (*Cosmicmuon_InnerTrack_OuterPoint_position)[Cosmicmuon_n]) TVector3(cosmicmuon->innerTrack()->outerPosition().x(), 
								      cosmicmuon->innerTrack()->outerPosition().y(), 
								      cosmicmuon->innerTrack()->outerPosition().z());
	new( (*Cosmicmuon_InnerTrack_OuterPoint_momentum)[Cosmicmuon_n]) TVector3(cosmicmuon->innerTrack()->outerMomentum().x(),
								      cosmicmuon->innerTrack()->outerMomentum().y(), 
								      cosmicmuon->innerTrack()->outerMomentum().z());
      }
      if(cosmicmuon->outerTrack().isNonnull()){
	new( (*Cosmicmuon_OuterTrack_InnerPoint_position)[Cosmicmuon_n]) TVector3(cosmicmuon->outerTrack()->innerPosition().x(),
								      cosmicmuon->outerTrack()->innerPosition().y(),
								      cosmicmuon->outerTrack()->innerPosition().z());
	new( (*Cosmicmuon_OuterTrack_InnerPoint_momentum)[Cosmicmuon_n]) TVector3(cosmicmuon->outerTrack()->innerMomentum().x(), 
								      cosmicmuon->outerTrack()->innerMomentum().y(), 
								      cosmicmuon->outerTrack()->innerMomentum().z());
	new( (*Cosmicmuon_OuterTrack_OuterPoint_position)[Cosmicmuon_n]) TVector3(cosmicmuon->outerTrack()->outerPosition().x(), 
								      cosmicmuon->outerTrack()->outerPosition().y(), 
								      cosmicmuon->outerTrack()->outerPosition().z());
	new( (*Cosmicmuon_OuterTrack_OuterPoint_momentum)[Cosmicmuon_n]) TVector3(cosmicmuon->outerTrack()->outerMomentum().x(),
								      cosmicmuon->outerTrack()->outerMomentum().y(), 
								      cosmicmuon->outerTrack()->outerMomentum().z());
      }
    }//if(!isAOD_)

    if(isAOD_){
      //FIXME: Seems needed  top and bottom referene point, but not sure how to do that !!!
      reco::TrackRef MuTrkref;
      Cosmicmuon_InnerTrack_isNonnull.push_back(false);
      Cosmicmuon_OuterTrack_isNonnull.push_back(false);
      if((cosmicmuon->isGlobalMuon()) || (cosmicmuon->isTrackerMuon()) ){
	MuTrkref = cosmicmuon->innerTrack();
	Cosmicmuon_InnerTrack_isNonnull[Cosmicmuon_n] = cosmicmuon->innerTrack().isNonnull();
      }
      else{   
	MuTrkref = cosmicmuon->outerTrack();
	Cosmicmuon_OuterTrack_isNonnull[Cosmicmuon_n] = cosmicmuon->outerTrack().isNonnull();
      }
      //For StandAlone
      new( (*Cosmicmuon_OuterPoint_position)[Cosmicmuon_n]) TVector3(0.,0.,0.);
      //For Global,Tracker
      new( (*Cosmicmuon_InnerPoint_position)[Cosmicmuon_n]) TVector3(0.,0.,0.);
      
      if(cosmicmuon->outerTrack().isNonnull()){//stand-alone
	new( (*Cosmicmuon_OuterPoint_position)[Cosmicmuon_n]) TVector3(MuTrkref->referencePoint().x(),
								       MuTrkref->referencePoint().y(),
								       MuTrkref->referencePoint().z());
      }
      if(cosmicmuon->innerTrack().isNonnull()){//global,tracker
	new( (*Cosmicmuon_InnerPoint_position)[Cosmicmuon_n]) TVector3(MuTrkref->referencePoint().x(),
								       MuTrkref->referencePoint().y(),
								       MuTrkref->referencePoint().z());
      }
    }


    Cosmicmuon_n++;
  }  
  if(debug_)    std::cout<<"got cosmicmuon info"<<std::endl;
}

void cosmicmuonInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&Cosmicmuon_n  ,"Cosmicmuon_n");
  AddBranch(&Cosmicmuon_4Momentum ,"Cosmicmuon_4Momentum");
  AddBranch(&Cosmicmuon_Vposition ,"Cosmicmuon_Vposition");
  AddBranch(&Cosmicmuon_OuterTrack_InnerPoint_position,"Cosmicmuon_OuterTrack_InnerPoint_position");
  AddBranch(&Cosmicmuon_OuterTrack_InnerPoint_momentum,"Cosmicmuon_OuterTrack_InnerPoint_momentum");
  AddBranch(&Cosmicmuon_OuterTrack_OuterPoint_position,"Cosmicmuon_OuterTrack_OuterPoint_position");
  AddBranch(&Cosmicmuon_OuterTrack_OuterPoint_momentum,"Cosmicmuon_OuterTrack_OuterPoint_momentum");
  AddBranch(&Cosmicmuon_InnerTrack_InnerPoint_position,"Cosmicmuon_Innertrack_InnerPoint_position");
  AddBranch(&Cosmicmuon_InnerTrack_InnerPoint_momentum,"Cosmicmuon_Innertrack_InnerPoint_momentum");
  AddBranch(&Cosmicmuon_InnerTrack_OuterPoint_position,"Cosmicmuon_Innertrack_OuterPoint_position");
  AddBranch(&Cosmicmuon_InnerTrack_OuterPoint_momentum,"Cosmicmuon_Innertrack_OuterPoint_momentum");

  AddBranch(&Cosmicmuon_charge                    ,"Cosmicmuon_charge");
  AddBranch(&Cosmicmuon_isGlobalCosmicmuon        ,"Cosmicmuon_isGlobalCosmicmuon");
  AddBranch(&Cosmicmuon_isTrackerCosmicmuon       ,"Cosmicmuon_isTrackerCosmicmuon");
  AddBranch(&Cosmicmuon_isStandAloneCosmicmuon    ,"Cosmicmuon_isStandAloneCosmicmuon");
  AddBranch(&Cosmicmuon_InnerTrack_isNonnull,"Cosmicmuon_InnerTrack_isNonnull");
  AddBranch(&Cosmicmuon_OuterTrack_isNonnull,"Cosmicmuon_OuterTrack_isNonnull");
  AddBranch(&Cosmicmuon_validHits           ,"Cosmicmuon_validHits");
  AddBranch(&Cosmicmuon_tkHits              ,"Cosmicmuon_tkHits");
  AddBranch(&Cosmicmuon_pixHits             ,"Cosmicmuon_pixHits");
  AddBranch(&Cosmicmuon_numberOfMatches     ,"Cosmicmuon_numberOfMatches");
  
  if(isAOD_){
    AddBranch(&Cosmicmuon_OuterPoint_position,"cosmicmuon_OuterPoint_position");
    //for Global,Tracker cosmicmuon 
    AddBranch(&Cosmicmuon_InnerPoint_position,"cosmicmuon_InnerPoint_position");
    }
  
  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void cosmicmuonInfo::Clear(){
  if(debug_)std::cout<<"clearing Cosmicmuon info"<<std::endl;
  Cosmicmuon_n =0;
  Cosmicmuon_4Momentum->Clear();
  Cosmicmuon_Vposition->Clear();
  Cosmicmuon_OuterTrack_InnerPoint_position->Clear();
  Cosmicmuon_OuterTrack_InnerPoint_momentum->Clear();
  Cosmicmuon_OuterTrack_OuterPoint_position->Clear();
  Cosmicmuon_OuterTrack_OuterPoint_momentum->Clear();
  Cosmicmuon_InnerTrack_InnerPoint_position->Clear();
  Cosmicmuon_InnerTrack_InnerPoint_momentum->Clear();
  Cosmicmuon_InnerTrack_OuterPoint_position->Clear();
  Cosmicmuon_InnerTrack_OuterPoint_momentum->Clear();
  Cosmicmuon_charge.clear(); 
  Cosmicmuon_isGlobalCosmicmuon.clear(); 
  Cosmicmuon_isTrackerCosmicmuon.clear();   
  Cosmicmuon_isStandAloneCosmicmuon.clear();   
  Cosmicmuon_InnerTrack_isNonnull.clear();
  Cosmicmuon_OuterTrack_isNonnull.clear();
  Cosmicmuon_validHits.clear();           
  Cosmicmuon_tkHits.clear();              
  Cosmicmuon_pixHits.clear();             
  Cosmicmuon_numberOfMatches.clear(); 
  if(isAOD_)
    {
      Cosmicmuon_OuterPoint_position->Clear();
      Cosmicmuon_InnerPoint_position->Clear();
    }
  if(debug_) std::cout<<"cleared"<<std::endl;
}




