#include "RKAnalyzer/TreeMaker/interface/triggerInfo.h"

triggerInfo::triggerInfo(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in triggerInfo constructor"<<std::endl;
  HLTriggerResults_ = iConfig.getUntrackedParameter<edm::InputTag>("HLTriggerResults_");
  triggerEventTag_  = iConfig.getUntrackedParameter<edm::InputTag>("triggerEventTag_");
  ntriggers = 0;
  if(debug) std::cout<<"in trigger constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

triggerInfo::~triggerInfo(){
  delete tree_;
}

void triggerInfo::Fill(const edm::Event& iEvent, const edm::EventSetup& iSetup, 
		       std::vector<std::string>& all_triggers, 
		       std::vector<std::string>& electron_triggers_in_run, 
		       std::vector<std::string>& muon_triggers_in_run, 
		       std::vector<std::string>& emu_triggers_in_run, 
		       HLTConfigProvider& hltConfig_, std::string& hltlabel_, const size_t& MaxN =200 ){
  if(debug_)    std::cout<<"getting HL Trigger info"<<std::endl;
  Clear();
  Handle<TriggerResults> HLTR;
  iEvent.getByLabel(HLTriggerResults_,HLTR);
  
  Handle<trigger::TriggerEvent> triggerEventHandle;
  iEvent.getByLabel(triggerEventTag_,triggerEventHandle);
  ntriggers = all_triggers.size();
  
  if (!triggerEventHandle.isValid()) {
    std::cout << "Error in getting TriggerEvent product from Event!" << std::endl;
    return;
  }

  if (HLTR.isValid())  {
    const edm::TriggerNames &triggerNames_ = iEvent.triggerNames(*HLTR);
    hlNames_=triggerNames_.triggerNames();
  }

  if (HLTR.isValid()) {
    const edm::TriggerNames &triggerNames_ = iEvent.triggerNames(*HLTR);
    hlNames_=triggerNames_.triggerNames();
    vector<int> idx;
    vector<int> idx_doubleMuon;
    vector<int> idx_doubleElectron;
    vector<int> idx_emu;
    
    int ntriggers = hlNames_.size();
    std::cout<<" ntriggers = "<<ntriggers<<std::endl;
    Int_t hsize = Int_t(HLTR->size());
    
    for ( int itrigger = 0 ; itrigger < (int)all_triggers.size(); itrigger++){
      idx.push_back(triggerNames_.triggerIndex(all_triggers[itrigger]));
      if(idx.size()>0)
      	if(idx[itrigger] < hsize){
	  all_ifTriggerpassed.push_back(HLTR->accept(idx[itrigger]));
	  //std::cout<<" trigger status for all triggers "<<all_triggers[itrigger]<< " is  "<<all_ifTriggerpassed[itrigger]<<std::endl;
	}
    }
    
    //for double electron only
    for ( int itrigger = 0 ; itrigger < (int)electron_triggers_in_run.size(); itrigger++){
      idx_doubleElectron.push_back(triggerNames_.triggerIndex(electron_triggers_in_run[itrigger]));
      if(idx_doubleElectron.size()>0)
      	if(idx_doubleElectron[itrigger] < hsize){
	  doubleElectron_ifTriggerpassed.push_back(HLTR->accept(idx_doubleElectron[itrigger]));
	  //std::cout<<" trigger status for electron triggers "<<electron_triggers_in_run[itrigger]<< "is "<<doubleElectron_ifTriggerpassed[itrigger]<<std::endl;
	}
    }

    //for double muon only
    for ( int itrigger = 0 ; itrigger < (int)muon_triggers_in_run.size(); itrigger++){
      idx_doubleMuon.push_back(triggerNames_.triggerIndex(muon_triggers_in_run[itrigger]));
      if(idx_doubleMuon.size()>0)
      	if(idx_doubleMuon[itrigger] < hsize){
	  doubleMuon_ifTriggerpassed.push_back(HLTR->accept(idx_doubleMuon[itrigger]));
	  //std::cout<<" trigger status for muon triggers"<<muon_triggers_in_run[itrigger]<< "is "<<doubleMuon_ifTriggerpassed[itrigger]
	  //   <<"  direct = "<< HLTR->accept(idx_doubleMuon[itrigger])<<std::endl;
	}
    }
    
    //for e-mu triggers only
    for ( int itrigger = 0 ; itrigger < (int)emu_triggers_in_run.size(); itrigger++){
      idx_emu.push_back(triggerNames_.triggerIndex(emu_triggers_in_run[itrigger]));
      if(idx_emu.size()>0)
        if(idx_emu[itrigger] < hsize){
          emu_ifTriggerpassed.push_back(HLTR->accept(idx_emu[itrigger]));
	  //std::cout<<" trigger status for emu triggers "<<emu_triggers_in_run[itrigger]<< "is "<<emu_ifTriggerpassed[itrigger]<<std::endl;
        }
    }
    

    
  }
  if(debug_)    std::cout<<"got trigger info"<<std::endl;
}

void triggerInfo::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&all_triggerprescales,"all_triggerprescales");
  AddBranch(&all_ifTriggerpassed ,"all_ifTriggerpassed" );
  AddBranch(&doubleElectron_ifTriggerpassed ,"doubleElectron_ifTriggerpassed" );
  AddBranch(&doubleMuon_ifTriggerpassed ,"doubleMuon_ifTriggerpassed" );
  AddBranch(&emu_ifTriggerpassed ,"emu_ifTriggerpassed" );

  
  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void triggerInfo::Clear(){
  if(debug_)std::cout<<"clearing trigger info"<<std::endl;
  all_triggerprescales.clear();
  all_ifTriggerpassed.clear();

  doubleElectron_ifTriggerpassed.clear();
  doubleMuon_ifTriggerpassed.clear();
  emu_ifTriggerpassed.clear();
  if(debug_) std::cout<<"cleared"<<std::endl;
}
