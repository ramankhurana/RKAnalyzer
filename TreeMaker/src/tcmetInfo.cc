#include "RKAnalyzer/TreeMaker/interface/tcmetInfo.h"

tcmetInfo::tcmetInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in tcmet constructor"<<std::endl;
  tcmetLabel_     = iConfig.getUntrackedParameter<edm::InputTag> ("tcmetLabel_");
  if(debug) std::cout<<"in tcmet constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

tcmetInfo::~tcmetInfo(){
  delete tree_;
}

void tcmetInfo::Fill(const edm::Event& iEvent){
  Clear();
  if(debug_)    std::cout<<"getting tcmet info"<<std::endl;
  edm::Handle<edm::View<pat::MET> > tcmetHandle;
  iEvent.getByLabel(tcmetLabel_,tcmetHandle);
  const edm::View<pat::MET> & tcmets = *tcmetHandle;

  if(not iEvent.getByLabel(tcmetLabel_,tcmetHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<tcmetLabel_<<std::endl; 
    exit(0);
  }  
  for(int i = 0; i<5; i++)
    {
      TcmetPt.push_back(-99);
      TcmetPx.push_back(-99);
      TcmetPy.push_back(-99);
      TcmetPhi.push_back(-99);
      TcmetSumEt.push_back(-99);
    }
  
  if ( tcmetHandle.isValid() ){
     // 0=full corr1=uncorrNone  2=uncorrALL 3=uncorrJES  4=uncorrMUON  5=TAU
     TcmetPt[0]                          = (float) tcmets[0].pt();
     TcmetPx[0]                          = (float) tcmets[0].px();
     TcmetPy[0]                          = (float) tcmets[0].py();
     TcmetPhi[0]                         = (float) correct_phi(tcmets[0].phi());
     TcmetSumEt[0]                       = (float) tcmets[0].sumEt();
     					   
     TcmetPt[1]                          = (float) tcmets[0].uncorrectedPt(pat::MET::uncorrNONE); 
     TcmetPhi[1]                         = (float) correct_phi(tcmets[0].uncorrectedPhi(pat::MET::uncorrNONE)); 
     TcmetPx[1]                          = (float) tcmets[0].corEx(pat::MET::uncorrNONE); 
     TcmetPy[1]                          = (float) tcmets[0].corEy(pat::MET::uncorrNONE); 
     TcmetSumEt[1]                       = (float) tcmets[0].corSumEt(pat::MET::uncorrNONE); 
     					   
     TcmetPt[2]                          = (float) tcmets[0].uncorrectedPt(pat::MET::uncorrALL); 
     TcmetPhi[2]                         = (float) correct_phi(tcmets[0].uncorrectedPhi(pat::MET::uncorrALL)); 
     TcmetPx[2]                          = (float) tcmets[0].corEx(pat::MET::uncorrALL); 
     TcmetPy[2]                          = (float) tcmets[0].corEy(pat::MET::uncorrALL); 
     TcmetSumEt[2]                       = (float) tcmets[0].corSumEt(pat::MET::uncorrALL); 
     					   
     TcmetPt[3]                          = (float) tcmets[0].uncorrectedPt(pat::MET::uncorrJES); 
     TcmetPhi[3]                         = (float) correct_phi(tcmets[0].uncorrectedPhi(pat::MET::uncorrJES)); 
     TcmetPx[3]                          = (float) tcmets[0].corEx(pat::MET::uncorrJES); 
     TcmetPy[3]                          = (float) tcmets[0].corEy(pat::MET::uncorrJES); 
     TcmetSumEt[3]                       = (float) tcmets[0].corSumEt(pat::MET::uncorrJES); 
     					   
     TcmetPt[4]                          = (float) tcmets[0].uncorrectedPt(pat::MET::uncorrMUON); 
     TcmetPhi[4]                         = (float) correct_phi(tcmets[0].uncorrectedPhi(pat::MET::uncorrMUON)); 
     TcmetPx[4]                          = (float) tcmets[0].corEx(pat::MET::uncorrMUON); 
     TcmetPy[4]                          = (float) tcmets[0].corEy(pat::MET::uncorrMUON); 
     TcmetSumEt[4]                       = (float) tcmets[0].corSumEt(pat::MET::uncorrMUON); 
     					   
     TcmetPt[5]                          = (float) tcmets[0].uncorrectedPt(pat::MET::uncorrTAU); 
     TcmetPhi[5]                         = (float) correct_phi(tcmets[0].uncorrectedPhi(pat::MET::uncorrTAU)); 
     TcmetPx[5]                          = (float) tcmets[0].corEx(pat::MET::uncorrTAU); 
     TcmetPy[5]                          = (float) tcmets[0].corEy(pat::MET::uncorrTAU); 
     TcmetSumEt[5]                       = (float) tcmets[0].corSumEt(pat::MET::uncorrTAU); 
   }

    if(debug_)    std::cout<<"got tcmet info"<<std::endl;
}

void tcmetInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&TcmetPt,"TcmetPt");
  AddBranch(&TcmetPx,"TcmetPx");
  AddBranch(&TcmetPy,"TcmetPy");
  AddBranch(&TcmetPhi,"TcmetPhi");
  AddBranch(&TcmetSumEt,"TcmetSumEt");
  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void tcmetInfo::Clear(){
  if(debug_)std::cout<<"clearing tcMet info"<<std::endl;
  TcmetPt.clear();
  TcmetPx.clear();
  TcmetPy.clear();
  TcmetPhi.clear();
  TcmetSumEt.clear();
  if(debug_) std::cout<<"cleared"<<std::endl;
}

float tcmetInfo::correct_phi(float phi){
	return (phi >= 0 ? phi : (2*TMath::Pi() + phi));
}



