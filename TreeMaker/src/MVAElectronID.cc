#include "../interface/MVAElectronID.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/DataKeyTags.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include<iostream>
//MVAElectronID::~MVAElectronID(){}
MVAElectronID::MVAElectronID(){
  myMVANonTrig = new EGammaMvaEleEstimator();
  
  //myManualCatWeigths.push_back(edm::FileInPath("RKAnalyzer/TreeMaker/data/Electrons_BDTG_NonTrigV0_Cat1.weights.xml"));
  //myManualCatWeigths.push_back(edm::FileInPath("RKAnalyzer/TreeMaker/data/Electrons_BDTG_NonTrigV0_Cat2.weights.xml"));
  //myManualCatWeigths.push_back(edm::FileInPath("RKAnalyzer/TreeMaker/data/Electrons_BDTG_NonTrigV0_Cat3.weights.xml"));
  //myManualCatWeigths.push_back(edm::FileInPath("RKAnalyzer/TreeMaker/data/Electrons_BDTG_NonTrigV0_Cat4.weights.xml"));
  //myManualCatWeigths.push_back(edm::FileInPath("RKAnalyzer/TreeMaker/data/Electrons_BDTG_NonTrigV0_Cat5.weights.xml"));
  //myManualCatWeigths.push_back(edm::FileInPath("RKAnalyzer/TreeMaker/data/Electrons_BDTG_NonTrigV0_Cat6.weights.xml"));

  edm::FileInPath NonTrigFile1("RKAnalyzer/TreeMaker/data/Electrons_BDTG_NonTrigV0_Cat1.weights.xml");
  edm::FileInPath NonTrigFile2("RKAnalyzer/TreeMaker/data/Electrons_BDTG_NonTrigV0_Cat2.weights.xml");
  edm::FileInPath NonTrigFile3("RKAnalyzer/TreeMaker/data/Electrons_BDTG_NonTrigV0_Cat3.weights.xml");
  edm::FileInPath NonTrigFile4("RKAnalyzer/TreeMaker/data/Electrons_BDTG_NonTrigV0_Cat4.weights.xml");
  edm::FileInPath NonTrigFile5("RKAnalyzer/TreeMaker/data/Electrons_BDTG_NonTrigV0_Cat5.weights.xml");
  edm::FileInPath NonTrigFile6("RKAnalyzer/TreeMaker/data/Electrons_BDTG_NonTrigV0_Cat6.weights.xml");
  myManualCatWeigths.push_back(NonTrigFile1.fullPath());
  myManualCatWeigths.push_back(NonTrigFile2.fullPath());
  myManualCatWeigths.push_back(NonTrigFile3.fullPath());
  myManualCatWeigths.push_back(NonTrigFile4.fullPath());
  myManualCatWeigths.push_back(NonTrigFile5.fullPath());
  myManualCatWeigths.push_back(NonTrigFile6.fullPath());
  Bool_t manualCat = true;
  
  myMVANonTrig->initialize("BDT",
                           EGammaMvaEleEstimator::kNonTrig,
                           manualCat,
                           myManualCatWeigths);
  
  //myManualCatWeigthsTrig.push_back(edm::FileInPath("RKAnalyzer/TreeMaker/data/Electrons_BDTG_TrigV0_Cat1.weights.xml"));
  //myManualCatWeigthsTrig.push_back(edm::FileInPath("RKAnalyzer/TreeMaker/data/Electrons_BDTG_TrigV0_Cat2.weights.xml"));
  //myManualCatWeigthsTrig.push_back(edm::FileInPath("RKAnalyzer/TreeMaker/data/Electrons_BDTG_TrigV0_Cat3.weights.xml"));
  //myManualCatWeigthsTrig.push_back(edm::FileInPath("RKAnalyzer/TreeMaker/data/Electrons_BDTG_TrigV0_Cat4.weights.xml"));
  //myManualCatWeigthsTrig.push_back(edm::FileInPath("RKAnalyzer/TreeMaker/data/Electrons_BDTG_TrigV0_Cat5.weights.xml"));
  //myManualCatWeigthsTrig.push_back(edm::FileInPath("RKAnalyzer/TreeMaker/data/Electrons_BDTG_TrigV0_Cat6.weights.xml"));
  edm::FileInPath TrigFile1("RKAnalyzer/TreeMaker/data/Electrons_BDTG_TrigV0_Cat1.weights.xml");
  edm::FileInPath TrigFile2("RKAnalyzer/TreeMaker/data/Electrons_BDTG_TrigV0_Cat2.weights.xml");
  edm::FileInPath TrigFile3("RKAnalyzer/TreeMaker/data/Electrons_BDTG_TrigV0_Cat3.weights.xml");
  edm::FileInPath TrigFile4("RKAnalyzer/TreeMaker/data/Electrons_BDTG_TrigV0_Cat4.weights.xml");
  edm::FileInPath TrigFile5("RKAnalyzer/TreeMaker/data/Electrons_BDTG_TrigV0_Cat5.weights.xml");
  edm::FileInPath TrigFile6("RKAnalyzer/TreeMaker/data/Electrons_BDTG_TrigV0_Cat6.weights.xml");
  myManualCatWeigthsTrig.push_back(TrigFile1.fullPath());
  myManualCatWeigthsTrig.push_back(TrigFile2.fullPath());
  myManualCatWeigthsTrig.push_back(TrigFile3.fullPath());
  myManualCatWeigthsTrig.push_back(TrigFile4.fullPath());
  myManualCatWeigthsTrig.push_back(TrigFile5.fullPath());
  myManualCatWeigthsTrig.push_back(TrigFile6.fullPath());
  
  myMVATrig = new EGammaMvaEleEstimator();
  myMVATrig->initialize("BDT",
                        EGammaMvaEleEstimator::kTrig,
                        manualCat,
                        myManualCatWeigthsTrig);
}


double MVAElectronID::FindMVA(const edm::Event& iEvent,const edm::EventSetup& iSetup, pat::Electron ele){
  //may be in constructor
  
  //may be upward in cons


  //InputTag gsfEleLabel(string("gsfElectrons"));
  //Handle<GsfElectronCollection> theEGammaCollection;
  //iEvent.getByLabel(gsfEleLabel,theEGammaCollection);
  //const GsfElectronCollection theEGamma = *(theEGammaCollection.product());


  //InputTag  vertexLabel(string("offlinePrimaryVertices"));
  InputTag  vertexLabel(string("offlineSlimmedPrimaryVertices"));
  Handle<reco::VertexCollection> thePrimaryVertexColl;
  iEvent.getByLabel(vertexLabel,thePrimaryVertexColl);

  Vertex dummy;
  const Vertex *pv = &dummy;
  if (thePrimaryVertexColl->size() != 0) {
    pv = &*thePrimaryVertexColl->begin();
  } else { // create a dummy PV                                                                                                                                         
    Vertex::Error e;
    e(0, 0) = 0.0015 * 0.0015;
    e(1, 1) = 0.0015 * 0.0015;
    e(2, 2) = 15. * 15.;
    Vertex::Point p(0, 0, 0);
    dummy = Vertex(p, e, 0, 0, 0);
  }
  InputTag  reducedEBRecHitCollection(string("reducedEcalRecHitsEB"));
  InputTag  reducedEERecHitCollection(string("reducedEcalRecHitsEE"));

  EcalClusterLazyTools lazyTools(iEvent, iSetup, reducedEBRecHitCollection, reducedEERecHitCollection);
  
  edm::ESHandle<TransientTrackBuilder> builder;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", builder);
  TransientTrackBuilder thebuilder = *(builder.product());
  
  
  //  bool debug = true;
  bool debugMVAclass = false;
  //bool debugMyVar = false;
  
  //for (uint j=0; j<theEGamma.size();j++) {
    myMVANonTrigMethod1 = myMVANonTrig->mvaValue((ele),*pv,thebuilder,lazyTools,debugMVAclass);
    myMVATrigMethod1 = myMVATrig->mvaValue((ele),*pv,thebuilder,lazyTools,debugMVAclass);
    //std::cout<<" Raman MVA in class ="<<myMVANonTrigMethod1<<std::endl;
    //  }
  //myMVANonTrigMethod1=1.0;

  return myMVANonTrigMethod1; 
}
