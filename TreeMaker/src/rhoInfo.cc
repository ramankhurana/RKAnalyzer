#include "RKAnalyzer/TreeMaker/interface/rhoInfo.h"

rhoInfo::rhoInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in rho constructor"<<std::endl;
  rhoLabel_       = iConfig.getUntrackedParameter<edm::InputTag> ("rhoLabel_");
  sigmaLabel_     = iConfig.getUntrackedParameter<edm::InputTag> ("sigmaLabel_");
  rho44Label_     = iConfig.getUntrackedParameter<edm::InputTag> ("rho44Label_");
  sigma44Label_     = iConfig.getUntrackedParameter<edm::InputTag> ("sigma44Label_");
  if(debug) std::cout<<"in rho constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

rhoInfo::~rhoInfo(){
  delete tree_;
}

void rhoInfo::Fill(const edm::Event& iEvent){
  if(debug_)    std::cout<<"getting rho,sigma info"<<std::endl;
  // Rho correction with 2.5
  edm::Handle<double> rhoHandle;
  iEvent.getByLabel(rhoLabel_, rhoHandle);
  rho=0.;
  std::cout<<" rhoHandle.isValid() = "<<rhoHandle.isValid()<<std::endl;
  if(rhoHandle.isValid()) {
    rho= *(rhoHandle.product());
    std::cout<<"--------------------------------------- rho = "<<rho<<std::endl;
  }


  edm::Handle<double> sigmaHandle;
  iEvent.getByLabel(sigmaLabel_, sigmaHandle);
  sigma =0.;
  if(sigmaHandle.isValid()) {
    sigma = *(sigmaHandle.product());
  }

  edm::Handle<double> rho44Handle;
  iEvent.getByLabel(rho44Label_, rhoHandle);
  rho44=0.;
  if(rho44Handle.isValid()) {
    rho44= *(rho44Handle.product());
  }


  edm::Handle<double> sigma44Handle;
  iEvent.getByLabel(sigma44Label_, sigma44Handle);
  sigma44 =0.;
  if(sigma44Handle.isValid()) {
    sigma44 = *(sigma44Handle.product());
  }
  if(debug_)    std::cout<<"got rho,sigma info"<<std::endl;
}

void rhoInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&rho  ,"rho");
  AddBranch(&rho44  ,"rho44");
  AddBranch(&sigma  ,"sigma");
  AddBranch(&sigma44  ,"sigma44");
  if(debug_)    std::cout<<"set branches"<<std::endl;
}

