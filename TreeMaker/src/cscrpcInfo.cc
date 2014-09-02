#include "RKAnalyzer/TreeMaker/interface/cscrpcInfo.h"

cscrpcInfo::cscrpcInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in cscrpc constructor"<<std::endl;
  rpcLabel_     = iConfig.getUntrackedParameter<edm::InputTag> ("rpcLabel_");
  cscLabel_     = iConfig.getUntrackedParameter<edm::InputTag> ("cscLabel_");
  CSCseg_position  = new TClonesArray("TVector3");
  CSCseg_direction = new TClonesArray("TVector3");
  RPChit_position  = new TClonesArray("TVector3");
  if(debug) std::cout<<"in cscrpc constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

cscrpcInfo::~cscrpcInfo(){
  delete tree_;
}

void cscrpcInfo::Fill(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  Clear();
  if(debug_)    std::cout<<"getting csc,rpc info"<<std::endl;
  
  edm::ESHandle<CSCGeometry> cscGeom;
  iSetup.get<MuonGeometryRecord>().get(cscGeom);
  // get CSC segment collection
  edm::Handle<CSCSegmentCollection> cscSegments;
  iEvent.getByLabel(cscLabel_, cscSegments);
  if(not iEvent.getByLabel(cscLabel_,cscSegments)){
  std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<cscLabel_<<std::endl; 
  exit(0);
  }      


  //get geometry of tracking stuff
  edm::ESHandle<RPCGeometry> geometry;
  iSetup.get<MuonGeometryRecord>().get(geometry);
  //get RPC hit collection
  edm::Handle<RPCRecHitCollection> rpcHits;
  iEvent.getByLabel(rpcLabel_,rpcHits);
  if(not iEvent.getByLabel(rpcLabel_,rpcHits)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<rpcLabel_<<std::endl; 
    exit(0);
  }      
             
  CSCSegmentCollection::const_iterator it;
  for(it=cscSegments->begin(); CSCseg_n<10000 && it != cscSegments->end(); it++){
    CSCDetId id  = (CSCDetId)it->cscDetId();
    LocalVector segDir = it->localDirection();
    LocalPoint localPos = it->localPosition();
    // global transformation
    const CSCChamber* cscchamber = cscGeom->chamber(id);
    if (cscchamber) {
         
      GlobalPoint globalPosition = cscchamber->toGlobal(localPos);
      GlobalVector globalDirection = cscchamber->toGlobal(segDir);
      int nhits = 0;
      float timeSum = 0;
      // Get the CSC recHits that contribute to this segment.
      std::vector<CSCRecHit2D> theseRecHits = it->specificRecHits();
      for( std::vector<CSCRecHit2D>::const_iterator iRH =theseRecHits.begin(); iRH != theseRecHits.end(); iRH++){
	if( !(iRH->isValid()) ) continue;  // only interested in valid hits
	nhits++;
	float rhTime = iRH->tpeak();
	timeSum += rhTime;
      }//end rechit loop

      float segmentTime = -999;
      if (nhits>0)
	segmentTime = timeSum/nhits;
      
      CSCseg_time.push_back(segmentTime);
      new( (*CSCseg_position)[CSCseg_n])  TVector3(globalPosition.x(),
						   globalPosition.y(),
						   globalPosition.z());
      new( (*CSCseg_direction)[CSCseg_n]) TVector3(globalDirection.x(),
						   globalDirection.y(),
						   globalDirection.z());
      CSCseg_n++;
    }
  }

  for(RPCRecHitCollection::const_iterator RecHitsIt = rpcHits->begin(); RPChit_n<10000 && RecHitsIt!=rpcHits->end(); RecHitsIt++){
    const GeomDet *det = geometry->idToDet(RecHitsIt->geographicalId());
    GlobalPoint pos = det->toGlobal(RecHitsIt->localPosition());
    new( (*RPChit_position)[RPChit_n]) TVector3(pos.x(),
						pos.y(),
						pos.z());
    //BunchX is in units of 25ns. BunchX=0 is the expected value for collision muons.
    RPChit_BunchX.push_back((int)RecHitsIt->BunchX());
    RPChit_n++;
  }//loop over rpc hits

}

void cscrpcInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&RPChit_n  ,"RPChit_n");
  AddBranch(&CSCseg_n  ,"CSCseg_n");
  AddBranch(&CSCseg_time,"CSCseg_time");
  AddBranch(&CSCseg_position ,"CSCseg_position");
  AddBranch(&CSCseg_direction,"CSCseg_direction");
  AddBranch(&RPChit_position ,"RPChit_position");
  AddBranch(&RPChit_BunchX ,"RPChit_BunchX");

}
void cscrpcInfo::Clear(){
  if(debug_)std::cout<<"clearing cscrpc info"<<std::endl;
  CSCseg_n = 0;
  RPChit_n =0;
  CSCseg_position->Clear();
  CSCseg_direction->Clear();
  RPChit_position->Clear();
  RPChit_BunchX.clear();
  CSCseg_time.clear();

  if(debug_) std::cout<<"cleared"<<std::endl;
}




