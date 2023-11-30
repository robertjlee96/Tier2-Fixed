#include "RecoMuon/TrackerSeedGenerator/interface/SeedMvaEstimator.h"

#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "CommonTools/MVAUtils/interface/GBRForestTools.h"

MvaCalculator::MvaCalculator(const edm::FileInPath& weightsfile)
:{
    gbrForest_ = createGBRForest(weightsfile);
}

MvaCalculator::~MvaCalculator() {}

namespace {
enum inputIndexes {
    rawEnergy,         // 0
    r9HLT,             // 1
    sigmaIEtaIEta,     // 2
    etaWidth,          // 3
    phiWidth,          // 4
    s4,                // 5
    eta,               // 6
    hOvrE,             // 7
    ecalPFIso,         // 8

};
}  // namespace

double MvaCalculator::calcS4(edm::Event& evt, const reco::RecoEcalCandidateCollection& candCollection){
    double s4 = 0;
    
    
    return s4;
}

double MvaCalculator::computeMva(edm::Event& evt, const reco::RecoEcalCandidateCollection& candCollection) const {
    float var[10];
    //RawEnergy, SiEiE, etaWidth & phiWidth all come from SuperCluster()
    var[rawEnergy] = candCollection.SuperCluster().rawEnergy();
    var[sigmaIEtaIEta] = candCollection.SuperCluster().rawEnergy();
    var[etaWidth] = candCollection.SuperCluster().etaWidth();
    var[phiWidth] = candCollection.SuperCluster().phiWidth();
    var[eta] = candCollection.SuperCluster().eta();
    //Must calculate s4 from supercluster()
    var[s4] = MvaCalculator::calcS4();
    //Other 2 variables come from trigger.EgammaObject (according to Sam's NTuple clode)
    var[hOvrE] = seed.startingState().parameters().dydz();
    var[ecalPFIso] = seed.startingState().parameters().dydz();
   
    
    return gbrForest_->GetResponse(var);
}
