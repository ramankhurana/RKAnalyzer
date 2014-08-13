#ifndef Analyzer_HZZAnalyzer_MVAElectronID_h
#define Analyzer_HZZAnalyzer_MVAElectronID_h 1
#include <memory>
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include <DataFormats/MuonReco/interface/Muon.h>
#include <DataFormats/MuonReco/interface/MuonFwd.h>
#include "DataFormats/Candidate/interface/Candidate.h"

#include "TLorentzVector.h"
#include <vector>
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
//for electron MVA
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaReco/interface/ElectronSeed.h"
#include "DataFormats/EgammaReco/interface/ElectronSeedFwd.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "RecoParticleFlow/PFProducer/interface/Utils.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "EgammaAnalysis/ElectronTools/interface/EGammaMvaEleEstimator.h"
//#include "EGamma/EGammaAnalysisTools/interface/EGammaMvaEleEstimator.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonTimeExtra.h"

// user include files                                                                                                                                                   
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include <cmath>
#include <vector>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TLorentzVector.h>
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"


using namespace reco;
using namespace pat;
using namespace edm;
class MVAElectronID {
 public :
  MVAElectronID();
  ~MVAElectronID(){}
  double myMVANonTrigMethod1;
  double myMVATrigMethod1 ;
  double FindMVA(const edm::Event& iEvent,const edm::EventSetup& iSetup, pat::Electron ele);
  EGammaMvaEleEstimator* myMVANonTrig;
  EGammaMvaEleEstimator* myMVATrig;
  std::vector<std::string> myManualCatWeigths;
  std::vector<std::string> myManualCatWeigthsTrig;

 private:
};
#endif
