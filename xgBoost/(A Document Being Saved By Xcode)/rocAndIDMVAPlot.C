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
void rocAndIDMVAPlot(){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);
    
    int nBinsPlot = 502; // From 0 to 1 in xgbScore with one overflow and one underflow
    int binFactor = 50;// Increase binning by factor of X. Get binFactor extra bins above and below plot
    
    string fOutR[4];
    fOutR[0] = "outROC/GGHROC_MD20NTreeSearch_Barrel_0211.root";
    fOutR[1] = "x";
    fOutR[2] = "outROC/GGHROC_MD20NTreeSearch_BandE_0211.root";
    fOutR[3] = "outROC/GGHROC_MD20NTreeSearch_Endcap_0211.root";
    
    string fOutX[4];
    fOutX[0] = "outROC/GGHROC_MD20NTreeSearch_Barrel_0211.root";
    fOutX[1] = "x";
    fOutX[2] = "outROC/GGHROC_MD20NTreeSearch_BandE_0211.root";
    fOutX[3] = "outROC/GGHROC_MD20NTreeSearch_Endcap_0211.root";
    
    string eta[4] = {"Barrel-Barrel","Barrel-Endcap","Endcap-Barrel","Endcap-Endcap"};
    
    //string fOutF = "idMVAPlots/GJetIDMVAF_LRSearch18_Endcap_0127.rooot";
        
    int nBins = nBinsPlot*binFactor;
    double binSize = 1.0/(nBinsPlot-2); // Plotting bin size
    double plotLow = 0.0 - binSize; //Min value to have 1 bin of size binsize below 0
    double plotHigh = 1.0 + binSize; // Same for max
    double binSizeFine = (plotHigh-plotLow)/nBins;
    
    string fileName[4];
    fileName[0] = "validationNTuples/0207/GGH/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root";
    fileName[1] = "validationNTuples/0207/GGH/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root";
    fileName[2] = "validationNTuples/0207/GGH/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_Validation_Endcap.root";
    fileName[3] = "validationNTuples/0207/GGH/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_Validation_Endcap.root";
   
//    string labels[7];
//    labels[0] = "Ntrees = 359";
//    labels[1] = "Ntrees = 300";
//    labels[2] = "Ntrees = 250";
//    labels[3] = "Ntrees = 200";
//    labels[4] = "Ntrees = 150";
//    labels[5] = "Ntrees = 100";

//    labels[6] = "MD 13 LR.07";


    
    int iLow = 0;
    int iHigh = 4;
    
    TH1F *hPrompt[7];
    TH1F *hFake[7];
    TGraph *sigEff_vs_bkgEff[7];

    for(int i = iLow; i < iHigh; i++){
        TFile *f = new TFile(fileName[i].c_str());
        TTree *tP = (TTree*)f->Get("promptPhotons");
        TTree *tF = (TTree*)f->Get("fakePhotons");
        
        double maxVal = 0.0;
        int maxInt = -1;
        
        string titleXGB = "XGBoost Score for GGH Sig and GJet Bkg " + eta[i] + ";yPred";
        string titleRoc = "RoC Curves for GGH Sig and GJet Bkg " + eta[i];
        
        string canXTitle = "canX" + to_string(i);
        string canRTitle = "canR" + to_string(i);
        
        TCanvas *canX = new TCanvas (canXTitle.c_str(),canXTitle.c_str(),10,10,1600,900);
        canX->SetGrid();
        //TCanvas *canF = new TCanvas ("canF","canF",10,10,1600,900);
        //canF->SetGrid();
        TCanvas *canR = new TCanvas (canRTitle.c_str(),canRTitle.c_str(),10,10,1600,900);
        canR->SetGrid();
        
        TLegend *legendX = new TLegend(0.35,0.6,0.65,0.9,"","brNDC");
        // TLegend *legendF = new TLegend(0.35,0.6,0.65,0.9,"","brNDC");
        TLegend *legendR = new TLegend(0.35,0.1,0.65,0.4,"","brNDC");
        
        THStack *xStack = new THStack("xStack",titleXGB.c_str());
        
        //string overallTitleStr = "XGBoost Scores for GJet Validation in Barrel;idMVA";
        //THStack *hStack = new THStack("hStack",overallTitleStr.c_str());
        
        string titlePrompt = "hPrompt" + to_string(i);
        string titleFake = "hFake" + to_string(i);
        
        hPrompt[i] = new TH1F (titlePrompt.c_str(),"",nBins,plotLow,plotHigh);
        hFake[i] = new TH1F (titleFake.c_str(),"",nBins,plotLow,plotHigh);
        
        TTreeReader pReader("promptPhotons", f);
        TTreeReaderValue<Double_t> yPredValsP(pReader, "XGBScores.yPred");
        TTreeReaderValue<Double_t> weightP(pReader, "XGBScores.weight");
        TTreeReaderValue<Double_t> secondScEtaP(pReader, "XGBScores.secondScEta");

        TTreeReader fReader("fakePhotons", f);
        TTreeReaderValue<Double_t> yPredValsF(fReader, "XGBScores.yPred");
        TTreeReaderValue<Double_t> weightF(fReader, "XGBScores.weight");
        TTreeReaderValue<Double_t> secondScEtaF(fReader, "XGBScores.secondScEta");

        while (pReader.Next()) {
            //cout<<*yPredValsP<<endl;
            if(((i == 0 || i == 2) && abs(*secondScEtaP) < 1.4442) || ((i == 1 || i == 3) && abs(*secondScEtaP) > 1.556))hPrompt[i]->Fill(*yPredValsP,*weightP);
        }
        while (fReader.Next()) {
            if(((i == 0 || i == 2) && abs(*secondScEtaF) < 1.4442) || ((i == 1 || i == 3) && abs(*secondScEtaF) > 1.556))hFake[i]->Fill(*yPredValsF,*weightF);
        }
        
        hPrompt[i]->SetLineColor(3);
        hFake[i]->SetLineColor(2);
        //hFake[i]->SetLineStyle(3);

        if (i == 0 || i == 2 || i == 3){
            if (i == 2){
                hPrompt[i]->Add(hPrompt[i-1]);
                hFake[i]->Add(hFake[i-1]);
            }
     
            double nEventsP = hPrompt[i]->GetSumOfWeights();
            double nEventsF = hFake[i]->GetSumOfWeights();
         
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
            //sigEff_vs_bkgEff[i]->SetLineColor(i+1);
           
            //sigEff_vs_bkgEff->Draw();
            
            TF1 f1("f1",[&](double *bkgEff, double *sigEff){ return sigEff_vs_bkgEff[i]->Eval(bkgEff[0]); },0,1,0); //Gets values of TGraph
            //f1.Draw();
            double AUC = f1.Integral(0,1);
            stringstream stream;
            stream << fixed << setprecision(6) << AUC;
            string AUCStr = stream.str();
            cout<<"AUCStr = "<<AUCStr<<endl;
            
            char buff1[100];
            snprintf(buff1, sizeof(buff1), "(%0.2f)", nEventsP);
            string nPStr = buff1;
            string labelP;
            labelP = "1000*GGH Prompt " + nPStr;
            legendX->AddEntry(hPrompt[i],labelP.c_str(),"pl");
            
            char buff2[100];
            snprintf(buff2, sizeof(buff2), "(%0.2f)", nEventsF);
            string nFStr = buff2;
            string labelF;
            labelF = "GJet Fake " + nFStr;
            //labelF = labels[i] + " Fake " + (nFStr+ ")") + "(" + AUCStr.c_str() + ")";
            legendX->AddEntry(hFake[i],labelF.c_str(),"pl");
            //legendF->AddEntry(hFake[i],labelF.c_str(),"pl");
            
            hPrompt[i]->Rebin(binFactor);
            hPrompt[i]->Scale(1000);
            xStack->Add(hPrompt[i]);
            hFake[i]->Rebin(binFactor);
            xStack->Add(hFake[i]);
    //        fStack->Add(hFake[i]);
            
            canR->cd();
            string labelR = AUCStr.c_str();
            
            legendR->AddEntry(sigEff_vs_bkgEff[i],labelR.c_str());
            
        
            sigEff_vs_bkgEff[i]->SetTitle(titleRoc.c_str());
            sigEff_vs_bkgEff[i]->Draw();
    
            legendR->Draw();
            
            canX->cd();
            xStack->Draw("nostackhist");
            legendX->Draw("same");
            
            //    canF->cd();
            //    fStack->Draw("nostack");
            //    legendF->Draw("same");
            
            canR->Print(fOutR[i].c_str());
            canX->Print(fOutX[i].c_str());
            //canF->Print(fOutF.c_str());
        }
    }
}
