#include "TROOT.h"
#include "TKey.h"
#include "TFile.h"
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

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <algorithm>

int findWidth(TH1F *histo, int centerBin);
int findMinWindow(TH1F *histo,int &centerBin);
double getExactWidth(TH1F *histo);

void xgbPlotVarsSideBySide(bool etaSplit, bool diphotonCuts){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);

    double xgbCut[5] = {0.0228358,0.0228358,0.0228358,0.0228358,0.0228358};
    string xgbCutStr[5] = {"0.0228358","0.0228358","0.0228358","0.0228358","0.0228358"};
    double xgbCut2[5] = {0.238955,0.238955,0.238955,0.238955,0.238955};
    string xgbCutStr2[5] = {"0.238955","0.238955","0.238955","0.238955","0.238955"};
    double xgbCut3[5] = {0.976468,0.976468,0.976468,0.976468,0.976468};
    string xgbCutStr3[5] = {"0.976468","0.976468","0.976468","0.976468","0.976468"};
    string eta[5] = {"Barrel-Barrel","Barrel-Endcap","Endcap-Barrel","Endcap-Endcap","All"};
    
    string fileNames[5] = {"validationNTuples/0306/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root",
        "validationNTuples/0306/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root",
        "validationNTuples/0306/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_Validation_Endcap.root",
        "validationNTuples/0306/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_Validation_Endcap.root",
        "validationNTuples/0306/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root"};
        
    string gjfLabelUncut = "GJet Fake All";
    string gjfLabelPresel = "GJet Fake Presel";
    string gjfLabelIDMVA1 = "GJet Fake W/ xgbScore > 0.0228";
    string gjfLabelIDMVA2 = "GJet Fake W/ xgbScore > 0.2390";
    string gjfLabelIDMVA3 = "GJet Fake W/ xgbScore > 0.2390";
    
    string gghLabelUncut = "GGH";
    string gghLabelPresel = "GGH Presel";
    string gghLabelIDMVA1 = "GGH W/ xgbScore > 0.0228";
    string gghLabelIDMVA2 = "GGH W/ xgbScore > 0.2390";
    string gghLabelIDMVA3 = "GGH W/ xgbScore > 0.976468";

    string dirStr ="varPlots/0311/";
    int nEta;
    string etaCuts[5];
    string etaLabels[5];
    string etaFLabels[5];
    
    if (etaSplit == true){
        nEta = 5;
        
        etaLabels[0] = "Barrel-Barrel";
        etaFLabels[0] = "_BB";
        
        etaLabels[1] = "Barrel-Endcap";
        etaFLabels[1] = "_BE";
        
        etaLabels[2] = "Barrel-Endcap";
        etaFLabels[2] = "_BE";

        etaLabels[3] = "Endcap-Endcap";
        etaFLabels[3] = "_EE";
        
        etaLabels[4] = "All";
        etaFLabels[4] = "_All";

//        etaLabels[2] = "All Eta";
//        etaFLabels[2] = "_All";
//        etaCuts[2] = "";
    }
    
    if (etaSplit == false){
        nEta = 1;
        
        etaLabels[0] = "All Eta";
        etaFLabels[0] = "";
        etaCuts[0] = "";
    }
    string outNameGen = dirStr + "SideBySide_GGHandGJet_SameXGBCut_CompareToPresel";
    if(diphotonCuts == true)outNameGen += "_DiphotonCuts_";
    if(diphotonCuts == false)outNameGen += "_SinglePhotonCuts_";
    
    int nVars = 22;
    string varNames[22] = {"SCRawE","r9","sigmaIetaIeta","etaWidth","phiWidth","covIEtaIPhi","s4","phoIso03","chgIsoWrtChosenVtx","chgIsoWrtWorstVtx","scEta","rho","hadTowOverEm","hadronicOverEm","esEffSigmaRR","esEnergyOverRawE","pt","hggMass","hggMassCut","ptOvrM","weight","xgbScore"};
    double limsLow[22] = {0.0,0.0,0.0,0.0,0.0,-0.001,0.0,0.0,0.0,0.0,-3.0,0.0,0.0,0.0,0.0,0.0,0.0,90.0,90.0,0.0,-1.0,0.0};
    double limsHigh[22] = {650,1.1,0.1,0.1,0.2,0.001,1.0,25.0,25.0,25.0,3.0,60.0,0.4,1.0,8.0,0.2,200,180,180,1.3,1.0,1.0};
    int nBins[22] = {650,440,400,400,400,400,500,500,500,600,600,400,400,400,400,400,400,4500,4500,520,500,500};
    
    TCanvas *can = new TCanvas ("can","can",10,10,1600,900);
    
//    for(int i = 2; i < 4; i++){
    for(int i = 0; i < nVars; i++){
//    for(int i = 18; i < 19; i++){
        
        TH1F *hPromptAll[5];
        TH1F *hPromptPresel[5];
        TH1F *hPromptIDMVA[5];
        TH1F *hPromptIDMVA2[5];
        TH1F *hPromptIDMVA3[5];
        
        TH1F *hFakeAll[5];
        TH1F *hFakePresel[5];
        TH1F *hFakeIDMVA[5];
        TH1F *hFakeIDMVA2[5];
        TH1F *hFakeIDMVA3[5];
    
        TTreeReader pReader[5];
        TTreeReader fReader[5];

        for (int e = 0; e < nEta; e++){
            hPromptAll[e] = new TH1F ("hPromptAll","",nBins[i],limsLow[i],limsHigh[i]);
            hPromptPresel[e] = new TH1F ("hPromptPresel","",nBins[i],limsLow[i],limsHigh[i]);
            hPromptIDMVA[e] = new TH1F ("hPromptIDMVA","",nBins[i],limsLow[i],limsHigh[i]);
            hPromptIDMVA2[e] = new TH1F ("hPromptIDMVA2","",nBins[i],limsLow[i],limsHigh[i]);
            hPromptIDMVA3[e] = new TH1F ("hPromptIDMVA3","",nBins[i],limsLow[i],limsHigh[i]);
            
            hFakeAll[e] = new TH1F ("hFakeAll","",nBins[i],limsLow[i],limsHigh[i]);
            hFakePresel[e] = new TH1F ("hFakePresel","",nBins[i],limsLow[i],limsHigh[i]);
            hFakeIDMVA[e] = new TH1F ("hFakeIDMVA","",nBins[i],limsLow[i],limsHigh[i]);
            hFakeIDMVA2[e] = new TH1F ("hFakeIDMVA2","",nBins[i],limsLow[i],limsHigh[i]);
            hFakeIDMVA3[e] = new TH1F ("hFakeIDMVA3","",nBins[i],limsLow[i],limsHigh[i]);
        }
        for (int e = 0; e < nEta; e++){
                                
            string outName = outNameGen + varNames[i] + etaFLabels[e];
            
            TFile *f = new TFile(fileNames[e].c_str());
    
            if (e != 4){
                TTreeReader pReader("promptPhotons", f);
                pReader.Restart();
                TTreeReaderValue<Float_t> weightP(pReader, "weight");
                TTreeReaderValue<Float_t> yPredValsP(pReader, "xgbScore");
                TTreeReaderArray<Float_t> varValsP(pReader, "varVals");
                            
                TTreeReader fReader("fakePhotons", f);
                fReader.Restart();
                TTreeReaderValue<Float_t> weightF(fReader, "weight");
                TTreeReaderValue<Float_t> yPredValsF(fReader, "xgbScore");
                TTreeReaderArray<Float_t> varValsF(fReader, "varVals");
                                
                if (i < 18){
                    while (pReader.Next()) {
                        if( !(!(varNames[i] =="hggMass") && (varValsP[17] > 128.5 || varValsP[17] < 120.0))){
                            if(((e == 0 || e == 2) && abs(varValsP[20]) < 1.4442) || ((e == 1 || e == 3) && abs(varValsP[20]) > 1.556)){
                                hPromptAll[e]->Fill(varValsP[i],*weightP);
                                if (varValsP[19] == 1.0)hPromptPresel[e]->Fill(varValsP[i],*weightP);
                                if (*yPredValsP > xgbCut[e])hPromptIDMVA[e]->Fill(varValsP[i],*weightP);
                                if (*yPredValsP > xgbCut2[e])hPromptIDMVA2[e]->Fill(varValsP[i],*weightP);
                                if (*yPredValsP > xgbCut3[e])hPromptIDMVA3[e]->Fill(varValsP[i],*weightP);
                            }
                        }
                    }
                    while (fReader.Next()) {
                        if(((e == 0 || e == 2) && abs(varValsF[20]) < 1.4442) || ((e == 1 || e == 3) && abs(varValsF[20]) > 1.556)){
                            hFakeAll[e]->Fill(varValsF[i],*weightF);
                            if (varValsF[19] == 1.0)hFakePresel[e]->Fill(varValsF[i],*weightF);
                            if (*yPredValsF > xgbCut[e])hFakeIDMVA[e]->Fill(varValsF[i],*weightF);
                            if (*yPredValsF > xgbCut2[e])hFakeIDMVA2[e]->Fill(varValsF[i],*weightF);
                            if (*yPredValsF > xgbCut3[e])hFakeIDMVA3[e]->Fill(varValsF[i],*weightF);
                        }
                    }
                }
                if (varNames[i] == "hggMassCut"){
                    while (pReader.Next()) {
                        if(varValsP[17] < 128.5 && varValsP[17] > 120.0){
                            if(((e == 0 || e == 2) && abs(varValsP[20]) < 1.4442) || ((e == 1 || e == 3) && abs(varValsP[20]) > 1.556)){
                                hPromptAll[e]->Fill(varValsP[17],*weightP);
                                if (varValsP[19] == 1.0)hPromptPresel[e]->Fill(varValsP[17],*weightP);
                                if (*yPredValsP > xgbCut[e])hPromptIDMVA[e]->Fill(varValsP[17],*weightP);
                                if (*yPredValsP > xgbCut2[e])hPromptIDMVA2[e]->Fill(varValsP[17],*weightP);
                                if (*yPredValsP > xgbCut3[e])hPromptIDMVA3[e]->Fill(varValsP[17],*weightP);
                            }
                        }
                    }
                    while (fReader.Next()) {
                        if(((e == 0 || e == 2) && abs(varValsF[20]) < 1.4442) || ((e == 1 || e == 3) && abs(varValsF[20]) > 1.556)){
                            hFakeAll[e]->Fill(varValsF[17],*weightF);
                            if (varValsF[19] == 1.0)hFakePresel[e]->Fill(varValsF[17],*weightF);
                            if (*yPredValsF > xgbCut[e])hFakeIDMVA[e]->Fill(varValsF[17],*weightF);
                            if (*yPredValsF > xgbCut2[e])hFakeIDMVA2[e]->Fill(varValsF[17],*weightF);
                            if (*yPredValsF > xgbCut3[e])hFakeIDMVA3[e]->Fill(varValsF[17],*weightF);
                        }
                    }
                }
                if (varNames[i] == "ptOvrM"){
                    while (pReader.Next()) {
                        if( !(!(varNames[i] =="hggMass") && (varValsP[17] > 128.5 || varValsP[17] < 120.0))){
                            if(((e == 0 || e == 2) && abs(varValsP[20]) < 1.4442) || ((e == 1 || e == 3) && abs(varValsP[20]) > 1.556)){
                                hPromptAll[e]->Fill(varValsP[16]/varValsP[17],*weightP);
                                if (varValsP[19] == 1.0)hPromptPresel[e]->Fill(varValsP[16]/varValsP[17],*weightP);
                                if (*yPredValsP > xgbCut[e])hPromptIDMVA[e]->Fill(varValsP[16]/varValsP[17],*weightP);
                                if (*yPredValsP > xgbCut2[e])hPromptIDMVA2[e]->Fill(varValsP[16]/varValsP[17],*weightP);
                                if (*yPredValsP > xgbCut3[e])hPromptIDMVA3[e]->Fill(varValsP[16]/varValsP[17],*weightP);
                            }
                        }
                    }
                    while (fReader.Next()) {
                        if(((e == 0 || e == 2) && abs(varValsF[20]) < 1.4442) || ((e == 1 || e == 3) && abs(varValsF[20]) > 1.556)){
                            hFakeAll[e]->Fill(varValsF[16]/varValsF[17],*weightF);
                            if (varValsF[19] == 1.0)hFakePresel[e]->Fill(varValsF[16]/varValsF[17],*weightF);
                            if (*yPredValsF > xgbCut[e])hFakeIDMVA[e]->Fill(varValsF[16]/varValsF[17],*weightF);
                            if (*yPredValsF > xgbCut2[e])hFakeIDMVA2[e]->Fill(varValsF[16]/varValsF[17],*weightF);
                            if (*yPredValsF > xgbCut3[e])hFakeIDMVA3[e]->Fill(varValsF[16]/varValsF[17],*weightF);
                        }
                    }
                }
                if (varNames[i] == "weight"){
                    while (pReader.Next()) {
                        if( !(!(varNames[i] =="hggMass") && (varValsP[17] > 128.5 || varValsP[17] < 120.0))){
                            if(((e == 0 || e == 2) && abs(varValsP[20]) < 1.4442) || ((e == 1 || e == 3) && abs(varValsP[20]) > 1.556)){
                                hPromptAll[e]->Fill(*weightP);
                                if (varValsP[19] == 1.0)hPromptPresel[e]->Fill(*weightP);
                                if (*yPredValsP > xgbCut[e])hPromptIDMVA[e]->Fill(*weightP);
                                if (*yPredValsP > xgbCut2[e])hPromptIDMVA2[e]->Fill(*weightP);
                                if (*yPredValsP > xgbCut3[e])hPromptIDMVA3[e]->Fill(*weightP);
                            }
                        }
                    }
                    while (fReader.Next()) {
                        if(((e == 0 || e == 2) && abs(varValsF[20]) < 1.4442) || ((e == 1 || e == 3) && abs(varValsF[20]) > 1.556)){
                            hFakeAll[e]->Fill(*weightF);
                            if (varValsF[19] == 1.0)hFakePresel[e]->Fill(*weightF);
                            if (*yPredValsF > xgbCut[e])hFakeIDMVA[e]->Fill(*weightF);
                            if (*yPredValsF > xgbCut2[e])hFakeIDMVA2[e]->Fill(*weightF);
                            if (*yPredValsF > xgbCut3[e])hFakeIDMVA3[e]->Fill(*weightF);
                        }
                    }
                }
                if (varNames[i] == "xgbScore"){
                    while (pReader.Next()) {
                        if( !(!(varNames[i] =="hggMass") && (varValsP[17] > 128.5 || varValsP[17] < 120.0))){
                            if(((e == 0 || e == 2) && abs(varValsP[20]) < 1.4442) || ((e == 1 || e == 3) && abs(varValsP[20]) > 1.556)){
                                hPromptAll[e]->Fill(*yPredValsP,*weightP);
                                if (varValsP[19] == 1.0)hPromptPresel[e]->Fill(*yPredValsP,*weightP);
                                if (*yPredValsP > xgbCut[e])hPromptIDMVA[e]->Fill(*yPredValsP,*weightP);
                                if (*yPredValsP > xgbCut2[e])hPromptIDMVA2[e]->Fill(*yPredValsP,*weightP);
                                if (*yPredValsP > xgbCut3[e])hPromptIDMVA3[e]->Fill(*yPredValsP,*weightP);
                            }
                        }
                    }
                    while (fReader.Next()) {
                        if(((e == 0 || e == 2) && abs(varValsF[20]) < 1.4442) || ((e == 1 || e == 3) && abs(varValsF[20]) > 1.556)){
                            hFakeAll[e]->Fill(*yPredValsF,*weightF);
                            if (varValsF[19] == 1.0)hFakePresel[e]->Fill(*yPredValsF,*weightF);
                            if (*yPredValsF > xgbCut[e])hFakeIDMVA[e]->Fill(*yPredValsF,*weightF);
                            if (*yPredValsF > xgbCut2[e])hFakeIDMVA2[e]->Fill(*yPredValsF,*weightF);
                            if (*yPredValsF > xgbCut3[e])hFakeIDMVA3[e]->Fill(*yPredValsF,*weightF);
                        }
                    }
                }
            }
            if (e == 0 || e == 2 || e == 3 || e == 4){
                
                if (e == 2){
                    hPromptAll[e]->Add(hPromptAll[e-1]);
                    hPromptPresel[e]->Add(hPromptPresel[e-1]);
                    hPromptIDMVA[e]->Add(hPromptIDMVA[e-1]);
                    hPromptIDMVA2[e]->Add(hPromptIDMVA2[e-1]);
                    hPromptIDMVA3[e]->Add(hPromptIDMVA3[e-1]);
                    
                    hFakeAll[e]->Add(hFakeAll[e-1]);
                    hFakePresel[e]->Add(hFakePresel[e-1]);
                    hFakeIDMVA[e]->Add(hFakeIDMVA[e-1]);
                    hFakeIDMVA2[e]->Add(hFakeIDMVA2[e-1]);
                    hFakeIDMVA3[e]->Add(hFakeIDMVA3[e-1]);
                }
                
                if (e == 4){
                    for (int p = 0; p < 4; p++){
                        if (p != 1){
//
                            hPromptAll[e]->Add(hPromptAll[p]);
                            hPromptPresel[e]->Add(hPromptPresel[p]);
                            hPromptIDMVA[e]->Add(hPromptIDMVA[p]);
                            hPromptIDMVA2[e]->Add(hPromptIDMVA2[p]);
                            hPromptIDMVA3[e]->Add(hPromptIDMVA3[p]);
                                                        
                            hFakeAll[e]->Add(hFakeAll[p]);
                            hFakePresel[e]->Add(hFakePresel[p]);
                            hFakeIDMVA[e]->Add(hFakeIDMVA[p]);
                            hFakeIDMVA2[e]->Add(hFakeIDMVA2[p]);
                            hFakeIDMVA3[e]->Add(hFakeIDMVA3[p]);
                        }
                    }
                }
                can->Clear();
                can->Divide(2,1);
                
                string plotTitleGGH = varNames[i] + " for GGH " + eta[e] + ";"+varNames[i];
                THStack *hStackGGH = new THStack("hStack",plotTitleGGH.c_str());
                hStackGGH->SetHistogram(new TH1F("hstot","",nBins[i],limsLow[i],limsHigh[i]));
                hStackGGH->GetHistogram()->GetXaxis()->SetNdivisions(-520);
                hStackGGH->GetHistogram()->GetXaxis()->SetLabelSize(0.012);
                
                string plotTitleGJet = varNames[i] + " for GJet Fake " + eta[e] + ";"+varNames[i];
                THStack *hStackGJet = new THStack("hStack",plotTitleGJet.c_str());
                hStackGJet->SetHistogram(new TH1F("hstot","",nBins[i],limsLow[i],limsHigh[i]));
                hStackGJet->GetHistogram()->GetXaxis()->SetNdivisions(-520);
                hStackGJet->GetHistogram()->GetXaxis()->SetLabelSize(0.012);


                TLegend *legendGGH;
                TLegend *legendGJet;
                if (varNames[i] == "IDMVA") legendGGH = new TLegend(0.35,0.6,0.65,0.9,"","brNDC");
                else legendGGH = new TLegend(0.5,0.7,0.9,0.9,"","brNDC");
                if (varNames[i] == "IDMVA") legendGJet = new TLegend(0.35,0.6,0.65,0.9,"","brNDC");
                else legendGJet = new TLegend(0.5,0.7,0.9,0.9,"","brNDC");
               
                string labelAllP = gghLabelUncut;
                string labelPreselP = gghLabelPresel;
                string labelIDMVAP = gghLabelIDMVA1;
                string labelIDMVA2P = gghLabelIDMVA2;
                string labelIDMVA3P = gghLabelIDMVA3;
                
                string labelAllF = gjfLabelUncut;
                string labelPreselF = gjfLabelPresel;
                string labelIDMVAF = gjfLabelIDMVA1;
                string labelIDMVA2F = gjfLabelIDMVA2;
                string labelIDMVA3F = gjfLabelIDMVA3;
                                
                char buff1[100], buff2[100], buff3[100], buff4[100], buff5[100], buff6[100],buff7[100], buff8[100],buff9[100],buff10[100],buff11[100],buff12[100],buff13[100],buff14[100],buff15[100];
                
                snprintf(buff1, sizeof(buff1), "(%0.2f)", hPromptAll[e]->GetSumOfWeights());
//                snprintf(buff1, sizeof(buff1), "(%0.2f)", hPromptAll[e]->GetEntries());
                string nEventsAllP = buff1;
                labelAllP += nEventsAllP;
                
                snprintf(buff2, sizeof(buff2), "(%0.2f)(%0.2f", hPromptPresel[e]->GetSumOfWeights(),100*(hPromptPresel[e]->GetSumOfWeights()/hPromptAll[e]->GetSumOfWeights()));
//                snprintf(buff2, sizeof(buff2), "(%0.2f)(%0.2f", hPromptPresel[e]->GetEntries(),100*(hPromptPresel[e]->GetSumOfWeights()/hPromptAll[e]->GetSumOfWeights()));
                string nEventsPreselP = buff2;
                labelPreselP += (nEventsPreselP + "%)");
                
                snprintf(buff3, sizeof(buff3), "(%0.2f)(%0.2f", hPromptIDMVA[e]->GetSumOfWeights(),100*(hPromptIDMVA[e]->GetSumOfWeights()/hPromptAll[e]->GetSumOfWeights()));
//                snprintf(buff3, sizeof(buff3), "(%0.2f)(%0.2f", hPromptIDMVA[e]->GetEntries(),100*(hPromptIDMVA[e]->GetSumOfWeights()/hPromptAll[e]->GetSumOfWeights()));
                string nEventsIDMVAP = buff3;
                labelIDMVAP += (nEventsIDMVAP + "%)");
                
                snprintf(buff4, sizeof(buff4), "(%0.2f)(%0.2f", hPromptIDMVA2[e]->GetSumOfWeights(),100*(hPromptIDMVA2[e]->GetSumOfWeights()/hPromptAll[e]->GetSumOfWeights()));
//                snprintf(buff4, sizeof(buff4), "(%0.2f)(%0.2f", hPromptIDMVA2[e]->GetEntries(),100*(hPromptIDMVA2[e]->GetSumOfWeights()/hPromptAll[e]->GetSumOfWeights()));
                string nEventsIDMVA2P = buff4;
                labelIDMVA2P += (nEventsIDMVA2P + "%)");
                
                snprintf(buff5, sizeof(buff5), "(%0.2f)(%0.2f", hPromptIDMVA3[e]->GetSumOfWeights(),100*(hPromptIDMVA3[e]->GetSumOfWeights()/hPromptAll[e]->GetSumOfWeights()));
                //                snprintf(buff4, sizeof(buff4), "(%0.2f)(%0.2f", hPromptIDMVA2[e]->GetEntries(),100*(hPromptIDMVA2[e]->GetSumOfWeights()/hPromptAll[e]->GetSumOfWeights()));
                string nEventsIDMVA3P = buff5;
                labelIDMVA3P += (nEventsIDMVA3P + "%)");
                
                snprintf(buff6, sizeof(buff6), "(%0.2f)", hFakeAll[e]->GetSumOfWeights());
//                snprintf(buff5, sizeof(buff5), "(%0.2f)", hFakeAll[e]->GetEntries());
                string nEventsAllF = buff6;
                labelAllF += nEventsAllF;
                
                snprintf(buff7, sizeof(buff7), "(%0.2f)(%0.2f", hFakePresel[e]->GetSumOfWeights(),100*(hFakePresel[e]->GetSumOfWeights()/hFakeAll[e]->GetSumOfWeights()));
//                snprintf(buff6, sizeof(buff6), "(%0.2f)(%0.2f", hFakePresel[e]->GetEntries(),100*(hFakePresel[e]->GetSumOfWeights()/hFakeAll[e]->GetSumOfWeights()));
                string nEventsFreselF = buff7;
                labelPreselF += (nEventsFreselF + "%)");
                
                snprintf(buff8, sizeof(buff8), "(%0.2f)(%0.2f", hFakeIDMVA[e]->GetSumOfWeights(),100*(hFakeIDMVA[e]->GetSumOfWeights()/hFakeAll[e]->GetSumOfWeights()));
//                snprintf(buff7, sizeof(buff7), "(%0.2f)(%0.2f", hFakeIDMVA[e]->GetEntries(),100*(hFakeIDMVA[e]->GetSumOfWeights()/hFakeAll[e]->GetSumOfWeights()));
                string nEventsIDMVAF = buff8;
                labelIDMVAF += (nEventsIDMVAF + "%)");
                
                snprintf(buff9, sizeof(buff9), "(%0.2f)(%0.2f", hFakeIDMVA2[e]->GetSumOfWeights(),100*(hFakeIDMVA2[e]->GetSumOfWeights()/hFakeAll[e]->GetSumOfWeights()));
//                snprintf(buff8, sizeof(buff8), "(%0.2f)(%0.2f", hFakeIDMVA2[e]->GetEntries(),100*(hFakeIDMVA2[e]->GetSumOfWeights()/hFakeAll[e]->GetSumOfWeights()));
                string nEventsIDMVA2F = buff9;
                labelIDMVA2F += (nEventsIDMVA2F + "%)");
                
                snprintf(buff10, sizeof(buff10), "(%0.2f)(%0.2f", hFakeIDMVA3[e]->GetSumOfWeights(),100*(hFakeIDMVA3[e]->GetSumOfWeights()/hFakeAll[e]->GetSumOfWeights()));
                //                snprintf(buff8, sizeof(buff8), "(%0.2f)(%0.2f", hFakeIDMVA2[e]->GetEntries(),100*(hFakeIDMVA2[e]->GetSumOfWeights()/hFakeAll[e]->GetSumOfWeights()));
                string nEventsIDMVA3F = buff10;
                labelIDMVA3F += (nEventsIDMVA3F + "%)");
                
                if(varNames[i] == "hggMass" || varNames[i] == "hggMassCut" ){
                    char buff9[100], buff10[100], buff11[100], buff12[100];

                    snprintf(buff11, sizeof(buff11), "(Width = %0.2f)",getExactWidth(hPromptAll[e]));
                    string widthAll = buff11;
                    labelAllP += widthAll;
                    
                    snprintf(buff12, sizeof(buff12), "(Width = %0.2f)",getExactWidth(hPromptPresel[e]));
                    string widthPresel = buff12;
                    labelPreselP += widthPresel;
                    
                    snprintf(buff13, sizeof(buff13), "(Width = %0.2f)",getExactWidth(hPromptIDMVA[e]));
                    string widthIDMVA = buff13;
                    labelIDMVAP += widthIDMVA;
                    
                    snprintf(buff14, sizeof(buff14), "(Width = %0.2f)",getExactWidth(hPromptIDMVA2[e]));
                    string widthIDMVA2 = buff14;
                    labelIDMVA2P += widthIDMVA2;
                    
                    snprintf(buff15, sizeof(buff15), "(Width = %0.2f)",getExactWidth(hPromptIDMVA3[e]));
                    string widthIDMVA3 = buff15;
                    labelIDMVA3P += widthIDMVA3;
                }
                hPromptAll[e]->SetLineColor(4);
//                hPromptAll[e]->Scale(400.0);
                hStackGGH->Add(hPromptAll[e]);
                legendGGH->AddEntry(hPromptAll[e],labelAllP.c_str(),"pl");
                
                hPromptPresel[e]->SetLineColor(3);
//                hPromptPresel[e]->Scale(400.0);
                hStackGGH->Add(hPromptPresel[e]);
                legendGGH->AddEntry(hPromptPresel[e],labelPreselP.c_str(),"pl");

                hPromptIDMVA[e]->SetLineColor(2);
//                hPromptIDMVA[e]->Scale(400.0);
                hStackGGH->Add(hPromptIDMVA[e]);
                legendGGH->AddEntry(hPromptIDMVA[e],labelIDMVAP.c_str(),"pl");
                
//                hPromptIDMVA2[e]->Scale(400.0);
                hStackGGH->Add(hPromptIDMVA2[e]);
                legendGGH->AddEntry(hPromptIDMVA2[e],labelIDMVA2P.c_str(),"pl");
                
                hPromptIDMVA3[e]->SetLineColor(5);
//                hPromptIDMVA[e]->Scale(400.0);
                hStackGGH->Add(hPromptIDMVA3[e]);
                legendGGH->AddEntry(hPromptIDMVA3[e],labelIDMVA3P.c_str(),"pl");

                hFakeAll[e]->SetLineColor(4);
                hFakeAll[e]->SetLineStyle(9);
                hStackGJet->Add(hFakeAll[e]);
                legendGJet->AddEntry(hFakeAll[e],labelAllF.c_str(),"pl");
                
                hFakePresel[e]->SetLineColor(3);
                hFakePresel[e]->SetLineStyle(9);
                hStackGJet->Add(hFakePresel[e]);
                legendGJet->AddEntry(hFakePresel[e],labelPreselF.c_str(),"pl");
                
                hFakeIDMVA[e]->SetLineColor(2);
                hFakeIDMVA[e]->SetLineStyle(9);
                hStackGJet->Add(hFakeIDMVA[e]);
                legendGJet->AddEntry(hFakeIDMVA[e],labelIDMVAF.c_str(),"pl");
                
                hFakeIDMVA2[e]->SetLineStyle(9);
                hStackGJet->Add(hFakeIDMVA2[e]);
                legendGJet->AddEntry(hFakeIDMVA2[e],labelIDMVA2F.c_str(),"pl");
                
                hFakeIDMVA3[e]->SetLineColor(5);
                hFakeIDMVA3[e]->SetLineStyle(9);
                hStackGJet->Add(hFakeIDMVA3[e]);
                legendGJet->AddEntry(hFakeIDMVA3[e],labelIDMVA3F.c_str(),"pl");
//
                if(e == 4 && (varNames[i] == "hggMass" || varNames[i] == "hggMassCut")){
                    hPromptAll[e]->Rebin(10);
                    hPromptPresel[e]->Rebin(10);
                    hPromptIDMVA[e]->Rebin(10);
                    hPromptIDMVA2[e]->Rebin(10);
                    hPromptIDMVA3[e]->Rebin(10);
                }
                can->cd(1);
                gPad->SetGrid();
                hStackGGH->Draw("nostackhist");
                legendGGH->Draw("same");
                
                can->cd(2);
                gPad->SetGrid();
                hStackGJet->Draw("nostackhist");
                legendGJet->Draw("same");

                if (varNames[i] == "xgbScore") can->SetLogy(1);
                else can->SetLogy(0);
            
                can->SaveAs((outName+".png").c_str());
                can->SaveAs((outName+".root").c_str());
                
                can->Clear();
//                hPromptAll[e]->Scale(1.0/400.0);
//                hPromptPresel[e]->Scale(1.0/400.0);
//                hPromptIDMVA[e]->Scale(1.0/400.0);
//                hPromptIDMVA2[e]->Scale(1.0/400.0);
            }
            f->Close();
            f->Delete();
        }
        
        for (int p = 0; p < 4; p++){
            hPromptAll[p]->Delete();
            hPromptPresel[p]->Delete();
            hPromptIDMVA[p]->Delete();
            hPromptIDMVA2[p]->Delete();
            
            hFakeAll[p]->Delete();
            hFakePresel[p]->Delete();
            hFakeIDMVA[p]->Delete();
            hFakeIDMVA2[p]->Delete();
        }
    }
}

int findWidth(TH1F *histo, int centerBin){
    //    int maxBin = histo->GetMaximumBin();
    //    cout<<"Center = "<<histo->GetXaxis()->GetBinCenter(maxBin)<<endl;;
    //    cout<<"Center = "<<histo->GetXaxis()->GetBinCenter(centerBin)<<endl;;
    double integral = 0.0;
    double total = histo->Integral();
    int nBinsFromCenter = 0;
    while (integral < 0.68*total){
        nBinsFromCenter += 1;
        integral = histo->Integral(centerBin-nBinsFromCenter,centerBin+nBinsFromCenter);
    }
    int widthInBins = (nBinsFromCenter*2)+1;
    //    cout<<"widthInBins = "<<widthInBins<<endl;
    double widthInGeV = (histo->GetBinWidth(0))*widthInBins;
    //    cout<<"widthInGeV = "<<widthInGeV<<endl;
    
    //return widthInGeV;
    return widthInBins;
}
int findMinWindow(TH1F *histo,int &centerBin){
    int maxBin = histo->GetMaximumBin(); //Seed with maximum bin
    
    int nBinsShift[101];// = {-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10};
    for (int i = 0; i < 101; i++)nBinsShift[i] = -50 + i;
    int minNumBins = 100000;
    double minCenter = 0.0;
    double minWidth = 0.0;
    double percent = 0.0;
    
    for(int i = 0; i < 41; i++){
        int centerBinTmp = maxBin + nBinsShift[i];
        int nBinsTmp = findWidth(histo,centerBinTmp);
        if(nBinsTmp < minNumBins){
            minNumBins = nBinsTmp;
            minCenter = histo->GetXaxis()->GetBinCenter(centerBin);
            minWidth = (histo->GetBinWidth(0))*minNumBins;
            centerBin = centerBinTmp;
        }
        
    }
    //    cout<<"Min Width = "<<minWidth<<" at Center = "<<minCenter<<endl;
    
    //return minWidth;
    return minNumBins;
}
double getExactWidth(TH1F *histo){
    
    int centralBin;
    int nBins = findMinWindow(histo,centralBin);
    //int centralBin = histo->GetXaxis()->GetBinCenter(bestCenter);
    double total = histo->Integral();
    
    double propGoal = 0.68;
    int nFromCenter = (nBins-1)/2;
    //cout<<"nFromCenter = "<<nFromCenter<<endl;
    double currentProp = (histo->Integral(centralBin-nFromCenter,centralBin+nFromCenter))/total;
    //    cout<<"nBins = "<<nBins<<" currentProp = "<<currentProp<<endl;
    double diffProp = currentProp - propGoal;
    double diffSum = diffProp*total; //Total sum of weights that must be removed to get to 68%
    
    int higherBin;
    
    if(histo->GetBinContent(centralBin-(nBins/2)) > histo->GetBinContent(centralBin+(nBins/2))) higherBin = centralBin-(nBins/2);
    if(histo->GetBinContent(centralBin-(nBins/2)) < histo->GetBinContent(centralBin+(nBins/2))) higherBin = centralBin+(nBins/2);
    
    double contentOfLargerBin = histo->GetBinContent(higherBin);
    
    double propToDiscard = (diffSum/contentOfLargerBin);
    //    cout<<"prop of larger bin to discard = "<<propToDiscard<<endl;
    
    double differenceInWidth = propToDiscard*(histo->GetBinWidth(0)); //ASSUME CONSTANT DISTRIBUTION THROUGH BIN. ONLY INCLUDE FRACTION OF BIN WIDTH IN OVERALL WIDTH CALCULATION
                                                                      //    cout<<"differenceInWidth = "<<differenceInWidth<<endl;
    
    double widthInGeV = (histo->GetBinWidth(0))*nBins - differenceInWidth;
    double finalProp = (histo->Integral(centralBin-nFromCenter,centralBin+nFromCenter))/total - (propToDiscard*(histo->GetBinContent(higherBin)))/total;
    
    cout<<"Final Width = "<<widthInGeV<<" With proportion of events = "<<finalProp<<endl;
    
    return widthInGeV;
    
}


