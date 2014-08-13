#include "RKAnalyzer/TreeMaker/interface/pfmetInfo.h"

pfmetInfo::pfmetInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in pfmet constructor"<<std::endl;
  pfmetLabel_     = iConfig.getUntrackedParameter<edm::InputTag> ("pfmetLabel_");
  if(debug) std::cout<<"in pfmet constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

pfmetInfo::~pfmetInfo(){
  delete tree_;
}

void pfmetInfo::Fill(const edm::Event& iEvent){
  Clear();
  if(debug_)    std::cout<<"getting pfmet info"<<std::endl;
  edm::Handle<edm::View<pat::MET> > metPFHandle;
  iEvent.getByLabel(pfmetLabel_,metPFHandle);
  const edm::View<pat::MET> & metsPF = *metPFHandle;

  if(not iEvent.getByLabel(pfmetLabel_,metPFHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<pfmetLabel_<<std::endl; 
    exit(0);
  }  
  for(int i = 0; i<5; i++)
    {
      PFMetPt.push_back(-99);
      PFMetPx.push_back(-99);
      PFMetPy.push_back(-99);
      PFMetPhi.push_back(-99);
      PFMetSumEt.push_back(-99);
    }
  
  if ( metPFHandle.isValid() ){
     // 0=full corr1=uncorrNone  2=uncorrALL 3=uncorrJES  4=uncorrMUON  5=TAU
     PFMetPt[0]                          = (float) metsPF[0].pt();
     PFMetPx[0]                          = (float) metsPF[0].px();
     PFMetPy[0]                          = (float) metsPF[0].py();
     PFMetPhi[0]                         = (float) correct_phi(metsPF[0].phi());
     PFMetSumEt[0]                       = (float) metsPF[0].sumEt();
     					   
     PFMetPt[1]                          = (float) metsPF[0].uncorrectedPt(pat::MET::uncorrNONE); 
     PFMetPhi[1]                         = (float) correct_phi(metsPF[0].uncorrectedPhi(pat::MET::uncorrNONE)); 
     PFMetPx[1]                          = (float) metsPF[0].corEx(pat::MET::uncorrNONE); 
     PFMetPy[1]                          = (float) metsPF[0].corEy(pat::MET::uncorrNONE); 
     PFMetSumEt[1]                       = (float) metsPF[0].corSumEt(pat::MET::uncorrNONE); 
     					   
     PFMetPt[2]                          = (float) metsPF[0].uncorrectedPt(pat::MET::uncorrALL); 
     PFMetPhi[2]                         = (float) correct_phi(metsPF[0].uncorrectedPhi(pat::MET::uncorrALL)); 
     PFMetPx[2]                          = (float) metsPF[0].corEx(pat::MET::uncorrALL); 
     PFMetPy[2]                          = (float) metsPF[0].corEy(pat::MET::uncorrALL); 
     PFMetSumEt[2]                       = (float) metsPF[0].corSumEt(pat::MET::uncorrALL); 
     					   
     PFMetPt[3]                          = (float) metsPF[0].uncorrectedPt(pat::MET::uncorrJES); 
     PFMetPhi[3]                         = (float) correct_phi(metsPF[0].uncorrectedPhi(pat::MET::uncorrJES)); 
     PFMetPx[3]                          = (float) metsPF[0].corEx(pat::MET::uncorrJES); 
     PFMetPy[3]                          = (float) metsPF[0].corEy(pat::MET::uncorrJES); 
     PFMetSumEt[3]                       = (float) metsPF[0].corSumEt(pat::MET::uncorrJES); 
     					   
     PFMetPt[4]                          = (float) metsPF[0].uncorrectedPt(pat::MET::uncorrMUON); 
     PFMetPhi[4]                         = (float) correct_phi(metsPF[0].uncorrectedPhi(pat::MET::uncorrMUON)); 
     PFMetPx[4]                          = (float) metsPF[0].corEx(pat::MET::uncorrMUON); 
     PFMetPy[4]                          = (float) metsPF[0].corEy(pat::MET::uncorrMUON); 
     PFMetSumEt[4]                       = (float) metsPF[0].corSumEt(pat::MET::uncorrMUON); 
     					   
     PFMetPt[5]                          = (float) metsPF[0].uncorrectedPt(pat::MET::uncorrTAU); 
     PFMetPhi[5]                         = (float) correct_phi(metsPF[0].uncorrectedPhi(pat::MET::uncorrTAU)); 
     PFMetPx[5]                          = (float) metsPF[0].corEx(pat::MET::uncorrTAU); 
     PFMetPy[5]                          = (float) metsPF[0].corEy(pat::MET::uncorrTAU); 
     PFMetSumEt[5]                       = (float) metsPF[0].corSumEt(pat::MET::uncorrTAU); 
   }

    if(debug_)    std::cout<<"got pfmet info"<<std::endl;
}

void pfmetInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&PFMetPt,"PFMetPt");
  AddBranch(&PFMetPx,"PFMetPx");
  AddBranch(&PFMetPy,"PFMetPy");
  AddBranch(&PFMetPhi,"PFMetPhi");
  AddBranch(&PFMetSumEt,"PFMetSumEt");
  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void pfmetInfo::Clear(){
  if(debug_)std::cout<<"clearing pfMet info"<<std::endl;
  PFMetPt.clear();
  PFMetPx.clear();
  PFMetPy.clear();
  PFMetPhi.clear();
  PFMetSumEt.clear();
  if(debug_) std::cout<<"cleared"<<std::endl;
}

float pfmetInfo::correct_phi(float phi){
	return (phi >= 0 ? phi : (2*TMath::Pi() + phi));
}



