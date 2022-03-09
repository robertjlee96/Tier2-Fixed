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

void trainingVarComp(bool genMatchSplit,bool includeIDMVA){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);

    string fileNameGJet = "../NTuples/SinglePhoton_GJet_NoCuts_1227_WithTest25FixSplitIDMVA_Validation.root";
    TFile *fGJet = new TFile(fileNameGJet.c_str());
    TTree *tGJetP = (TTree*)fGJet->Get("promptPhotons");
    TTree *tGJetF = (TTree*)fGJet->Get("fakePhotons");
    string gjpLabelUncut = "GJet Prompt Minimal Cuts";
    string gjfLabelUncut = "GJet Fake Minimal Cuts";
    string gjpLabelCut = "GJet Prompt Training Cuts";
    string gjfLabelCut = "GJet Fake Training Cuts";
    string gjpLabelIDMVA = "GJet Prompt W/ idMVA > -0.99";
    string gjfLabelIDMVA = "GJet Fake W/ idMVA > -0.99";
    
    //bool etaSplit == false;
    int nEta;
    string etaCuts[3];
    string etaLabels[3];
    string etaFLabels[3];
    
    if (genMatchSplit == true){
        nEta = 3;
        
        etaLabels[0] = "Prompt-Prompt";
        etaFLabels[0] = "_PP";
        etaCuts[0] = " && leadGenMatchType == 1 && subGenMatchType == 1";
       
        etaLabels[1] = "Fake-Fake";
        etaFLabels[1] = "_FF";
        etaCuts[1] = " && leadGenMatchType != 1 && subGenMatchType != 1";
        
        etaLabels[2] = "Prompt-Fake";
        etaFLabels[2] = "_PF";
        etaCuts[2] = " && ((leadGenMatchType == 1 && subGenMatchType != 1) || (leadGenMatchType != 1 && subGenMatchType == 1))";
    }
    
    if (genMatchSplit == false){
        nEta = 1;
        
        etaLabels[0] = "All Eta";
        etaFLabels[0] = "";
        etaCuts[0] = "";
    }

    
    string dirStr ="plots/1229/Training/";


    int nTrees = 2;
    TTree *allTrees[2] = {tGJetP,tGJetF};
    string allLabelsUncut[2] = {gjpLabelUncut,gjfLabelUncut};
    string allLabelsCut[2] = {gjpLabelCut,gjfLabelCut};
    string allLabelsIDMVA[2] = {gjpLabelIDMVA,gjfLabelIDMVA};
    
    if(genMatchSplit == true) dirStr += "GenMatchSplit/";
    string outNameGen = dirStr + "ValidationSample";
    outNameGen += "_SinglePhotonCuts";
        
    string kinCutLoose = "hggMass > 95 && pt > 17 && (pt/hggMass) > 0.15";
    string trainingCutsP = "hggMass > 95 && pt > 17 && (pt/hggMass) > 0.25 && abs(pt-genPt)/(pt) < 0.075";
    string trainingCutsF = "hggMass > 95 && pt > 17 && (pt/hggMass) > 0.25";
    string idMVACuts = kinCutLoose + " && idMVA > -0.99";
    for (int e = 0; e < nEta; e++){
        
        string cutStringLoose,cutStringTrainingP,cutStringTrainingF,cutStringIDMVA;
        cutStringLoose = "(" + kinCutLoose + etaCuts[e] + ")*weight";
        cutStringTrainingP ="(" + trainingCutsP + etaCuts[e] +  ")*weight";
        cutStringTrainingF ="(" + trainingCutsF + etaCuts[e] +  ")*weight";
        cutStringIDMVA = "(" + idMVACuts + etaCuts[e] + ")*weight";
        
        int nVars = 20;
        string varNames[20] = {"pt","pt/hggMass","abs(pt-genPt)/pt","hggMass","sigmaIetaIeta","SCRawE","r9","etaWidth","phiWidth","covIEtaIPhi","s4","phoIso03","chgIsoWrtChosenVtx","chgIsoWrtWorstVtx","scEta","rho","hadTowOverEm","hadronicOverEm","esEffSigmaRR","esEnergyOverRawE"};
        double limsLow[20] = {0.0,0.0,0.0,80.0,0.0,0.0,0.0,0.0,0.0,-0.002,0.0,0.0,0.0,0.0,-5.0,0.0,0.0,0.0,0.0,0.0};
        double limsHigh[20] = {200,1.3,1.0,180,0.1,650,1.1,0.2,0.2,0.002,1.0,25.0,25.0,25.0,5.0,60.0,0.2,0.4,8.0,0.2};
        int nBins[20] = {400,520,450,400,400,650,440,400,400,400,400,500,500,500,400,600,400,400,400,400};
         
        int nEventsLoose, nEventsTight;
        
        TCanvas *can = new TCanvas ("can","can",10,10,1600,900);
        
        for(int i = 0; i < nVars; i++){
            TLegend *legend;
            if (varNames[i] == "IDMVA") legend = new TLegend(0.35,0.6,0.65,0.9,"","brNDC");
            else legend = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
            
            double maxVal = 0.0;
            int maxInt = -1;
            
            string plotTitleStr;
            plotTitleStr = varNames[i] + " Single Photon Cuts ";
            string overallTitleStr = plotTitleStr + etaLabels[e]+ ";" + varNames[i];
            
            overallTitleStr = plotTitleStr + etaLabels[e]+ ";" + varNames[i];
            THStack *hStack = new THStack("hStack",overallTitleStr.c_str());
                               
            TH1F *hLoose[2];
            TH1F *hTight[2];
            TH1F *hIDMVA[2];
            string hNamesLoose[2] = {"hLoose1","hLoose2"};
            string hNamesTight[2] = {"hTight1","hTight2"};
            string hNamesIDMVA[3] = {"hIDMVA1","hIDMVA2"};
           
            for(int j = 0; j < nTrees; j++){
               
                string cutStringTraining;
                if (j == 0) cutStringTraining = cutStringTrainingP;
                else if (j == 1) cutStringTraining = cutStringTrainingF;

                hLoose[j] = new TH1F (hNamesLoose[j].c_str(),"",nBins[i],limsLow[i],limsHigh[i]);
                allTrees[j]->Project(hNamesLoose[j].c_str(),varNames[i].c_str(),cutStringLoose.c_str());
                
                hTight[j] = new TH1F (hNamesTight[j].c_str(),"",nBins[i],limsLow[i],limsHigh[i]);
                allTrees[j]->Project(hNamesTight[j].c_str(),varNames[i].c_str(),cutStringTraining.c_str());
                
                hIDMVA[j] = new TH1F (hNamesIDMVA[j].c_str(),"",nBins[i],limsLow[i],limsHigh[i]);
                if (includeIDMVA == true)allTrees[j]->Project(hNamesIDMVA[j].c_str(),varNames[i].c_str(),cutStringIDMVA.c_str());
            
                
                string labelUncut = allLabelsUncut[j];
                string labelCut = allLabelsCut[j];
                string labelIDMVA = allLabelsIDMVA[j];
                
                char buff1[100], buff2[100], buff3[100], buff4[100], buff5[100];
                
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
                
                snprintf(buff4, sizeof(buff4), "(%0.0f)", hIDMVA[j]->GetEntries());
                string nEventsIDMVA = buff4;
                labelIDMVA += nEventsIDMVA;
                snprintf(buff5, sizeof(buff5), "(%0.2f", 100*(hIDMVA[j]->GetSumOfWeights()/hLoose[j]->GetSumOfWeights()));
                string propIDMVA = buff5;
                labelIDMVA += (propIDMVA + "%)");
                if (includeIDMVA == true)legend->AddEntry(hIDMVA[j],labelIDMVA.c_str(),"pl");
                
                hLoose[j]->SetLineColor(j+1);
                hTight[j]->SetLineColor(j+1);
                hIDMVA[j]->SetLineColor(j+1);
                hTight[j]->SetLineStyle(9);
                hIDMVA[j]->SetLineStyle(3);
                
                
                hStack->Add(hLoose[j]);
                hStack->Add(hTight[j]);
                if (includeIDMVA == true) hStack->Add(hIDMVA[j]);
            }
            hStack->Draw("nostackhist");
            legend->Draw("same");
            can->SetGrid();
            if (varNames[i] == "IDMVA" || varNames[i] == "ChgIsoWrtChosenVtx" || varNames[i] == "HadTowOverEm" || varNames[i] == "HadronicOverEm" || varNames[i] == "EsEffSigmaRR" || varNames[i] == "EsEnergyOverRawE") can->SetLogy(1);
            else can->SetLogy(0);
            
            string outName;
            if (varNames[i] == "pt/hggMass") outName = outNameGen + "_PtOvrM" + etaFLabels[e];
            else if (varNames[i] == "abs(pt-genPt)/pt") outName = outNameGen + "_DeltaPT" + etaFLabels[e];
            else outName = outNameGen + "_" + varNames[i] + etaFLabels[e];
            can->SaveAs((outName+".png").c_str());
            can->SaveAs((outName+".root").c_str());
            
            if (i != nVars-1)can->Clear();
            
            for(int j = 0; j < nTrees; j++){
                hLoose[j]->Delete();
                hTight[j]->Delete();
                hIDMVA[j]->Delete();
            }
        }
    }
}
