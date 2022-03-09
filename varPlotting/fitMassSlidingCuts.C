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

    string fileName1 = "../NTuples/GGH_And_GJets_M95PTM25_DPT075_HovrE_OnlyPFPairs_1223.root";
    TFile *f1 = new TFile(fileName1.c_str());
    TTree *tGJet1 = (TTree*)f1->Get("GJets");
    //TTree *tGJet1 = (TTree*)f1->Get("diphotons");
    
    string fileName2 = "../NTuples/GGH_And_GJets_M95PTM25_DPT075_HovrE_OnlyPFPairs_NoPtReweight_0110.root";
    TFile *f2 = new TFile(fileName2.c_str());
    TTree *tGJet2 = (TTree*)f2->Get("GJets");
    
    //string gjpLabelUncut = "GJet Prompt";
    //string gjpLabelIDMVA1 = "GJet Prompt W/ Old idMVA";
    //string gjpLabelIDMVA2 = "GJet Prompt W/ New idMVA";
    
    string gjfLabelUncut = "GJet Fake";
    string gjfLabelIDMVA1 = "W/ Old idMVA";
    string gjfLabelIDMVA2 = "W/ New idMVA";
    
    string outStr ="plots/0114/MassFitComp_GJet_";
    string out1 = "WithPtReweight.root";
    string out2 = "NoPtReweight.root";
    
    string idMVACutsLead[7] = {"","&& leadIDMVA > -0.99","&& leadIDMVA > -0.9","&& leadIDMVA > -0.5","&& leadIDMVA > 0.0","&& leadIDMVA > 0.5","&& leadIDMVA > 0.9"};
    string idMVACutsSub[7] = {"","&& subIDMVA > -0.99","&& subIDMVA > -0.9","&& subIDMVA > -0.5","&& subIDMVA > 0.0","&& subIDMVA > 0.5","&& subIDMVA > 0.9"};
    
    string idMVACutTitles[7] = {"", " > -0.99"," > -0.9", " > -0.5", " > 0.0", "> 0.5", "> 0.9"};
    
    string massCutLoose = " && hggMass > 95 ";
    
    string kinCutLeadLoose = " && leadPt > 17 && leadPt/hggMass > 0.15";
    string kinCutSubLoose = " && subPt > 17 && subPt/hggMass > 0.15";
    
    string cutLeadGen = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + kinCutLeadLoose;
    string cutSubGen = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + kinCutSubLoose;
    
    //string cutLeadGen = "(leadGenMatchType == 1" + massCutLoose + kinCutLeadLoose;
    //string cutSubGen = "(subGenMatchType == 1" + massCutLoose + kinCutSubLoose;
    
    TCanvas *c1 = new TCanvas ("c1","c1",10,10,1600,900);
    TCanvas *c2 = new TCanvas ("c2","c2",10,10,1600,900);
        
    TTree *tList[2] = {tGJet1,tGJet2};
    TCanvas *cList[2] = {c1,c2};
    //string legListP[2] = {gjpLabelIDMVA1,gjpLabelIDMVA2};
    string legList[2] = {gjfLabelIDMVA1,gjfLabelIDMVA2};
    string outList[2] = {outStr + out1, outStr + out2};
    
    for(int i = 0; i < 2; i++)
    {
        TH1F *h[7];
        TH1F *hSub[7];
        string hNames[7] = {"hPUncut","hIDMVACut1P","hIDMVACut2P","hIDMVACut3P","hIDMVACut4P","hIDMVACut5P","hIDMVACut6P"};
        string hNamesSub[7] = {"hPUncutSub","hIDMVACut1PSub","hIDMVACut2PSub","hIDMVACut3PSub","hIDMVACut4PSub","hIDMVACut5PSub","hIDMVACut6PSub"};
        
        TF1  *tFit[7];
        string fitNames[7] = {"fit1","fit2","fit3","fit4","fit5","fit6","fit7"};

        cList[i]->cd();
        //cList[i]->SetLogy();
  
        THStack *hStack = new THStack("hStack","GJet Fake Mass;hggMass");
        TLegend *legend = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
        for (int j = 0; j < 7; j++)
        {
            h[j] = new TH1F (hNames[j].c_str(),"",48,95,180);
            hSub[j] = new TH1F (hNamesSub[j].c_str(),"",48,95,180);
            
            string cutStringLead = cutLeadGen + idMVACutsLead[j] + ")*weight";
            string cutStringSub = cutSubGen + idMVACutsSub[j] + ")*weight";
            
            tList[i]->Project(hNames[j].c_str(),"hggMass",cutStringLead.c_str());
            tList[i]->Project(hNamesSub[j].c_str(),"hggMass",cutStringSub.c_str());
            h[j]->Add(hSub[j]);
            
            string label;
            if(j == 0)label = gjfLabelUncut;
            else label = gjfLabelUncut + legList[i] + idMVACutTitles[j];
                
            char buff1[100], buff2[100];
            
            snprintf(buff1, sizeof(buff1), "(%0.0f)", h[j]->GetEntries());
            string nEvents = buff1;
            label += nEvents;
            if (j != 0){
                snprintf(buff2, sizeof(buff2), "(%0.2f", 100*(h[j]->GetSumOfWeights()/h[0]->GetSumOfWeights()));
                string prop = buff2;
                label += (prop + "%)");
            }
            
            legend->AddEntry(h[j],label.c_str(),"pl");
            
            tFit[j] = new TF1(fitNames[j].c_str(),"expo",95,180);
            h[j]->Fit(fitNames[j].c_str(),"R+","",95,180);

            if (j == 0){
                //h[j]->SetTitle("GJet Fake Mass");
                //h[j]->GetXaxis()->SetTitle("hggMass");
                h[j]->Fit(fitNames[j].c_str(),"","hist",95,180);
                //h[j]->Draw();
                
            }
            else{
                h[j]->Fit(fitNames[j].c_str(),"+","histsame",95,180);
                //h[j]->Draw("Same");
            }
            h[j]->SetLineColor(j+1);
            hStack->Add(h[j]);
            //tFit[j]->Draw("same");
            //cList[i]->Update();
        }
        hStack->Draw("nostack");
        legend->Draw("same");
        cList[i]->Print(outList[i].c_str());
    }
}


