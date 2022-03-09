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

void findSameCutsEtaSplit(bool etaSplit, bool diphotonCuts){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);
    
    int nBins = 20000000;

    string fileNameIDMVA1 = "../NTuples/GGH_And_GJets_M95PTM25_Old0916_0113.root";
    TFile *f1 = new TFile(fileNameIDMVA1.c_str());
    TTree *tGJet1 = (TTree*)f1->Get("GJets");
    
<<<<<<< HEAD
    string fileNameIDMVA2 = "../NTuples/GGH_And_GJets_M95PTM15_MD20LR045_DPT075_0202.root";
=======
    string fileNameIDMVA2 = "../NTuples/GGH_And_GJets_M95PTM25_DPT075_HovrE_OnlyPFPairs_1223.root";
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
    TFile *f2 = new TFile(fileNameIDMVA2.c_str());
    TTree *tGJet2 = (TTree*)f2->Get("GJets");
    
    int nEta;
    string etaCutsLead[2];
    string etaCutsSub[2];
    string etaLabels[2];
    string etaFLabels[2];
    double propPresel[2];
    
    if (etaSplit == true){
        nEta = 2;
        
        etaLabels[0] = "Barrel";
        etaFLabels[0] = "_B";
        etaCutsLead[0] = " && abs(leadScEta) < 1.4442 ";
        etaCutsSub[0] = " && abs(subScEta) < 1.4442 ";
        propPresel[0] = 0.0961;
        
        etaLabels[1] = "Endcap";
        etaFLabels[1] = "_E";
        etaCutsLead[1] = " && abs(leadScEta) > 1.566 ";
        etaCutsSub[1] = " && abs(subScEta) > 1.566";
        propPresel[1] = 0.145468;
    }
    
    if (etaSplit == false){
        nEta = 1;
        
        etaLabels[0] = "All Eta";
        etaFLabels[0] = "";
        etaCutsLead[0] = "";
        etaCutsSub[0] = "";
    }
   
    string outNameGen = "plots/0117/idMVACutLocation/";
    if(diphotonCuts == true)outNameGen += "_DiphotonCuts";
    if(diphotonCuts == false)outNameGen += "_SinglePhotonCuts";
    
    string massCut = " && hggMass > 95";
    
    string originalIDMVACutLead = " && leadIDMVA > -0.99";
    string originalIDMVACutSub = " && subIDMVA > -0.99";
    
    string kinCutLeadLoose = " && leadPt > 17 && leadPt/hggMass > 0.15";
    string kinCutSubLoose = " && subPt > 17 && subPt/hggMass > 0.15";
    
    for (int e = 0; e < nEta; e++){
        
        string cutStringLeadUncut,cutStringSubUncut,cutStringLeadIDMVA1,cutStringSubIDMVA1,cutStringLeadIDMVA2,cutStringSubIDMVA2;
        
        if(diphotonCuts == true){
            cutStringLeadIDMVA1 = "(leadGenMatchType != 1" + etaCutsLead[e] + massCut + originalIDMVACutLead + kinCutLeadLoose + originalIDMVACutSub + kinCutSubLoose + ")*weight";
            cutStringSubIDMVA1 = "(subGenMatchType != 1" + etaCutsSub[e] + massCut + originalIDMVACutSub + kinCutSubLoose + originalIDMVACutLead + kinCutLeadLoose + ")*weight";
            cutStringLeadUncut = "(leadGenMatchType != 1" + etaCutsLead[e] + massCut + kinCutLeadLoose + kinCutSubLoose + ")*weight";
            cutStringSubUncut = "(subGenMatchType != 1" + etaCutsSub[e] + massCut + kinCutSubLoose + kinCutLeadLoose + ")*weight";
            cutStringLeadIDMVA2 = "(leadGenMatchType != 1" + etaCutsLead[e] + massCut + kinCutLeadLoose + kinCutSubLoose + ")*weight";
            cutStringSubIDMVA2 = "(subGenMatchType != 1" + etaCutsSub[e] + massCut + kinCutSubLoose + kinCutLeadLoose + ")*weight";
        }
        if(diphotonCuts == false){
            cutStringLeadIDMVA1 = "(leadGenMatchType != 1" + etaCutsLead[e] + massCut + kinCutLeadLoose + originalIDMVACutLead  + ")*weight";
            cutStringSubIDMVA1 = "(subGenMatchType != 1" + etaCutsSub[e] + massCut + kinCutSubLoose + originalIDMVACutSub + ")*weight";
            cutStringLeadUncut = "(leadGenMatchType != 1" + etaCutsLead[e] + massCut + kinCutLeadLoose + ")*weight";
            cutStringSubUncut = "(subGenMatchType != 1" + etaCutsSub[e] + massCut + kinCutSubLoose + ")*weight";
            cutStringLeadIDMVA2 = "(leadGenMatchType != 1" + etaCutsLead[e] + massCut + kinCutLeadLoose + ")*weight";
            cutStringSubIDMVA2 = "(subGenMatchType != 1" + etaCutsSub[e] + massCut + kinCutSubLoose + ")*weight";
        }

        TCanvas *can = new TCanvas ("can","can",10,10,1600,900);
        
        TLegend *legend = new TLegend(0.35,0.6,0.65,0.9,"","brNDC");
        
        double maxVal = 0.0;
        int maxInt = -1;
        
        string plotTitleStr;
        if(diphotonCuts == true)plotTitleStr = "Diphoton Mass Diphoton Cuts ";
        if(diphotonCuts == false)plotTitleStr = "Diphoton Mass Single Photon Cuts ";
        string overallTitleStr = plotTitleStr + etaLabels[e]+ ";idMVA";
        THStack *hStack = new THStack("hStack",overallTitleStr.c_str());
        
       
        TH1F *hUncut = new TH1F ("hUncut","",nBins,-1.0,1.0);
        TH1F *hUncutSub = new TH1F ("hUncutSub","",nBins,-1.0,1.0);
        tGJet1->Project("hUncut","leadIDMVA",cutStringLeadUncut.c_str());
        tGJet1->Project("hUncutSub","subIDMVA",cutStringSubUncut.c_str());
        hUncut->Add(hUncutSub);
            
        int nEventsUncut = hUncut->GetEntries();
        double totalInt = hUncut->Integral();
        
        char buff1[100];
        snprintf(buff1, sizeof(buff1), "(%0.2d", nEventsUncut);
        string nUncutStr = buff1;
        string labelUncut = "GJet Fake Uncut" + (nUncutStr + ")");
        legend->AddEntry(hUncut,labelUncut.c_str(),"pl");
        
        TH1F *hIDMVA1 = new TH1F ("hIDMVA1","",nBins,-1.0,1.0);
        TH1F *hIDMVA1Sub = new TH1F ("hIDMVA1Sub","",nBins,-1.0,1.0);
        tGJet1->Project("hIDMVA1","leadIDMVA",cutStringLeadIDMVA1.c_str());
        tGJet1->Project("hIDMVA1Sub","subIDMVA",cutStringSubIDMVA1.c_str());
        hIDMVA1->Add(hIDMVA1Sub);
        
        TH1F *hIDMVA2 = new TH1F ("hIDMVA2","",nBins,-1.0,1.0);
        TH1F *hIDMVA2Sub = new TH1F ("hIDMVA2Sub","",nBins,-1.0,1.0);
        tGJet2->Project("hIDMVA2","leadIDMVA",cutStringLeadIDMVA2.c_str());
        tGJet2->Project("hIDMVA2Sub","subIDMVA",cutStringSubIDMVA2.c_str());
        hIDMVA2->Add(hIDMVA2Sub);
                
        int nEventsIDMVA1 = hIDMVA1->GetEntries();
        double propIDMVA1 = hIDMVA1->Integral()/totalInt;
        
        char buff2[100],buff3[100];
        snprintf(buff2, sizeof(buff2), "(%0.2d", nEventsIDMVA1);
        snprintf(buff3, sizeof(buff3), "(%0.2f", 100*propIDMVA1);
        string nIDMVA1Str = buff2;
        string propIDMVA1Str = buff3;
        string labelCut = "GJet Prompt IDMVA1 > -0.99" + (nIDMVA1Str + ")") + (propIDMVA1Str + "%)");
        legend->AddEntry(hIDMVA1,labelCut.c_str(),"pl");

        int nCuts = nBins;
        double cutStep = 2.0/nCuts;
        double cutVal = -1.0 - cutStep;
        
        double propIDMVACut;
        double equivalentIDMVACut;
        
        int mvaMaxBin = hIDMVA2->GetXaxis()->FindBin(1) + 1;
        
        for(int i = 0; i < nCuts; i++){
            cutVal+= cutStep;
            
            int mvaBin = hIDMVA2->GetXaxis()->FindBin(cutVal);
            double tmpIDMVAInt = hIDMVA2->Integral(mvaBin,mvaMaxBin);
            
            propIDMVACut = tmpIDMVAInt/totalInt;
<<<<<<< HEAD
            //if(propIDMVACut <= propIDMVA1)break;
            if(propIDMVACut <= propPresel[e])break;
=======
            if(propIDMVACut <= propIDMVA1)break;
            //if(propIDMVACut <= 0.75)break;
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
        }
        cout<<"Eta Label = "<<etaLabels[e]<<endl;
        cout<<"PropIDMVA1 < -0.99 = "<<propIDMVA1<<endl;
        cout<<"PropIDMVA2 = "<<propIDMVACut<<endl;
        cout<<"cutVal For IDMVA2 = "<<cutVal<<endl;
//
//
//        TH1F *hIDMVA = new TH1F (hNamesIDMVA.c_str(),"",nBins,80,180);
//        TH1F *hIDMVASub = new TH1F (hNamesIDMVASub.c_str(),"",nBins,80,180);
//        allTrees->Project(hNamesIDMVA.c_str(),"leadIDMVA",cutStringLeadIDMVA.c_str());
//        allTrees->Project(hNamesIDMVASub.c_str(),"subIDMVA",cutStringSubIDMVA.c_str());
//        hIDMVA->Add(hIDMVASub);
//
//
//        hIDMVA->SetLineColor(j+1);
//        hPresel->SetLineColor(j+1);
//        hPresel->SetLineStyle(9);
//
//        hStack->Add(hIDMVA);
//        hStack->Add(hPresel);
//
//        hStack->Draw("nostackhist");
//        legend->Draw("same");
//        can->SetGrid();
//        if (varNames[i] == "IDMVA") can->SetLogy(1);
//        else can->SetLogy(0);
//
//        string outName;
//        if (varNames[i] == "Pt/hggMass") outName = outNameGen + "_PtOvrM" + etaFLabels[e];
//        else outName = outNameGen + "_" + varNames[i] + etaFLabels[e];
//        can->SaveAs((outName+".png").c_str());
//        can->SaveAs((outName+".root").c_str());
//
//
        hIDMVA1->Delete();
        hIDMVA1Sub->Delete();
        hIDMVA2->Delete();
        hIDMVA2Sub->Delete();
        hUncut->Delete();
        hUncutSub->Delete();
    }
}
