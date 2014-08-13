#include "RKAnalyzer/TreeMaker/interface/tauInfo.h"

tauInfo::tauInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in tau constructor"<<std::endl;
  tauLabel_          = iConfig.getUntrackedParameter<edm::InputTag> ("tauLabel_");
  fillDetailTauInfo_ = iConfig.getUntrackedParameter<bool>     ("fillDetailTauInfo_");
  Tau_4Momentum = new TClonesArray("TLorentzVector");
  Tau_Vposition = new TClonesArray("TVector3");
  if(debug) std::cout<<"in tau constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

tauInfo::~tauInfo(){
  delete tree_;
}

void tauInfo::Fill(const edm::Event& iEvent){
  Clear();
  if(debug_)    std::cout<<"getting tau info"<<std::endl;

  edm::Handle<edm::View<pat::Tau> > tauHandle;
  iEvent.getByLabel(tauLabel_,tauHandle);

  if(not iEvent.getByLabel(tauLabel_,tauHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<tauLabel_<<std::endl; 
    exit(0);
  }     
  const edm::View<pat::Tau> & taus = *tauHandle;   
  for(edm::View<pat::Tau>::const_iterator tau = taus.begin(); tau!=taus.end(); ++tau)
    {
      TLorentzVector p4(tau->px(),tau->py(),tau->pz(),tau->energy());
      new( (*Tau_4Momentum)[Tau_n]) TLorentzVector(p4);
      new( (*Tau_Vposition)[Tau_n]) TVector3(tau->vx(),
					     tau->vy(),
					     tau->vz());
      
      Tau_charge.push_back((int)tau->charge());
      
      if(fillDetailTauInfo_){//-------Bhawna need this infor

	Tau_nPions.push_back(0);
	Tau_nPi0.push_back(0);
	Tau_nPhotons.push_back(0);
	oneProng0Pi0.push_back(0);
	oneProng1Pi0.push_back(0);
	oneProng2Pi0.push_back(0);
	threeProng0Pi0.push_back(0);
	threeProng1Pi0.push_back(0);
	tauelectron.push_back(0);
	taumuon.push_back(0);
	
	if(tau->genJet())
	  {      
	    genTauDecayMode = JetMCTagUtils::genTauDecayMode(*tau->genJet());
	    genTauDecayMode1.push_back(JetMCTagUtils::genTauDecayMode(*tau->genJet()));
            
	    if (genTauDecayMode == "oneProng0Pi0")
	      { oneProng0Pi0[Tau_n]=1;
		oneProng0Pi0Pt.push_back(tau->genJet()->pt()) ;
		oneProng0Pi0Eta.push_back(tau->genJet()->eta());
		oneProng0Pi0Phi.push_back(tau->genJet()->phi());
	      }  
                        
	    if(genTauDecayMode == "oneProng1Pi0") oneProng1Pi0[Tau_n]=1;
	    if(genTauDecayMode == "oneProng2Pi0") oneProng2Pi0[Tau_n]=1;
	    if (genTauDecayMode == "threeProng0Pi0"){ 
	      threeProng0Pi0[Tau_n]=1;
	      nthreeProng0Pi0++;
	    }
	    if (genTauDecayMode == "threeProng1Pi0"){ 
	      threeProng1Pi0[Tau_n]=1;
	      nthreeProng1Pi0++;
	    }
	    if (genTauDecayMode == "electron"){
	      tauelectron[Tau_n]=1;
	      ntauelectron++;
	    }
	    if (genTauDecayMode == "muon"){ 
	      taumuon[Tau_n]=1;
	      ntaumuon++;
	    }
	    
	    if (  (genTauDecayMode == "oneProng0Pi0"   ||   genTauDecayMode == "oneProng1Pi0"   ||
		   genTauDecayMode == "oneProng2Pi0"   ||   genTauDecayMode == "threeProng0Pi0" ||
		   genTauDecayMode == "threeProng1Pi0" ||   genTauDecayMode == "electron"       ||
		   genTauDecayMode == "muon"
		   )  )   
	      {     
		genHadTauPt.push_back(tau->genJet()->pt()) ;
		genHadTauEta.push_back(tau->genJet()->eta());
		genHadTauPhi.push_back(tau->genJet()->phi());

		if( (tau->genJet()->getGenConstituents()).size() !=0 )
		  {  
		    std::vector<int>   PionPdgId, PhotonPdgId, Pi0PdgId; 
		    std::vector<float> PionPt, PhotonPt, Pi0Pt; 
		    std::vector<float> PionEta, PhotonEta, Pi0Eta;
 		    std::vector<float> PionPhi, PhotonPhi, Pi0Phi; 
		    for ( int i = 0; i < 5; i++)
		      { 
			PionPdgId.push_back(-999);
			PionPt.push_back(-999);
			PionEta.push_back(-999);
			PionPhi.push_back(-999);
			PhotonPdgId.push_back(-999);
			PhotonPt.push_back(-999);
			PhotonEta.push_back(-999);
			PhotonPhi.push_back(-999);
			Pi0PdgId.push_back(-999);
			Pi0Pt.push_back(-999);
			Pi0Eta.push_back(-999);
			Pi0Phi.push_back(-999);
		      }                 
		    /*
		    std::vector <const GenParticle*>  genParticleList = tau->genJet()->getGenConstituents();
                    for (int iList = 0; iList <int(genParticleList.size()); iList++)
                         {
			   
                           if(abs(genParticleList[iList]->pdgId()) == 211)
                             { 
			       if(Tau_npions[Tau_n]>5) continue;
                               PionPt[Tau_npions[Tau_n]]    = genParticleList[iList]->pt();
                               PionEta[Tau_npions[Tau_n]]   = genParticleList[iList]->eta();
                               PionPhi[Tau_npions[Tau_n]]   = genParticleList[iList]->phi();
                               PionPdgId[Tau_npions[Tau_n]] = genParticleList[iList]->pdgId();
                               Tau_npions[Tau_n]++;
                             }
                          if(abs(genParticleList[iList]->pdgId()) == 22)
                             { if(Tau_nPhotons[Tau_n]> 5)continue;
                               PhotonPt[Tau_nPhotons[Tau_n]]     = genParticleList[iList]->pt();
                               PhotonEta[Tau_nPhotons[Tau_n]]    = genParticleList[iList]->eta();
                               PhotonPhi[Tau_nPhotons[Tau_n]]    = genParticleList[iList]->phi();
                               PhotonPdgId[Tau_nPhotons[Tau_n]]  = genParticleList[iList]->pdgId();
                               Tau_nPhotons[Tau_n]++;
                             }
                           if(abs(genParticleList[iList]->pdgId()) == 111)
                             { if(Tau_nPi0[Tau_n]> 5 ) continue;
                             Pi0Pt[Tau_nPi0[Tau_n]]     = genParticleList[iList]->pt();
                             Pi0Eta[Tau_nPi0[Tau_n]]    = genParticleList[iList]->eta();
                             Pi0Phi[Tau_nPi0[Tau_n]]    = genParticleList[iList]->phi();
                             Pi0PdgId[Tau_nPi0[Tau_n]]  = genParticleList[iList]->pdgId();
                             if(debug_)cout<<"Pi0Pt  = "<<Pi0Pt[Tau_nPi0[Tau_n]]<<endl;
                             Tau_nPi0[Tau_n]++;
                             }
                         }//end of  for (int iList = 0; iList <int(genParticleList.size()); iList++)
		    */
		    Tau_PionPdgId.push_back(PionPdgId);
		    Tau_PionPt.push_back(PionPt);
		    Tau_PionEta.push_back(PionEta);
		    Tau_PionPhi.push_back(PionPhi);
		    Tau_PhotonPdgId.push_back(PhotonPdgId);
		    Tau_PhotonPt.push_back(PhotonPt);
		    Tau_PhotonEta.push_back(PhotonEta);
		    Tau_PhotonPhi.push_back(PhotonPhi);
		    Tau_Pi0PdgId.push_back(Pi0PdgId);
		    Tau_Pi0Pt.push_back(Pi0Pt);
		    Tau_Pi0Eta.push_back(Pi0Eta);
		    Tau_Pi0Phi.push_back(Pi0Phi);
		  }
	      }
	  }
      }
    
      Tau_n++;
    }
  
    if(debug_)    std::cout<<"got tau info"<<std::endl;
}
  
void tauInfo::SetBranches(){
    if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
    AddBranch(&Tau_n  ,"Tau_n");
    AddBranch(&Tau_4Momentum ,"Tau_4Momentum");
    AddBranch(&Tau_Vposition ,"Tau_Vposition");
    AddBranch(&Tau_charge    ,"Tau_charge");
    if(fillDetailTauInfo_){
      AddBranch(&Tau_nPions       ,"Tau_nPions"); 
      AddBranch(&Tau_nPi0	        ,"Tau_nPi0");  
      AddBranch(&Tau_nPhotons     ,"Tau_nPhotons");     
      AddBranch(&oneProng0Pi0     ,"oneProng0Pi0");
      AddBranch(&oneProng1Pi0     ,"oneProng1Pi0");     
      AddBranch(&oneProng2Pi0     ,"oneProng2Pi0");     
      AddBranch(&nthreeProng0Pi0  ,"nthreeProng0Pi0");  
      AddBranch(&threeProng0Pi0   ,"threeProng0Pi0");   
      AddBranch(&nthreeProng1Pi0  ,"nthreeProng1Pi0");  
      AddBranch(&threeProng1Pi0   ,"threeProng1Pi0");   
      AddBranch(&ntauelectron     ,"ntauelectron");     
      AddBranch(&tauelectron      ,"tauelectron");
      AddBranch(&ntaumuon	        ,"ntaumuon");	         
      AddBranch(&taumuon	        ,"taumuon");	     
      
      AddBranch(&oneProng0Pi0Pt   ,"oneProng0Pi0Pt");
      AddBranch(&oneProng0Pi0Eta  ,"oneProng0Pi0Eta");  
      AddBranch(&oneProng0Pi0Phi  ,"oneProng0Pi0Phi");  
      AddBranch(&genHadTauPt      ,"genHadTauPt");	         
      AddBranch(&genHadTauEta     ,"genHadTauEta");     
      AddBranch(&genHadTauPhi     ,"genHadTauPhi");     
      
      AddBranch(&Tau_PionPdgId    ,"Tau_PionPdgId");    
      AddBranch(&Tau_PionPt       ,"Tau_PionPt");	         
      AddBranch(&Tau_PionEta      ,"Tau_PionEta");	         
      AddBranch(&Tau_PionPhi      ,"Tau_PionPhi");	         
      AddBranch(&Tau_Pi0PdgId     ,"Tau_Pi0PdgId");     
      AddBranch(&Tau_Pi0Pt        ,"Tau_Pi0Pt");	         
      AddBranch(&Tau_Pi0Eta       ,"Tau_Pi0Eta");	         
      AddBranch(&Tau_Pi0Phi       ,"Tau_Pi0Phi");	         
      AddBranch(&Tau_PhotonPdgId  ,"Tau_PhotonPdgId");  
      AddBranch(&Tau_PhotonPt     ,"Tau_PhotonPt");     
      AddBranch(&Tau_PhotonEta    ,"Tau_PhotonEta");    
      AddBranch(&Tau_PhotonPhi    ,"Tau_PhotonPhi");    
    }  
    
    if(debug_)    std::cout<<"set branches"<<std::endl;
}

void tauInfo::Clear(){
  if(debug_)std::cout<<"clearing Tau info"<<std::endl;
  Tau_n = 0;  
  Tau_4Momentum->Clear();
  Tau_Vposition->Clear();
  Tau_charge.clear();
  if(fillDetailTauInfo_){
    nthreeProng1Pi0 = nthreeProng0Pi0 = ntauelectron = ntaumuon = 0; 
    Tau_nPions.clear();
    Tau_nPi0.clear();
    Tau_nPhotons.clear();
    oneProng0Pi0.clear();
    oneProng1Pi0.clear();
    oneProng2Pi0.clear();
    threeProng0Pi0.clear();
    threeProng1Pi0.clear();
    tauelectron.clear();
    taumuon.clear();
    
    oneProng0Pi0Pt.clear();
    oneProng0Pi0Eta.clear();
    oneProng0Pi0Phi.clear();
    genHadTauPt.clear();
    genHadTauEta.clear();
    genHadTauPhi.clear();
    
    Tau_PionPdgId.clear();
    Tau_PionPt.clear();
    Tau_PionEta.clear();
    Tau_PionPhi.clear();
    Tau_Pi0PdgId.clear();
    Tau_Pi0Pt.clear();
    Tau_Pi0Eta.clear();
    Tau_Pi0Phi.clear();
    Tau_PhotonPdgId.clear();
    Tau_PhotonPt.clear();
    Tau_PhotonEta.clear();
    Tau_PhotonPhi.clear();
  }
  if(debug_) std::cout<<"cleared"<<std::endl;
}
