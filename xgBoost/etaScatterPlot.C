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

void etaScatterPlot(){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);

    double xgbCut[5] = {0.0228358,0.0228358,0.0228358,0.0228358,0.0228358};
    string xgbCutStr[5] = {"0.0228358","0.0228358","0.0228358","0.0228358","0.0228358"};
    double xgbCut2[5] = {0.238955,0.238955,0.238955,0.238955,0.238955};
    string xgbCutStr2[5] = {"0.238955","0.238955","0.238955","0.238955","0.238955"};
    string eta[5] = {"Barrel-Barrel","Barrel-Endcap","Endcap-Barrel","Endcap-Endcap","All"};
    int nEta = 5;
    
    
    string fileNames[5] = {"validationNTuples/0306/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root",
        "validationNTuples/0306/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root",
        "validationNTuples/0306/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_Validation_Endcap.root",
        "validationNTuples/0306/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_Validation_Endcap.root","validationNTuples/0306/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root"};
    
    string gjfLabelUncut = "GJet Fake All";
    string gjfLabelPresel = "GJet Fake Presel";
    string gjfLabelIDMVA1 = "GJet Fake W/ xgbScore > 0.0228";
    string gjfLabelIDMVA2 = "GJet Fake W/ xgbScore > 0.2390";
    
    string gghLabelUncut = "GGH";
    string gghLabelPresel = "GGH Presel";
    string gghLabelIDMVA1 = "GGH W/ xgbScore > 0.0228";
    string gghLabelIDMVA2 = "GGH W/ xgbScore > 0.2390";
    
    string dirStr ="varPlots/0308/NegWeight_";
    
    
    TH2F *hPromptAll[5];
    TH2F *hPromptPresel[5];
    TH2F *hPromptIDMVA[5];
    TH2F *hPromptIDMVA2[5];
    
    TH2F *hFakeAll[5];
    TH2F *hFakePresel[5];
    TH2F *hFakeIDMVA[5];
    TH2F *hFakeIDMVA2[5];
    
    TTreeReader pReader[5];
    TTreeReader fReader[5];
    //
    for (int e = 0; e < nEta; e++){
        hPromptAll[e] = new TH2F ("hPromptAll","",300,-3.0,3.0,300,-3.0,3.0);
        hPromptPresel[e] = new TH2F ("hPromptPresel","",300,-3.0,3.0,300,-3.0,3.0);
        hPromptIDMVA[e] = new TH2F ("hPromptIDMVA","",300,-3.0,3.0,300,-3.0,3.0);
        hPromptIDMVA2[e] = new TH2F ("hPromptIDMVA2","",300,-3.0,3.0,300,-3.0,3.0);
        
        hFakeAll[e] = new TH2F ("hFakeAll","",300,-3.0,3.0,300,-3.0,3.0);
        hFakePresel[e] = new TH2F ("hFakePresel","",300,-3.0,3.0,300,-3.0,3.0);
        hFakeIDMVA[e] = new TH2F ("hFakeIDMVA","",300,-3.0,3.0,300,-3.0,3.0);
        hFakeIDMVA2[e] = new TH2F ("hFakeIDMVA2","",300,-3.0,3.0,300,-3.0,3.0);
    }
    for (int e = 0; e < nEta; e++){
    
        TFile *f = new TFile(fileNames[e].c_str());

        if (e != 4){
            TTreeReader pReader("promptPhotons", f);
            pReader.Restart();
            TTreeReaderValue<Float_t> weightP(pReader, "weight");
            TTreeReaderValue<Float_t> yPredValsP(pReader, "xgbScore");
            TTreeReaderArray<Float_t> varValsP(pReader,"varVals");
            
            TTreeReader fReader("fakePhotons", f);
            fReader.Restart();
            TTreeReaderValue<Float_t> weightF(fReader, "weight");
            TTreeReaderValue<Float_t> yPredValsF(fReader, "xgbScore");
            TTreeReaderArray<Float_t> varValsF(fReader,"varVals");
        
            while (pReader.Next()) {
//                if( !(!(varNames[i] =="hggMass") && (varValsP[17] > 128.5 || varValsP[17] < 120.0))){
                if(*weightP < 0.0 && ((e == 0 || e == 2) && abs(varValsP[20]) < 1.4442) || ((e == 1 || e == 3) && abs(varValsP[20]) > 1.556)){
                    hPromptAll[e]->Fill(varValsP[10],varValsP[20],*weightP);
                    if (varValsP[19] == 1.0)hPromptPresel[e]->Fill(varValsP[10],varValsP[20],*weightP);
                    if (*yPredValsP > xgbCut[e])hPromptIDMVA[e]->Fill(varValsP[10],varValsP[20],*weightP);
                    if (*yPredValsP > xgbCut2[e])hPromptIDMVA2[e]->Fill(varValsP[10],varValsP[20],*weightP);
                }
            }
            while (fReader.Next()) {
                if(((e == 0 || e == 2) && abs(varValsF[20]) < 1.4442) || ((e == 1 || e == 3) && abs(varValsF[20]) > 1.556)){
                    hFakeAll[e]->Fill(varValsF[10],varValsF[20],*weightF);
                    if (varValsF[19] == 1.0)hFakePresel[e]->Fill(varValsF[10],varValsF[20],*weightF);
                    if (*yPredValsF > xgbCut[e])hFakeIDMVA[e]->Fill(varValsF[10],varValsF[20],*weightF);
                    if (*yPredValsF > xgbCut2[e])hFakeIDMVA2[e]->Fill(varValsF[10],varValsF[20],*weightF);
                }
            }
        }
        if (e == 0 || e == 2 || e == 3 || e == 4){
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
        
            if (e == 4){
                for (int p = 0; p < 4; p++){
                    if (p != 1){
                        hPromptAll[e]->Add(hPromptAll[p]);
                        hPromptPresel[e]->Add(hPromptPresel[p]);
                        hPromptIDMVA[e]->Add(hPromptIDMVA[p]);
                        hPromptIDMVA2[e]->Add(hPromptIDMVA2[p]);
                        
                        //
                        hFakeAll[e]->Add(hFakeAll[p]);
                        hFakePresel[e]->Add(hFakePresel[p]);
                        hFakeIDMVA[e]->Add(hFakeIDMVA[p]);
                        hFakeIDMVA2[e]->Add(hFakeIDMVA2[p]);
                    }
                }
            }
            if (e == 4){
                TCanvas *can = new TCanvas ("can","can",10,10,1200,1200);
               
                can->cd();
                hPromptAll[e]->SetTitle("GGH without Cuts");
                hPromptAll[e]->GetXaxis()->SetTitle("#eta_{1}");
                hPromptAll[e]->GetYaxis()->SetTitle("#eta_{2}");
                hPromptAll[e]->Draw("colz");
                string outPAll = dirStr + "EtaScatter_PromptAll.root";
                can->Print(outPAll.c_str());
                can->Clear();
                
                can->cd();
                hPromptPresel[e]->SetTitle("GGH with Presel Cuts");
                hPromptPresel[e]->GetXaxis()->SetTitle("#eta_{1}");
                hPromptPresel[e]->GetYaxis()->SetTitle("#eta_{2}");
                hPromptPresel[e]->Draw("colz");
                string outPPre = dirStr + "EtaScatter_PromptPresel.root";
                can->Print(outPPre.c_str());
                can->Clear();
                
                can->cd();
                hPromptIDMVA[e]->SetTitle("GGH with idMVA > 0.023");
                hPromptIDMVA[e]->GetXaxis()->SetTitle("#eta_{1}");
                hPromptIDMVA[e]->GetYaxis()->SetTitle("#eta_{2}");
                hPromptIDMVA[e]->Draw("colz");
                string outPID = dirStr + "EtaScatter_PromptLoseIDMVA.root";
                can->Print(outPID.c_str());
                can->Clear();
                
                can->cd();
                hPromptIDMVA2[e]->SetTitle("GGH with idMVA > 0.23");
                hPromptIDMVA2[e]->GetXaxis()->SetTitle("#eta_{1}");
                hPromptIDMVA2[e]->GetYaxis()->SetTitle("#eta_{2}");
                hPromptIDMVA2[e]->Draw("colz");
                string outPID2 = dirStr + "EtaScatter_PromptTightIDMVA.root";
                can->Print(outPID2.c_str());
                can->Clear();
                
                can->cd();
                hFakeAll[e]->SetTitle("GJet Fake without Cuts");
                hFakeAll[e]->GetXaxis()->SetTitle("#eta_{1}");
                hFakeAll[e]->GetYaxis()->SetTitle("#eta_{2}");
                hFakeAll[e]->Draw("colz");
                string outFAll = dirStr + "EtaScatter_FakeAll.root";
                can->Print(outFAll.c_str());
                can->Clear();
                
                can->cd();
                hFakePresel[e]->SetTitle("GJet Fake wit Presel Cuts");
                hFakePresel[e]->GetXaxis()->SetTitle("#eta_{1}");
                hFakePresel[e]->GetYaxis()->SetTitle("#eta_{2}");
                hFakePresel[e]->Draw("colz");
                string outFPre = dirStr + "EtaScatter_FakePresel.root";
                can->Print(outFPre.c_str());
                can->Clear();
                
                can->cd();
                hFakeIDMVA[e]->SetTitle("GJet Fake with idMVA > 0.023");
                hFakeIDMVA[e]->GetXaxis()->SetTitle("#eta_{1}");
                hFakeIDMVA[e]->GetYaxis()->SetTitle("#eta_{2}");
                hFakeIDMVA[e]->Draw("colz");
                string outFID = dirStr + "EtaScatter_FakeLoseIDMVA.root";
                can->Print(outFID.c_str());
                can->Clear();
                
                can->cd();
                hFakeIDMVA2[e]->SetTitle("GJet Fake with idMVA > 0.23");
                hFakeIDMVA2[e]->GetXaxis()->SetTitle("#eta_{1}");
                hFakeIDMVA2[e]->GetYaxis()->SetTitle("#eta_{2}");
                hFakeIDMVA2[e]->Draw("colz");
                string outFID2 = dirStr + "EtaScatter_FakeTightIDMVA.root";
                can->Print(outFID2.c_str());
                can->Clear();
            }
        }
    }
}
