#ifndef  TREE_MAKER_H
#define  TREE_MAKER_H

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RKAnalyzer/TreeMaker/interface/genHiggsInfo.h"

#include "RKAnalyzer/TreeMaker/interface/eventInfo.h"
#include "RKAnalyzer/TreeMaker/interface/pileUpInfo.h"
#include "RKAnalyzer/TreeMaker/interface/triggerInfo.h"
#include "RKAnalyzer/TreeMaker/interface/vertexInfo.h"
#include "RKAnalyzer/TreeMaker/interface/trackInfo.h"
#include "RKAnalyzer/TreeMaker/interface/muonInfo.h"
#include "RKAnalyzer/TreeMaker/interface/cosmicmuonInfo.h"
#include "RKAnalyzer/TreeMaker/interface/photonInfo.h"
#include "RKAnalyzer/TreeMaker/interface/ucphotonInfo.h"
#include "RKAnalyzer/TreeMaker/interface/beamhaloInfo.h"
#include "RKAnalyzer/TreeMaker/interface/cscrpcInfo.h"
#include "RKAnalyzer/TreeMaker/interface/pfmetInfo.h"
#include "RKAnalyzer/TreeMaker/interface/tcmetInfo.h"
#include "RKAnalyzer/TreeMaker/interface/calometInfo.h"
#include "RKAnalyzer/TreeMaker/interface/calojetInfo.h"
#include "RKAnalyzer/TreeMaker/interface/pfjetInfo.h"
#include "RKAnalyzer/TreeMaker/interface/genjetInfo.h"
#include "RKAnalyzer/TreeMaker/interface/electronInfo.h"
#include "RKAnalyzer/TreeMaker/interface/tauInfo.h"
#include "RKAnalyzer/TreeMaker/interface/calotowerInfo.h"
#include "RKAnalyzer/TreeMaker/interface/rhoInfo.h"
#include "RKAnalyzer/TreeMaker/interface/hpstauInfo.h"

#include "TTree.h"
#include "TFile.h"

#ifdef __MAKECINT__
#pragma link C++ class vector<std::vector<int> >+;
#pragma link C++ class vector<std::vector<std::string> >+;
#pragma link C++ class vector<std::vector<float> >+;
#pragma link C++ class vector<std::vector<bool> >+;
#pragma extra_include "vector";
#endif

class TreeMaker : public edm::EDAnalyzer {
   public:
      explicit TreeMaker(const edm::ParameterSet&);
      ~TreeMaker();


   private:
      virtual void beginJob() ;
      virtual void beginRun(const edm::Run& , const edm::EventSetup&);
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      TFile* file;
      TTree* tree_;
      const size_t MaxN;
      
      std::vector<std::string> electron_triggers_in_run;
      std::vector<std::string> muon_triggers_in_run;
      std::vector<std::string> emu_triggers_in_run;
      
      std::vector<std::string> all_triggers;
      std::vector<std::string> all_triggers_emu;
      int ntriggers ;
      std::vector <int> all_triggerprescales;
      std::vector <bool> all_ifTriggerpassed;

      bool debug_;
      bool fillPhotInfo_;
      bool fillUcphotInfo_;
      bool filleventInfo_;
      bool fillpileUpInfo_;
      bool filltriggerInfo_;
      bool fillvertexInfo_;
      bool filltrackInfo_;
      bool fillmuonInfo_;
      bool fillcosmicmuonInfo_;
      bool fillbeamhaloInfo_;
      bool fillcscrpcInfo_;
      bool fillpfmetInfo_;
      bool filltcmetInfo_;
      bool fillcalometInfo_;
      bool fillcalojetInfo_;
      bool fillpfjetInfo_;
      bool fillgenjetInfo_;
      bool fillelectronInfo_;
      bool filltauInfo_;
      bool fillcalotowerInfo_;
      bool fillrhoInfo_;
      bool fillhpstauInfo_;
      bool fillgenhiggsInfo_;

      photonInfo      *photonInfo_;
      ucphotonInfo    *ucphotonInfo_;
      eventInfo       *eventInfo_;
      pileUpInfo      *pileUpInfo_;
      triggerInfo     *triggerInfo_;
      vertexInfo      *vertexInfo_;
      trackInfo       *trackInfo_;
      muonInfo        *muonInfo_;
      beamhaloInfo    *beamhaloInfo_;
      cscrpcInfo      *cscrpcInfo_;
      pfmetInfo       *pfmetInfo_;
      tcmetInfo       *tcmetInfo_;
      calometInfo     *calometInfo_;
      calojetInfo     *calojetInfo_;
      pfjetInfo       *pfjetInfo_;
      genjetInfo      *genjetInfo_;
      electronInfo    *electronInfo_;
      tauInfo         *tauInfo_;
      calotowerInfo   *calotowerInfo_;
      cosmicmuonInfo  *cosmicmuonInfo_;
      rhoInfo         *rhoInfo_;
      hpstauInfo      *hpstauInfo_;

      genHiggsInfo    *genhiggsInfo_;
      
      HLTConfigProvider hltConfig_;
      std::string hltlabel_;
      
};


#endif
