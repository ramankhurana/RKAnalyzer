import FWCore.ParameterSet.Config as cms
process = cms.Process("PAT")
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
#process.load("Configuration.StandardSequences.Reconstruction_cff")

process.GlobalTag.globaltag = cms.string("GR_R_70_V2::All")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

#from PhysicsTools.PatAlgos.patTemplate_cfg import *
process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, ['All'], outputModules = [])

#from PhysicsTools.PatAlgos.tools.metTools import *
#addPfMET(process,'PF')
#addTcMET(process,"TC")

# Add the files 
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
readFiles.extend( [
        'root://eoscms//eos/cms/store/user/khurana/CSA14/AODForTry/DYJetsToLL_M-50_13TeV-pythia6_Spring14dr_PU20bx25_POSTLS170_V5-v1.root'
        ] );

process.source = cms.Source("PoolSource",
    fileNames = readFiles
)

#closes files after code is done running on that file
process.options = cms.untracked.PSet(
	fileMode = cms.untracked.string('NOMERGE')
)


###################Electron corrections##################
process.load('EgammaAnalysis.ElectronTools.electronRegressionEnergyProducer_cfi')
process.load("EgammaAnalysis.ElectronTools.calibratedPatElectrons_cfi")

# dataset to correct
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                                                   calibratedPatElectrons = cms.PSet(
    initialSeed = cms.untracked.uint32(1),
    engineName = cms.untracked.string('TRandom3')
    )
                                                   )

process.calibratedPatElectrons.isAOD = cms.bool(False)
process.calibratedPatElectrons.isMC = cms.bool(True)
process.calibratedPatElectrons.inputDataset = cms.string("Summer12_DR53X_HCP2012")
process.calibratedPatElectrons.updateEnergyError = cms.bool(True)
process.calibratedPatElectrons.applyCorrections = cms.int32(0)
#process.calibratedPatElectrons.verbose = cms.bool(True)
#process.calibratedPatElectrons.synchronization = cms.bool(True)

### for MC only
process.correctelectron = cms.Sequence(process.eleRegressionEnergy * process.calibratedPatElectrons)
###################Electron corrections##################



#gsfElectrons

process.load("CommonTools.ParticleFlow.PFBRECO_cff")
from CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff import *
##elPFIsoDepositCharged.src    = cms.InputTag("gsfElectrons")
##elPFIsoDepositChargedAll.src = cms.InputTag("gsfElectrons")
##elPFIsoDepositNeutral.src    = cms.InputTag("gsfElectrons")
##elPFIsoDepositGamma.src      = cms.InputTag("gsfElectrons")
##elPFIsoDepositPU.src         = cms.InputTag("gsfElectrons")
##
process.load("CommonTools.ParticleFlow.PFBRECO_cff")
from CommonTools.ParticleFlow.Isolation.pfMuonIsolation_cff import *
##muPFIsoDepositCharged.src    = cms.InputTag("muons")
##muPFIsoDepositChargedAll.src = cms.InputTag("muons")
##muPFIsoDepositNeutral.src    = cms.InputTag("muons")
##muPFIsoDepositGamma.src      = cms.InputTag("muons")
##muPFIsoDepositPU.src         = cms.InputTag("muons")
##
##
##process.patMuons.isolationValues = cms.PSet(
##    pfAllParticles   = cms.InputTag("muPFIsoValueChargedAll04"),
##    pfChargedHadrons = cms.InputTag("muPFIsoValueCharged04"),
##    pfNeutralHadrons = cms.InputTag("muPFIsoValueNeutral04"),
##    pfPhotons        = cms.InputTag("muPFIsoValueGamma04"),
##    user             = cms.VInputTag(cms.InputTag("muPFIsoValuePU04")
##                                     )
##    )
####
##process.elPFIsoValueChargedAll04PFId = cms.EDProducer("CandIsolatorFromDeposits",
##                                                      deposits = cms.VPSet(cms.PSet(
##    src = cms.InputTag("elPFIsoDepositChargedAll"),
##    deltaR = cms.double(0.4),
##    weight = cms.string('1'),
##    vetos = cms.vstring('EcalBarrel:ConeVeto(0.01)','EcalEndcaps:ConeVeto(0.015)'),
##    skipDefaultVeto = cms.bool(True),
##    mode = cms.string('sum')
##    ))
##                                                      )
##
##
##
##process.elPFIsoValueGamma04PFId.deposits[0].vetos=  cms.vstring('EcalBarrel:ConeVeto(0.08)','EcalEndcaps:ConeVeto(0.08)')
##
process.patElectrons.isolationValues = cms.PSet(
    pfAllParticles   = cms.InputTag("elPFIsoValueChargedAll04PFId"),
    pfChargedHadrons = cms.InputTag("elPFIsoValueCharged04PFId"),
    pfNeutralHadrons = cms.InputTag("elPFIsoValueNeutral04PFId"),
    pfPhotons        = cms.InputTag("elPFIsoValueGamma04PFId"),
    user             = cms.VInputTag(cms.InputTag("elPFIsoValuePU04PFId")
                                     )
    )

process.patElectrons.isolationValuesNoPFId = cms.PSet(
    pfAllParticles   = cms.InputTag("elPFIsoValueChargedAll04NoPFId"),
    pfChargedHadrons = cms.InputTag("elPFIsoValueCharged04NoPFId"),
    pfNeutralHadrons = cms.InputTag("elPFIsoValueNeutral04NoPFId"),
    pfPhotons        = cms.InputTag("elPFIsoValueGamma04NoPFId"),
    user             = cms.VInputTag(cms.InputTag("elPFIsoValuePU04NoPFId")
                                     )
    )



###PFIsolation

process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.load("RecoJets.JetProducers.kt4PFJets_cfi")
process.kt6PFJets = process.kt4PFJets.clone()
process.kt6PFJets.rParam = 0.6
process.kt6PFJets.doRhoFastjet = True
process.kt6PFJets.Rho_EtaMax = cms.double(2.5)
process.kt6PFJets.Ghost_EtaMax = cms.double(2.5)
process.kt6PFJets.doAreaFastjet = True

process.load('RecoTauTag/Configuration/RecoPFTauTag_cff')
process.load("PhysicsTools.PatAlgos.patSequences_cff")

from PhysicsTools.PatAlgos.tools.tauTools import *
switchToPFTauHPS(process)
process.load("PhysicsTools.PatAlgos.patSequences_cff")
#removeMCMatching(process,['All'])


# Add PFJet
from PhysicsTools.PatAlgos.tools.jetTools import *                                                                                                                    
##addJetCollection(process,cms.InputTag('ak5PFJets'),
##                 'AK5', 'PF',
##                 doJTA        = True,
##                 #doBTagging   = True,
##                 jetCorrLabel = ('AK5PF', cms.vstring(['L1FastJet','L2Relative','L3Absolute'])),
##                 doType1MET    = True,
##                 doL1Cleaning  = True,
##                 doL1Counters  = False,
##                 genJetCollection=cms.InputTag("ak5GenJets"),
##                 doJetID       = False#,
##                 #jetIdLabel    = "ak5"
##                 )
##
##
## Add Calo Jet
process.patJetCorrFactors.levels = cms.vstring(['L1Offset','L2Relative','L3Absolute'])

## Otgher type of rho corrections
process.kt6PFJets25 = process.kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJets25.Rho_EtaMax = cms.double(2.5)
process.kt6PFJets25.Ghost_EtaMax = cms.double(2.5)
process.fastjetSequence25 = cms.Sequence( process.kt6PFJets25 )

process.kt6PFJets44 = process.kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJets44.Rho_EtaMax = cms.double(4.4)
process.kt6PFJets44.Ghost_EtaMax = cms.double(5.0)
process.fastjetSequence44 = cms.Sequence( process.kt6PFJets44 )

process.load("RecoJets.JetProducers.ak5PFJets_cfi")
process.ak5PFJets.doAreaFastjet = True



process.content = cms.EDAnalyzer("EventContentAnalyzer")

process.demo = cms.EDAnalyzer('TreeMaker',
                              debug_               = cms.untracked.bool(False),
                              filleventInfo_       = cms.untracked.bool(True),
                              lumiLabel_           = cms.untracked.InputTag("conditionsInEdm"),
                              lumiProducerLabel_   = cms.untracked.InputTag("lumiProducer"),
                              fillpileUpInfo_      = cms.untracked.bool(True),
                              pileUpLabel_         = cms.untracked.InputTag("addPileupInfo"),
                              filltriggerInfo_     = cms.untracked.bool(True),
                              hltlabel_            = cms.untracked.string("HLT"),
                              HLTriggerResults_    = cms.untracked.InputTag("TriggerResults","","HLT"),
                              triggerEventTag_     = cms.untracked.InputTag("hltTriggerSummaryAOD","","HLT"),
                              fillvertexInfo_      = cms.untracked.bool(True),
                              vertexLabel_         = cms.untracked.InputTag("offlinePrimaryVertices","",""),
                              filltrackInfo_       = cms.untracked.bool(True),
                              trackLabel_          = cms.untracked.InputTag("generalTracks"),
                              fillmuonInfo_        = cms.untracked.bool(True),
                              muonLabel_           = cms.untracked.InputTag("selectedPatMuons"),
                              fillcosmicmuonInfo_  = cms.untracked.bool(False),
                              cosmicmuonLabel_     = cms.untracked.InputTag("muonsFromCosmics"),
                              isAOD_               = cms.untracked.bool(True),
                              fillPhotInfo_        = cms.untracked.bool(False),
                              photonLabel_         = cms.untracked.InputTag("selectedPatPhotons"),
                              fillUcphotInfo_      = cms.untracked.bool(False),
                              ucphotonLabel_       = cms.untracked.InputTag("selecteduncleanPatPhotons"),
                              fillHErechitInfo_    = cms.untracked.bool(False),
                              hcalrechitLabel_     = cms.untracked.InputTag("reducedHcalRecHits:hbhereco"),
                              fillrechitInfo_      = cms.untracked.bool(False),
                              rechitBLabel_        = cms.untracked.InputTag("reducedEcalRecHitsEB"),
                              rechitELabel_        = cms.untracked.InputTag("reducedEcalRecHitsEE"),
                              fillgenMatchInfo_    = cms.untracked.bool(False),
                              fillbeamhaloInfo_    = cms.untracked.bool(False),
                              beamhaloLabel_       = cms.untracked.InputTag("BeamHaloSummary"),
                              fillcscrpcInfo_      = cms.untracked.bool(False),
                              cscLabel_            = cms.untracked.InputTag("cscSegments"),
                              rpcLabel_            = cms.untracked.InputTag("rpcRecHits"),
                              fillpfmetInfo_       = cms.untracked.bool(True),
                              #pfmetLabel_          = cms.untracked.InputTag("patMETsPF"),
                              pfmetLabel_          = cms.untracked.InputTag("patMETs"),
                              filltcmetInfo_       = cms.untracked.bool(False),
                              tcmetLabel_          = cms.untracked.InputTag("patMETsTC"),
                              fillcalometInfo_     = cms.untracked.bool(False),
                              fillgenmetInfo_      = cms.untracked.bool(False),
                              calometLabel_        = cms.untracked.InputTag("patMETs"),
                              fillcalojetInfo_     = cms.untracked.bool(False),
                              calojetLabel_        = cms.untracked.InputTag("selectedPatJets"),
                              fillpfjetInfo_       = cms.untracked.bool(True),
                              #pfjetLabel_          = cms.untracked.InputTag("selectedPatJetsAK5PF"),  ## used for 2012
                              ### Change the input for pat jets :: it should be PFJets not AK5PFJetsCHS
                              pfjetLabel_          = cms.untracked.InputTag("selectedPatJets"), ## by default it is AK5PFJetsCHS :: change it to PF jets later
                              fillgenjetInfo_      = cms.untracked.bool(False),
                              genjetLabel_         = cms.untracked.InputTag("ak5GenJets"),
                              fillelectronInfo_    = cms.untracked.bool(True),
                              electronLabel_       = cms.untracked.InputTag("selectedPatElectrons"), ## seelcted pat electrons
                              #electronLabel_       = cms.untracked.InputTag("calibratedPatElectrons"), ## this one was used for 2012
                              #electronLabel_       = cms.untracked.InputTag("eleRegressionEnergy"),  ## i dont know why i kept this one
                              filltauInfo_         = cms.untracked.bool(False),## this is not HPS Tau this is gen tau
                              tauLabel_            = cms.untracked.InputTag("selectedPatTaus"),
                              fillDetailTauInfo_   = cms.untracked.bool(True),
                              fillcalotowerInfo_   = cms.untracked.bool(True),
                              calotowerLabel_      = cms.untracked.InputTag("towerMaker"),
                              fillrhoInfo_         = cms.untracked.bool(True),
                              fillhpstauInfo_      = cms.untracked.bool(True),
                              rhoLabel_            = cms.untracked.InputTag("kt6PFJets", "rho"),
                              sigmaLabel_          = cms.untracked.InputTag("kt6PFJets25", "sigma"),
                              rho44Label_          = cms.untracked.InputTag("kt6PFJets44", "rho"),
                              sigma44Label_        = cms.untracked.InputTag("kt6PFJets44", "sigma"),
                              )


process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("patTupleForHZZto2l2Tauanalysis.root"),
                                   #fileName = cms.string("/lustre/home/khurana/ZZ_2e2mu_batch.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )

process.load('RecoTauTag/Configuration/RecoPFTauTag_cff')
process.load("PhysicsTools.PatAlgos.patSequences_cff")

from PhysicsTools.PatAlgos.tools.tauTools import *
switchToPFTauHPS(process)

process.p = cms.Path(
    process.kt6PFJets*
    process.fastjetSequence25*
    process.fastjetSequence44*
    process.ak5PFJets*
    process.PFTau*
    ### These three commented for 13 TeV
    process.pfParticleSelectionSequence                 *
    process.pfElectronIsolationSequence                *
    process.pfMuonIsolationSequence                     *
    process.patDefaultSequence*
    #process.correctelectron*
    process.demo
    )


# reduce verbosity
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)

# process all the events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

#process.schedule=cms.Schedule(process.p)


#print process.dumpPython()




#processDumpFile = open('producePatTupleForOneProngStudy.dump' , 'w')
#print >> processDumpFile, process.dumpPython()   
