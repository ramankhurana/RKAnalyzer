#include "RKAnalyzer/TreeMaker/interface/calotowerInfo.h"

calotowerInfo::calotowerInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in calotower constructor"<<std::endl;
  calotowerLabel_       = iConfig.getUntrackedParameter<edm::InputTag> ("calotowerLabel_");
  CaloTower_HadPosition = new TClonesArray("TLorentzVector");
  if(debug) std::cout<<"in calotower constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

calotowerInfo::~calotowerInfo(){
  delete tree_;
}

void calotowerInfo::Fill(const edm::Event& iEvent){
  Clear();
  if(debug_)    std::cout<<"getting calotower info"<<std::endl;


  edm::Handle<CaloTowerCollection> caloTowers;
  iEvent.getByLabel(calotowerLabel_, caloTowers );
  if(not iEvent.getByLabel(calotowerLabel_,caloTowers)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<calotowerLabel_<<std::endl; 
    exit(0);
  }    
  CaloTowerCollection::const_iterator cal;
  for( cal = caloTowers->begin(); cal != caloTowers->end(); ++ cal ) {
      
        CaloTower_eta.push_back((float)cal->eta());
        CaloTower_phi.push_back((float)cal->phi());
        CaloTower_E.push_back((float)cal->energy());
        CaloTower_Et.push_back((float)cal->et());
        CaloTower_emEnergy.push_back((float)cal->emEnergy());
        CaloTower_hadEnergy.push_back((float)cal->hadEnergy());
        CaloTower_p.push_back((float)cal->p());
        CaloTower_EMEt.push_back((float)cal->emEt());
        CaloTower_HadEt.push_back((float)cal->hadEt());
        CaloTower_EMPhi.push_back((float)cal->emPosition().phi());
        CaloTower_EMEta.push_back((float)cal->emPosition().eta());
        CaloTower_HE_E.push_back((float)cal->energyInHE());
        CaloTower_HB_E.push_back((float)cal->energyInHB());
        CaloTower_EMTime.push_back((float)cal->ecalTime());
        CaloTower_HadTime.push_back((float)cal->hcalTime());
        //CaloTower_recoFlag.push_back((float)cal->recoFlag());

	TVector3 v3(cal->hadPosition().x(), cal->hadPosition().y(), cal->hadPosition().z());
	new( (*CaloTower_HadPosition)[CaloTower_n] ) TVector3(v3);
        CaloTower_n++;  
  }
  if(debug_)    std::cout<<"got calotower info"<<std::endl;
}

void calotowerInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&CaloTower_n  ,"CaloTower_n");
  AddBranch(&CaloTower_HadPosition ,"CaloTower_HadPosition");
  AddBranch(&CaloTower_eta,      "CaloTower_eta");            
  AddBranch(&CaloTower_phi,     "CaloTower_phi");        
  AddBranch(&CaloTower_E,       "CaloTower_E");          
  AddBranch(&CaloTower_Et,    	"CaloTower_Et");	    
  AddBranch(&CaloTower_emEnergy,   	"CaloTower_emEnergy");   
  AddBranch(&CaloTower_hadEnergy, 	"CaloTower_hadEnergy");  
  AddBranch(&CaloTower_p,	"CaloTower_p");	    
  AddBranch(&CaloTower_EMEt,	"CaloTower_EMEt");	        
  AddBranch(&CaloTower_HadEt,	"CaloTower_HadEt");	        
  AddBranch(&CaloTower_EMPhi,	"CaloTower_EMPhi");	        
  AddBranch(&CaloTower_EMEta,	"CaloTower_EMEta");	        
  AddBranch(&CaloTower_HE_E,	"CaloTower_HE_E");	        
  AddBranch(&CaloTower_HB_E,	"CaloTower_HB_E");	        
  AddBranch(&CaloTower_EMTime,	"CaloTower_EMTime");	        
  AddBranch(&CaloTower_HadTime,	"CaloTower_HadTime");    
  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void calotowerInfo::Clear(){
  if(debug_)std::cout<<"clearing Calotower info"<<std::endl;
  CaloTower_n = 0;  
  CaloTower_HadPosition->Clear();
  CaloTower_eta.clear();        
  CaloTower_phi.clear();        
  CaloTower_E.clear();          
  CaloTower_Et.clear();	    
  CaloTower_emEnergy.clear();   
  CaloTower_hadEnergy.clear();  
  CaloTower_p.clear();	    
  CaloTower_EMEt.clear();	    
  CaloTower_HadEt.clear();	    
  CaloTower_EMPhi.clear();	    
  CaloTower_EMEta.clear();	    
  CaloTower_HE_E.clear();	    
  CaloTower_HB_E.clear();	    
  CaloTower_EMTime.clear();	    
  CaloTower_HadTime.clear();    
  if(debug_) std::cout<<"cleared"<<std::endl;
}
