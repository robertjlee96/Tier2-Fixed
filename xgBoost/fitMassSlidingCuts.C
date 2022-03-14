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

void fitMassSlidingCuts(){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);
    
    double xgbCut[5] = {0.0228358,0.0228358,0.0228358,0.0228358,0.0228358};
    string xgbCutStr[5] = {"0.0228358","0.0228358","0.0228358","0.0228358","0.0228358"};
        double xgbCut2[5] = {0.238955,0.238955,0.238955,0.238955,0.238955};
//    double xgbCut2[5] = {0.976468,0.976468,0.976468,0.976468,0.976468};
    string xgbCutStr2[5] = {"0.238955","0.238955","0.238955","0.238955","0.238955"};
//    string xgbCutStr2[5] = {"0.976468","0.976468","0.976468","0.976468","0.976468"};
    string eta[5] = {"Barrel-Barrel","Barrel-Endcap","Endcap-Barrel","Endcap-Endcap","All"};
    
    string fileNames[5] = {"validationNTuples/0306/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root",
        "validationNTuples/0306/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root",
        "validationNTuples/0306/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_Validation_Endcap.root",
        "validationNTuples/0306/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_Validation_Endcap.root","validationNTuples/0306/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root"};
    
    string gjfLabelUncut = "GJet Fake All";
    string gjfLabelPresel = "GJet Fake Presel";
    string gjfLabelIDMVA1 = "GJet Fake W/ xgbScore > 0.0228";
    string gjfLabelIDMVA2 = "GJet Fake W/ xgbScore > 0.238955";
    
    string labels[4] = {gjfLabelUncut,gjfLabelPresel,gjfLabelIDMVA1,gjfLabelIDMVA2};
    
    string outFileStr ="varPlots/0311/FitGJetFakeMass_PreselAndTwoCuts.root";
    
    TCanvas *c1 = new TCanvas ("c1","c1",10,10,1600,900);
    
    TH1F *hFakeAll[5];
    TH1F *hFakePresel[5];
    TH1F *hFakeIDMVA[5];
    TH1F *hFakeIDMVA2[5];
    
    TTreeReader pReader[5];
    TTreeReader fReader[5];
    
    for (int e = 0; e < 5; e++){
        hFakeAll[e] = new TH1F ("hFakeAll","",450,90,180);
        hFakePresel[e] = new TH1F ("hFakePresel","",450,90,180);
        hFakeIDMVA[e] = new TH1F ("hFakeIDMVA","",450,90,180);
        hFakeIDMVA2[e] = new TH1F ("hFakeIDMVA2","",450,90,180);
    }
    for (int e = 0; e < 5; e++){
        
        TFile *f = new TFile(fileNames[e].c_str());
        
        if (e != 4){
            
            TTreeReader fReader("fakePhotons", f);
            fReader.Restart();
            TTreeReaderValue<Float_t> weightF(fReader, "weight");
            TTreeReaderValue<Float_t> yPredValsF(fReader, "xgbScore");
            TTreeReaderValue<Float_t> ptOvrMF(fReader, "ptOvrM");
            TTreeReaderArray<Float_t> varValsF(fReader,"varVals");
            
                
            while (fReader.Next()) {
                if(((e == 0 || e == 2) && abs(varValsF[20]) < 1.4442) || ((e == 1 || e == 3) && abs(varValsF[20]) > 1.556)){
                    hFakeAll[e]->Fill(varValsF[17],*weightF);
                    if (varValsF[19] == 1.0)hFakePresel[e]->Fill(varValsF[17],*weightF);
                    if (*yPredValsF > xgbCut[e])hFakeIDMVA[e]->Fill(varValsF[17],*weightF);
                    if (*yPredValsF > xgbCut2[e])hFakeIDMVA2[e]->Fill(varValsF[17],*weightF);
                }
            }
        }
        if (e == 4){
            for (int p = 0; p < 4; p++){
                if (p != 1){
                    hFakeAll[e]->Add(hFakeAll[p]);
                    hFakePresel[e]->Add(hFakePresel[p]);
                    hFakeIDMVA[e]->Add(hFakeIDMVA[p]);
                    hFakeIDMVA2[e]->Add(hFakeIDMVA2[p]);
                }
            }
        }
    }
    
    
    TH1F *h[4] = {hFakeAll[4],hFakePresel[4],hFakeIDMVA[4],hFakeIDMVA2[4]};
    TF1  *tFit[4];
    TF1  *tFitImproved[4];
    string fitNames[4] = {"fit1","fit2","fit3","fit4"};
    string fitNamesImproved[4] = {"fit1Imp","fit2Imp","fit3Imp","fit4Imp"};
  
    THStack *hStack = new THStack("hStack","GJet Fake Mass;hggMass");
    TLegend *legend = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");

    for (int j = 0; j < 4; j++){

        char buff1[100], buff2[100];

        snprintf(buff1, sizeof(buff1), "(%0.0f)", h[j]->GetEntries());
        string nEvents = buff1;
        labels[j] += nEvents;
        if (j != 0){
            snprintf(buff2, sizeof(buff2), "(%0.2f", 100*(h[j]->GetSumOfWeights()/h[0]->GetSumOfWeights()));
            string prop = buff2;
            labels[j] += (prop + "%)");
        }

        legend->AddEntry(h[j],labels[j].c_str(),"pl");

        tFit[j] = new TF1(fitNames[j].c_str(),"expo",95,180);
        //tFit[j] = new TF1(fitNames[j].c_str(),"[0] + [1]*pow(x,[2])",95,180);
        tFitImproved[j] = new TF1(fitNamesImproved[j].c_str(),"[0]*pow(x,[1])",95,180);
        h[j]->Fit(fitNames[j].c_str(),"R0","",95,180);
        
        double scalePrelim = tFit[j]->GetParameter(0);
        double slopePrelim = tFit[j]->GetParameter(1);
        
        tFitImproved[j]->SetParameter(0,scalePrelim);
        tFitImproved[j]->SetParameter(1,slopePrelim);

        if (j == 0){
            //h[j]->SetTitle("GJet Fake Mass");
            //h[j]->GetXaxis()->SetTitle("hggMass");
            h[j]->Fit(fitNamesImproved[j].c_str(),"","hist",95,180);
            //h[j]->Draw();

        }
        else{
            h[j]->Fit(fitNamesImproved[j].c_str(),"+","histsame",95,180);
            //h[j]->Draw("Same");
        }
        
        if (j == 0){
            h[j]->SetLineColor(4);
            tFitImproved[j]->SetLineColor(4);
        }
        if (j == 1){
            h[j]->SetLineColor(3);
            tFitImproved[j]->SetLineColor(3);
        }
        if (j == 2){
            h[j]->SetLineColor(2);
            tFitImproved[j]->SetLineColor(2);
        }
        
        hStack->Add(h[j]);
        //tFit[j]->Draw("same");
        //cList[i]->Update();
    }
    hStack->Draw("nostack");
    legend->Draw("same");
    c1->SetLogy();
    c1->Print(outFileStr.c_str());
}
            
//            h[j] = new TH1F (hNames[j].c_str(),"",48,95,180);
//            hSub[j] = new TH1F (hNamesSub[j].c_str(),"",48,95,180);
//
//            string cutStringLead = cutLeadGen + idMVACutsLead[j] + ")*weight";
//            string cutStringSub = cutSubGen + idMVACutsSub[j] + ")*weight";
//
//            tList[i]->Project(hNames[j].c_str(),"hggMass",cutStringLead.c_str());
//            tList[i]->Project(hNamesSub[j].c_str(),"hggMass",cutStringSub.c_str());
//            h[j]->Add(hSub[j]);
//
//            string label;
//            if(j == 0)label = gjfLabelUncut;
//            else label = gjfLabelUncut + legList[i] + idMVACutTitles[j];
//
//            char buff1[100], buff2[100];
//
//            snprintf(buff1, sizeof(buff1), "(%0.0f)", h[j]->GetEntries());
//            string nEvents = buff1;
//            label += nEvents;
//            if (j != 0){
//                snprintf(buff2, sizeof(buff2), "(%0.2f", 100*(h[j]->GetSumOfWeights()/h[0]->GetSumOfWeights()));
//                string prop = buff2;
//                label += (prop + "%)");
//            }
//
//            legend->AddEntry(h[j],label.c_str(),"pl");
//
//            tFit[j] = new TF1(fitNames[j].c_str(),"expo",95,180);
//            h[j]->Fit(fitNames[j].c_str(),"R+","",95,180);
//
//            if (j == 0){
//                //h[j]->SetTitle("GJet Fake Mass");
//                //h[j]->GetXaxis()->SetTitle("hggMass");
//                h[j]->Fit(fitNames[j].c_str(),"","hist",95,180);
//                //h[j]->Draw();
//
//            }
//            else{
//                h[j]->Fit(fitNames[j].c_str(),"+","histsame",95,180);
//                //h[j]->Draw("Same");
//            }
//            h[j]->SetLineColor(j+1);
//            hStack->Add(h[j]);
//            //tFit[j]->Draw("same");
//            //cList[i]->Update();
//        }
//        hStack->Draw("nostack");
//        legend->Draw("same");
//        cList[i]->Print(outList[i].c_str());
//    }


