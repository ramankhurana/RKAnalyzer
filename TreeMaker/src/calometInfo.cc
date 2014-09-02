#include "RKAnalyzer/TreeMaker/interface/calometInfo.h"

calometInfo::calometInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in calomet constructor"<<std::endl;
  calometLabel_     = iConfig.getUntrackedParameter<edm::InputTag> ("calometLabel_");
  fillgenmetInfo_   = iConfig.getUntrackedParameter<bool> ("fillgenmetInfo_");
  if(debug) std::cout<<"in calomet constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

calometInfo::~calometInfo(){
  delete tree_;
}

void calometInfo::Fill(const edm::Event& iEvent){
  Clear();
  if(debug_)    std::cout<<"getting calomet info"<<std::endl;
  edm::Handle<edm::View<pat::MET> > calometHandle;
  iEvent.getByLabel(calometLabel_,calometHandle);
  const edm::View<pat::MET> & calomets = *calometHandle;

  if(not iEvent.getByLabel(calometLabel_,calometHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<calometLabel_<<std::endl; 
    exit(0);
  }  
  for(int i = 0; i<5; i++)
    {
      CalometPt.push_back(-99);
      CalometPx.push_back(-99);
      CalometPy.push_back(-99);
      CalometPhi.push_back(-99);
      CalometSumEt.push_back(-99);
    }
  
  if ( calometHandle.isValid() ){
     // 0=full corr1=uncorrNone  2=uncorrALL 3=uncorrJES  4=uncorrMUON  5=TAU
     CalometPt[0]                          = (float) calomets[0].pt();
     CalometPx[0]                          = (float) calomets[0].px();
     CalometPy[0]                          = (float) calomets[0].py();
     CalometPhi[0]                         = (float) correct_phi(calomets[0].phi());
     CalometSumEt[0]                       = (float) calomets[0].sumEt();
     					   
     CalometPt[1]                          = (float) calomets[0].uncorrectedPt(pat::MET::uncorrNONE); 
     CalometPhi[1]                         = (float) correct_phi(calomets[0].uncorrectedPhi(pat::MET::uncorrNONE)); 
     CalometPx[1]                          = (float) calomets[0].corEx(pat::MET::uncorrNONE); 
     CalometPy[1]                          = (float) calomets[0].corEy(pat::MET::uncorrNONE); 
     CalometSumEt[1]                       = (float) calomets[0].corSumEt(pat::MET::uncorrNONE); 
     					   
     CalometPt[2]                          = (float) calomets[0].uncorrectedPt(pat::MET::uncorrALL); 
     CalometPhi[2]                         = (float) correct_phi(calomets[0].uncorrectedPhi(pat::MET::uncorrALL)); 
     CalometPx[2]                          = (float) calomets[0].corEx(pat::MET::uncorrALL); 
     CalometPy[2]                          = (float) calomets[0].corEy(pat::MET::uncorrALL); 
     CalometSumEt[2]                       = (float) calomets[0].corSumEt(pat::MET::uncorrALL); 
     					   
     CalometPt[3]                          = (float) calomets[0].uncorrectedPt(pat::MET::uncorrJES); 
     CalometPhi[3]                         = (float) correct_phi(calomets[0].uncorrectedPhi(pat::MET::uncorrJES)); 
     CalometPx[3]                          = (float) calomets[0].corEx(pat::MET::uncorrJES); 
     CalometPy[3]                          = (float) calomets[0].corEy(pat::MET::uncorrJES); 
     CalometSumEt[3]                       = (float) calomets[0].corSumEt(pat::MET::uncorrJES); 
     					   
     CalometPt[4]                          = (float) calomets[0].uncorrectedPt(pat::MET::uncorrMUON); 
     CalometPhi[4]                         = (float) correct_phi(calomets[0].uncorrectedPhi(pat::MET::uncorrMUON)); 
     CalometPx[4]                          = (float) calomets[0].corEx(pat::MET::uncorrMUON); 
     CalometPy[4]                          = (float) calomets[0].corEy(pat::MET::uncorrMUON); 
     CalometSumEt[4]                       = (float) calomets[0].corSumEt(pat::MET::uncorrMUON); 
     					   
     CalometPt[5]                          = (float) calomets[0].uncorrectedPt(pat::MET::uncorrTAU); 
     CalometPhi[5]                         = (float) correct_phi(calomets[0].uncorrectedPhi(pat::MET::uncorrTAU)); 
     CalometPx[5]                          = (float) calomets[0].corEx(pat::MET::uncorrTAU); 
     CalometPy[5]                          = (float) calomets[0].corEy(pat::MET::uncorrTAU); 
     CalometSumEt[5]                       = (float) calomets[0].corSumEt(pat::MET::uncorrTAU); 


     if(fillgenmetInfo_){
       const reco::GenMET *genMet = calomets[0].genMET();
       genMetPt     = genMet->et();
       genMetPhi    = correct_phi(genMet->phi());
       genMetSumEt  = genMet->sumEt();
       genMetPx     = genMet->px();
       genMetPy     = genMet->py();
     }
  }
  
  if(debug_)    std::cout<<"got calomet info"<<std::endl;
}

void calometInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&CalometPt,"CalometPt");
  AddBranch(&CalometPx,"CalometPx");
  AddBranch(&CalometPy,"CalometPy");
  AddBranch(&CalometPhi,"CalometPhi");
  AddBranch(&CalometSumEt,"CalometSumEt");
  if(fillgenmetInfo_){
    AddBranch(&genMetPt     ,"genMetPt");     
    AddBranch(&genMetPhi    ,"genMetPhi");    
    AddBranch(&genMetSumEt  ,"genMetSumEt");  
    AddBranch(&genMetPx     ,"genMetPx");   
    AddBranch(&genMetPy     ,"genMetPy");     
  }
  
  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void calometInfo::Clear(){
  if(debug_)std::cout<<"clearing caloMet info"<<std::endl;
  if(fillgenmetInfo_){
    genMetPt     =-99.;
    genMetPhi    =-99.;
    genMetSumEt  =-99.;
    genMetPx     =-99.;
    genMetPy     =-99.;
  }
  CalometPt.clear();
  CalometPx.clear();
  CalometPy.clear();
  CalometPhi.clear();
  CalometSumEt.clear();
  if(debug_) std::cout<<"cleared"<<std::endl;
}

float calometInfo::correct_phi(float phi){
	return (phi >= 0 ? phi : (2*TMath::Pi() + phi));
}



