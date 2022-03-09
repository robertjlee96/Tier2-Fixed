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

void varCompOnlyGGH(bool diphotonCuts){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);

    string fileName1 = "../NTuples/GGH_And_GJet_M95PTM25_HovrE_DPT075_1118.root";
    TFile *f1 = new TFile(fileName1.c_str());
    TTree *tGGH1 = (TTree*)f1->Get("ggh_125");
    TTree *tGJet1 = (TTree*)f1->Get("GJets");
    string gghLabel1 = "GGH No Presel, Old Model";
    string gjetLabelP1 = "GJet Prompt No Presel, Old Model";
    string gjetLabelF1 = "GJet Fake No Presel, Old Model";
    
    string fileName2 = "../NTuples/GGH_And_GJets_M95PTM25_DPT075_HovrE_OnlyPFPairs_1223.root";
    TFile *f2 = new TFile(fileName2.c_str());
    TTree *tGGH2 = (TTree*)f2->Get("ggh_125");
    TTree *tGJet2 = (TTree*)f2->Get("GJets");
    string gghLabel2 = "GGH No Presel, New Model";
    string gjetLabelP2 = "GJet Prompt No Presel, New Model";
    string gjetLabelF2 = "GJet Fake No Presel, New Model";
    
    string dirStr = "plots/1223GGHComp/";
    
    
    int nTrees = 6;
    TTree *allTrees[6] = {tGGH1,tGJet1,tGJet1,tGGH2,tGJet2,tGJet2};
    string allLabels[6] = {gghLabel1,gjetLabelP1,gjetLabelF1,gghLabel2,gjetLabelP2,gjetLabelF2};
    
    string outNameGen = dirStr + "Signal_GGHAndGJet";
    if(diphotonCuts == true) outNameGen += "_DiphotonCuts";
    if(diphotonCuts == false) outNameGen += "_SinglePhotonCuts";
    
    string massCutSig = " && hggMass > 121 && hggMass < 129";
    string massCutBkg = " && hggMass > 95";
    
    string kinCutLeadLoose = " && leadPt > 17 && leadPt/hggMass > 0.15";
    string kinCutSubLoose = " && subPt > 17 && subPt/hggMass > 0.15";
    
    string kinCutLeadTight = " && leadPt > 17 && leadPt/hggMass > 0.33";
    string kinCutSubTight = " && subPt > 17 && subPt/hggMass > 0.25";
    
    string preselLead = " && leadR9 > 0.5 && leadHadronicOverEm < 0.08 && !(abs(leadScEta) < 1.5 && leadR9 < 0.85 && (leadSigmaIetaIeta > 0.015 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0)) && !(abs(leadScEta) > 1.5 && leadR9 < 0.9 && (leadSigmaIetaIeta > 0.035 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0))";
    string preselSub =  " && subR9 > 0.5 && subHadronicOverEm < 0.08 && !(abs(subScEta) < 1.5 && subR9 < 0.85 && (subSigmaIetaIeta > 0.015 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0)) && !(abs(subScEta) > 1.5 && subR9 < 0.9 && (subSigmaIetaIeta > 0.035 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0))";
        
    string cutStringLeadGGHGen,cutStringSubGGHGen,cutStringLeadGJetGen,cutStringSubGJetGen;

    if(diphotonCuts == true){
        cutStringLeadGGHGen = "(leadGenMatchType == 1 " + massCutSig + kinCutLeadLoose + kinCutSubLoose + ")*weight";
        cutStringSubGGHGen =  "(subGenMatchType == 1 " + massCutSig + kinCutSubLoose + kinCutLeadLoose + ")*weight";
                                                      
        cutStringLeadGJetGen = massCutBkg + kinCutLeadLoose + kinCutSubLoose + ")*weight";
        cutStringSubGJetGen =  massCutBkg + kinCutSubLoose + kinCutLeadLoose + ")*weight";
        
    }
    if(diphotonCuts == false){
        cutStringLeadGGHGen = "(leadGenMatchType == 1 " + massCutSig + kinCutLeadLoose + ")*weight";
        cutStringSubGGHGen =  "(subGenMatchType == 1 " + massCutSig + kinCutSubLoose + ")*weight";
        
        cutStringLeadGJetGen = massCutBkg + kinCutLeadLoose + ")*weight";
        cutStringSubGJetGen =  massCutBkg + kinCutSubLoose + ")*weight";
    }
    
  
    
    int nEventsLoose, nEventsTight;
    
    TCanvas *can = new TCanvas ("can","can",10,10,1600,900);


    TLegend *legend;
    legend = new TLegend(0.35,0.6,0.65,0.9,"","brNDC");

    double maxVal = 0.0;
    int maxInt = -1;
    
    string plotTitleStr = "IDMVA Scores";
    if(diphotonCuts == true)plotTitleStr += " Diphoton Cuts ";
    if(diphotonCuts == false)plotTitleStr += " Single Photon Cuts ";
    string overallTitleStr = plotTitleStr + ";IDMVA";
    THStack *hStack = new THStack("hStack",overallTitleStr.c_str());
    
    string leadVarStr = "leadIDMVA";
    string subVarStr = "subIDMVA";
    
    TH1F *hLead[6];
    TH1F *hSub[6];
    string hNamesLead[6] = {"hLead1","hLead2","hLead3","hLead4","hLead5","hLead6"};
    string hNamesSub[6] = {"hSub1","hSub2","hSub3","hSub4","hSub5","hSub6"};
    for(int j = 0; j < nTrees; j++){
        string cutStringLeadInside;
        string cutStringSubInside;

        if(j == 0 || j == 3){
            cutStringLeadInside = cutStringLeadGGHGen;
            cutStringSubInside = cutStringSubGGHGen;
        }
        if(j == 1 || j == 4){
            cutStringLeadInside = "(leadGenMatchType == 1 " + cutStringLeadGJetGen;
            cutStringSubInside = "(subGenMatchType == 1 " + cutStringSubGJetGen;
        }
        if(j == 2 || j == 5){
            cutStringLeadInside = "(leadGenMatchType != 1 " + cutStringLeadGJetGen;
            cutStringSubInside = "(subGenMatchType != 1 " + cutStringSubGJetGen;
        }
//
        hLead[j] = new TH1F (hNamesLead[j].c_str(),"",440,-1.1,1.1);
        hSub[j] = new TH1F (hNamesSub[j].c_str(),"",440,-1.1,1.1);
        allTrees[j]->Project(hNamesLead[j].c_str(),leadVarStr.c_str(),cutStringLeadInside.c_str());
        allTrees[j]->Project(hNamesSub[j].c_str(),subVarStr.c_str(),cutStringSubInside.c_str());
        hLead[j]->Add(hSub[j]);

        string label = allLabels[j];
        
        char buff1[100];
        
        snprintf(buff1, sizeof(buff1), "(%0.0f)", hLead[j]->GetEntries());
        string nEvents = buff1;
        label += nEvents;
        legend->AddEntry(hLead[j],label.c_str(),"pl");

        if(j == 0 || j == 3){
            hLead[j]->SetLineColor(1);
            if (j == 3)hLead[j]->SetLineStyle(9);
        }
        if(j == 1 || j == 4){
            hLead[j]->SetLineColor(2);
            if (j == 4)hLead[j]->SetLineStyle(9);
        }
        if(j == 2 || j == 5){
            hLead[j]->SetLineColor(3);
            if (j == 5)hLead[j]->SetLineStyle(9);
        }
        //
        
        hStack->Add(hLead[j]);
    }
    hStack->Draw("nostackhist");
    legend->Draw("same");
    can->SetGrid();
    can->SetLogy(1);
    
    string outName = outNameGen + "_IDMVA";
    can->SaveAs((outName+".png").c_str());
    can->SaveAs((outName+".root").c_str());
            
    for(int j = 0; j < nTrees; j++){
        hLead[j]->Delete();
        hSub[j]->Delete();
    }
}


