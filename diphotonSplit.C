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

 //TRAINING = ODD, VALIDATION = EVEN
void diphotonSplit(){
    
    TFile* fileIn = TFile::Open("./NTuples/GGH_And_GJets_M95PTM25_DPT075_HovrE_FixSplit_AllEvents_1229.root");
    //TTreeReader myReader("diphotonDumper/trees/GJets",fileIn);
    //TTree* treeIn = (TTree*)fileIn->Get("diphotonDumper/trees/GJets");
    TTreeReader myReader("GJets",fileIn);
    TTree* treeIn = (TTree*)fileIn->Get("GJets");
    
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
    TTreeReaderValue<Float_t> leadIDMVA(myReader, "leadIDMVA");
    TTreeReaderValue<Float_t> subIDMVA(myReader, "subIDMVA");
    TTreeReaderValue<Float_t> rhoIn(myReader, "rho");
    //TRAINING = ODD, VALIDATION = EVEN
    TFile* fileOut = TFile::Open("NTuples/SinglePhoton_GJet_NoCuts_1227_WithTest25FixSplitIDMVA_Validation.root","RECREATE");
    string treeNames[4] = {"PromptLead","PromptSub","FakeLead","FakeSub"};
    string treeStrs[2] = {"promptPhotons","fakePhotons"};
    
    TTree* treeOutP1 = new TTree(treeNames[0].c_str(),treeStrs[0].c_str());
    TTree* treeOutP2 = new TTree(treeNames[1].c_str(),treeStrs[0].c_str());
    TTree* treeOutF1 = new TTree(treeNames[2].c_str(),treeStrs[1].c_str());
    TTree* treeOutF2 = new TTree(treeNames[3].c_str(),treeStrs[1].c_str());

    Float_t hggMassP1,eventP1,genPtP1,genMatchTypeP1,leadGenMatchTypeP1,subGenMatchTypeP1,ptP1,weightP1,scRawEP1,r9P1,sigmaIetaIetaP1,etaWidthP1,phiWidthP1,covIEtaIPhiP1,s4P1,phoIso03P1,chgIsoWrtChosenVtxP1,chgIsoWrtWorstVtxP1,scEtaP1,leadScEtaP1,subScEtaP1,esEffSigmaRRP1,esEnergyOverRawEP1,rhoP1,hadTowOverEmP1,hadronicOverEmP1,idMVAP1;
    treeOutP1->Branch("hggMass",&hggMassP1,"hggMass/F");
    treeOutP1->Branch("event",&eventP1,"event/F");
    treeOutP1->Branch("genPt",&genPtP1,"genPt/F");
    treeOutP1->Branch("genMatchType",&genMatchTypeP1,"genMatchType/F");
    treeOutP1->Branch("leadGenMatchType",&leadGenMatchTypeP1,"leadGenMatchType/F");
    treeOutP1->Branch("subGenMatchType",&subGenMatchTypeP1,"subGenMatchType/F");
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
    treeOutP1->Branch("leadScEta",&leadScEtaP1,"leadScEta/F");
    treeOutP1->Branch("subScEta",&subScEtaP1,"subScEta/F");
    treeOutP1->Branch("esEffSigmaRR",&esEffSigmaRRP1,"esEffSigmaRR/F");
    treeOutP1->Branch("esEnergyOverRawE",&esEnergyOverRawEP1,"esEnergyOverRawE/F");
    treeOutP1->Branch("hadTowOverEm",&hadTowOverEmP1,"hadTowOverEm/F");
    treeOutP1->Branch("hadronicOverEm",&hadronicOverEmP1,"hadronicOverEm/F");
    treeOutP1->Branch("idMVA",&idMVAP1,"idMVA/F");
    treeOutP1->Branch("rho",&rhoP1,"rho/F");
        
    Float_t hggMassP2,eventP2,genPtP2,genMatchTypeP2,leadGenMatchTypeP2,subGenMatchTypeP2,ptP2,weightP2,scRawEP2,r9P2,sigmaIetaIetaP2,etaWidthP2,phiWidthP2,covIEtaIPhiP2,s4P2,phoIso03P2,chgIsoWrtChosenVtxP2,chgIsoWrtWorstVtxP2,scEtaP2,leadScEtaP2,subScEtaP2,esEffSigmaRRP2,esEnergyOverRawEP2,rhoP2,hadTowOverEmP2,hadronicOverEmP2,idMVAP2;
    treeOutP2->Branch("hggMass",&hggMassP2,"hggMass/F");
    treeOutP2->Branch("event",&eventP2,"event/F");
    treeOutP2->Branch("genPt",&genPtP2,"genPt/F");
    treeOutP2->Branch("genMatchType",&genMatchTypeP2,"genMatchType/F");
    treeOutP2->Branch("leadGenMatchType",&leadGenMatchTypeP2,"leadGenMatchType/F");
    treeOutP2->Branch("subGenMatchType",&subGenMatchTypeP2,"subGenMatchType/F");
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
    treeOutP2->Branch("leadScEta",&leadScEtaP2,"leadScEta/F");
    treeOutP2->Branch("subScEta",&subScEtaP2,"subScEta/F");
    treeOutP2->Branch("esEffSigmaRR",&esEffSigmaRRP2,"esEffSigmaRR/F");
    treeOutP2->Branch("esEnergyOverRawE",&esEnergyOverRawEP2,"esEnergyOverRawE/F");
    treeOutP2->Branch("hadTowOverEm",&hadTowOverEmP2,"hadTowOverEm/F");
    treeOutP2->Branch("hadronicOverEm",&hadronicOverEmP2,"hadronicOverEm/F");
    treeOutP2->Branch("idMVA",&idMVAP2,"idMVA/F");
    treeOutP2->Branch("rho",&rhoP2,"rho/F");
       
    Float_t hggMassF1,eventF1,genPtF1,genMatchTypeF1,leadGenMatchTypeF1,subGenMatchTypeF1,ptF1,weightF1,scRawEF1,r9F1,sigmaIetaIetaF1,etaWidthF1,phiWidthF1,covIEtaIPhiF1,s4F1,phoIso03F1,chgIsoWrtChosenVtxF1,chgIsoWrtWorstVtxF1,scEtaF1,leadScEtaF1,subScEtaF1,esEffSigmaRRF1,esEnergyOverRawEF1,rhoF1,hadTowOverEmF1,hadronicOverEmF1,idMVAF1;
    treeOutF1->Branch("hggMass",&hggMassF1,"hggMass/F");
    treeOutF1->Branch("event",&eventF1,"event/F");
    treeOutF1->Branch("genPt",&genPtF1,"genPt/F");
    treeOutF1->Branch("genMatchType",&genMatchTypeF1,"genMatchType/F");
    treeOutF1->Branch("leadGenMatchType",&leadGenMatchTypeF1,"leadGenMatchType/F");
    treeOutF1->Branch("subGenMatchType",&subGenMatchTypeF1,"subGenMatchType/F");
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
    treeOutF1->Branch("leadScEta",&leadScEtaF1,"leadScEta/F");
    treeOutF1->Branch("subScEta",&subScEtaF1,"subScEta/F");
    treeOutF1->Branch("esEffSigmaRR",&esEffSigmaRRF1,"esEffSigmaRR/F");
    treeOutF1->Branch("esEnergyOverRawE",&esEnergyOverRawEF1,"esEnergyOverRawE/F");
    treeOutF1->Branch("hadTowOverEm",&hadTowOverEmF1,"hadTowOverEm/F");
    treeOutF1->Branch("hadronicOverEm",&hadronicOverEmF1,"hadronicOverEm/F");
    treeOutF1->Branch("idMVA",&idMVAF1,"idMVA/F");
    treeOutF1->Branch("rho",&rhoF1,"rho/F");
        
    Float_t hggMassF2,eventF2,genPtF2,genMatchTypeF2,leadGenMatchTypeF2,subGenMatchTypeF2,ptF2,weightF2,scRawEF2,r9F2,sigmaIetaIetaF2,etaWidthF2,phiWidthF2,covIEtaIPhiF2,s4F2,phoIso03F2,chgIsoWrtChosenVtxF2,chgIsoWrtWorstVtxF2,scEtaF2,leadScEtaF2,subScEtaF2,esEffSigmaRRF2,esEnergyOverRawEF2,rhoF2,hadTowOverEmF2,hadronicOverEmF2,idMVAF2;
    treeOutF2 ->Branch("hggMass",&hggMassF2 ,"hggMass/F");
    treeOutF2 ->Branch("event",&eventF2 ,"event/F");
    treeOutF2 ->Branch("genPt",&genPtF2 ,"genPt/F");
    treeOutF2->Branch("genMatchType",&genMatchTypeF2,"genMatchType/F");
    treeOutF2->Branch("leadGenMatchType",&leadGenMatchTypeF2,"leadGenMatchType/F");
    treeOutF2->Branch("subGenMatchType",&subGenMatchTypeF2,"subGenMatchType/F");
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
    treeOutF2 ->Branch("scEta",&scEtaF2,"scEta/F");
    treeOutF2 ->Branch("leadScEta",&leadScEtaF2,"leadScEta/F");
    treeOutF2 ->Branch("subScEta",&subScEtaF2,"subScEta/F");
    treeOutF2 ->Branch("esEffSigmaRR",&esEffSigmaRRF2 ,"esEffSigmaRR/F");
    treeOutF2 ->Branch("esEnergyOverRawE",&esEnergyOverRawEF2 ,"esEnergyOverRawE/F");
    treeOutF2 ->Branch("hadTowOverEm",&hadTowOverEmF2 ,"hadTowOverEm/F");
    treeOutF2 ->Branch("hadronicOverEm",&hadronicOverEmF2 ,"hadronicOverEm/F");
    treeOutF2 ->Branch("idMVA",&idMVAF2,"idMVA/F");
    treeOutF2 ->Branch("rho",&rhoF2 ,"rho/F");
        
    while (myReader.Next()){
        //if(((int)*eventIn)%2 != 0){
        if(((int)*eventIn)%2 == 0){
            if (*leadGenMatchType == 1.0){
                hggMassP1 = *hggMassIn;
                eventP1 = *eventIn;
                genMatchTypeP1 = *leadGenMatchType;
                leadGenMatchTypeP1 = *leadGenMatchType;
                subGenMatchTypeP1 = *subGenMatchType;
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
                leadScEtaP1 = *leadScEta;
                subScEtaP1 = *subScEta;
                esEffSigmaRRP1 = *leadEsEffSigmaRR;
                esEnergyOverRawEP1 = *leadEsEnergyOverRawE;
                hadTowOverEmP1 = *leadHadTowOverEm;
                hadronicOverEmP1 = *leadHadronicOverEm;
                idMVAP1 = *leadIDMVA;
                rhoP1 = *rhoIn;
                treeOutP1->Fill();
            }
            if (*subGenMatchType == 1.0){
                hggMassP2 = *hggMassIn;
                eventP2 = *eventIn;
                genMatchTypeP2 = *subGenMatchType;
                leadGenMatchTypeP2 = *leadGenMatchType;
                subGenMatchTypeP2 = *subGenMatchType;
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
                leadScEtaP2 = *leadScEta;
                subScEtaP2 = *subScEta;
                esEffSigmaRRP2 = *subEsEffSigmaRR;
                esEnergyOverRawEP2 = *subEsEnergyOverRawE;
                hadTowOverEmP2 = *subHadTowOverEm;
                hadronicOverEmP2 = *subHadronicOverEm;
                idMVAP2 = *subIDMVA;
                rhoP2 = *rhoIn;
                treeOutP2->Fill();
            }
            if (*leadGenMatchType != 1.0){
                hggMassF1 = *hggMassIn;
                eventF1 = *eventIn;
                genMatchTypeF1 = *leadGenMatchType;
                leadGenMatchTypeF1 = *leadGenMatchType;
                subGenMatchTypeF1 = *subGenMatchType;
                genPtF1 = *leadGenPt;
                ptF1 = *leadPt;
                weightF1 = *weightIn;
                scRawEF1 = *leadSCRawE;
                r9F1 = *leadR9;
                sigmaIetaIetaF1 = *leadSigmaIetaIeta;
                etaWidthF1 = *leadEtaWidth;
                phiWidthF1 = *leadPhiWidth;
                covIEtaIPhiF1 = *leadCovIEtaIPhi;
                s4F1 = *leadS4;
                phoIso03F1 = *leadPhoIso03;
                chgIsoWrtChosenVtxF1 = *leadChgIsoWrtChosenVtx;
                chgIsoWrtWorstVtxF1 = *leadChgIsoWrtWorstVtx;
                scEtaF1 = *leadScEta;
                leadScEtaF1 = *leadScEta;
                subScEtaF1 = *subScEta;
                esEffSigmaRRF1 = *leadEsEffSigmaRR;
                esEnergyOverRawEF1 = *leadEsEnergyOverRawE;
                hadTowOverEmF1 = *leadHadTowOverEm;
                hadronicOverEmF1 = *leadHadronicOverEm;
                idMVAF1 = *leadIDMVA;
                rhoF1 = *rhoIn;
                treeOutF1->Fill();
            }
            if (*subGenMatchType != 1.0){
                hggMassF2 = *hggMassIn;
                eventF2 = *eventIn;
                genMatchTypeF2 = *subGenMatchType;
                leadGenMatchTypeF2 = *leadGenMatchType;
                subGenMatchTypeF2 = *subGenMatchType;
                genPtF2 = *subGenPt;
                ptF2 = *subPt;
                weightF2 = *weightIn;
                scRawEF2 = *subSCRawE;
                r9F2 = *subR9;
                sigmaIetaIetaF2 = *subSigmaIetaIeta;
                etaWidthF2 = *subEtaWidth;
                phiWidthF2 = *subPhiWidth;
                covIEtaIPhiF2 = *subCovIEtaIPhi;
                s4F2 = *subS4;
                phoIso03F2 = *subPhoIso03;
                chgIsoWrtChosenVtxF2 = *subChgIsoWrtChosenVtx;
                chgIsoWrtWorstVtxF2 = *subChgIsoWrtWorstVtx;
                scEtaF2 = *subScEta;
                leadScEtaF2 = *leadScEta;
                subScEtaF2 = *subScEta;
                esEffSigmaRRF2 = *subEsEffSigmaRR;
                esEnergyOverRawEF2 = *subEsEnergyOverRawE;
                hadTowOverEmF2 = *subHadTowOverEm;
                hadronicOverEmF2 = *subHadronicOverEm;
                idMVAF2 = *subIDMVA;
                rhoF2 = *rhoIn;
                treeOutF2->Fill();
            }
        }
    }
    TList *listPrompt = new TList;
    listPrompt->Add(treeOutP1);
    listPrompt->Add(treeOutP2);
    
    TTree *treeOutP = TTree::MergeTrees(listPrompt);
    treeOutP->SetName("promptPhotons");
    
    treeOutP->Print();
    //treeOutP->Write();
    
    TList *listFake = new TList;
    listFake->Add(treeOutF1);
    listFake->Add(treeOutF2);
    TTree *treeOutF = TTree::MergeTrees(listFake);
    treeOutF->SetName("fakePhotons");

    treeOutF->Print();
    //treeOutF->Write();

    treeOutP1->Delete();
    treeOutP2->Delete();
    treeOutF1->Delete();
    treeOutF2->Delete();
    
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

