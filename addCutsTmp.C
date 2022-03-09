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
class EventVars {
public:
    Float_t pt,weight,SCRawE,r9,sigmaIetaIeta,etaWidth,phiWidth,covIEtaIPhi,s4,phoIso03,chgIsoWrtChosenVtx,chgIsoWrtWorstVtx,scEta,rho,esEffSigmaRR,esEnergyOverRawE;
    void SetVals (Float_t ptIn, Float_t weightIn, Float_t SCRawEIn, Float_t r9In, Float_t sigmaIetaIetaIn, Float_t etaWidthIn, Float_t phiWidthIn, Float_t covIEtaIPhiIn, Float_t s4In, Float_t phoIso03In, Float_t chgIsoWrtChosenVtxIn, Float_t chgIsoWrtWorstVtxIn, Float_t scEtaIn, Float_t rhoIn, Float_t esEffSigmaRRIn, Float_t esEnergyOverRawEIn) {
        pt = ptIn;
        weight = weightIn;
        SCRawE = SCRawEIn;
        r9 = r9In;
        sigmaIetaIeta = sigmaIetaIetaIn;
        etaWidth = etaWidthIn;
        phiWidth = phiWidthIn;
        covIEtaIPhi = covIEtaIPhiIn;
        s4 = s4In;
        phoIso03 = phoIso03In;
        chgIsoWrtChosenVtx = chgIsoWrtChosenVtxIn;
        chgIsoWrtWorstVtx = chgIsoWrtWorstVtxIn;
        scEta = scEtaIn;
        rho = rhoIn;
        esEffSigmaRR = esEffSigmaRRIn;
        esEnergyOverRawE = esEnergyOverRawEIn;
    }
};

void addCuts(){
    TFile* fileIn = TFile::Open("./NTuples/GJets_M95PTM25_HovrE_DPT075_1117.root");
    TTreeReader myReader("diphotonDumper/trees/GJets",fileIn);
    TTree* treeIn = (TTree*)fileIn->Get("diphotonDumper/trees/GJets");
    
    TTreeReaderValue<Float_t> hggMassIn(myReader, "hggMass");
    TTreeReaderValue<Float_t> leadGenPt(myReader, "leadGenPt");
    TTreeReaderValue<Float_t> subGenPt(myReader, "subGenPt");
    TTreeReaderValue<Float_t> leadGenMatchType(myReader, "leadGenMatchType");
    TTreeReaderValue<Float_t> subGenMatchType(myReader, "subGenMatchType");
    TTreeReaderValue<Float_t> leadPt(myReader, "leadPt");
    TTreeReaderValue<Float_t> subPt(myReader, "subPt");
    TTreeReaderValue<Float_t> weight(myReader, "weight");
    TTreeReaderValue<Float_t> leadSCRawE(myReader, "leadSCRawE");
    TTreeReaderValue<Float_t> subSCRawE(myReader, "subSCRawE");
    TTreeReaderValue<Float_t> leadR9(myReader, "leadR9");
    TTreeReaderValue<Float_t> subR9(myReader, "subR9");
    TTreeReaderValue<Float_t> leadSigmaIetaIeta(myReader, "leadSigmaIetaIeta");
    TTreeReaderValue<Float_t> subSigmaIetaIeta(myReader, "subSigmaIetaIeta");
    TTreeReaderValue<Float_t> leadEtaWidth(myReader, "leadEtaWidth");
    TTreeReaderValue<Float_t> subEtaWidth(myReader, "subEtaWidth");
    TTreeReaderValue<Float_t> leadPhiWidth(myReader, "leadPhiWidth");
    TTreeReaderValue<Float_t> subPhiWidth(myReader, "subPhiWidth");
    TTreeReaderValue<Float_t> leadCovIEtaIPhi(myReader, "leadCovIEtaIPhi");
    TTreeReaderValue<Float_t> subCovIEtaIPhi(myReader, "subCovIEtaIPhi");
    TTreeReaderValue<Float_t> leadS4(myReader, "leadS4");
    TTreeReaderValue<Float_t> subS4(myReader, "subS4");
    TTreeReaderValue<Float_t> leadPhoIso03(myReader, "leadPhoIso03");
    TTreeReaderValue<Float_t> subPhoIso03(myReader, "subPhoIso03");
    TTreeReaderValue<Float_t> leadChgIsoWrtChosenVtx(myReader, "leadChgIsoWrtChosenVtx");
    TTreeReaderValue<Float_t> subChgIsoWrtChosenVtx(myReader, "subChgIsoWrtChosenVtx");
    TTreeReaderValue<Float_t> leadChgIsoWrtWorstVtx(myReader, "leadChgIsoWrtWorstVtx");
    TTreeReaderValue<Float_t> subChgIsoWrtWorstVtx(myReader, "subChgIsoWrtWorstVtx");
    TTreeReaderValue<Float_t> leadScEta(myReader, "leadScEta");
    TTreeReaderValue<Float_t> subScEta(myReader, "subScEta");
    TTreeReaderValue<Float_t> leadEsEffSigmaRR(myReader, "leadEsEffSigmaRR");
    TTreeReaderValue<Float_t> subEsEffSigmaRR(myReader, "subEsEffSigmaRR");
    TTreeReaderValue<Float_t> leadEsEnergyOverRawE(myReader, "leadEsEnergyOverRawE");
    TTreeReaderValue<Float_t> subEsEnergyOverRawE(myReader, "subEsEnergyOverRawE");
    TTreeReaderValue<Float_t> rho(myReader, "rho");
    
    TFile* fileOut = TFile::Open("NTuples/SinglePhotonCutsTest.root","RECREATE");
    
    string treeStrs[2] = {"promptPhotons","fakePhotons"};
    
    TTree* treeOutP = new TTree(treeStrs[0].c_str(),treeStrs[0].c_str());
    TTree* treeOutF = new TTree(treeStrs[1].c_str(),treeStrs[1].c_str());
    
    Float_t hggMassP,genPtP,genMatchTypeP,ptP,weightP,scRawEP,r9P,sigmaIetaIetaP,etaWidthP,phiWidthP,covIEtaIPhiP,s4P,phoIso03P,chgIsoWrtChosenVtxP,chgIsoWrtWorstVtxP,scEtaP,esEffSigmaRRP,esEnergyOverRawEP;
    treeOutP->Branch("hggMass",&hggMassP,"hggMass/F");
    treeOutP->Branch("genPt",&genPtP,"genPt/F");
    treeOutP->Branch("genMatchType",&genMatchTypeP,"genMatchType/F");
    treeOutP->Branch("pt",&ptP,"pt/F");
    treeOutP->Branch("weight",&weightP,"weight/F");
    treeOutP->Branch("scRawE",&scRawEP,"scRawE/F");
    treeOutP->Branch("r9",&r9P,"r9/F");
    treeOutP->Branch("sigmaIetaIeta",&sigmaIetaIetaP,"sigmaIetaIeta/F");
    treeOutP->Branch("etaWidth",&etaWidthP,"etaWidth/F");
    treeOutP->Branch("phiWidth",&phiWidthP,"phiWidth/F");
    treeOutP->Branch("covIEtaIPhi",&covIEtaIPhiP,"covIEtaIPhi/F");
    treeOutP->Branch("covIEtaIPhi",&covIEtaIPhiP,"covIEtaIPhi/F");
    treeOutP->Branch("s4",&s4P,"s4/F");
    treeOutP->Branch("phoIso03",&phoIso03P,"phoIso03/F");
    treeOutP->Branch("chgIsoWrtChosenVtx",&chgIsoWrtChosenVtxP,"chgIsoWrtChosenVtx/F");
    treeOutP->Branch("chgIsoWrtWorstVtx",&chgIsoWrtWorstVtxP,"chgIsoWrtWorstVtx/F");
    treeOutP->Branch("scEta",&scEtaP,"scEta/F");
    treeOutP->Branch("esEffSigmaRR",&esEffSigmaRRP,"esEffSigmaRR/F");
    treeOutP->Branch("esEnergyOverRawE",&esEnergyOverRawEP,"esEnergyOverRawE/F");
    
    Float_t hggMassF,genPtF,genMatchTypeF,ptF,weightF,scRawEF,r9F,sigmaIetaIetaF,etaWidthF,phiWidthF,covIEtaIPhiF,s4F,phoIso03F,chgIsoWrtChosenVtxF,chgIsoWrtWorstVtxF,scEtaF,esEffSigmaRRF,esEnergyOverRawEF;
    treeOutF->Branch("hggMass",&hggMassF,"hggMass/F");
    treeOutF->Branch("genPt",&genPtF,"genPt/F");
    treeOutF->Branch("genMatchType",&genMatchTypeF,"genMatchType/F");
    treeOutF->Branch("pt",&ptF,"pt/F");
    treeOutF->Branch("weight",&weightF,"weight/F");
    treeOutF->Branch("scRawE",&scRawEF,"scRawE/F");
    treeOutF->Branch("r9",&r9F,"r9/F");
    treeOutF->Branch("sigmaIetaIeta",&sigmaIetaIetaF,"sigmaIetaIeta/F");
    treeOutF->Branch("etaWidth",&etaWidthF,"etaWidth/F");
    treeOutF->Branch("phiWidth",&phiWidthF,"phiWidth/F");
    treeOutF->Branch("covIEtaIPhi",&covIEtaIPhiF,"covIEtaIPhi/F");
    treeOutF->Branch("covIEtaIPhi",&covIEtaIPhiF,"covIEtaIPhi/F");
    treeOutF->Branch("s4",&s4F,"s4/F");
    treeOutF->Branch("phoIso03",&phoIso03F,"phoIso03/F");
    treeOutF->Branch("chgIsoWrtChosenVtx",&chgIsoWrtChosenVtxF,"chgIsoWrtChosenVtx/F");
    treeOutF->Branch("chgIsoWrtWorstVtx",&chgIsoWrtWorstVtxF,"chgIsoWrtWorstVtx/F");
    treeOutF->Branch("scEta",&scEtaF,"scEta/F");
    treeOutF->Branch("esEffSigmaRR",&esEffSigmaRRF,"esEffSigmaRR/F");
    treeOutF->Branch("esEnergyOverRawE",&esEnergyOverRawEF,"esEnergyOverRawE/F");
    
    while (myReader.Next()){
        //cout<<"hggMassIn = "<<*hggMassIn<<endl;
        if (*hggMassIn > 95.0 && *leadGenMatchType == 1.0 && *leadPt > 17.0 && *leadPt/(*hggMassIn) > 0.25 && abs(*leadGenPt-*leadPt)/(*leadPt) < 0.075){
            hggMassP = *hggMassIn;
            treeOutP->Fill();
        }
        if (*hggMassIn > 95.0 && *subGenMatchType == 1.0 && *subPt > 17.0 && *subPt/(*hggMassIn) > 0.25 && abs(*subGenPt-*subPt)/(*subPt) < 0.075){
            hggMassP = *hggMassIn;
            treeOutP->Fill();
        }
        if (*hggMassIn > 95.0 && *leadGenMatchType != 1.0 && *leadPt > 17.0 && *leadPt/(*hggMassIn) > 0.25){
            hggMassF = *hggMassIn;
            treeOutF->Fill();
        }
        if (*hggMassIn > 95.0 && *subGenMatchType != 1.0 && *subPt > 17.0 && *subPt/(*hggMassIn) > 0.25){
            hggMassF = *hggMassIn;
            treeOutF->Fill();
        }
    }
    treeOutP->Print();
    //treeOutP->Write();

    treeOutF->Print();
    //treeOutF->Write();

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
    
    fileOut->Write();
    fileIn->Close();
    fileOut->Close();
}

