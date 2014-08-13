// system include files
#include <memory>
#include <string>

#include "RKAnalyzer/TreeMaker/interface/TreeMaker.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

TreeMaker::TreeMaker(const edm::ParameterSet& iConfig):
  MaxN(200)
{
  debug_               = iConfig.getUntrackedParameter<bool>("debug_");
  filleventInfo_       = iConfig.getUntrackedParameter<bool>("filleventInfo_");
  fillpileUpInfo_      = iConfig.getUntrackedParameter<bool>("fillpileUpInfo_");
  filltriggerInfo_     = iConfig.getUntrackedParameter<bool>("filltriggerInfo_");
  hltlabel_            = iConfig.getUntrackedParameter<string>("hltlabel_");
  fillvertexInfo_      = iConfig.getUntrackedParameter<bool>("fillvertexInfo_");
  filltrackInfo_       = iConfig.getUntrackedParameter<bool>("filltrackInfo_");
  fillmuonInfo_        = iConfig.getUntrackedParameter<bool>("fillmuonInfo_");
  fillcosmicmuonInfo_  = iConfig.getUntrackedParameter<bool>("fillcosmicmuonInfo_");
  fillPhotInfo_        = iConfig.getUntrackedParameter<bool>("fillPhotInfo_");
  fillUcphotInfo_      = iConfig.getUntrackedParameter<bool>("fillUcphotInfo_");
  fillbeamhaloInfo_    = iConfig.getUntrackedParameter<bool>("fillbeamhaloInfo_");
  fillcscrpcInfo_      = iConfig.getUntrackedParameter<bool>("fillcscrpcInfo_");
  fillpfmetInfo_       = iConfig.getUntrackedParameter<bool>("fillpfmetInfo_");
  filltcmetInfo_       = iConfig.getUntrackedParameter<bool>("filltcmetInfo_");
  fillcalometInfo_     = iConfig.getUntrackedParameter<bool>("fillcalometInfo_");
  fillcalojetInfo_     = iConfig.getUntrackedParameter<bool>("fillcalojetInfo_");
  fillpfjetInfo_       = iConfig.getUntrackedParameter<bool>("fillpfjetInfo_");
  fillgenjetInfo_      = iConfig.getUntrackedParameter<bool>("fillgenjetInfo_");
  fillelectronInfo_    = iConfig.getUntrackedParameter<bool>("fillelectronInfo_");
  filltauInfo_         = iConfig.getUntrackedParameter<bool>("filltauInfo_");
  fillcalotowerInfo_   = iConfig.getUntrackedParameter<bool>("fillcalotowerInfo_");
  fillrhoInfo_         = iConfig.getUntrackedParameter<bool>("fillrhoInfo_");
  fillhpstauInfo_      = iConfig.getUntrackedParameter<bool>("fillhpstauInfo_");

  all_triggers.clear();
  muon_triggers_in_run.clear();
  electron_triggers_in_run.clear();
  emu_triggers_in_run.clear();
  
  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>("tree","tree");
  tree_->Branch("all_triggers", &all_triggers);  

  tree_->Branch("electron_triggers_in_run", &electron_triggers_in_run);  
  tree_->Branch("muon_triggers_in_run", &muon_triggers_in_run);  
  tree_->Branch("emu_triggers_in_run", &emu_triggers_in_run);  

  if( filltriggerInfo_)       triggerInfo_    = new triggerInfo   ("pat", tree_, debug_, iConfig);
  if( filleventInfo_)         eventInfo_      = new eventInfo     ("pat", tree_, debug_, iConfig);
  if( fillpileUpInfo_)        pileUpInfo_     = new pileUpInfo    ("pat", tree_, debug_, iConfig);
  if( fillvertexInfo_)        vertexInfo_     = new vertexInfo    ("pat", tree_, debug_, iConfig);
  if( filltrackInfo_)         trackInfo_      = new trackInfo     ("pat", tree_, debug_, iConfig);
  if( fillmuonInfo_)          muonInfo_       = new muonInfo      ("pat", tree_, debug_, iConfig);
  if( fillcosmicmuonInfo_)    cosmicmuonInfo_ = new cosmicmuonInfo("pat", tree_, debug_, iConfig);
  if( fillPhotInfo_)          photonInfo_     = new photonInfo    ("pat", tree_, debug_, iConfig); 
  if( fillUcphotInfo_)        ucphotonInfo_   = new ucphotonInfo  ("pat", tree_, debug_, iConfig); 
  if( fillbeamhaloInfo_)      beamhaloInfo_   = new beamhaloInfo  ("pat", tree_, debug_, iConfig); 
  if( fillcscrpcInfo_)        cscrpcInfo_     = new cscrpcInfo    ("pat", tree_, debug_, iConfig); 
  if( fillpfmetInfo_)         pfmetInfo_      = new pfmetInfo     ("pat", tree_, debug_, iConfig); 
  if( filltcmetInfo_)         tcmetInfo_      = new tcmetInfo     ("pat", tree_, debug_, iConfig); 
  if( fillcalometInfo_)       calometInfo_    = new calometInfo   ("pat", tree_, debug_, iConfig); 
  if( fillcalojetInfo_)       calojetInfo_    = new calojetInfo   ("pat", tree_, debug_, iConfig); 
  if( fillpfjetInfo_)         pfjetInfo_      = new pfjetInfo     ("pat", tree_, debug_, iConfig); 
  if( fillgenjetInfo_)        genjetInfo_     = new genjetInfo    ("pat", tree_, debug_, iConfig); 
  if( fillelectronInfo_)      electronInfo_   = new electronInfo  ("pat", tree_, debug_, iConfig); 
  if( filltauInfo_)           tauInfo_        = new tauInfo       ("pat", tree_, debug_, iConfig); 
  if( fillrhoInfo_)           rhoInfo_        = new rhoInfo       ("pat", tree_, debug_, iConfig);  
  if( fillhpstauInfo_)        hpstauInfo_     = new hpstauInfo    ("pat", tree_, debug_, iConfig);  
  if( false/*fillgenhiggsInfo_*/)      genhiggsInfo_   = new genHiggsInfo  ("pat", tree_, debug_, iConfig);

  
  if(debug_) std::cout<<"got all the objects and branches in the tree"<<std::endl;
}


TreeMaker::~TreeMaker()
{}

void
TreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  using namespace edm;
  if( filleventInfo_)       eventInfo_     ->Fill(iEvent);
  if( fillpileUpInfo_)      pileUpInfo_    ->Fill(iEvent);
  if( filltriggerInfo_)     triggerInfo_   ->Fill(iEvent, iSetup, all_triggers, electron_triggers_in_run, muon_triggers_in_run,
						  emu_triggers_in_run,
						  hltConfig_, hltlabel_, MaxN);
  if( fillvertexInfo_)      vertexInfo_    ->Fill(iEvent);
  if( filltrackInfo_)       trackInfo_     ->Fill(iEvent);
  if( fillmuonInfo_)        muonInfo_      ->Fill(iEvent, iSetup);
  std::cout<<" muon info filled "<<std::endl;
  if( fillcosmicmuonInfo_)  cosmicmuonInfo_->Fill(iEvent);
  if( fillPhotInfo_)        photonInfo_    ->Fill(iEvent, iSetup);
  if( fillUcphotInfo_)      ucphotonInfo_  ->Fill(iEvent, iSetup);
  if( fillbeamhaloInfo_)    beamhaloInfo_  ->Fill(iEvent);
  if( fillcscrpcInfo_)      cscrpcInfo_    ->Fill(iEvent, iSetup);
  if( fillpfmetInfo_)       pfmetInfo_     ->Fill(iEvent);
  if(debug_)  std::cout<<" met info filled "<<std::endl;
  if( filltcmetInfo_)       tcmetInfo_     ->Fill(iEvent);
  if( fillcalometInfo_)     calometInfo_   ->Fill(iEvent);
  if( fillcalojetInfo_)     calojetInfo_   ->Fill(iEvent, iSetup);
  if(debug_)  std::cout<<" before pfjet info filled "<<std::endl;
  if( fillpfjetInfo_)       pfjetInfo_     ->Fill(iEvent, iSetup);
  if(debug_)  std::cout<<" after pfjet info filled "<<std::endl;
  if( fillgenjetInfo_)      genjetInfo_    ->Fill(iEvent);
  if( fillelectronInfo_)    electronInfo_  ->Fill(iEvent, iSetup);
  if( filltauInfo_)         tauInfo_       ->Fill(iEvent);
  if( fillhpstauInfo_)      hpstauInfo_    ->Fill(iEvent, iSetup);
  if( false/*&fillhpstauInfo_*/)    genhiggsInfo_->Fill(iEvent);
  if(debug_) std::cout<<" after filling taus "<<std::endl;
  if(fillrhoInfo_)          rhoInfo_       ->Fill(iEvent);
  if(debug_) std::cout<<"Filling tree"<<std::endl;
  tree_->Fill();
  if(debug_) std::cout<<"Filled tree"<<std::endl;
}

void
TreeMaker::beginJob(){

}

void
TreeMaker::endJob() {
  //tree_->Print();
  //tree_Draw("Pulse");
}

void TreeMaker::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup){
  if(debug_){
    cout<<"\n----------------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"----------------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"BEGIN NEW RUN: "<<iRun.run()<<endl;
  }
  
  cout<<"running"<<endl;
  bool changed(true);
  (hltConfig_.init(iRun,iSetup,"HLT",changed)) ;
  using namespace std;
  ntriggers = hltConfig_.size();
  electron_triggers_in_run.clear();
  muon_triggers_in_run.clear();
  emu_triggers_in_run.clear();
  all_triggers.clear();
  all_triggers_emu.clear();
  
  for ( int itrigger = 0 ; itrigger < ntriggers; itrigger++){
    //std::cout<<"name of triggers = "<<hltConfig_.triggerName(itrigger)<<std::endl;
    std::string hltname(hltConfig_.triggerName(itrigger));
    std::string string_doubleEle1 ("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL");
    std::string string_doubleEle2 ("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL");
    std::string string_doubleMu1("HLT_Mu17_TkMu8");
    std::string string_doubleMu2("HLT_Mu17_Mu8");
    
    //for moriond
    std::string string_emu1 ("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL");
    std::string string_emu2 ("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL");
    
    size_t found_doubleEle1  = hltname.find(string_doubleEle1);
    size_t found_doubleEle2  = hltname.find(string_doubleEle2);
    size_t found_doubleMu1   = hltname.find(string_doubleMu1);
    size_t found_doubleMu2   = hltname.find(string_doubleMu2);
    
    //for moriond
    size_t found_emu1 = hltname.find(string_emu1);
    size_t found_emu2 = hltname.find(string_emu2);
    
    
    if(found_doubleEle1 != string::npos ) {
      //cout<<"  "<<itrigger<<" Electron HLT path in this run\t"<<hltname<<endl;
      electron_triggers_in_run.push_back(hltname);
    }
    
    if(found_doubleEle2 != string::npos ) {
      //cout<<"  "<<itrigger<<" Electron HLT path in this run\t"<<hltname<<endl;
      electron_triggers_in_run.push_back(hltname);
    }
    
    if(found_doubleMu1 != string::npos ) {
      //cout<<"  "<<itrigger<<" Muon HLT path in this run\t"<<hltname<<endl;
      muon_triggers_in_run.push_back(hltname);
    }
    
    if(found_doubleMu2 != string::npos ) {
      //cout<<"  "<<itrigger<<" Muon HLT path in this run\t"<<hltname<<endl;
      muon_triggers_in_run.push_back(hltname);
    }
    
    if(found_emu1 != string::npos ){
      emu_triggers_in_run.push_back(hltname);
    }
    if(found_emu2 != string::npos ){
      emu_triggers_in_run.push_back(hltname);
    }
    

    //SAVE ELECTRON TRIGGER INFO
    for(int x = 0; x< (int)electron_triggers_in_run.size();x++){
      bool found = false;
      for(int i = 0; i< (int)all_triggers.size();i++){
	if(all_triggers[i]==electron_triggers_in_run[x]) found = true;
      }//loop all triggers  
      if(!found){
	all_triggers.push_back(electron_triggers_in_run[x]);
      }
    }//loop electron triggers
    
    //SAVE MUON TRIGGER INFO                                                                                                                        
    for(int x = 0; x< (int)muon_triggers_in_run.size();x++){
      bool found = false;
      for(int i = 0; i< (int)all_triggers.size();i++){
	if(all_triggers[i]==muon_triggers_in_run[x]) found = true;
      }//loop all triggers                                                                                                                          
      if(!found){
	all_triggers.push_back(muon_triggers_in_run[x]);
      }
    }

    //SAVE E-MU TRIGGER INFO
    for(int x = 0; x< (int)emu_triggers_in_run.size();x++){
      bool found = false;
      for(int i = 0; i< (int)all_triggers.size();i++){
	if(all_triggers[i]==emu_triggers_in_run[x]) found = true;
      }//loop all triggers                                                                                                                          
      if(!found){
	all_triggers_emu.push_back(emu_triggers_in_run[x]);
      }
    }


  }//loop muon triggers                                                                                                                           
  if(debug_){
    cout<<"----------------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"----------------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"the triggers in HLT list till now are:"<<endl;
    for(int i = 0; i< (int)all_triggers.size();i++) cout<<"\t"<<all_triggers[i]<<endl;
  }  
}



DEFINE_FWK_MODULE(TreeMaker);
