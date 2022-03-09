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
void gghCompIDMVAScores(){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);
    
    int nBinsPlot = 402; // From 0 to 1 in xgbScore with one overflow and one underflow
    int binFactor = 50;// Increase binning by factor of X. Get binFactor extra bins above and below plot
    
    string fOutR = "outROC/GGHROC_MD20NTreeSearch_Barrel_0203.root";
    string fOutX = "idMVAPlots/GGHIDMVA_MD20NTreeSearch_Barrel_0203.root";
    //string fOutF = "idMVAPlots/GJetIDMVAF_LRSearch18_Endcap_0127.rooot";
        
    int nBins = nBinsPlot*binFactor;
    double binSize = 1.0/(nBinsPlot-2); // Plotting bin size
    double plotLow = 0.0 - binSize; //Min value to have 1 bin of size binsize below 0
    double plotHigh = 1.0 + binSize; // Same for max
    double binSizeFine = (plotHigh-plotLow)/nBins;
    
    string fileName[7];
    fileName[0] = "validationNTuples/0202/GGH/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root";
    fileName[1] = "validationNTuples/0202/GGH/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_Validation.root";
    fileName[2] = "validationNTuples/0202/GGH/MD20_LR045_NTree250_M95PTM15_GGH_UL2017_Validation.root";
    fileName[3] = "validationNTuples/0202/GGH/MD20_LR045_NTree200_M95PTM15_GGH_UL2017_Validation.root";
    fileName[4] = "validationNTuples/0202/GGH/MD20_LR045_NTree150_M95PTM15_GGH_UL2017_Validation.root";
    fileName[5] = "validationNTuples/0202/GGH/MD20_LR045_NTree100_M95PTM15_GGH_UL2017_Validation.root";
   
//
//    fileName[0] = "validationNTuples/0124/MD18_LR01_M95PTM15_OnlyPFPairs_UL2017_Validation_Endcap_0122.root";
//    fileName[1] = "validationNTuples/0124/MD18_LR02_M95PTM15_OnlyPFPairs_UL2017_Validation_Endcap_0122.root";
//    fileName[2] = "validationNTuples/0124/MD18_LR03_M95PTM15_OnlyPFPairs_UL2017_Validation_Endcap_0122.root";
//    fileName[3] = "validationNTuples/0124/MD18_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation_Endcap_0122.root";
//    fileName[4] = "validationNTuples/0124/MD18_LR05_M95PTM15_OnlyPFPairs_UL2017_Validation_Endcap_0122.root";
//    fileName[5] = "validationNTuples/0124/MD18_LR06_M95PTM15_OnlyPFPairs_UL2017_Validation_Endcap_0122.root";
//    fileName[6] = "validationNTuples/0124/MD18_LR07_M95PTM15_OnlyPFPairs_UL2017_Validation_Endcap_0122.root";
    
    string labels[7];
    labels[0] = "Ntrees = 359";
    labels[1] = "Ntrees = 300";
    labels[2] = "Ntrees = 250";
    labels[3] = "Ntrees = 200";
    labels[4] = "Ntrees = 150";
    labels[5] = "Ntrees = 100";

//    labels[6] = "MD 13 LR.07";


    
    int iLow = 0;
    int iHigh = 6;
    
    string titleXGB = "XGBoost Score for GGH Barrel Prompt and Fake;yPred";
    string titleRoc = "RoC Curves for GGH Barrel";
    
    TCanvas *canX = new TCanvas ("canX","canX",10,10,1600,900);
    canX->SetGrid();
    //TCanvas *canF = new TCanvas ("canF","canF",10,10,1600,900);
    //canF->SetGrid();
    TCanvas *canR = new TCanvas ("canR","canR",10,10,1600,900);
    canR->SetGrid();
    
    TLegend *legendX = new TLegend(0.35,0.6,0.65,0.9,"","brNDC");
   // TLegend *legendF = new TLegend(0.35,0.6,0.65,0.9,"","brNDC");
    TLegend *legendR = new TLegend(0.35,0.1,0.65,0.4,"","brNDC");
    
    TH1F *hPrompt[7];
    TH1F *hFake[7];
    TGraph *sigEff_vs_bkgEff[7];
    
    THStack *xStack = new THStack("xStack",titleXGB.c_str());
    
//    string titleFake = "XGBoost Score for GJet Endcap Fake;yPred";
//    THStack *fStack = new THStack("fStack",titleFake.c_str());
    
    for(int i = iLow; i < iHigh; i++){
        TFile *f = new TFile(fileName[i].c_str());
        TTree *tP = (TTree*)f->Get("promptPhotons");
        TTree *tF = (TTree*)f->Get("fakePhotons");
        
        double maxVal = 0.0;
        int maxInt = -1;
        
        //string overallTitleStr = "XGBoost Scores for GJet Validation in Barrel;idMVA";
        //THStack *hStack = new THStack("hStack",overallTitleStr.c_str());
        
        string titlePrompt = "hPrompt" + to_string(i);
        string titleFake = "hFake" + to_string(i);
        
        hPrompt[i] = new TH1F (titlePrompt.c_str(),"",nBins,plotLow,plotHigh);
        hFake[i] = new TH1F (titleFake.c_str(),"",nBins,plotLow,plotHigh);
        
        TTreeReader pReader("promptPhotons", f);
        TTreeReaderValue<Double_t> yPredValsP(pReader, "XGBScores.yPred");
        TTreeReaderValue<Double_t> weightP(pReader, "XGBScores.weight");

        TTreeReader fReader("fakePhotons", f);
        TTreeReaderValue<Double_t> yPredValsF(fReader, "XGBScores.yPred");
        TTreeReaderValue<Double_t> weightF(fReader, "XGBScores.weight");
        
        while (pReader.Next()) {
            //cout<<*yPredValsP<<endl;
            hPrompt[i]->Fill(*yPredValsP,*weightP*500);
        }
        while (fReader.Next()) {
            hFake[i]->Fill(*yPredValsF,*weightF*500);
        }
        
        hPrompt[i]->SetLineColor(i+1);
        hFake[i]->SetLineColor(i+1);
        hFake[i]->SetLineStyle(3);

     
        int nEventsP = hPrompt[i]->GetEntries();
        int nEventsF = hFake[i]->GetEntries();

     
        int nCuts = nBins;
        double stepVal = binSizeFine;
        double cutVal = 0.0 - stepVal;
    //
        float Nsig[nCuts+1], Nbkg[nCuts+1];
        float sigEff[nCuts+1], bkgEff[nCuts+1];
        float cutsVal[nCuts+1];
        float mvaResCutVal = 0.0 - stepVal;
        
        int mvaSMaxBin = hPrompt[i]->GetXaxis()->FindBin(1.0) + 1;
        int mvaBMaxBin = hFake[i]->GetXaxis()->FindBin(1.0) + 1;

    for(int k = 0; k < nCuts+1; k++){
        mvaResCutVal+= stepVal;
        cutsVal[k] = mvaResCutVal;

        int mvaBinSig = hPrompt[i]->GetXaxis()->FindBin(mvaResCutVal);
        Nsig[k] = hPrompt[i]->Integral(mvaBinSig,mvaSMaxBin);
        int mvaBinBkg = hFake[i]->GetXaxis()->FindBin(mvaResCutVal);
        Nbkg[k] = hFake[i]->Integral(mvaBinBkg,mvaBMaxBin);
        
        sigEff[k] = Nsig[k]/Nsig[0];
        bkgEff[k] = Nbkg[k]/Nbkg[0];
        }
        
        sigEff_vs_bkgEff[i] = new TGraph (nCuts, bkgEff, sigEff);
        sigEff_vs_bkgEff[i]->SetLineColor(i+1);
       
        //sigEff_vs_bkgEff->Draw();
        
        TF1 f1("f1",[&](double *bkgEff, double *sigEff){ return sigEff_vs_bkgEff[i]->Eval(bkgEff[0]); },0,1,0); //Gets values of TGraph
        //f1.Draw();
        double AUC = f1.Integral(0,1);
        stringstream stream;
        stream << fixed << setprecision(6) << AUC;
        string AUCStr = stream.str();
        cout<<"AUCStr = "<<AUCStr<<endl;
        
//        //ALTERNATIVE AUC CALCULATION WITH TRAPEZOID RULE
//        double aucTry = 0.0;
//        Double_t pointX[nBins],pointY[nBins];
//        for(int k = 1; k < nCuts+1; k++){
//            sigEff_vs_bkgEff[i]->GetPoint(k,pointX[k],pointY[k]);
//            double sumY,diffX;
//
//            sumY = pointY[k]+pointY[k-1];
//            diffX = pointX[k]-pointX[k-1];
//
//            //aucTry += (sumY/2.0)*diffX;
//            aucTry += (sumY/2.0)*stepVal;
//        }
//        cout<<"AUC Calculated from trapezoid rule = "<<aucTry<<endl;
//
        
        char buff1[100];
        snprintf(buff1, sizeof(buff1), "(%0.2d", nEventsP);
        string nPStr = buff1;
        string labelP;
        if (i == iLow) labelP = labels[i] + " GGH Prompt " + (nPStr + ")") + "(" + AUCStr.c_str() + ")";
        else labelP = labels[i] + " GGH Prompt " + "(" + AUCStr.c_str() + ")";
        legendX->AddEntry(hPrompt[i],labelP.c_str(),"pl");
        
        char buff2[100];
        snprintf(buff2, sizeof(buff2), "(%0.2d", nEventsF);
        string nFStr = buff2;
        string labelF;
        if (i == iLow) labelF = labels[i] + " Fake " + (nFStr + ")");
        else labelF = labels[i] + " Fake";
        //labelF = labels[i] + " Fake " + (nFStr+ ")") + "(" + AUCStr.c_str() + ")";
        legendX->AddEntry(hFake[i],labelF.c_str(),"pl");
        //legendF->AddEntry(hFake[i],labelF.c_str(),"pl");
        
        hPrompt[i]->Rebin(binFactor);
        xStack->Add(hPrompt[i]);
        hFake[i]->Rebin(binFactor);
        xStack->Add(hFake[i]);
//        fStack->Add(hFake[i]);
        
        canR->cd();
        string labelR = labels[i] + "(" + AUCStr.c_str() + ")";
        
        legendR->AddEntry(sigEff_vs_bkgEff[i],labelR.c_str());
        
        if (i == iLow){
            sigEff_vs_bkgEff[i]->SetTitle(titleRoc.c_str());
            sigEff_vs_bkgEff[i]->Draw();
        }
        else sigEff_vs_bkgEff[i]->Draw("same");
    }
    legendR->Draw();
    
    canX->cd();
    xStack->Draw("nostackhist");
    legendX->Draw("same");
    
//    canF->cd();
//    fStack->Draw("nostack");
//    legendF->Draw("same");
    
    canR->Print(fOutR.c_str());
    canX->Print(fOutX.c_str());
    //canF->Print(fOutF.c_str());
    
}
