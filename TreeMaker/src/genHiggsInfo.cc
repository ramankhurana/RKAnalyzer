#include "RKAnalyzer/TreeMaker/interface/genHiggsInfo.h"

genHiggsInfo::genHiggsInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in tau constructor"<<std::endl;
  tauLabel_       = iConfig.getUntrackedParameter<edm::InputTag> ("tauLabel_");
  H_Gen_4Momentum           = new TClonesArray("TLorentzVector");
    if(debug) std::cout<<"in genHiggsInfo constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

genHiggsInfo::~genHiggsInfo(){
  delete tree_;
}

void genHiggsInfo::Fill(const edm::Event& iEvent){
  Clear();
  if(debug_)    std::cout<<"getting Higgs Information"<<std::endl;
  if(!iEvent.isRealData()) { // only look for MC   
    edm::Handle<reco::GenParticleCollection> genParticles;
    iEvent.getByLabel("genParticles", genParticles);
    unsigned int gensize = genParticles->size();
    for (unsigned int i = 0; i<gensize; ++i) {
      if ((*genParticles)[i].pdgId() == 25 && (*genParticles)[i].status()==3 ){
	//std::cout<<" pdgid :  "<<(*genParticles)[i].pdgId()<<", status : "<<(*genParticles)[i].status()<<std::endl;
	TLorentzVector p4((*genParticles)[i].px(),(*genParticles)[i].py(),(*genParticles)[i].pz(),(*genParticles)[i].energy());
	new( (*H_Gen_4Momentum)[i]) TLorentzVector(p4);
	HMass.push_back((*genParticles)[i].mass());
	
      } 
    }
  }
  if(debug_)    std::cout<<"got Hgenerated level higgs info "<<std::endl;
}

void genHiggsInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;

  AddBranch(&H_Gen_4Momentum ,"H_Gen_4Momentum");
  AddBranch(&HMass,"HMass");
  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void genHiggsInfo::Clear(){
  if(debug_)std::cout<<"clearing Higgs info"<<std::endl;
  H_Gen_4Momentum->Clear();
  HMass.clear();
  if(debug_) std::cout<<"cleared"<<std::endl;
}

