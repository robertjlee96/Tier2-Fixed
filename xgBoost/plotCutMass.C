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

struct myStruct {
    Double_t     yVal;
    Double_t     yPred;
};

double findWidth(TH1F *histo){
    int maxBin = histo->GetMaximumBin();
    cout<<"Center = "<<histo->GetXaxis()->GetBinCenter(maxBin)<<endl;;
    double integral = 0.0;
    double total = histo->Integral();
    int nBinsFromCenter = 0;
    while (integral < 0.68*total){
        nBinsFromCenter += 1;
        integral = histo->Integral(maxBin-nBinsFromCenter,maxBin+nBinsFromCenter);
    }
    int widthInBins = (nBinsFromCenter*2)+1;
    cout<<"widthInBins = "<<widthInBins<<endl;
    double widthInGeV = (histo->GetBinWidth(0))*widthInBins;
    
    return widthInGeV;
}

void plotCutMass(){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);
    

    string fOut = "varPlots/GGH_Mass_MD20LR045_BandE_PassFailPresel_3EtaCuts_0209.root";
    //string fOutF = "idMVAPlots/GJetIDMVAF_LRSearch18_Endcap_0127.rooot";
    
    double xgbCut[4] = {0.00696517,0.00696517,0.132935,0.132935};
    string xgbCutStr[4] = {"0.00696517","0.00696517","0.132935","0.132935"};
    string eta[4] = {"Barrel-Barrel","Barrel-Endcap","Endcap-Barrel","Endcap-Endcap"};
        
    string fileNames[4] = {"validationNTuples/0207/GGH/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root","validationNTuples/0207/GGH/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root","validationNTuples/0207/GGH/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_Validation_Endcap.root","validationNTuples/0207/GGH/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_Validation_Endcap.root"};
    
    TCanvas *can = new TCanvas ("can","can",10,10,1920,1080);
    can->Divide(3);
    
    int nBins = 400;
    int plotFactor = 10;
    int nBinsInit = nBins * plotFactor;
    
    TH1F *hPromptAll[4];
    TH1F *hPromptPassPass[4];
    TH1F *hPromptPassFail[4];
    TH1F *hPromptFailPass[4];
    TH1F *hPromptFailFail[4];
    
    for (int i = 0; i < 4; i++){
        if (i == 0)can->cd(1);
        if (i == 2)can->cd(2);
        if (i == 3)can->cd(3);
        TLegend *legend = new TLegend(0.53,0.60,0.89,0.89,"","brNDC");
    //    hPromptPassPass where first Pass/Fail is for Presel, second is for idMVA cut
        hPromptAll[i] = new TH1F ("hPromptAll","",nBinsInit,90,180);
        hPromptPassPass[i] = new TH1F ("hPromptPassPass","",nBinsInit,90,180);
        hPromptPassFail[i] = new TH1F ("hPromptPassFail","",nBinsInit,90,180);
        TH1F *hFakePassPass = new TH1F ("hFakePassPass","",nBinsInit,90,180);
        TH1F *hFakePassFail = new TH1F ("hFakePassFail","",nBinsInit,90,180);
        hPromptFailPass[i] = new TH1F ("hPromptFailPass","",nBinsInit,90,180);
        hPromptFailFail[i] = new TH1F ("hPromptFailPFail","",nBinsInit,90,180);
        TH1F *hFakeFailPass = new TH1F ("hFakeFailPass","",nBinsInit,90,180);
        TH1F *hFakeFailFail = new TH1F ("hFakeFailFail","",nBinsInit,90,180);
        
        TH1F *hPass = new TH1F ("hPass","",nBinsInit,90,180);
        TH1F *hFail= new TH1F ("hFail","",nBinsInit,90,180);
        
        string titleMass = "hggMass for GGH " + eta[i] + ";hggMass(GeV)";
        THStack *mStack = new THStack("mStack",titleMass.c_str());
        
    //    THStack *fStack = new THStack("fStack",titleFake.c_str());
        TFile *f = new TFile(fileNames[i].c_str());
        TTree *tP = (TTree*)f->Get("promptPhotons");
        TTree *tF = (TTree*)f->Get("fakePhotons");
        
        TTreeReader pReader("promptPhotons", f);
        TTreeReaderValue<Double_t> yPredValsP(pReader, "XGBScores.yPred");
        TTreeReaderValue<Double_t> passDP(pReader, "XGBScores.passDPresel");
        TTreeReaderValue<Double_t> weightP(pReader, "XGBScores.weight");
        TTreeReaderValue<Double_t> massP(pReader, "XGBScores.hggMass");
        TTreeReaderValue<Double_t> secondScEtaP(pReader, "XGBScores.secondScEta");
        
        TTreeReader fReader("fakePhotons", f);
        TTreeReaderValue<Double_t> yPredValsF(fReader, "XGBScores.yPred");
        TTreeReaderValue<Double_t> passDF(fReader, "XGBScores.passDPresel");
        TTreeReaderValue<Double_t> weightF(fReader, "XGBScores.weight");
        TTreeReaderValue<Double_t> massF(fReader, "XGBScores.hggMass");
        TTreeReaderValue<Double_t> secondScEtaF(fReader, "XGBScores.secondScEta");
       
        while (pReader.Next()) {
            if(((i == 0 || i == 2) && abs(*secondScEtaP) < 1.4442) || ((i == 1 || i == 3) && abs(*secondScEtaP) > 1.556)){
                hPromptAll[i]->Fill(*massP,*weightP);
                if (*yPredValsP > xgbCut[i] && *passDP == 1.0)hPromptPassPass[i]->Fill(*massP,*weightP);
                if (*yPredValsP < xgbCut[i] && *passDP == 1.0)hPromptPassFail[i]->Fill(*massP,*weightP);
                if (*yPredValsP > xgbCut[i] && *passDP != 1.0)hPromptFailPass[i]->Fill(*massP,*weightP);
                if (*yPredValsP < xgbCut[i] && *passDP != 1.0)hPromptFailFail[i]->Fill(*massP,*weightP);
            }
        }
        while (fReader.Next()) {
            if((i == 0 && abs(*secondScEtaF) < 1.4442) || i == 1){
                if (*yPredValsF > xgbCut[i] && *passDF == 1.0)hFakePassPass->Fill(*massF,*weightF);
                if (*yPredValsF < xgbCut[i] && *passDF == 1.0)hFakePassFail->Fill(*massF,*weightF);
                if (*yPredValsF > xgbCut[i] && *passDF != 1.0)hFakeFailPass->Fill(*massF,*weightF);
                if (*yPredValsF < xgbCut[i] && *passDF != 1.0)hFakeFailFail->Fill(*massF,*weightF);
            }
        }
        hPromptAll[i]->SetLineColor(4);
        hPromptPassPass[i]->SetLineColor(1);
        hPromptPassFail[i]->SetLineColor(1);
        hPromptPassFail[i]->SetLineStyle(3);
        
        hPromptFailPass[i]->SetLineColor(2);
        hPromptFailFail[i]->SetLineColor(2);
        hPromptFailFail[i]->SetLineStyle(3);
        
        if (i == 0 || i == 2 || i == 3){
            if (i == 2){
                hPromptAll[i]->Add(hPromptAll[i-1]);
                hPromptPassPass[i]->Add(hPromptPassPass[i-1]);
                hPromptPassFail[i]->Add(hPromptPassFail[i-1]);
                hPromptFailPass[i]->Add(hPromptFailPass[i-1]);
                hPromptFailFail[i]->Add(hPromptFailFail[i-1]);
            }
            TPad *grid = new TPad("grid","",0,0,1,1);
            grid->Draw();
            grid->cd();
            grid->SetGrid();
            
            Double_t nEventsAllP = hPromptAll[i]->GetSumOfWeights();
            Double_t nEventsPassPassP = hPromptPassPass[i]->GetSumOfWeights();
            Double_t nEventsPassFailP = hPromptPassFail[i]->GetSumOfWeights();
            Double_t nEventsFailPassP = hPromptFailPass[i]->GetSumOfWeights();
            Double_t nEventsFailFailP = hPromptFailFail[i]->GetSumOfWeights();
            
            double widthAll = findWidth(hPromptAll[i]);
            double widthPassPass = findWidth(hPromptPassPass[i]);
            double widthPassFail = findWidth(hPromptPassFail[i]);
            double widthFailPass = findWidth(hPromptFailPass[i]);
            double widthFailFail = findWidth(hPromptFailFail[i]);
    //        Double_t nEventsAllP = hPromptAll->GetEntries();
    //        Double_t nEventsPassPassP = hPromptPassPass->GetEntries();
    //        Double_t nEventsPassFailP = hPromptPassFail->GetEntries();
    //        Double_t nEventsFailPassP = hPromptFailPass->GetEntries();
    //        Double_t nEventsFailFailP = hPromptFailFail->GetEntries();
            
            char buff0[200];
            snprintf(buff0, sizeof(buff0), "(%0.2f)(Width = %0.1f)", nEventsAllP, widthAll);
            string nAllP = buff0;
            string labelAllP;
            labelAllP = "All Prompt " + nAllP;
            legend->AddEntry(hPromptAll[i],labelAllP.c_str(),"pl");
            mStack->Add(hPromptAll[i]);
            
            char buff1[200];
            snprintf(buff1, sizeof(buff1), "(%0.2f)(Width = %0.1f)", nEventsPassPassP, widthPassPass);
            string nPassPassP = buff1;
            string labelPassPassP;
            labelPassPassP = "#splitline{Prompt Passing idMVA > " + xgbCutStr[i] +"}{ & Passing Presel "+ nPassPassP + "}";
            legend->AddEntry(hPromptPassPass[i],labelPassPassP.c_str(),"pl");
            mStack->Add(hPromptPassPass[i]);
            
            char buff2[200];
            snprintf(buff2, sizeof(buff2), "(%0.2f)(Width = %0.1f)", nEventsPassFailP, widthPassFail);
            string nPassFailP = buff2;
            string labelPassFailP;
            labelPassFailP = "#splitline{Prompt Failing idMVA > " + xgbCutStr[i] +"}{ & Passing Presel " + nPassFailP +  "}";
            legend->AddEntry(hPromptPassFail[i],labelPassFailP.c_str(),"pl");
            mStack->Add(hPromptPassFail[i]);
            
            char buff3[200];
            snprintf(buff3, sizeof(buff3), "(%0.2f)(Width = %0.1f)", nEventsFailPassP, widthFailPass);
            string nFailPassP = buff3;
            string labelFailPassP;
            labelFailPassP = "#splitline{Prompt Passing idMVA > " + xgbCutStr[i] +"}{ & Failing Presel " + nFailPassP +  "}";
            legend->AddEntry(hPromptFailPass[i],labelFailPassP.c_str(),"pl");
            mStack->Add(hPromptFailPass[i]);
           
            char buff4[200];
            snprintf(buff4, sizeof(buff4), "(%0.2f)(Width = %0.1f)", nEventsFailFailP, widthFailFail);
            string nFailFailP = buff4;
            string labelFailFailP;
            labelFailFailP = "#splitline{Prompt Failing idMVA > " + xgbCutStr[i] +"}{ & Failing Presel " + nFailFailP +  "}" ;
            legend->AddEntry(hPromptFailFail[i],labelFailFailP.c_str(),"pl");
            mStack->Add(hPromptFailFail[i]);
            
            hPromptAll[i]->Rebin(plotFactor);
            hPromptPassPass[i]->Rebin(plotFactor);
            hPromptPassFail[i]->Rebin(plotFactor);
            hPromptFailPass[i]->Rebin(plotFactor);
            hPromptFailFail[i]->Rebin(plotFactor);
            
            if (i == 0)hPromptFailPass[i]->Scale(11.0);
            if (i == 2)hPromptFailPass[i]->Scale(2.5);
            if (i == 3)hPromptFailPass[i]->Scale(20.0);

            mStack->Draw("nostackhist");
            legend->Draw("same");
        }
    }
    can->Print(fOut.c_str());
}
