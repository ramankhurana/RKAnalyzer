#include "RKAnalyzer/TreeMaker/interface/trackInfo.h"

trackInfo::trackInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in track constructor"<<std::endl;
  trackLabel_      = iConfig.getUntrackedParameter<edm::InputTag> ("trackLabel_");
  Track_Vposition  = new TClonesArray("TVector3");
  Track_3Momentum  = new TClonesArray("TVector3");
  if(debug) std::cout<<"in track constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

trackInfo::~trackInfo(){
  delete tree_;
}

void trackInfo::Fill(const edm::Event& iEvent){
  Clear();
  if(debug_)    std::cout<<"getting track info"<<std::endl;
  edm::Handle<reco::TrackCollection> tracks;
  iEvent.getByLabel(trackLabel_,tracks);
  reco::TrackCollection tkColl(*(tracks.product()));
  
  if(not iEvent.getByLabel(trackLabel_,tracks)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<trackLabel_<<std::endl; 
    exit(0);
  }  

  std::sort(tkColl.begin(),tkColl.end(),PtGreater());
  reco::TrackCollection::const_iterator track;
  for(track = tkColl.begin(); track != tkColl.end(); track++) {
    if(track->pt()>1.){
      TVector3 p3(track->px(),track->py(),track->pz());
      new( (*Track_3Momentum)[Track_n]) TVector3(p3);
      TVector3 v3(track->vx(),track->vy(),track->vz());
      new( (*Track_Vposition)[Track_n]) TVector3(v3);
      Track_n++;
    }
  }

  int numhighpurity=0;
  reco::TrackBase::TrackQuality _trackQuality = reco::TrackBase::qualityByName("highPurity");
  
  if(tkColl.size()>10){
    reco::TrackCollection::const_iterator itk   = tkColl.begin();
    reco::TrackCollection::const_iterator itk_e = tkColl.end();
    for(;itk!=itk_e;++itk){
      if(itk->quality(_trackQuality)) numhighpurity++;
    }
    float Scraping_fractionOfGoodTracks = (float)numhighpurity/(float)tkColl.size();
    if(Scraping_fractionOfGoodTracks>0.25) Scraping_isScrapingEvent = false;
  }else{
    Scraping_isScrapingEvent = false;
  }
  if(debug_)    std::cout<<"got track info"<<std::endl;
}

void trackInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&Track_n,         "Track_n");
  AddBranch(&Track_3Momentum, "Track_4Momentum");
  AddBranch(&Track_Vposition, "Track_Vposition");
  AddBranch(&Scraping_isScrapingEvent, "Scraping_isScrapingEvent");
  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void trackInfo::Clear(){
  if(debug_)std::cout<<"clearing track info"<<std::endl;
  Track_n =0;
  Track_Vposition->Clear();
  Track_3Momentum->Clear();
  Scraping_isScrapingEvent = true;
  if(debug_) std::cout<<"cleared"<<std::endl;
}
