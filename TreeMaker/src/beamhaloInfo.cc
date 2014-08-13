#include "RKAnalyzer/TreeMaker/interface/beamhaloInfo.h"

beamhaloInfo::beamhaloInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in beamhaloInfo constructor"<<std::endl;
  beamhaloLabel_             = iConfig.getUntrackedParameter<edm::InputTag> ("beamhaloLabel_");
  if(debug) std::cout<<"in beamhalo constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

beamhaloInfo::~beamhaloInfo(){
  delete tree_;
}

void beamhaloInfo::Fill(const edm::Event& iEvent){

  if(debug_)    std::cout<<"getting beam halo info"<<std::endl;

  edm::Handle<reco::BeamHaloSummary> TheBeamHaloSummary ;
  iEvent.getByLabel(beamhaloLabel_, TheBeamHaloSummary) ;

  if(not iEvent.getByLabel(beamhaloLabel_, TheBeamHaloSummary)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<beamhaloLabel_<<std::endl; 
    exit(0);
  }  

  isBeamHaloIDTightPass     = false;
  isBeamHaloIDLoosePass     = false;

  isBeamHaloEcalLoosePass   = false;
  isBeamHaloHcalLoosePass   = false;
  isBeamHaloCSCLoosePass    = false;
  isBeamHaloGlobalLoosePass = false;

  isBeamHaloEcalTightPass   = false;
  isBeamHaloHcalTightPass   = false;
  isBeamHaloCSCTightPass    = false;
  isBeamHaloGlobalTightPass = false;


  isSmellsLikeHalo_Tag      = false;
  isLooseHalo_Tag           = false;
  isTightHalo_Tag           = false;
  isExtremeTightHalo_Tag    = false;

  if(TheBeamHaloSummary.isValid()) {
    const reco::BeamHaloSummary TheSummary = (*TheBeamHaloSummary.product() );

    if(TheSummary.EcalLooseHaloId())isBeamHaloEcalLoosePass   = true;
    if(TheSummary.HcalLooseHaloId())isBeamHaloHcalLoosePass   = true;

    if(TheSummary.EcalTightHaloId())isBeamHaloEcalTightPass   = true;
    if(TheSummary.HcalTightHaloId())isBeamHaloHcalTightPass   = true;

    if(TheSummary.CSCLooseHaloId())isBeamHaloCSCLoosePass    = true;
    if(TheSummary.CSCTightHaloId())isBeamHaloCSCTightPass    = true;

    if(TheSummary.GlobalLooseHaloId())isBeamHaloGlobalLoosePass = true;
    if(TheSummary.GlobalTightHaloId())isBeamHaloGlobalTightPass = true;

    if(TheSummary.EventSmellsLikeHalo())isSmellsLikeHalo_Tag = true;
    if(TheSummary.LooseId())isLooseHalo_Tag = true;
    if(TheSummary.TightId())isTightHalo_Tag = true;
    if(TheSummary.ExtremeTightId())isExtremeTightHalo_Tag = true;


    if( TheSummary.EcalLooseHaloId()  && TheSummary.HcalLooseHaloId() &&
	TheSummary.CSCLooseHaloId()   && TheSummary.GlobalLooseHaloId() )
      isBeamHaloIDLoosePass = true;
    if( TheSummary.EcalLooseHaloId()  && TheSummary.HcalLooseHaloId() &&
	TheSummary.CSCLooseHaloId()   && TheSummary.GlobalLooseHaloId() )
      isBeamHaloIDLoosePass = true;

    if( TheSummary.EcalTightHaloId()  && TheSummary.HcalTightHaloId() &&
	TheSummary.CSCTightHaloId()   && TheSummary.GlobalTightHaloId() )
      isBeamHaloIDTightPass = true;

  }//not empty
  if(debug_)    std::cout<<"got beam halo info"<<std::endl;
}

void beamhaloInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&isBeamHaloIDTightPass     ,"isBeamHaloIDTightPass");
  AddBranch(&isBeamHaloIDLoosePass     ,"isBeamHaloIDLoosePass");

  AddBranch(&isBeamHaloEcalLoosePass   ,"isBeamHaloEcalLoosePass");
  AddBranch(&isBeamHaloHcalLoosePass   ,"isBeamHaloHcalLoosePass");
  AddBranch(&isBeamHaloCSCLoosePass    ,"isBeamHaloCSCLoosePass");    
  AddBranch(&isBeamHaloGlobalLoosePass ,"isBeamHaloGlobalLoosePass");
  AddBranch(&isBeamHaloEcalTightPass   ,"isBeamHaloEcalTightPass");
  AddBranch(&isBeamHaloHcalTightPass   ,"isBeamHaloHcalTightPass");
  AddBranch(&isBeamHaloCSCTightPass    ,"isBeamHaloCSCTightPass");
  AddBranch(&isBeamHaloGlobalTightPass ,"isBeamHaloGlobalTightPass");

  AddBranch(&isSmellsLikeHalo_Tag      ,"isSmellsLikeHalo_Tag");
  AddBranch(&isLooseHalo_Tag           ,"isLooseHalo_Tag");   
  AddBranch(&isTightHalo_Tag           ,"isTightHalo_Tag");        
  AddBranch(&isExtremeTightHalo_Tag    ,"isExtremeTightHalo_Tag");    
    
  if(debug_)    std::cout<<"set branches"<<std::endl;
}


