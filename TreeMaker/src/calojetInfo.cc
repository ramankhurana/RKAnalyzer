#include "RKAnalyzer/TreeMaker/interface/calojetInfo.h"

calojetInfo::calojetInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in calojet constructor"<<std::endl;
  calojetLabel_     = iConfig.getUntrackedParameter<edm::InputTag> ("calojetLabel_");
  Calojet_4Momentum = new TClonesArray("TLorentzVector");
  UcCalojet_4Momentum = new TClonesArray("TLorentzVector");
  Calojet_Vposition = new TClonesArray("TVector3");
  if(debug) std::cout<<"in calojet constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

calojetInfo::~calojetInfo(){
  delete tree_;
}

void calojetInfo::Fill(const edm::Event& iEvent,const  edm::EventSetup& iSetup){
  Clear();
  if(debug_)    std::cout<<"getting calojet info"<<std::endl;
  //for jec uncert
  edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
  iSetup.get<JetCorrectionsRecord>().get("AK5PF",JetCorParColl);
  JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
  JetCorrectionUncertainty *pfjecUnc = new JetCorrectionUncertainty(JetCorPar);


  edm::Handle<edm::View<pat::Jet> > calojetHandle;
  iEvent.getByLabel(calojetLabel_,calojetHandle);
  const edm::View<pat::Jet> & calojets = *calojetHandle;
  if(not iEvent.getByLabel(calojetLabel_,calojetHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<calojetLabel_<<std::endl; 
    exit(0);
  }    

  size_t ncalojetscounter=0;
  edm::View<pat::Jet>::const_iterator calojet;
  for( calojet = calojets.begin(); calojet!=calojets.end(); ++calojet){
    if(calojet->pt()>30)  ncalojetscounter++;
    TLorentzVector p4(calojet->px(),calojet->py(),calojet->pz(),calojet->energy());
    new( (*Calojet_4Momentum)[Calojet_n]) TLorentzVector(p4);
    TVector3 v3(calojet->vx(),calojet->vy(),calojet->vz());
    new( (*Calojet_Vposition)[Calojet_n]) TVector3(v3);
    
    if(calojet->jecFactor("Uncorrected")!= 0)
      Calojet_jecCorr.push_back(1.0/calojet->jecFactor("Uncorrected")); 
    else Calojet_jecCorr.push_back(0.);
    
    Calojet_emEnergyFraction.push_back((float)calojet->emEnergyFraction());
    Calojet_energyFractionHadronic.push_back((float)calojet->energyFractionHadronic());
    Calojet_fHPD.push_back((float)calojet->jetID().fHPD);
    Calojet_fRBX.push_back((float)calojet->jetID().fRBX);
    Calojet_RHF.push_back((float)(calojet->jetID().fLong - calojet->jetID().fShort)/
			  (calojet->jetID().fLong + calojet->jetID().fShort));
    Calojet_nTowers.push_back((int)calojet->jetID().nECALTowers + calojet->jetID().nHCALTowers);
    Calojet_hitsInN90.push_back((int)calojet->jetID().hitsInN90);
    Calojet_n90Hits.push_back((int)calojet->jetID().n90Hits);
    
    
    //jet energy uncertiany
    pfjecUnc->setJetEta(calojet->eta());
    pfjecUnc->setJetPt(calojet->pt());
    Calojet_jecUncer.push_back(pfjecUnc->getUncertainty(true));

    //get the uncorrected jet and fill them
    pat::Jet unccalojet = calojet->correctedJet("Uncorrected");
    TLorentzVector ucp4(unccalojet.px(),unccalojet.py(),unccalojet.pz(),unccalojet.energy());
    new( (*UcCalojet_4Momentum)[Calojet_n]) TLorentzVector(ucp4);
    
    Calojet_n++;
  }//end of for loop
  if(debug_)    std::cout<<"got calojet info"<<std::endl;
}

void calojetInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&Calojet_n  ,"Calojet_n");
  AddBranch(&Calojet_4Momentum ,"Calojet_4Momentum");
  AddBranch(&UcCalojet_4Momentum ,"UcCalojet_4Momentum");
  AddBranch(&Calojet_Vposition ,"Calojet_Vposition");
  AddBranch(&Calojet_emEnergyFraction  	    ,"Calojet_emEnergyFraction");
  AddBranch(&Calojet_energyFractionHadronic ,"Calojet_energyFractionHadronic");
  AddBranch(&Calojet_fHPD	,"Calojet_fHPD");
  AddBranch(&Calojet_fRBX	,"Calojet_fRBX");
  AddBranch(&Calojet_RHF	,"Calojet_RHF");
  AddBranch(&Calojet_nTowers	,"Calojet_nTowers");
  AddBranch(&Calojet_hitsInN90	,"Calojet_hitsInN90");
  AddBranch(&Calojet_n90Hits    ,"Calojet_n90Hits");
  AddBranch(&Calojet_jecUncer   ,"Calojet_jecUncer");
  AddBranch(&Calojet_jecCorr    ,"Calojet_jecCorr");  

if(debug_)std::cout<<"setbranches"<<std::endl;
}

void calojetInfo::Clear(){
  if(debug_)std::cout<<"clearing Calojet info"<<std::endl;
  Calojet_n = 0;  
  Calojet_4Momentum->Clear();
  UcCalojet_4Momentum->Clear();
  Calojet_Vposition->Clear();
  Calojet_emEnergyFraction.clear();      
  Calojet_energyFractionHadronic.clear();
  Calojet_fHPD.clear();       
  Calojet_fRBX.clear();     
  Calojet_RHF.clear();      
  Calojet_nTowers.clear();  
  Calojet_hitsInN90.clear();
  Calojet_n90Hits.clear();  
  Calojet_jecUncer.clear(); 
  Calojet_jecCorr.clear();  
  
  if(debug_) std::cout<<"cleared"<<std::endl;
}
