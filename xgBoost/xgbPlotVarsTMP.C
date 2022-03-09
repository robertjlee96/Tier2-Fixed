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

void xgbPlotVars(bool etaSplit, bool diphotonCuts){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);

    double xgbCut[4] = {0.1,0.1,0.1,0.1};
    string xgbCutStr[4] = {"0.1","0.1","0.1","0.1"};
    string eta[4] = {"Barrel-Barrel","Barrel-Endcap","Endcap-Barrel","Endcap-Endcap"};
    
    string fileNames[4] = {"validationNTuples/0217/GGH/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root",
        "validationNTuples/0217/GGH/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root",
        "validationNTuples/0217/GGH/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_Validation_Endcap.root",
        "validationNTuples/0217/GGH/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_Validation_Endcap.root"};
        
    string gjfLabelUncut = "GJet Fake All";
    string gjfLabelPresel = "GJet Fake Presel";
    string gjfLabelIDMVA1 = "GJet Fake W/ xgbScore > 0.1";
    string gjfLabelIDMVA2 = "GJet Fake Passing xgbScore but Not Presel";
    
    string gghLabelUncut = "400*GGH";
    string gghLabelPresel = "400*GGH Presel";
    string gghLabelIDMVA1 = "400*GGH W/ xgbScore > 0.1";
    string gghLabelIDMVA2 = "400*GGH Passing xgbScore but Not Presel";

    string dirStr ="varPlots/0216/";
    int nEta;
    string etaCuts[4];
    string etaLabels[4];
    string etaFLabels[4];
    
    if (etaSplit == true){
        nEta = 4;
        
        etaLabels[0] = "Barrel-Barrel";
        etaFLabels[0] = "_BB";
        
        etaLabels[1] = "Barrel-Endcap";
        etaFLabels[1] = "_BE";
        
        etaLabels[2] = "Barrel-Endcap";
        etaFLabels[2] = "_BE";

        etaLabels[3] = "Endcap-Endcap";
        etaFLabels[3] = "_EE";

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
    string outNameGen = dirStr + "GGHandGJet_NewVsOld_idMVACut";
    if(diphotonCuts == true)outNameGen += "_DiphotonCuts";
    if(diphotonCuts == false)outNameGen += "_SinglePhotonCuts";
    
    int nVars = 5;
    string varNames[5] = {"pt","scEta","hggMass","ptOverM","xgbScore"};
    string structVarNames[5] = {"XGBScores.pt","XGBScores.scEta","XGBScores.hggMass","XGBScores.ptOvrM","XGBScores.yPred"};
    double limsLow[5] = {0.0,-3.0,80.0,0.0,0.0};
    double limsHigh[5] = {200,3.0,180,1.3,1.05};
    int nBins[5] = {400,400,400,520,400};
    
    
    for(int i = 0; i < nVars; i++){
        
        TH1F *hPromptAll[4];
        TH1F *hPromptPresel[4];
        TH1F *hPromptIDMVA[4];
        TH1F *hPromptIDMVA2[4];
        
        TH1F *hFakeAll[4];
        TH1F *hFakePresel[4];
        TH1F *hFakeIDMVA[4];
        TH1F *hFakeIDMVA2[4];
    
        TTreeReader pReader[4];
        TTreeReader fReader[4];

        for (int e = 0; e < nEta; e++){
            hPromptAll[e] = new TH1F ("hPromptAll","",nBins[i],limsLow[i],limsHigh[i]);
            hPromptPresel[e] = new TH1F ("hPromptPresel","",nBins[i],limsLow[i],limsHigh[i]);
            hPromptIDMVA[e] = new TH1F ("hPromptIDMVA","",nBins[i],limsLow[i],limsHigh[i]);
            hPromptIDMVA2[e] = new TH1F ("hPromptIDMVA2","",nBins[i],limsLow[i],limsHigh[i]);
            
            hFakeAll[e] = new TH1F ("hFakeAll","",nBins[i],limsLow[i],limsHigh[i]);
            hFakePresel[e] = new TH1F ("hFakePresel","",nBins[i],limsLow[i],limsHigh[i]);
            hFakeIDMVA[e] = new TH1F ("hFakeIDMVA","",nBins[i],limsLow[i],limsHigh[i]);
            hFakeIDMVA2[e] = new TH1F ("hFakeIDMVA2","",nBins[i],limsLow[i],limsHigh[i]);
        }
        for (int e = 0; e < nEta; e++){
                                
            string outName = outNameGen + varNames[i] + etaFLabels[e];
    
            TFile *f = new TFile(fileNames[e].c_str());
            
            TTreeReader pReader("promptPhotons", f);
            pReader.Restart();
            TTreeReaderValue<Double_t> weightP(pReader, "XGBScores.weight");
            TTreeReaderValue<Double_t> secondScEtaP(pReader, "XGBScores.secondScEta");
            TTreeReaderValue<Double_t> yPredValsP(pReader, "XGBScores.yPred");
            TTreeReaderValue<Double_t> passDP(pReader, "XGBScores.passDPresel");
                        
            TTreeReader fReader("fakePhotons", f);
            fReader.Restart();
            TTreeReaderValue<Double_t> weightF(fReader, "XGBScores.weight");
            TTreeReaderValue<Double_t> secondScEtaF(fReader, "XGBScores.secondScEta");
            TTreeReaderValue<Double_t> yPredValsF(fReader, "XGBScores.yPred");
            TTreeReaderValue<Double_t> passDF(fReader, "XGBScores.passDPresel");
            
            TTreeReaderValue<Double_t> varP(pReader,structVarNames[i].c_str());
            TTreeReaderValue<Double_t> varF(fReader,structVarNames[i].c_str());
            
            
//            TFile *f = new TFile(fileNames[e].c_str());
//            TTree *tP = (TTree*)f->Get("promptPhotons");
//            TTree *tF = (TTree*)f->Get("fakePhotons");
//
//            TTreeReader pReader("promptPhotons", f);
//            TTreeReaderValue<Double_t> yPredValsP(pReader, "XGBScores.yPred");
//            TTreeReaderValue<Double_t> passDP(pReader, "XGBScores.passDPresel");
//            TTreeReaderValue<Double_t> massP(pReader, "XGBScores.hggMass");
//            TTreeReaderValue<Double_t> secondScEtaP(pReader, "XGBScores.secondScEta");
//            TTreeReaderValue<Double_t> ptP(pReader, "XGBScores.pt");
//            TTreeReaderValue<Double_t> scEtaP(pReader, "XGBScores.scEta");
//
//            TTreeReader fReader("fakePhotons", f);
//            TTreeReaderValue<Double_t> yPredValsF(fReader, "XGBScores.yPred");
//            TTreeReaderValue<Double_t> passDF(fReader, "XGBScores.passDPresel");
//            TTreeReaderValue<Double_t> weightF(fReader, "XGBScores.weight");
//            TTreeReaderValue<Double_t> massF(fReader, "XGBScores.hggMass");
//            TTreeReaderValue<Double_t> secondScEtaF(fReader, "XGBScores.secondScEta");
//            TTreeReaderValue<Double_t> ptF(fReader, "XGBScores.pt");
//            TTreeReaderValue<Double_t> scEtaF(fReader, "XGBScores.scEta");
            while (pReader.Next()) {
                if(((e == 0 || e == 2) && abs(*secondScEtaP) < 1.4442) || ((e == 1 || e == 3) && abs(*secondScEtaP) > 1.556)){
                    hPromptAll[e]->Fill(*varP,*weightP);
                    if (*passDP == 1.0)hPromptPresel[e]->Fill(*varP,*weightP);
                    if (*yPredValsP > xgbCut[i])hPromptIDMVA[e]->Fill(*varP,*weightP);
                    if (*yPredValsP > xgbCut[i] && *passDP != 1.0)hPromptIDMVA2[e]->Fill(*varP,*weightP);
                }
            }
            while (fReader.Next()) {
                if(((e == 0 || e == 2) && abs(*secondScEtaF) < 1.4442) || ((e == 1 || e == 3) && abs(*secondScEtaF) > 1.556)){
                    hFakeAll[e]->Fill(*varF,*weightF);
                    if (*passDF == 1.0)hFakePresel[e]->Fill(*varF,*weightF);
                    if (*yPredValsF > xgbCut[i])hFakeIDMVA[e]->Fill(*varF,*weightF);
                    if (*yPredValsF > xgbCut[i] && *passDF != 1.0)hFakeIDMVA2[e]->Fill(*varF,*weightF);
                }
            }
            if (e == 0 || e == 2 || e == 3){
                if (e == 2){
                    hPromptAll[e]->Add(hPromptAll[e-1]);
                    hPromptPresel[e]->Add(hPromptPresel[e-1]);
                    hPromptIDMVA[e]->Add(hPromptIDMVA[e-1]);
                    hPromptIDMVA2[e]->Add(hPromptIDMVA2[e-1]);
                    
                    hFakeAll[e]->Add(hFakeAll[e-1]);
                    hFakePresel[e]->Add(hFakePresel[e-1]);
                    hFakeIDMVA[e]->Add(hFakeIDMVA[e-1]);
                    hFakeIDMVA2[e]->Add(hFakeIDMVA2[e-1]);
                }
                TCanvas *can = new TCanvas ("can","can",10,10,1600,900);
                
                string plotTitle = varNames[i] + " for GGH " + eta[e] + ";"+varNames[i];
                THStack *hStack = new THStack("hStack",plotTitle.c_str());
                hStack->SetHistogram(new TH1F("hstot","",nBins[i],limsLow[i],limsHigh[i]));
                hStack->GetHistogram()->GetXaxis()->SetNdivisions(-520);


                TLegend *legend;
                if (varNames[i] == "IDMVA") legend = new TLegend(0.35,0.6,0.65,0.9,"","brNDC");
                else legend = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
               
                string labelAllP = gghLabelUncut;
                string labelPreselP = gghLabelPresel;
                string labelIDMVAP = gghLabelIDMVA1;
                string labelIDMVA2P = gghLabelIDMVA2;
                
                string labelAllF = gjfLabelUncut;
                string labelPreselF = gjfLabelPresel;
                string labelIDMVAF = gjfLabelIDMVA1;
                string labelIDMVA2F = gjfLabelIDMVA2;
                                
                char buff1[100], buff2[100], buff3[100], buff4[100], buff5[100], buff6[100],buff7[100], buff8[100];
                
                snprintf(buff1, sizeof(buff1), "(%0.2f)", hPromptAll[e]->GetSumOfWeights());
                string nEventsAllP = buff1;
                labelAllP += nEventsAllP;
                
                snprintf(buff2, sizeof(buff2), "(%0.2f)(%0.2f", hPromptPresel[e]->GetSumOfWeights(),100*(hPromptPresel[e]->GetSumOfWeights()/hPromptAll[e]->GetSumOfWeights()));
                string nEventsPreselP = buff2;
                labelPreselP += (nEventsPreselP + "%)");
                
                snprintf(buff3, sizeof(buff3), "(%0.2f)(%0.2f", hPromptIDMVA[e]->GetSumOfWeights(),100*(hPromptIDMVA[e]->GetSumOfWeights()/hPromptAll[e]->GetSumOfWeights()));
                string nEventsIDMVAP = buff3;
                labelIDMVAP += (nEventsIDMVAP + "%)");
                
                snprintf(buff4, sizeof(buff4), "(%0.2f)(%0.2f", hPromptIDMVA2[e]->GetSumOfWeights(),100*(hPromptIDMVA2[e]->GetSumOfWeights()/hPromptAll[e]->GetSumOfWeights()));
                string nEventsIDMVA2P = buff4;
                labelIDMVA2P += (nEventsIDMVA2P + "%)");
                
                snprintf(buff5, sizeof(buff5), "(%0.2f)", hFakeAll[e]->GetSumOfWeights());
                string nEventsAllF = buff5;
                labelAllF += nEventsAllF;
                
                snprintf(buff6, sizeof(buff6), "(%0.2f)(%0.2f", hFakePresel[e]->GetSumOfWeights(),100*(hFakePresel[e]->GetSumOfWeights()/hFakeAll[e]->GetSumOfWeights()));
                string nEventsFreselF = buff6;
                labelPreselF += (nEventsFreselF + "%)");
                
                snprintf(buff7, sizeof(buff7), "(%0.2f)(%0.2f", hFakeIDMVA[e]->GetSumOfWeights(),100*(hFakeIDMVA[e]->GetSumOfWeights()/hFakeAll[e]->GetSumOfWeights()));
                string nEventsIDMVAF = buff7;
                labelIDMVAF += (nEventsIDMVAF + "%)");
                
                snprintf(buff8, sizeof(buff8), "(%0.2f)(%0.2f", hFakeIDMVA2[e]->GetSumOfWeights(),100*(hFakeIDMVA2[e]->GetSumOfWeights()/hFakeAll[e]->GetSumOfWeights()));
                string nEventsIDMVA2F = buff8;
                labelIDMVA2F += (nEventsIDMVA2F + "%)");
                
                if(varNames[i] == "hggMass"){
                    char buff9[100], buff10[100], buff11[100], buff12[100];

                    snprintf(buff9, sizeof(buff9), "(Width = %0.2f)",getExactWidth(hPromptAll[e]));
                    string widthAll = buff9;
                    labelAllP += widthAll;
                    
                    snprintf(buff10, sizeof(buff10), "(Width = %0.2f)",getExactWidth(hPromptPresel[e]));
                    string widthPresel = buff10;
                    labelPreselP += widthPresel;
                    
                    snprintf(buff11, sizeof(buff11), "(Width = %0.2f)",getExactWidth(hPromptIDMVA[e]));
                    string widthIDMVA = buff11;
                    labelIDMVAP += widthIDMVA;
                    
                    snprintf(buff12, sizeof(buff12), "(Width = %0.2f)",getExactWidth(hPromptIDMVA2[e]));
                    string widthIDMVA2 = buff12;
                    labelIDMVA2P += widthIDMVA2;
                }
                hPromptAll[e]->SetLineColor(4);
                hPromptAll[e]->Scale(400);
                hStack->Add(hPromptAll[e]);
                legend->AddEntry(hPromptAll[e],labelAllP.c_str(),"pl");
                
                hPromptPresel[e]->SetLineColor(3);
                hPromptPresel[e]->Scale(400);
                hStack->Add(hPromptPresel[e]);
                legend->AddEntry(hPromptPresel[e],labelPreselP.c_str(),"pl");

                hPromptIDMVA[e]->SetLineColor(2);
                hPromptIDMVA[e]->Scale(400);
                hStack->Add(hPromptIDMVA[e]);
                legend->AddEntry(hPromptIDMVA[e],labelIDMVAP.c_str(),"pl");
                
                hPromptIDMVA2[e]->Scale(400);
                hStack->Add(hPromptIDMVA2[e]);
                legend->AddEntry(hPromptIDMVA2[e],labelIDMVA2P.c_str(),"pl");

                hFakeAll[e]->SetLineColor(4);
                hFakeAll[e]->SetLineStyle(9);
                hStack->Add(hFakeAll[e]);
                legend->AddEntry(hFakeAll[e],labelAllF.c_str(),"pl");
                
                hFakePresel[e]->SetLineColor(3);
                hFakePresel[e]->SetLineStyle(9);
                hStack->Add(hFakePresel[e]);
                legend->AddEntry(hFakePresel[e],labelPreselF.c_str(),"pl");
                
                hFakeIDMVA[e]->SetLineColor(2);
                hFakeIDMVA[e]->SetLineStyle(9);
                hStack->Add(hFakeIDMVA[e]);
                legend->AddEntry(hFakeIDMVA[e],labelIDMVAF.c_str(),"pl");
                
                hFakeIDMVA2[e]->SetLineStyle(9);
                hStack->Add(hFakeIDMVA2[e]);
                legend->AddEntry(hFakeIDMVA2[e],labelIDMVA2F.c_str(),"pl");
                
                hStack->Draw("nostackhist");
                legend->Draw("same");
                can->SetGrid();
                if (varNames[i] == "IDMVA") can->SetLogy(1);
                else can->SetLogy(0);
            
                can->SaveAs((outName+".png").c_str());
                can->SaveAs((outName+".root").c_str());
                
                can->Clear();
            }
            f->Close();
            f->Delete();
        }
        
        TH1F *hPromptAllTot = new TH1F ("hPromptAllTot","",nBins[i],limsLow[i],limsHigh[i]);
        TH1F *hPromptPreselTot = new TH1F ("hPromptPreselTot","",nBins[i],limsLow[i],limsHigh[i]);
        TH1F *hPromptIDMVATot = new TH1F ("hPromptIDMVATot","",nBins[i],limsLow[i],limsHigh[i]);
        TH1F *hPromptIDMVA2Tot = new TH1F ("hPromptIDMVA2Tot","",nBins[i],limsLow[i],limsHigh[i]);
        
        TH1F *hFakeAllTot = new TH1F ("hFakeAllTot","",nBins[i],limsLow[i],limsHigh[i]);
        TH1F *hFakePreselTot = new TH1F ("hFakePreselTot","",nBins[i],limsLow[i],limsHigh[i]);
        TH1F *hFakeIDMVATot = new TH1F ("hFakeIDMVATot","",nBins[i],limsLow[i],limsHigh[i]);
        TH1F *hFakeIDMVA2Tot = new TH1F ("hFakeIDMVA2Tot","",nBins[i],limsLow[i],limsHigh[i]);

        
        for (int p = 0; p < 4; p++){
            hPromptAllTot->Add(hPromptAll[p]);
            hPromptPreselTot->Add(hPromptPresel[p]);
            hPromptIDMVATot->Add(hPromptIDMVA[p]);
            hPromptIDMVA2Tot->Add(hPromptIDMVA2[p]);
            hFakeAllTot->Add(hFakeAll[p]);
            hFakePreselTot->Add(hFakePresel[p]);
            hFakeIDMVATot->Add(hFakeIDMVA[p]);
            hFakeIDMVA2Tot->Add(hFakeIDMVA2[p]);
            
            hPromptAll[p]->Delete();
            hPromptPresel[p]->Delete();
            hPromptIDMVA[p]->Delete();
            hPromptIDMVA2[p]->Delete();
            
            hFakeAll[p]->Delete();
            hFakePresel[p]->Delete();
            hFakeIDMVA[p]->Delete();
            hFakeIDMVA2[p]->Delete();
        }
        TCanvas *can = new TCanvas ("can","can",10,10,1600,900);
    
        string plotTitle = varNames[i] + " for GGH All Eta;"+varNames[i];
        THStack *hStack = new THStack("hStack",plotTitle.c_str());
        hStack->SetHistogram(new TH1F("hstot","",nBins[i],limsLow[i],limsHigh[i]));
        hStack->GetHistogram()->GetXaxis()->SetNdivisions(-520);
    
        TLegend *legend;
        if (varNames[i] == "IDMVA") legend = new TLegend(0.35,0.6,0.65,0.9,"","brNDC");
        else legend = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
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



