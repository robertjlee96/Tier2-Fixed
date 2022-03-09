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

void addCutsGGH(){
    
    TFile* fileIn = TFile::Open("./NTuples/GGH_M95PTM25_Old0916_0113.root");
    TTreeReader myReader("diphotonDumper/trees/ggh_125",fileIn);
    TTree* treeIn = (TTree*)fileIn->Get("diphotonDumper/trees/ggh_125");

    TTreeReaderValue<Float_t> hggMassIn(myReader, "hggMass");
    TTreeReaderValue<unsigned long long> eventIn(myReader, "event");
    TTreeReaderValue<Float_t> leadGenPt(myReader, "leadGenPt");
    TTreeReaderValue<Float_t> subGenPt(myReader, "subGenPt");
    TTreeReaderValue<Float_t> leadGenMatchType(myReader, "leadGenMatchType");
    TTreeReaderValue<Float_t> subGenMatchType(myReader, "subGenMatchType");
    TTreeReaderValue<Float_t> leadPt(myReader, "leadPt");
    TTreeReaderValue<Float_t> subPt(myReader, "subPt");
    TTreeReaderValue<Float_t> weightIn(myReader, "weight");
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
    TTreeReaderValue<Float_t> leadTrkSumPtHollowConeDR03(myReader, "leadTrkSumPtHollowConeDR03");
    TTreeReaderValue<Float_t> subTrkSumPtHollowConeDR03(myReader, "subTrkSumPtHollowConeDR03");
    TTreeReaderValue<Float_t> leadScEta(myReader, "leadScEta");
    TTreeReaderValue<Float_t> subScEta(myReader, "subScEta");
    TTreeReaderValue<Float_t> leadEsEffSigmaRR(myReader, "leadEsEffSigmaRR");
    TTreeReaderValue<Float_t> subEsEffSigmaRR(myReader, "subEsEffSigmaRR");
    TTreeReaderValue<Float_t> leadEsEnergyOverRawE(myReader, "leadEsEnergyOverRawE");
    TTreeReaderValue<Float_t> subEsEnergyOverRawE(myReader, "subEsEnergyOverRawE");
    TTreeReaderValue<Float_t> leadHadTowOverEm(myReader, "leadHadTowOverEm");
    TTreeReaderValue<Float_t> subHadTowOverEm(myReader, "subHadTowOverEm");
    TTreeReaderValue<Float_t> leadHadronicOverEm(myReader, "leadHadronicOverEm");
    TTreeReaderValue<Float_t> subHadronicOverEm(myReader, "subHadronicOverEm");
    TTreeReaderValue<Float_t> rhoIn(myReader, "rho");
    
    TFile* fileOut = TFile::Open("NTuples/SinglePhoton_GGH_M121to129_PTM15_0204_Validation.root","RECREATE");
    string treeNames[4] = {"PromptLead","PromptSub","FakeLead","FakeSub"};
    string treeStrs[2] = {"promptPhotons","fakePhotons"};
    
    TTree* treeOutP1 = new TTree(treeNames[0].c_str(),treeStrs[0].c_str());
    TTree* treeOutP2 = new TTree(treeNames[1].c_str(),treeStrs[0].c_str());
    TTree* treeOutF1 = new TTree(treeNames[2].c_str(),treeStrs[1].c_str());
    TTree* treeOutF2 = new TTree(treeNames[3].c_str(),treeStrs[1].c_str());

    Float_t hggMassP1,passSPreselP1,passDPreselP1,eventP1,genPtP1,genMatchTypeP1,ptP1,weightP1,scRawEP1,r9P1,sigmaIetaIetaP1,etaWidthP1,phiWidthP1,covIEtaIPhiP1,s4P1,phoIso03P1,chgIsoWrtChosenVtxP1,chgIsoWrtWorstVtxP1,scEtaP1,esEffSigmaRRP1,esEnergyOverRawEP1,rhoP1,hadTowOverEmP1,hadronicOverEmP1;
    treeOutP1->Branch("hggMass",&hggMassP1,"hggMass/F");
    treeOutP1->Branch("passSPresel",&passSPreselP1,"passSPresel/F");
    treeOutP1->Branch("passDPresel",&passDPreselP1,"passDPresel/F");
    treeOutP1->Branch("event",&eventP1,"event/F");
    treeOutP1->Branch("genPt",&genPtP1,"genPt/F");
    treeOutP1->Branch("genMatchType",&genMatchTypeP1,"genMatchType/F");
    treeOutP1->Branch("pt",&ptP1,"pt/F");
    treeOutP1->Branch("weight",&weightP1,"weight/F");
    treeOutP1->Branch("SCRawE",&scRawEP1,"SCRawE/F");
    treeOutP1->Branch("r9",&r9P1,"r9/F");
    treeOutP1->Branch("sigmaIetaIeta",&sigmaIetaIetaP1,"sigmaIetaIeta/F");
    treeOutP1->Branch("etaWidth",&etaWidthP1,"etaWidth/F");
    treeOutP1->Branch("phiWidth",&phiWidthP1,"phiWidth/F");
    treeOutP1->Branch("covIEtaIPhi",&covIEtaIPhiP1,"covIEtaIPhi/F");
    treeOutP1->Branch("s4",&s4P1,"s4/F");
    treeOutP1->Branch("phoIso03",&phoIso03P1,"phoIso03/F");
    treeOutP1->Branch("chgIsoWrtChosenVtx",&chgIsoWrtChosenVtxP1,"chgIsoWrtChosenVtx/F");
    treeOutP1->Branch("chgIsoWrtWorstVtx",&chgIsoWrtWorstVtxP1,"chgIsoWrtWorstVtx/F");
    treeOutP1->Branch("scEta",&scEtaP1,"scEta/F");
    treeOutP1->Branch("esEffSigmaRR",&esEffSigmaRRP1,"esEffSigmaRR/F");
    treeOutP1->Branch("esEnergyOverRawE",&esEnergyOverRawEP1,"esEnergyOverRawE/F");
    treeOutP1->Branch("hadTowOverEm",&hadTowOverEmP1,"hadTowOverEm/F");
    treeOutP1->Branch("hadronicOverEm",&hadronicOverEmP1,"hadronicOverEm/F");
    treeOutP1->Branch("rho",&rhoP1,"rho/F");
        
    Float_t hggMassP2,passSPreselP2,passDPreselP2,eventP2,genPtP2,genMatchTypeP2,ptP2,weightP2,scRawEP2,r9P2,sigmaIetaIetaP2,etaWidthP2,phiWidthP2,covIEtaIPhiP2,s4P2,phoIso03P2,chgIsoWrtChosenVtxP2,chgIsoWrtWorstVtxP2,scEtaP2,esEffSigmaRRP2,esEnergyOverRawEP2,rhoP2,hadTowOverEmP2,hadronicOverEmP2;
    treeOutP2->Branch("hggMass",&hggMassP2,"hggMass/F");
    treeOutP2->Branch("passSPresel",&passSPreselP2,"passSPresel/F");
    treeOutP2->Branch("passDPresel",&passDPreselP2,"passDPresel/F");
    treeOutP2->Branch("event",&eventP2,"event/F");
    treeOutP2->Branch("genPt",&genPtP2,"genPt/F");
    treeOutP2->Branch("genMatchType",&genMatchTypeP2,"genMatchType/F");
    treeOutP2->Branch("pt",&ptP2,"pt/F");
    treeOutP2->Branch("weight",&weightP2,"weight/F");
    treeOutP2->Branch("SCRawE",&scRawEP2,"SCRawE/F");
    treeOutP2->Branch("r9",&r9P2,"r9/F");
    treeOutP2->Branch("sigmaIetaIeta",&sigmaIetaIetaP2,"sigmaIetaIeta/F");
    treeOutP2->Branch("etaWidth",&etaWidthP2,"etaWidth/F");
    treeOutP2->Branch("phiWidth",&phiWidthP2,"phiWidth/F");
    treeOutP2->Branch("covIEtaIPhi",&covIEtaIPhiP2,"covIEtaIPhi/F");
    treeOutP2->Branch("s4",&s4P2,"s4/F");
    treeOutP2->Branch("phoIso03",&phoIso03P2,"phoIso03/F");
    treeOutP2->Branch("chgIsoWrtChosenVtx",&chgIsoWrtChosenVtxP2,"chgIsoWrtChosenVtx/F");
    treeOutP2->Branch("chgIsoWrtWorstVtx",&chgIsoWrtWorstVtxP2,"chgIsoWrtWorstVtx/F");
    treeOutP2->Branch("scEta",&scEtaP2,"scEta/F");
    treeOutP2->Branch("esEffSigmaRR",&esEffSigmaRRP2,"esEffSigmaRR/F");
    treeOutP2->Branch("esEnergyOverRawE",&esEnergyOverRawEP2,"esEnergyOverRawE/F");
    treeOutP2->Branch("hadTowOverEm",&hadTowOverEmP2,"hadTowOverEm/F");
    treeOutP2->Branch("hadronicOverEm",&hadronicOverEmP2,"hadronicOverEm/F");
    treeOutP2->Branch("rho",&rhoP2,"rho/F");
       
    Float_t hggMassF1,passSPreselF1,passDPreselF1,eventF1,genPtF1,genMatchTypeF1,ptF1,weightF1,scRawEF1,r9F1,sigmaIetaIetaF1,etaWidthF1,phiWidthF1,covIEtaIPhiF1,s4F1,phoIso03F1,chgIsoWrtChosenVtxF1,chgIsoWrtWorstVtxF1,scEtaF1,esEffSigmaRRF1,esEnergyOverRawEF1,rhoF1,hadTowOverEmF1,hadronicOverEmF1;
    treeOutF1->Branch("hggMass",&hggMassF1,"hggMass/F");
    treeOutF1->Branch("passSPresel",&passSPreselF1,"passSPresel/F");
    treeOutF1->Branch("passDPresel",&passDPreselF1,"passDPresel/F");
    treeOutF1->Branch("event",&eventF1,"event/F");
    treeOutF1->Branch("genPt",&genPtF1,"genPt/F");
    treeOutF1->Branch("genMatchType",&genMatchTypeF1,"genMatchType/F");
    treeOutF1->Branch("pt",&ptF1,"pt/F");
    treeOutF1->Branch("weight",&weightF1,"weight/F");
    treeOutF1->Branch("SCRawE",&scRawEF1,"SCRawE/F");
    treeOutF1->Branch("r9",&r9F1,"r9/F");
    treeOutF1->Branch("sigmaIetaIeta",&sigmaIetaIetaF1,"sigmaIetaIeta/F");
    treeOutF1->Branch("etaWidth",&etaWidthF1,"etaWidth/F");
    treeOutF1->Branch("phiWidth",&phiWidthF1,"phiWidth/F");
    treeOutF1->Branch("covIEtaIPhi",&covIEtaIPhiF1,"covIEtaIPhi/F");
    treeOutF1->Branch("covIEtaIPhi",&covIEtaIPhiF1,"covIEtaIPhi/F");
    treeOutF1->Branch("s4",&s4F1,"s4/F");
    treeOutF1->Branch("phoIso03",&phoIso03F1,"phoIso03/F");
    treeOutF1->Branch("chgIsoWrtChosenVtx",&chgIsoWrtChosenVtxF1,"chgIsoWrtChosenVtx/F");
    treeOutF1->Branch("chgIsoWrtWorstVtx",&chgIsoWrtWorstVtxF1,"chgIsoWrtWorstVtx/F");
    treeOutF1->Branch("scEta",&scEtaF1,"scEta/F");
    treeOutF1->Branch("esEffSigmaRR",&esEffSigmaRRF1,"esEffSigmaRR/F");
    treeOutF1->Branch("esEnergyOverRawE",&esEnergyOverRawEF1,"esEnergyOverRawE/F");
    treeOutF1->Branch("hadTowOverEm",&hadTowOverEmF1,"hadTowOverEm/F");
    treeOutF1->Branch("hadronicOverEm",&hadronicOverEmF1,"hadronicOverEm/F");
    treeOutF1->Branch("rho",&rhoF1,"rho/F");
        
    Float_t hggMassF2,passSPreselF2,passDPreselF2,eventF2 ,genPtF2 ,genMatchTypeF2 ,ptF2 ,weightF2 ,scRawEF2 ,r9F2 ,sigmaIetaIetaF2 ,etaWidthF2 ,phiWidthF2 ,covIEtaIPhiF2 ,s4F2 ,phoIso03F2 ,chgIsoWrtChosenVtxF2 ,chgIsoWrtWorstVtxF2 ,scEtaF2 ,esEffSigmaRRF2 ,esEnergyOverRawEF2 ,rhoF2 ,hadTowOverEmF2 ,hadronicOverEmF2;
    treeOutF2 ->Branch("hggMass",&hggMassF2 ,"hggMass/F");
    treeOutF2 ->Branch("passSPresel",&passSPreselF2,"passSPresel/F");
    treeOutF2 ->Branch("passDPresel",&passDPreselF2,"passDPresel/F");
    treeOutF2 ->Branch("event",&eventF2 ,"event/F");
    treeOutF2 ->Branch("genPt",&genPtF2 ,"genPt/F");
    treeOutF2 ->Branch("genMatchType",&genMatchTypeF2 ,"genMatchType/F");
    treeOutF2 ->Branch("pt",&ptF2 ,"pt/F");
    treeOutF2 ->Branch("weight",&weightF2 ,"weight/F");
    treeOutF2 ->Branch("SCRawE",&scRawEF2 ,"SCRawE/F");
    treeOutF2 ->Branch("r9",&r9F2 ,"r9/F");
    treeOutF2 ->Branch("sigmaIetaIeta",&sigmaIetaIetaF2 ,"sigmaIetaIeta/F");
    treeOutF2 ->Branch("etaWidth",&etaWidthF2 ,"etaWidth/F");
    treeOutF2 ->Branch("phiWidth",&phiWidthF2 ,"phiWidth/F");
    treeOutF2 ->Branch("covIEtaIPhi",&covIEtaIPhiF2 ,"covIEtaIPhi/F");
    treeOutF2 ->Branch("covIEtaIPhi",&covIEtaIPhiF2 ,"covIEtaIPhi/F");
    treeOutF2 ->Branch("s4",&s4F2 ,"s4/F");
    treeOutF2 ->Branch("phoIso03",&phoIso03F2 ,"phoIso03/F");
    treeOutF2 ->Branch("chgIsoWrtChosenVtx",&chgIsoWrtChosenVtxF2 ,"chgIsoWrtChosenVtx/F");
    treeOutF2 ->Branch("chgIsoWrtWorstVtx",&chgIsoWrtWorstVtxF2 ,"chgIsoWrtWorstVtx/F");
    treeOutF2 ->Branch("scEta",&scEtaF2 ,"scEta/F");
    treeOutF2 ->Branch("esEffSigmaRR",&esEffSigmaRRF2 ,"esEffSigmaRR/F");
    treeOutF2 ->Branch("esEnergyOverRawE",&esEnergyOverRawEF2 ,"esEnergyOverRawE/F");
    treeOutF2 ->Branch("hadTowOverEm",&hadTowOverEmF2 ,"hadTowOverEm/F");
    treeOutF2 ->Branch("hadronicOverEm",&hadronicOverEmF2 ,"hadronicOverEm/F");
    treeOutF2 ->Branch("rho",&rhoF2 ,"rho/F");
        
    int nTotal = 0.0;
    int nPassD = 0.0;
    
    while (myReader.Next()){
        //ODD FOR TRAINING EVEN FOR VALIDATION
        //if(((int)*eventIn)%2 == 0 && ((*leadGenMatchType == 1 && *subGenMatchType != 1)||(*leadGenMatchType != 1 && *subGenMatchType == 1) )  ){
        if(((int)*eventIn)%2 == 0){
            if (*hggMassIn > 95 && *leadGenMatchType == 1.0 && *leadPt > 17.0 && *leadPt/(*hggMassIn) > 0.15){
//            if (*hggMassIn > 121 && *hggMassIn < 129 && *leadGenMatchType == 1.0 && *leadPt > 17.0 && *leadPt/(*hggMassIn) > 0.15){
                nTotal += 1.0;
                hggMassP1 = *hggMassIn;
                eventP1 = *eventIn;
                genMatchTypeP1 = *leadGenMatchType;
                genPtP1 = *leadGenPt;
                ptP1 = *leadPt;
                weightP1 = *weightIn;
                scRawEP1 = *leadSCRawE;
                r9P1 = *leadR9;
                sigmaIetaIetaP1 = *leadSigmaIetaIeta;
                etaWidthP1 = *leadEtaWidth;
                phiWidthP1 = *leadPhiWidth;
                covIEtaIPhiP1 = *leadCovIEtaIPhi;
                s4P1 = *leadS4;
                phoIso03P1 = *leadPhoIso03;
                chgIsoWrtChosenVtxP1 = *leadChgIsoWrtChosenVtx;
                chgIsoWrtWorstVtxP1 = *leadChgIsoWrtWorstVtx;
                scEtaP1 = *leadScEta;
                esEffSigmaRRP1 = *leadEsEffSigmaRR;
                esEnergyOverRawEP1 = *leadEsEnergyOverRawE;
                hadTowOverEmP1 = *leadHadTowOverEm;
                hadronicOverEmP1 = *leadHadronicOverEm;
                rhoP1 = *rhoIn;
                double passSPresel = 0.0;
                double passDPresel = 0.0;
                if (*leadPt/(*hggMassIn) > 0.33 && *leadR9 > 0.5 && *leadHadronicOverEm < 0.08 && !(abs(*leadScEta) < 1.5 && *leadR9 < 0.85 && (*leadSigmaIetaIeta > 0.015 || *leadPhoIso03 > 4.0 || *leadTrkSumPtHollowConeDR03 > 6.0)) && !(abs(*leadScEta) > 1.5 && *leadR9 < 0.9 && (*leadSigmaIetaIeta > 0.035 || *leadPhoIso03 > 4.0 || *leadTrkSumPtHollowConeDR03 > 6.0))){
                    
                    passSPresel = 1.0;
                    if (*subPt/(*hggMassIn) > 0.25 && *subR9 > 0.5 && *subHadronicOverEm < 0.08 && !(abs(*subScEta) < 1.5 && *subR9 < 0.85 && (*subSigmaIetaIeta > 0.015 || *subPhoIso03 > 4.0 || *subTrkSumPtHollowConeDR03 > 6.0)) && !(abs(*subScEta) > 1.5 && *subR9 < 0.9 && (*subSigmaIetaIeta > 0.035 || *subPhoIso03 > 4.0 || *subTrkSumPtHollowConeDR03 > 6.0))){
                        passDPresel = 1.0;
                        nPassD += 1.0;
                    }
                }
                passSPreselP1 = passSPresel;
                passDPreselP1 = passDPresel;
                treeOutP1->Fill();
            }
            if (*hggMassIn > 95  && *subGenMatchType == 1.0 && *subPt > 17.0 && *subPt/(*hggMassIn) > 0.15){
//            if (*hggMassIn > 121 && *hggMassIn < 129 && *subGenMatchType == 1.0 && *subPt > 17.0 && *subPt/(*hggMassIn) > 0.15){
                nTotal += 1.0;
                hggMassP2 = *hggMassIn;
                eventP2 = *eventIn;
                genMatchTypeP2 = *subGenMatchType;
                genPtP2 = *subGenPt;
                ptP2 = *subPt;
                weightP2 = *weightIn;
                scRawEP2 = *subSCRawE;
                r9P2 = *subR9;
                sigmaIetaIetaP2 = *subSigmaIetaIeta;
                etaWidthP2 = *subEtaWidth;
                phiWidthP2 = *subPhiWidth;
                covIEtaIPhiP2 = *subCovIEtaIPhi;
                s4P2 = *subS4;
                phoIso03P2 = *subPhoIso03;
                chgIsoWrtChosenVtxP2 = *subChgIsoWrtChosenVtx;
                chgIsoWrtWorstVtxP2 = *subChgIsoWrtWorstVtx;
                scEtaP2 = *subScEta;
                esEffSigmaRRP2 = *subEsEffSigmaRR;
                esEnergyOverRawEP2 = *subEsEnergyOverRawE;
                hadTowOverEmP2 = *subHadTowOverEm;
                hadronicOverEmP2 = *subHadronicOverEm;
                rhoP2 = *rhoIn;
                double passSPresel = 0.0;
                double passDPresel = 0.0;
                if (*subPt/(*hggMassIn) > 0.25 && *subR9 > 0.5 && *subHadronicOverEm < 0.08 && !(abs(*subScEta) < 1.5 && *subR9 < 0.85 && (*subSigmaIetaIeta > 0.015 || *subPhoIso03 > 4.0 || *subTrkSumPtHollowConeDR03 > 6.0)) && !(abs(*subScEta) > 1.5 && *subR9 < 0.9 && (*subSigmaIetaIeta > 0.035 || *subPhoIso03 > 4.0 || *subTrkSumPtHollowConeDR03 > 6.0))){
                    
                    passSPresel = 1.0;
                    if (*leadPt/(*hggMassIn) > 0.33 && *leadR9 > 0.5 && *leadHadronicOverEm < 0.08 && !(abs(*leadScEta) < 1.5 && *leadR9 < 0.85 && (*leadSigmaIetaIeta > 0.015 || *leadPhoIso03 > 4.0 || *leadTrkSumPtHollowConeDR03 > 6.0)) && !(abs(*leadScEta) > 1.5 && *leadR9 < 0.9 && (*leadSigmaIetaIeta > 0.035 || *leadPhoIso03 > 4.0 || *leadTrkSumPtHollowConeDR03 > 6.0))){
                        passDPresel = 1.0;
                        nPassD += 1.0;
                    }
                }
                passSPreselP2 = passSPresel;
                passDPreselP2 = passDPresel;
                treeOutP2->Fill();
            }
//            if (*hggMassIn > 95.0 && *leadGenMatchType != 1.0 && *leadPt > 17.0 && *leadPt/(*hggMassIn) > 0.15){
//                hggMassF1 = *hggMassIn;
//                eventF1 = *eventIn;
//                genMatchTypeF1 = *leadGenMatchType;
//                genPtF1 = *leadGenPt;
//                ptF1 = *leadPt;
//                weightF1 = *weightIn;
//                scRawEF1 = *leadSCRawE;
//                r9F1 = *leadR9;
//                sigmaIetaIetaF1 = *leadSigmaIetaIeta;
//                etaWidthF1 = *leadEtaWidth;
//                phiWidthF1 = *leadPhiWidth;
//                covIEtaIPhiF1 = *leadCovIEtaIPhi;
//                s4F1 = *leadS4;
//                phoIso03F1 = *leadPhoIso03;
//                chgIsoWrtChosenVtxF1 = *leadChgIsoWrtChosenVtx;
//                chgIsoWrtWorstVtxF1 = *leadChgIsoWrtWorstVtx;
//                scEtaF1 = *leadScEta;
//                esEffSigmaRRF1 = *leadEsEffSigmaRR;
//                esEnergyOverRawEF1 = *leadEsEnergyOverRawE;
//                hadTowOverEmF1 = *leadHadTowOverEm;
//                hadronicOverEmF1 = *leadHadronicOverEm;
//                rhoF1 = *rhoIn;
//                treeOutF1->Fill();
//            }
//            if (*hggMassIn > 95.0 && *subGenMatchType != 1.0 && *subPt > 17.0 && *subPt/(*hggMassIn) > 0.15){
//                hggMassF2 = *hggMassIn;
//                eventF2 = *eventIn;
//                genMatchTypeF2 = *subGenMatchType;
//                genPtF2 = *subGenPt;
//                ptF2 = *subPt;
//                weightF2 = *weightIn;
//                scRawEF2 = *subSCRawE;
//                r9F2 = *subR9;
//                sigmaIetaIetaF2 = *subSigmaIetaIeta;
//                etaWidthF2 = *subEtaWidth;
//                phiWidthF2 = *subPhiWidth;
//                covIEtaIPhiF2 = *subCovIEtaIPhi;
//                s4F2 = *subS4;
//                phoIso03F2 = *subPhoIso03;
//                chgIsoWrtChosenVtxF2 = *subChgIsoWrtChosenVtx;
//                chgIsoWrtWorstVtxF2 = *subChgIsoWrtWorstVtx;
//                scEtaF2 = *subScEta;
//                esEffSigmaRRF2 = *subEsEffSigmaRR;
//                esEnergyOverRawEF2 = *subEsEnergyOverRawE;
//                hadTowOverEmF2 = *subHadTowOverEm;
//                hadronicOverEmF2 = *subHadronicOverEm;
//                rhoF2 = *rhoIn;
//                treeOutF2->Fill();
//            }
        }
    }
    TList *listPrompt = new TList;
    listPrompt->Add(treeOutP1);
    listPrompt->Add(treeOutP2);
    TTree *treeOutP = TTree::MergeTrees(listPrompt);
    treeOutP->SetName("promptPhotons");
    
    treeOutP->Print();
    //treeOutP->Write();
    
//    TList *listFake = new TList;
//    listFake->Add(treeOutF1);
//    listFake->Add(treeOutF2);
//    TTree *treeOutF = TTree::MergeTrees(listFake);
//    treeOutF->SetName("fakePhotons");
//
//    treeOutF->Print();
    //treeOutF->Write();

    treeOutP1->Delete();
    treeOutP2->Delete();
//    treeOutF1->Delete();
//    treeOutF2->Delete();
    
    fileOut->Write();
    fileIn->Close();
    fileOut->Close();
}

