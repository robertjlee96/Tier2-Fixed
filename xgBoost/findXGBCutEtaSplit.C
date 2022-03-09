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
void findXGBCutEtaSplit(){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);
    
    int nBinsPlot = 402; // From 0 to 1 in xgbScore with one overflow and one underflow
    int binFactor = 100;// Increase binning by factor of X. Get binFactor extra bins above and below plot
    
    double propPresel = 0.5*0.19221;
    //double propPresel = 0.0624773;
    
    int nBins = nBinsPlot*binFactor;
    double binSize = 1.0/(nBinsPlot-2); // Plotting bin size
    double plotLow = 0.0 - binSize; //Min value to have 1 bin of size binsize below 0
    double plotHigh = 1.0 + binSize; // Same for max
    double binSizeFine = (plotHigh-plotLow)/nBins;
    string fileName = "validationNTuples/0217/GGH/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_Validation_Endcap.root";
    
    TH1F *hPrompt = new TH1F ("hPrompt","",nBins,plotLow,plotHigh);
    TH1F *hFake = new TH1F ("hFake","",nBins,plotLow,plotHigh);
    
    
    TFile *f = new TFile(fileName.c_str());
    TTree *tP = (TTree*)f->Get("promptPhotons");
    TTree *tF = (TTree*)f->Get("fakePhotons");
        
    double maxVal = 0.0;
    int maxInt = -1;
        
    TTreeReader pReader("promptPhotons", f);
    TTreeReaderValue<Double_t> yPredValsP(pReader, "XGBScores.yPred");
    TTreeReaderValue<Double_t> weightP(pReader, "XGBScores.weight");

    TTreeReader fReader("fakePhotons", f);
    TTreeReaderValue<Double_t> yPredValsF(fReader, "XGBScores.yPred");
    TTreeReaderValue<Double_t> weightF(fReader, "XGBScores.weight");
        
    while (pReader.Next()) {
            //cout<<*yPredValsP<<endl;
        hPrompt->Fill(*yPredValsP,*weightP);
    }
    while (fReader.Next()) {
        hFake->Fill(*yPredValsF,*weightF);
    }
        
    int nCuts = nBins;
    double cutStep = 2.0/nCuts;
    double cutVal = -1.0 - cutStep;
    
    double propIDMVACut;
    double equivalentIDMVACut;
    
    int mvaMaxBin = hPrompt->GetXaxis()->FindBin(1) + 1;
    
    double totalIntBkg = hFake->Integral();
    double totalIntSig = hPrompt->Integral();
    
    int mvaBinXGBCut = hFake->GetXaxis()->FindBin(0.0899005);
    double propXGBCut = (hFake->Integral(mvaBinXGBCut,mvaMaxBin)/totalIntBkg);
    cout<<"For XGBScore > 0.0899005, Bkg Eff = "<<propXGBCut<<endl;
    
    for(int i = 0; i < nCuts; i++){
        cutVal+= cutStep;
        
        int mvaBin = hFake->GetXaxis()->FindBin(cutVal);
        double tmpIDMVAInt = hFake->Integral(mvaBin,mvaMaxBin);
        
        propIDMVACut = tmpIDMVAInt/totalIntBkg;
        if(propIDMVACut <= propPresel)break;
    }
    
    int sigBin = hPrompt->GetXaxis()->FindBin(cutVal);
    double sigEff = hPrompt->Integral(sigBin,nBins)/totalIntSig;
    
    cout<<"PropPresel = "<<propPresel<<endl;
    cout<<"PropIDMVA = "<<propIDMVACut<<endl;
    cout<<"cutVal = "<<cutVal<<endl;
    cout<<"siggEff = "<<sigEff<<endl;
}
