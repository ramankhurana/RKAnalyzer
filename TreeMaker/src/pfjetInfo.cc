#include "RKAnalyzer/TreeMaker/interface/pfjetInfo.h"

pfjetInfo::pfjetInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in pfjet constructor"<<std::endl;
  pfjetLabel_     = iConfig.getUntrackedParameter<edm::InputTag> ("pfjetLabel_");
  PFJet_4Momentum = new TClonesArray("TLorentzVector");
  UcPFJet_4Momentum = new TClonesArray("TLorentzVector");
  PFJet_Vposition = new TClonesArray("TVector3");
  if(debug) std::cout<<"in pfjet constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

pfjetInfo::~pfjetInfo(){
  delete tree_;
}

void pfjetInfo::Fill(const edm::Event& iEvent,const  edm::EventSetup& iSetup){
  Clear();
  if(debug_)    std::cout<<"getting pfjet info"<<std::endl;
  
  //for jec uncert
  //edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
  //iSetup.get<JetCorrectionsRecord>().get("AK5PF",JetCorParColl);
  //JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
  //JetCorrectionUncertainty *pfjecUnc = new JetCorrectionUncertainty(JetCorPar);


  edm::Handle<edm::View<pat::Jet> > pfjetHandle;
  iEvent.getByLabel(pfjetLabel_,pfjetHandle);
  const edm::View<pat::Jet> & pfjets = *pfjetHandle;
  if(not iEvent.getByLabel(pfjetLabel_,pfjetHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<pfjetLabel_<<std::endl; 
    exit(0);
  }    
  if(debug_) std::cout<<" Handle is valid "<<std::endl;
  
  size_t npfjetscounter=0;
  edm::View<pat::Jet>::const_iterator pfjet;
  
  for( pfjet = pfjets.begin(); pfjet!=pfjets.end(); ++pfjet){
    if(debug_)  std::cout<<" in jets loop "<<std::endl;
    if(pfjet->pt()>30)  npfjetscounter++;
    TLorentzVector p4(pfjet->px(),pfjet->py(),pfjet->pz(),pfjet->energy());
    new( (*PFJet_4Momentum)[PFJet_n]) TLorentzVector(p4);
    TVector3 v3(pfjet->vx(),pfjet->vy(),pfjet->vz());
    new( (*PFJet_Vposition)[PFJet_n]) TVector3(v3);
    if(debug_)  std::cout<<" saved momentum and position "<<std::endl;

  //  if(pfjet->jecFactor("Uncorrected")!= 0)
  //    PFJet_jecCorr.push_back(1.0/pfjet->jecFactor("Uncorrected")); 
  //  else PFJet_jecCorr.push_back(0.);
    //std::cout<<" b jet info = "<<pfjet->bDiscriminator("combinedSecondaryVertexBJetTags")<<std::endl;
    PFJet_CEF.push_back((float)pfjet->chargedEmEnergyFraction());
    if(debug_)  std::cout<<" chargedEmEnergyFraction  "<<std::endl;

    PFJet_NEF.push_back((float)pfjet->neutralEmEnergyFraction());
    PFJet_CHF.push_back((float)pfjet->chargedHadronEnergyFraction());
    PFJet_NHF.push_back((float)pfjet->neutralHadronEnergyFraction());
    PFJet_HFEME.push_back((float)pfjet->HFEMEnergy());
    PFJet_HFHAE.push_back((float)pfjet->HFHadronEnergy());
    PFJet_NCH.push_back((int)pfjet->chargedMultiplicity());
    if(debug_)  std::cout<<" charged multiplicity saved "<<std::endl;
    //PFJet_NConstituents.push_back((int)pfjet->getPFConstituents().size());
    
    
    // b-tagging
    PFJet_TrackCountHiEffBJetTags.push_back((float)pfjet->bDiscriminator("trackCountingHighEffBJetTags"));
    PFJet_TrackCountHiPurBJetTags.push_back((float)pfjet->bDiscriminator("trackCountingHighPurBJetTags"));
    PFJet_SimpleSVHiEffBJetTags.push_back((float)pfjet->bDiscriminator("simpleSecondaryVertexHighEffBJetTags"));
    PFJet_SimpleSVHiPurBJetTags.push_back((float)pfjet->bDiscriminator("simpleSecondaryVertexHighPurBJetTags"));
    PFJet_CombinedSecondaryVertexBJetTags.push_back((float)pfjet->bDiscriminator("combinedSecondaryVertexBJetTags"));
    PFJet_JetProbabilityBJetTags.push_back((float)pfjet->bDiscriminator("jetProbabilityBJetTags"));
    
    
    //jet energy uncertiany
    //pfjecUnc->setJetEta(pfjet->eta());
    //pfjecUnc->setJetPt(pfjet->pt());
    //PFJet_jecUncer.push_back(pfjecUnc->getUncertainty(true));

    //get the uncorrected jet and fill them
    //pat::Jet uncpfjet = pfjet->correctedJet("Uncorrected");
    //TLorentzVector ucp4(uncpfjet.px(),uncpfjet.py(),uncpfjet.pz(),uncpfjet.energy());
    //new( (*UcPFJet_4Momentum)[PFJet_n]) TLorentzVector(ucp4);
    
    PFJet_n++;
  }//end of for loop
  if(debug_)    std::cout<<"got pfjet info"<<std::endl;
}

void pfjetInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&PFJet_n  ,"PFJet_n");
  AddBranch(&PFJet_4Momentum ,"PFJet_4Momentum");
  AddBranch(&UcPFJet_4Momentum ,"UcPFJet_4Momentum");
  AddBranch(&PFJet_Vposition ,"PFJet_Vposition");
  AddBranch(&PFJet_CEF     ,"PFJet_CEF");
  AddBranch(&PFJet_NEF     ,"PFJet_NEF");
  AddBranch(&PFJet_CHF     ,"PFJet_CHF");
  AddBranch(&PFJet_NHF     ,"PFJet_NHF");
  AddBranch(&PFJet_HFHAE   ,"PFJet_HFHAE");
  AddBranch(&PFJet_HFEME   ,"PFJet_HFEME");
  AddBranch(&PFJet_NCH     ,"PFJet_NCH");
  AddBranch(&PFJet_NConstituents           ,"PFJet_NConstituents");
  AddBranch(&PFJet_TrackCountHiEffBJetTags ,"PFJet_TrackCountHiEffBJetTags");
  AddBranch(&PFJet_TrackCountHiPurBJetTags ,"PFJet_TrackCountHiPurBJetTags");
  AddBranch(&PFJet_SimpleSVHiEffBJetTags   ,"PFJet_SimpleSVHiEffBJetTags");
  AddBranch(&PFJet_SimpleSVHiPurBJetTags   ,"PFJet_SimpleSVHiPurBJetTags");
  AddBranch(&PFJet_CombinedSecondaryVertexBJetTags   ,"PFJet_CombinedSecondaryVertexBJetTags");
  AddBranch(&PFJet_JetProbabilityBJetTags   ,"PFJet_JetProbabilityBJetTags");
  AddBranch(&PFJet_jecUncer ,"PFJet_jecUncer");
  AddBranch(&PFJet_jecCorr ,"PFJet_jecCorr");

  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void pfjetInfo::Clear(){
  if(debug_)std::cout<<"clearing PFjet info"<<std::endl;
  PFJet_n = 0;  
  PFJet_4Momentum->Clear();
  UcPFJet_4Momentum->Clear();
  PFJet_Vposition->Clear();
  PFJet_CEF.clear();    
  PFJet_NEF.clear();
  PFJet_CHF.clear();
  PFJet_NHF.clear();
  PFJet_HFHAE.clear();
  PFJet_HFEME.clear();
  PFJet_NCH.clear();
  PFJet_NConstituents.clear();         
  PFJet_TrackCountHiEffBJetTags.clear();
  PFJet_TrackCountHiPurBJetTags.clear();
  PFJet_SimpleSVHiEffBJetTags.clear();
  PFJet_SimpleSVHiPurBJetTags.clear();  
  PFJet_CombinedSecondaryVertexBJetTags.clear();  
  PFJet_JetProbabilityBJetTags.clear();  
  PFJet_jecUncer.clear();
  PFJet_jecCorr.clear();                  


  if(debug_) std::cout<<"cleared"<<std::endl;
}
