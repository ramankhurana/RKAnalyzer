#include "RKAnalyzer/TreeMaker/interface/genjetInfo.h"

genjetInfo::genjetInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in genjet constructor"<<std::endl;
  genjetLabel_     = iConfig.getUntrackedParameter<edm::InputTag> ("genjetLabel_");
  GENJet_4Momentum = new TClonesArray("TLorentzVector");
  if(debug) std::cout<<"in genjet constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

genjetInfo::~genjetInfo(){
  delete tree_;
}

void genjetInfo::Fill(const edm::Event& iEvent){
  Clear();
  if(debug_)    std::cout<<"getting genjet info"<<std::endl;
 
  edm::Handle<reco::GenJetCollection>  genjetHandle;
  iEvent.getByLabel(genjetLabel_,genjetHandle);
  reco::GenJetCollection::const_iterator genjet;
  if(not iEvent.getByLabel(genjetLabel_,genjetHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<genjetLabel_<<std::endl; 
    exit(0);
  }    

  for( genjet = genjetHandle->begin(); genjet!=genjetHandle->end(); ++genjet){
 
    TLorentzVector p4(genjet->px(),genjet->py(),genjet->pz(),genjet->energy());
    new( (*GENJet_4Momentum)[GENJet_n]) TLorentzVector(p4);
    
    GENJet_n++;
  }//end of for loop
  if(debug_)    std::cout<<"got genjet info"<<std::endl;
}

void genjetInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&GENJet_n  ,"GENJet_n");
  AddBranch(&GENJet_4Momentum ,"GENJet_4Momentum");
 
  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void genjetInfo::Clear(){
  if(debug_)std::cout<<"clearing GENjet info"<<std::endl;
  GENJet_n = 0;  
  GENJet_4Momentum->Clear();
 
  if(debug_) std::cout<<"cleared"<<std::endl;
}
