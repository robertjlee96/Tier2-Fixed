#ifndef MVAProducer_MVAProducerTest_MVACalculator_h
#define MVAProducer_MVAProducerTest_MVACalculator_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "HLTrigger/HLTcore/interface/HLTFilter.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/RefToBase.h"

#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateIsolation.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

#include <memory>

class GBRForest;

namespace edm {
class FileInPath;
}

class MvaCalculator {
public:
    MvaCalculator(const edm::FileInPath& weightsfile);
    ~MvaCalculator();
    
    double computeMva(const RecoCondidate&,
                      const GlobalVector&,
                      const reco::RecoEcalCandidateCollection&) const;
    
private:
    std::unique_ptr<const GBRForest> gbrForest_;

};
#endif
