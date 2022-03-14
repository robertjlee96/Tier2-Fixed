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
void findXGBCutAllEta(){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);
    
    int nBinsPlot = 402; // From 0 to 1 in xgbScore with one overflow and one underflow
    int binFactor = 100;// Increase binning by factor of X. Get binFactor extra bins above and below plot
    
    //double propPresel = 0.5*0.163225;
    //double propPresel = 0.0624773;
    
    int nBins = nBinsPlot*binFactor;
    double binSize = 1.0/(nBinsPlot-2); // Plotting bin size
    double plotLow = 0.0 - binSize; //Min value to have 1 bin of size binsize below 0
    double plotHigh = 1.0 + binSize; // Same for max
    double binSizeFine = (plotHigh-plotLow)/nBins;
    string fileNameB = "validationNTuples/0306/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation.root";
    string fileNameE= "validationNTuples/0306/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_Validation_Endcap.root";
    
    TH1F *hPrompt = new TH1F ("hPrompt","",nBins,plotLow,plotHigh);
    TH1F *hPromptPresel = new TH1F ("hPrompPresel","",nBins,plotLow,plotHigh);
    TH1F *hFake = new TH1F ("hFake","",nBins,plotLow,plotHigh);
    TH1F *hFakePresel = new TH1F ("hFakePresel","",nBins,plotLow,plotHigh);
    
    TFile *fB = new TFile(fileNameB.c_str());
    TFile *fE = new TFile(fileNameE.c_str());

    
    double maxVal = 0.0;
    int maxInt = -1;
        
    TTreeReader pReaderB("promptPhotons", fB);
    TTreeReaderValue<Float_t> yPredValsPB(pReaderB, "xgbScore");
    TTreeReaderValue<Float_t> weightPB(pReaderB, "weight");
    TTreeReaderArray<Float_t> varValsPB(pReaderB, "varVals");

    TTreeReader fReaderB("fakePhotons", fB);
    TTreeReaderValue<Float_t> yPredValsFB(fReaderB, "xgbScore");
    TTreeReaderValue<Float_t> weightFB(fReaderB, "weight");
    TTreeReaderArray<Float_t> varValsFB(fReaderB, "varVals");
        
    while (pReaderB.Next()) {
        if(varValsPB[19] == 1)hPromptPresel->Fill(*yPredValsPB,*weightPB);
        hPrompt->Fill(*yPredValsPB,*weightPB);
    }
    while (fReaderB.Next()) {
        if(varValsFB[19] == 1)hFakePresel->Fill(*yPredValsFB,*weightFB);
        hFake->Fill(*yPredValsFB,*weightFB);
    }
    
    TTreeReader pReaderE("promptPhotons", fE);
    TTreeReaderValue<Float_t> yPredValsPE(pReaderE, "xgbScore");
    TTreeReaderValue<Float_t> weightPE(pReaderE, "weight");
    TTreeReaderArray<Float_t> varValsPE(pReaderE, "varVals");
    
    TTreeReader fReaderE("fakePhotons", fE);
    TTreeReaderValue<Float_t> yPredValsFE(fReaderE, "xgbScore");
    TTreeReaderValue<Float_t> weightFE(fReaderE, "weight");
    TTreeReaderArray<Float_t> varValsFE(fReaderE, "varVals");
    
    while (pReaderE.Next()) {
        if(varValsPE[19] == 1)hPromptPresel->Fill(*yPredValsPE,*weightPE);
        hPrompt->Fill(*yPredValsPE,*weightPE);
    }
    while (fReaderE.Next()) {
        if(varValsFE[19] == 1)hFakePresel->Fill(*yPredValsFE,*weightFE);
        hFake->Fill(*yPredValsFE,*weightFE);
    }

    int nCuts = nBins;
    double cutStep = 2.0/nCuts;
    double cutVal = -1.0 - cutStep;
    
    double propIDMVACut;
    double equivalentIDMVACut;
    
    int mvaMaxBin = hPrompt->GetXaxis()->FindBin(1) + 1;
    
    double totalIntBkg = hFake->Integral();
    double totalIntSig = hPrompt->Integral();
    
    double propPresel = hPromptPresel->Integral()/totalIntSig;
    //double propPresel = hFakePresel->Integral()/totalIntBkg;
    
    int mvaBinXGBCut = hFake->GetXaxis()->FindBin(0.0899005);
    double propXGBCut = (hFake->Integral(mvaBinXGBCut,mvaMaxBin)/totalIntBkg);
//    cout<<"For XGBScore > 0.0899005, Bkg Eff = "<<propXGBCut<<endl;
    
    for(int i = 0; i < nCuts; i++){
        cutVal+= cutStep;
        
        int mvaBin = hFake->GetXaxis()->FindBin(cutVal);
        //double tmpIDMVAInt = hFake->Integral(mvaBin,mvaMaxBin);
        double tmpIDMVAInt = hPrompt->Integral(mvaBin,mvaMaxBin);
        
        //propIDMVACut = tmpIDMVAInt/totalIntBkg;
        propIDMVACut = tmpIDMVAInt/totalIntSig;
        if(propIDMVACut <= propPresel)break;
    }
    
    int sigBin = hPrompt->GetXaxis()->FindBin(cutVal);
    double sigEff = hPrompt->Integral(sigBin,nBins)/totalIntSig;
    
    cout<<"PropPresel = "<<propPresel<<endl;
    cout<<"PropIDMVA = "<<propIDMVACut<<endl;
    cout<<"cutVal = "<<cutVal<<endl;
    cout<<"siggEff = "<<sigEff<<endl;
}
