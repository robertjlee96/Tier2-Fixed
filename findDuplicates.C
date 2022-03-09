#include "TROOT.h"
#include "TKey.h"
#include "TFile.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TObjArray.h"
#include "THStack.h"
#include "TLegend.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"
#include "TMath.h"
#include "TCut.h"
#include "TPaletteAxis.h"
#include "TMVA/Reader.h"
#include "../test/Event.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

    //ROBERT'S SPLITTING -- Even->Validation   Odd->Training

void findDuplicates(){
    
    cout<<"TEST"<<endl;

    TFile* fileIn = TFile::Open("./NTuples/GJets_M95PTM25_HovrE_DPT075_1117.root");
    TTreeReader myReader("diphotonDumper/trees/GJets",fileIn);
    TTree* treeIn = (TTree*)fileIn->Get("diphotonDumper/trees/GJets");

    TTreeReaderValue<unsigned long long> eventIn(myReader, "event");
    TTreeReaderValue<Float_t> leadGenMatchType(myReader, "leadGenMatchType");
    TTreeReaderValue<Float_t> subGenMatchType(myReader, "subGenMatchType");
    
    int nPairsSameGenMatch;
    int nSameEventMultiplePairs;
    
    int eventValsLast = 0;
    int index = 0;

    while (myReader.Next()){
        if(((int)*eventIn)%2 != 0){
            if (index != 0 && (int)*eventIn == eventValsLast)nSameEventMultiplePairs+=1;
            index += 1;
            eventValsLast = (int)*eventIn;
            if (*leadGenMatchType == *subGenMatchType){
                nPairsSameGenMatch+=1;
                cout<<"EventNumber = "<<(int)*eventIn<<endl;
            }
        }
    }
    cout<<"Number of Pairs with Same Gen Match Type = "<<nPairsSameGenMatch<<endl;
    cout<<"Number of Events W/ Multiple Pairs = "<<nSameEventMultiplePairs<<endl;

    
}



//
    
    
//    TFile* fileOut = TFile::Open("NTuples/SinglePhoton_GJet_M95PTM25_DPT075_1208_Training.root","RECREATE");
//
//    string treeStrs[2] = {"promptPhotons","fakePhotons"};
//
//    TTree* treeOutP = new TTree(treeStrs[0].c_str(),treeStrs[0].c_str());
//    TTree* treeOutF = new TTree(treeStrs[1].c_str(),treeStrs[1].c_str());
//
//    Float_t hggMassP,genPtP,genMatchTypeP,ptP,weightP,scRawEP,r9P,sigmaIetaIetaP,etaWidthP,phiWidthP,covIEtaIPhiP,s4P,phoIso03P,chgIsoWrtChosenVtxP,chgIsoWrtWorstVtxP,scEtaP,esEffSigmaRRP,esEnergyOverRawEP,rhoP,hadTowOverEmP,hadronicOverEmP;
//    treeOutP->Branch("hggMass",&hggMassP,"hggMass/F");
//    treeOutP->Branch("genPt",&genPtP,"genPt/F");
//    treeOutP->Branch("genMatchType",&genMatchTypeP,"genMatchType/F");
//    treeOutP->Branch("pt",&ptP,"pt/F");
//    treeOutP->Branch("weight",&weightP,"weight/F");
//    treeOutP->Branch("SCRawE",&scRawEP,"SCRawE/F");
//    treeOutP->Branch("r9",&r9P,"r9/F");
//    treeOutP->Branch("sigmaIetaIeta",&sigmaIetaIetaP,"sigmaIetaIeta/F");
//    treeOutP->Branch("etaWidth",&etaWidthP,"etaWidth/F");
//    treeOutP->Branch("phiWidth",&phiWidthP,"phiWidth/F");
//    treeOutP->Branch("covIEtaIPhi",&covIEtaIPhiP,"covIEtaIPhi/F");
//    treeOutP->Branch("covIEtaIPhi",&covIEtaIPhiP,"covIEtaIPhi/F");
//    treeOutP->Branch("s4",&s4P,"s4/F");
//    treeOutP->Branch("phoIso03",&phoIso03P,"phoIso03/F");
//    treeOutP->Branch("chgIsoWrtChosenVtx",&chgIsoWrtChosenVtxP,"chgIsoWrtChosenVtx/F");
//    treeOutP->Branch("chgIsoWrtWorstVtx",&chgIsoWrtWorstVtxP,"chgIsoWrtWorstVtx/F");
//    treeOutP->Branch("scEta",&scEtaP,"scEta/F");
//    treeOutP->Branch("esEffSigmaRR",&esEffSigmaRRP,"esEffSigmaRR/F");
//    treeOutP->Branch("esEnergyOverRawE",&esEnergyOverRawEP,"esEnergyOverRawE/F");
//    treeOutP->Branch("hadTowOverEm",&hadTowOverEmP,"hadTowOverEm/F");
//    treeOutP->Branch("hadronicOverEm",&hadronicOverEmP,"hadronicOverEm/F");
//    treeOutP->Branch("rho",&rhoP,"rho/F");
//
//    Float_t hggMassF,genPtF,genMatchTypeF,ptF,weightF,scRawEF,r9F,sigmaIetaIetaF,etaWidthF,phiWidthF,covIEtaIPhiF,s4F,phoIso03F,chgIsoWrtChosenVtxF,chgIsoWrtWorstVtxF,scEtaF,esEffSigmaRRF,esEnergyOverRawEF,rhoF,hadTowOverEmF,hadronicOverEmF;
//    treeOutF->Branch("hggMass",&hggMassF,"hggMass/F");
//    treeOutF->Branch("genPt",&genPtF,"genPt/F");
//    treeOutF->Branch("genMatchType",&genMatchTypeF,"genMatchType/F");
//    treeOutF->Branch("pt",&ptF,"pt/F");
//    treeOutF->Branch("weight",&weightF,"weight/F");
//    treeOutF->Branch("SCRawE",&scRawEF,"SCRawE/F");
//    treeOutF->Branch("r9",&r9F,"r9/F");
//    treeOutF->Branch("sigmaIetaIeta",&sigmaIetaIetaF,"sigmaIetaIeta/F");
//    treeOutF->Branch("etaWidth",&etaWidthF,"etaWidth/F");
//    treeOutF->Branch("phiWidth",&phiWidthF,"phiWidth/F");
//    treeOutF->Branch("covIEtaIPhi",&covIEtaIPhiF,"covIEtaIPhi/F");
//    treeOutF->Branch("covIEtaIPhi",&covIEtaIPhiF,"covIEtaIPhi/F");
//    treeOutF->Branch("s4",&s4F,"s4/F");
//    treeOutF->Branch("phoIso03",&phoIso03F,"phoIso03/F");
//    treeOutF->Branch("chgIsoWrtChosenVtx",&chgIsoWrtChosenVtxF,"chgIsoWrtChosenVtx/F");
//    treeOutF->Branch("chgIsoWrtWorstVtx",&chgIsoWrtWorstVtxF,"chgIsoWrtWorstVtx/F");
//    treeOutF->Branch("scEta",&scEtaF,"scEta/F");
//    treeOutF->Branch("esEffSigmaRR",&esEffSigmaRRF,"esEffSigmaRR/F");
//    treeOutF->Branch("esEnergyOverRawE",&esEnergyOverRawEF,"esEnergyOverRawE/F");
//    treeOutF->Branch("hadTowOverEm",&hadTowOverEmF,"hadTowOverEm/F");
//    treeOutF->Branch("hadronicOverEm",&hadronicOverEmF,"hadronicOverEm/F");
//    treeOutF->Branch("rho",&rhoF,"rho/F");
//
//    while (myReader.Next()){
//        if(((int)*eventIn)%2 != 0){
//            if (*hggMassIn > 95.0 && *leadGenMatchType == 1.0 && *leadPt > 17.0 && *leadPt/(*hggMassIn) > 0.25 && abs(*leadGenPt-*leadPt)/(*leadPt) < 0.075){
//                hggMassP = *hggMassIn;
//                genMatchTypeP = *leadGenMatchType;
//                genPtP = *leadGenPt;
//                ptP = *leadPt;
//                weightP = *weightIn;
//                scRawEP = *leadSCRawE;
//                r9P = *leadR9;
//                sigmaIetaIetaP = *leadSigmaIetaIeta;
//                etaWidthP = *leadEtaWidth;
//                phiWidthP = *leadPhiWidth;
//                covIEtaIPhiP = *leadCovIEtaIPhi;
//                s4P = *leadS4;
//                phoIso03P = *leadPhoIso03;
//                chgIsoWrtChosenVtxP = *leadChgIsoWrtChosenVtx;
//                chgIsoWrtWorstVtxP = *leadChgIsoWrtWorstVtx;
//                scEtaP = *leadScEta;
//                esEffSigmaRRP = *leadEsEffSigmaRR;
//                esEnergyOverRawEP = *leadEsEnergyOverRawE;
//                hadTowOverEmP = *leadHadTowOverEm;
//                hadronicOverEmP = *leadHadronicOverEm;
//                rhoP = *rhoIn;
//                treeOutP->Fill();
//            }
//            if (*hggMassIn > 95.0 && *subGenMatchType == 1.0 && *subPt > 17.0 && *subPt/(*hggMassIn) > 0.25 && abs(*subGenPt-*subPt)/(*subPt) < 0.075){
//                hggMassP = *hggMassIn;
//                genMatchTypeP = *subGenMatchType;
//                genPtP = *subGenPt;
//                ptP = *subPt;
//                weightP = *weightIn;
//                scRawEP = *subSCRawE;
//                r9P = *subR9;
//                sigmaIetaIetaP = *subSigmaIetaIeta;
//                etaWidthP = *subEtaWidth;
//                phiWidthP = *subPhiWidth;
//                covIEtaIPhiP = *subCovIEtaIPhi;
//                s4P = *subS4;
//                phoIso03P = *subPhoIso03;
//                chgIsoWrtChosenVtxP = *subChgIsoWrtChosenVtx;
//                chgIsoWrtWorstVtxP = *subChgIsoWrtWorstVtx;
//                scEtaP = *subScEta;
//                esEffSigmaRRP = *subEsEffSigmaRR;
//                esEnergyOverRawEP = *subEsEnergyOverRawE;
//                hadTowOverEmP = *subHadTowOverEm;
//                hadronicOverEmP = *subHadronicOverEm;
//                rhoP = *rhoIn;
//                treeOutP->Fill();
//            }
//            if (*hggMassIn > 95.0 && *leadGenMatchType != 1.0 && *leadPt > 17.0 && *leadPt/(*hggMassIn) > 0.25){
//                hggMassF = *hggMassIn;
//                genMatchTypeF = *leadGenMatchType;
//                genPtF = *leadGenPt;
//                ptF = *leadPt;
//                weightF = *weightIn;
//                scRawEF = *leadSCRawE;
//                r9F = *leadR9;
//                sigmaIetaIetaF = *leadSigmaIetaIeta;
//                etaWidthF = *leadEtaWidth;
//                phiWidthF = *leadPhiWidth;
//                covIEtaIPhiF = *leadCovIEtaIPhi;
//                s4F = *leadS4;
//                phoIso03F = *leadPhoIso03;
//                chgIsoWrtChosenVtxF = *leadChgIsoWrtChosenVtx;
//                chgIsoWrtWorstVtxF = *leadChgIsoWrtWorstVtx;
//                scEtaF = *leadScEta;
//                esEffSigmaRRF = *leadEsEffSigmaRR;
//                esEnergyOverRawEF = *leadEsEnergyOverRawE;
//                hadTowOverEmF = *leadHadTowOverEm;
//                hadronicOverEmF = *leadHadronicOverEm;
//                rhoF = *rhoIn;
//                treeOutF->Fill();
//            }
//            if (*hggMassIn > 95.0 && *subGenMatchType != 1.0 && *subPt > 17.0 && *subPt/(*hggMassIn) > 0.25){
//                hggMassF = *hggMassIn;
//                genMatchTypeF = *subGenMatchType;
//                genPtF = *subGenPt;
//                ptF = *subPt;
//                weightF = *weightIn;
//                scRawEF = *subSCRawE;
//                r9F = *subR9;
//                sigmaIetaIetaF = *subSigmaIetaIeta;
//                etaWidthF = *subEtaWidth;
//                phiWidthF = *subPhiWidth;
//                covIEtaIPhiF = *subCovIEtaIPhi;
//                s4F = *subS4;
//                phoIso03F = *subPhoIso03;
//                chgIsoWrtChosenVtxF = *subChgIsoWrtChosenVtx;
//                chgIsoWrtWorstVtxF = *subChgIsoWrtWorstVtx;
//                scEtaF = *subScEta;
//                esEffSigmaRRF = *subEsEffSigmaRR;
//                esEnergyOverRawEF = *subEsEnergyOverRawE;
//                hadTowOverEmF = *subHadTowOverEm;
//                hadronicOverEmF = *subHadronicOverEm;
//                rhoF = *rhoIn;
//                treeOutF->Fill();
//            }
//        }
//    }
//    treeOutP->Print();
//    //treeOutP->Write();
//
//    treeOutF->Print();
//    //treeOutF->Write();

//
//
//    while (myReader.Next()) {
//        if (*hggMass > 95.0 && *leadGenMatchType == 1.0 && *leadPt > 17.0 && *leadPt/(*hggMass) > 0.25 && abs(*leadGenPt-*leadPt)/(*leadPt) < 0.075){
//            eventVarsPrompt.SetVals(*leadPt,*weight,*leadSCRawE,*leadR9,*leadSigmaIetaIeta,*leadEtaWidth,*leadPhiWidth,*leadCovIEtaIPhi,*leadS4,*leadPhoIso03,*leadChgIsoWrtChosenVtx,*leadChgIsoWrtWorstVtx,*leadScEta,*rho,*leadEsEffSigmaRR,*leadEsEnergyOverRawE);
//            branchP->Fill();
//        }
//        if (*hggMass > 95.0 && *subGenMatchType == 1.0 && *subPt > 17.0 && *subPt/(*hggMass) > 0.25 && abs(*subGenPt-*subPt)/(*subPt) < 0.075){
//            eventVarsPrompt.SetVals(*subPt,*weight,*subSCRawE,*subR9,*subSigmaIetaIeta,*subEtaWidth,*subPhiWidth,*subCovIEtaIPhi,*subS4,*subPhoIso03,*subChgIsoWrtChosenVtx,*subChgIsoWrtWorstVtx,*subScEta,*rho,*subEsEffSigmaRR,*subEsEnergyOverRawE);
//            branchP->Fill();
//        }
//        if (*hggMass > 95.0 && *leadGenMatchType != 1.0 && *leadPt > 17.0 && *leadPt/(*hggMass) > 0.25){
//            eventVarsFake.SetVals(*leadPt,*weight,*leadSCRawE,*leadR9,*leadSigmaIetaIeta,*leadEtaWidth,*leadPhiWidth,*leadCovIEtaIPhi,*leadS4,*leadPhoIso03,*leadChgIsoWrtChosenVtx,*leadChgIsoWrtWorstVtx,*leadScEta,*rho,*leadEsEffSigmaRR,*leadEsEnergyOverRawE);
//            branchF->Fill();
//        }
//        if (*hggMass > 95.0 && *subGenMatchType != 1.0 && *subPt > 17.0 && *subPt/(*hggMass) > 0.25){
//            eventVarsFake.SetVals(*subPt,*weight,*subSCRawE,*subR9,*subSigmaIetaIeta,*subEtaWidth,*subPhiWidth,*subCovIEtaIPhi,*subS4,*subPhoIso03,*subChgIsoWrtChosenVtx,*subChgIsoWrtWorstVtx,*subScEta,*rho,*subEsEffSigmaRR,*subEsEnergyOverRawE);
//            branchF->Fill();
//        }
//    }
//
//    treeOut->Print();
//    treeOut->Write();
    
//    fileOut->Write();
//    fileIn->Close();
//    fileOut->Close();

