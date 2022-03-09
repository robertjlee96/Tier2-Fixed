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

void allEtaVarCompSignal(){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);

    string fileNameGGH = "../NTuples/GGH_And_GJet_M95PTM25_HovrE_DPT075_1118.root";
    TFile *fGGH = new TFile(fileNameGGH.c_str());
    TTree *tGGH = (TTree*)fGGH->Get("ggh_125");
    string gghLabelUncut = "GGH No Presel";
    string gghLabelCut = "GGH W/ Presel";

    string fileNameVBF = "../NTuples/VBF_And_GJet_M95PTM25_HovrE_DPT075_1118.root";
    TFile *fVBF = new TFile(fileNameVBF.c_str());
    TTree *tVBF = (TTree*)fVBF->Get("vbf_125");
    string vbfLabelUncut = "100*VBF No Presel";
    string vbfLabelCut = "100*VBF W/ Presel";

    string fileNameTTH = "../NTuples/TTH_And_GJet_M95PTM25_HovrE_DPT075_1118.root";
    TFile *fTTH = new TFile(fileNameTTH.c_str());
    TTree *tTTH = (TTree*)fTTH->Get("tth_125");
    string tthLabelUncut = "500*TTH No Presel";
    string tthLabelCut = "500*TTH W/ Presel";


    string fileNameBBH = "../NTuples/DiHiggs_And_GJet_M95PTM25_HovrE_DPT075_1129.root";
    TFile *fBBH = new TFile(fileNameBBH.c_str());
    TTree *tBBH = (TTree*)fBBH->Get("dihiggs_125");
    string bbhLabelUncut = "500*HHTo2B2G No Presel";
    string bbhLabelCut = "500*HHTo2B2G W/ Presel";
    
    int nTrees = 4;
    TTree *allTrees[4] = {tGGH,tVBF,tTTH,tBBH};
    string allLabelsUncut[4] = {gghLabelUncut,vbfLabelUncut,tthLabelUncut,bbhLabelUncut};
    string allLabelsCut[4] = {gghLabelCut,vbfLabelCut,tthLabelCut,bbhLabelCut};
    
    string outNameGen = "plots/1201/SignalVariableDistributions_1201";

    string massCutSig = " && hggMass > 121 && hggMass < 129";
    string massCutLoose = " && hggMass > 95";
    
    string kinCutLeadLoose = " && leadPt > 17 && leadPt/hggMass > 0.15";
    string kinCutSubLoose = " && subPt > 17 && subPt/hggMass > 0.15";
    
    string kinCutLeadTight = " && leadPt > 17 && leadPt/hggMass > 0.33";
    string kinCutSubTight = " && subPt > 17 && subPt/hggMass > 0.25";
    
    string preselLead = " && leadR9 > 0.5 && leadHadronicOverEm < 0.08 && !(abs(leadScEta) < 1.5 && leadR9 < 0.85 && (leadSigmaIetaIeta > 0.015 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0)) && !(abs(leadScEta) > 1.5 && leadR9 < 0.9 && (leadSigmaIetaIeta > 0.035 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0))";
    string preselSub =  " && subR9 > 0.5 && subHadronicOverEm < 0.08 && !(abs(subScEta) < 1.5 && subR9 < 0.85 && (subSigmaIetaIeta > 0.015 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0)) && !(abs(subScEta) > 1.5 && subR9 < 0.9 && (subSigmaIetaIeta > 0.035 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0))";
    
    string cutStringLeadLooseGen = "(leadGenMatchType == 1" + massCutSig + kinCutLeadLoose + kinCutSubLoose + ")*weight";
    string cutStringSubLooseGen = "(subGenMatchType == 1" + massCutSig + kinCutSubLoose + kinCutLeadLoose + ")*weight";

    string cutStringLeadTightGen = "(leadGenMatchType == 1" + massCutSig + kinCutLeadTight + preselLead + kinCutSubTight + preselSub + ")*weight";
    string cutStringSubTightGen = "(subGenMatchType == 1" + massCutSig + kinCutSubTight + preselSub + kinCutLeadTight + preselLead + ")*weight";
    
    string cutStringLeadLooseMass = "(leadGenMatchType == 1" + massCutLoose + kinCutLeadLoose + kinCutSubLoose + ")*weight";
    string cutStringSubLooseMass = "(subGenMatchType == 1" + massCutLoose + kinCutSubLoose + kinCutLeadLoose + ")*weight";
    
    string cutStringLeadTightMass = "(leadGenMatchType == 1" + massCutLoose + kinCutLeadTight + preselLead + kinCutSubTight + preselSub + ")*weight";
    string cutStringSubTightMass = "(subGenMatchType == 1" + massCutLoose + kinCutSubTight + preselSub + kinCutLeadTight + preselLead + ")*weight";
    
    int nVars = 5;
    string varNames[5] = {"Pt","Pt/hggMass","hggMass","SigmaIetaIeta","IDMVA"};
    double limsLow[5] = {0.0,0.0,80.0,0.0,-1.1};
    double limsHigh[5] = {200,1.3,180,0.1,1.1};
    int nBins[5] = {400,520,400,400,440};
    
    int nEventsLoose, nEventsTight;
    
    TCanvas *can = new TCanvas ("can","can",10,10,1500,1000);
    
    for(int i = 0; i < nVars; i++){
        TLegend *legend = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
        
        double maxVal = 0.0;
        int maxInt = -1;
        
        string plotTitleStr = varNames[i] + " All Eta Diphoton Cuts";
        string overallTitleStr = plotTitleStr + ";" + varNames[i];
        THStack *hStack = new THStack("hStack",overallTitleStr.c_str());
        
        string leadVarStr = "lead" + varNames[i];
        string subVarStr = "sub" + varNames[i];
        
        string cutStringLeadLoose,cutStringSubLoose,cutStringLeadTight,cutStringSubTight;
        
        if(varNames[i] == "hggMass"){
            leadVarStr = varNames[i];
            subVarStr = varNames[i];
            cutStringLeadLoose = cutStringLeadLooseMass;
            cutStringSubLoose = cutStringSubLooseMass;
            cutStringLeadTight = cutStringLeadTightMass;
            cutStringSubTight = cutStringSubTightMass;
            
        }
        else{
            cutStringLeadLoose = cutStringLeadLooseGen;
            cutStringSubLoose = cutStringSubLooseGen;
            cutStringLeadTight = cutStringLeadTightGen;
            cutStringSubTight = cutStringSubTightGen;
            
        }
        
        TH1F *hLoose[4];
        TH1F *hLooseSub[4];
        TH1F *hTight[4];
        TH1F *hTightSub[4];
        string hNamesLoose[4] = {"hLoose1","hLoose2","hLoose3","hLoose4"};
        string hNamesLooseSub[4] = {"hLooseSub1","hLooseSub2","hLooseSub3","hLooseSub4"};
        string hNamesTight[4] = {"hTight1","hTight2","hTight3","hTight4"};
        string hNamesTightSub[4] = {"hTightSub1","hTightSub2","hTightSub3","hTightSub4"};
        for(int j = 0; j < nTrees; j++){
            string cutStringLeadLooseInside = cutStringLeadLoose;
            string cutStringLeadTightInside = cutStringLeadTight;
            
            string cutStringSubLooseInside = cutStringSubLoose;
            string cutStringSubTightInside = cutStringSubTight;
            
            
            if(j == 1){
                cutStringLeadLooseInside += "*100";
                cutStringLeadTightInside += "*100";
                
                cutStringSubLooseInside += "*100";
                cutStringSubTightInside += "*100";
            }
            if(j == 2){
                cutStringLeadLooseInside += "*500";
                cutStringLeadTightInside += "*500";

                cutStringSubLooseInside += "*500";
                cutStringSubTightInside += "*500";
            }
            if(j == 3){
                cutStringLeadLooseInside += "*500";
                cutStringLeadTightInside += "*500";
                
                cutStringSubLooseInside += "*500";
                cutStringSubTightInside += "*500";
            }
//
            hLoose[j] = new TH1F (hNamesLoose[j].c_str(),"",nBins[i],limsLow[i],limsHigh[i]);
            hLooseSub[j] = new TH1F (hNamesLooseSub[j].c_str(),"",nBins[i],limsLow[i],limsHigh[i]);
            allTrees[j]->Project(hNamesLoose[j].c_str(),leadVarStr.c_str(),cutStringLeadLooseInside.c_str());
            allTrees[j]->Project(hNamesLooseSub[j].c_str(),subVarStr.c_str(),cutStringSubLooseInside.c_str());
            hLoose[j]->Add(hLooseSub[j]);
            
            hTight[j] = new TH1F (hNamesTight[j].c_str(),"",nBins[i],limsLow[i],limsHigh[i]);
            hTightSub[j] = new TH1F (hNamesTightSub[j].c_str(),"",nBins[i],limsLow[i],limsHigh[i]);
            allTrees[j]->Project(hNamesTight[j].c_str(),leadVarStr.c_str(),cutStringLeadTightInside.c_str());
            allTrees[j]->Project(hNamesTightSub[j].c_str(),subVarStr.c_str(),cutStringSubTightInside.c_str());
            hTight[j]->Add(hTightSub[j]);
            
            string labelUncut = allLabelsUncut[j];
            string labelCut = allLabelsCut[j];
            
            char buff1[100], buff2[100], buff3[100];
            
            snprintf(buff1, sizeof(buff1), "(%0.0f)", hLoose[j]->GetEntries());
            string nEventsLoose = buff1;
            labelUncut += nEventsLoose;
            legend->AddEntry(hLoose[j],labelUncut.c_str(),"pl");
            
            snprintf(buff2, sizeof(buff2), "(%0.0f)", hTight[j]->GetEntries());
            string nEventsTight = buff2;
            labelCut += nEventsTight;
            
            snprintf(buff3, sizeof(buff3), "(%0.2f", 100*(hTight[j]->GetSumOfWeights()/hLoose[j]->GetSumOfWeights()));
            string propTight = buff3;
            labelCut += (propTight + "%)");
            legend->AddEntry(hTight[j],labelCut.c_str(),"pl");
            
//            double maxValLoose = hLoose[j]->GetMaximum();
//
//            if(maxValLoose > maxVal){
//                maxInt = j;
//                maxVal = maxValLoose;
//            }
//
//            if(j == (nTrees - 1)) hLoose[j]->SetMaximum(maxVal);
            
            hLoose[j]->SetLineColor(j+1);
            hTight[j]->SetLineColor(j+1);
            hTight[j]->SetLineStyle(9);
            
            hStack->Add(hLoose[j]);
            hStack->Add(hTight[j]);
        }
        hStack->Draw("nostackhist");
        legend->Draw("same");
        can->SetGrid();
        string outName;
        if (varNames[i] == "Pt/hggMass") outName = outNameGen + "_PtOvrM";
        else outName = outNameGen + "_" + varNames[i];
        can->SaveAs((outName+".png").c_str());
        can->SaveAs((outName+".root").c_str());
        
        if (i != nVars-1)can->Clear();
        
        for(int j = 0; j < nTrees; j++){
            hLoose[j]->Delete();
            hLooseSub[j]->Delete();
            hTight[j]->Delete();
            hTightSub[j]->Delete();

        }

    }
}


