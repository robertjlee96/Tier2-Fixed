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

void scoreCutCompDiffModels(bool genMatchSplit, bool diphotonCuts){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);

    string fileName1 = "../NTuples/GGH_And_GJet_M95PTM25_HovrE_DPT075_1118.root";
    TFile *f1 = new TFile(fileName1.c_str());
    TTree *tGJet1 = (TTree*)f1->Get("GJets");
    
    string fileName2 = "../NTuples/GGH_And_GJets_M95PTM25_DPT075_HHovrE_Test25Percent_FixSplit_1222.root";
    TFile *f2 = new TFile(fileName2.c_str());
    TTree *tGJet2 = (TTree*)f2->Get("GJets");
    
    string gjpLabelUncut = "GJet Prompt";
    string gjpLabelIDMVA1 = "GJet Prompt W/ Old idMVA > -0.99";
    string gjpLabelIDMVA2 = "GJet Prompt W/ New idMVABarrel > -0.95386 & idMVAEndcap > -0.85947";
    string gjfLabelUncut = "GJet Fake";
    string gjfLabelIDMVA1 = "GJet Fake W/ Old idMVA > -0.99";
    string gjfLabelIDMVA2 = "GJet Fake W/ New idMVABarrel > -0.95386 & idMVAEndcap > -0.85947";

    string dirStr ="plots/0105/CompModels/";
    
    int nGenMatch;
    string genMatchCuts[3];
    string genMatchLabels[3];
    string genMatchFLabels[3];
    
    if (genMatchSplit == true){
        nGenMatch = 3;
        
        genMatchLabels[0] = "Prompt-Prompt All Eta";
        genMatchFLabels[0] = "_PP";
        genMatchCuts[0] = " && leadGenMatchType == 1  && subGenMatchType == 1";
        
        genMatchLabels[1] = "Fake-Fake All Eta";
        genMatchFLabels[1] = "_FF";
        genMatchCuts[1] = " && leadGenMatchType != 1  && subGenMatchType != 1";
        
        genMatchLabels[2] = "Prompt-Fake All Eta";
        genMatchFLabels[2] = "_PF";
        genMatchCuts[2] = "&& ((leadGenMatchType == 1  && subGenMatchType != 1) || (leadGenMatchType != 1  && subGenMatchType == 1))";
    }
    
    if (genMatchSplit == false){
        nGenMatch = 1;
        
        genMatchLabels[0] = "All Eta";
        genMatchFLabels[0] = "";
        genMatchCuts[0] = "";
    }

    int nTrees = 6;
    TTree *allTrees[6] = {tGJet1,tGJet1,tGJet2,tGJet1,tGJet1,tGJet2};
    string allLabels[6] = {gjpLabelUncut,gjpLabelIDMVA1,gjpLabelIDMVA2,gjfLabelUncut,gjfLabelIDMVA1,gjfLabelIDMVA2};
    
    if(genMatchSplit == true) dirStr += "GenMatchSplit/";
    string outNameGen = dirStr + "HighWeight_ValidationSamples_idMVACut";
    if(diphotonCuts == true)outNameGen += "_DiphotonCuts";
    if(diphotonCuts == false)outNameGen += "_SinglePhotonCuts";
    
    string massCutLoose = " && hggMass > 95 && weight > 2.0 ";
    
    string kinCutLeadLoose = " && leadPt > 17 && leadPt/hggMass > 0.15";
    string kinCutSubLoose = " && subPt > 17 && subPt/hggMass > 0.15";
    
    string idMVACutLead1 = "&& leadIDMVA > -0.99";
    string idMVACutSub1 = "&& subIDMVA > -0.99";
    
    string idMVACutLead2 = " && !(abs(leadScEta) < 1.4442 && leadIDMVA < -0.95386) && !(abs(leadScEta) > 1.556 && leadIDMVA < -0.85947)";
    string idMVACutSub2 = " && !(abs(subScEta) < 1.4442 && subIDMVA < -0.95386) && !(abs(subScEta) > 1.556 && subIDMVA < -0.85947)";
    
    //string idMVACutLead2 = "&& leadIDMVA > -0.95";
    //string idMVACutSub2 = "&& subIDMVA > -0.95";
    
    //for (int e = 0; e < nGenMatch; e++){
   for (int e = 2; e < nGenMatch; e++){
        string cutStringLeadLoose,cutStringSubLoose,cutStringLeadIDMVA1,cutStringSubIDMVA1,cutStringLeadIDMVA2,cutStringSubIDMVA2;
        if(diphotonCuts == true){
            cutStringLeadLoose = genMatchCuts[e] + massCutLoose + kinCutLeadLoose + kinCutSubLoose + ")*weight";
            cutStringSubLoose =  genMatchCuts[e] + massCutLoose + kinCutSubLoose + kinCutLeadLoose + ")*weight";
    
            cutStringLeadIDMVA1 = genMatchCuts[e] + massCutLoose + kinCutLeadLoose + idMVACutLead1 + kinCutSubLoose + idMVACutSub1 + ")*weight";
            cutStringSubIDMVA1 = genMatchCuts[e] + massCutLoose + kinCutSubLoose + idMVACutSub1 + kinCutLeadLoose + idMVACutLead1 + ")*weight";
            cutStringLeadIDMVA2 = genMatchCuts[e] + massCutLoose + kinCutLeadLoose + idMVACutLead2 + kinCutSubLoose + idMVACutSub2 + ")*weight";
            cutStringSubIDMVA2 = genMatchCuts[e] + massCutLoose + kinCutSubLoose + idMVACutSub2 + kinCutLeadLoose + idMVACutLead2 + ")*weight";
        }
        if(diphotonCuts == false){
            cutStringLeadLoose = genMatchCuts[e] + massCutLoose + kinCutLeadLoose + ")*weight";
            cutStringSubLoose = genMatchCuts[e] + massCutLoose + kinCutSubLoose + ")*weight";
            
            cutStringLeadIDMVA1 = genMatchCuts[e] + massCutLoose + kinCutLeadLoose + idMVACutLead1 + ")*weight";
            cutStringSubIDMVA1 = genMatchCuts[e] + massCutLoose + kinCutSubLoose + idMVACutSub1 + ")*weight";
            
            cutStringLeadIDMVA2 = genMatchCuts[e] + massCutLoose + kinCutLeadLoose + idMVACutLead2 + ")*weight";
            cutStringSubIDMVA2 = genMatchCuts[e] + massCutLoose + kinCutSubLoose + idMVACutSub2+ ")*weight";
        }
        
        int nVars = 21;
        string varNames[21] = {"Pt","Pt/hggMass","hggMass","SigmaIetaIeta","IDMVA","weight","SCRawE","R9","EtaWidth","PhiWidth","CovIEtaIPhi","S4","PhoIso03","ChgIsoWrtChosenVtx","ChgIsoWrtWorstVtx","ScEta","rho","HadTowOverEm","HadronicOverEm","EsEffSigmaRR","EsEnergyOverRawE"};
        double limsLow[21] = {0.0,0.0,80.0,0.0,-1.1,-1.0,0.0,0.0,0.0,0.0,-0.002,0.0,0.0,0.0,0.0,-3.0,0.0,0.0,0.0,0.0,0.0};
        double limsHigh[21] = {200,1.3,180,0.1,1.1,5.0,650,1.1,0.2,0.2,0.002,1.0,25.0,25.0,25.0,3.0,60.0,0.4,1.0,8.0,0.2};
        int nBins[21] = {400,520,400,400,440,500,650,440,400,400,400,400,500,500,500,400,600,400,400,400,400};
        int nEventsLoose, nEventsTight;
        
        TCanvas *can = new TCanvas ("can","can",10,10,1600,900);
        
        for(int i = 0; i < nVars; i++){
        //for(int i = 5; i < 6; i++){
        //for(int i = 16; i < 20; i++){
            TLegend *legend;
            if (varNames[i] == "IDMVA") legend = new TLegend(0.35,0.6,0.65,0.9,"","brNDC");
            else legend = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
            
            double maxVal = 0.0;
            int maxInt = -1;
            
            string plotTitleStr;
            if(diphotonCuts == true)plotTitleStr = varNames[i] + " Diphoton Cuts ";
            if(diphotonCuts == false)plotTitleStr = varNames[i] + " Single Photon Cuts ";
            string overallTitleStr = plotTitleStr + genMatchLabels[e] + " Weight > 2.0;" + varNames[i];
            THStack *hStack = new THStack("hStack",overallTitleStr.c_str());
            
            string leadVarStr = "lead" + varNames[i];
            string subVarStr = "sub" + varNames[i];
            
            if(varNames[i] == "hggMass" || varNames[i] == "rho" || varNames[i] == "weight"){
                leadVarStr = varNames[i];
                subVarStr = varNames[i];
            }
        
            
            TH1F *h[6];
            TH1F *hSub[6];
            string hNames[6] = {"hPUncut","hIDMVA1P","hIDMVA2P","hFUncut","hIDMVA1F","hIDMVA2F"};
            string hNamesSub[6] = {"hPUncutSub","hIDMVA1PSub","hIDMVA2PSub","hFUncutSub","hIDMVA1FSub","hIDMVA2FSub"};
            for(int j = 0; j < nTrees; j++){
        
                string cutStringLead;
                string cutStringSub;
                
                if (j == 0){
                    cutStringLead = "(leadGenMatchType == 1" + cutStringLeadLoose;
                    cutStringSub = "(subGenMatchType == 1" + cutStringSubLoose;
                }
                if (j == 1){
                    cutStringLead = "(leadGenMatchType == 1" + cutStringLeadIDMVA1;
                    cutStringSub = "(subGenMatchType == 1" + cutStringSubIDMVA1;
                }
                if (j == 2){
                    cutStringLead = "(leadGenMatchType == 1" + cutStringLeadIDMVA2;
                    cutStringSub = "(subGenMatchType == 1" + cutStringSubIDMVA2;
                }
                if (j == 3){
                    cutStringLead = "(leadGenMatchType != 1" + cutStringLeadLoose;
                    cutStringSub = "(subGenMatchType != 1" + cutStringSubLoose;
                }
                if (j == 4){
                    cutStringLead = "(leadGenMatchType != 1" + cutStringLeadIDMVA1;
                    cutStringSub = "(subGenMatchType != 1" + cutStringSubIDMVA1;
                }
                if(j == 5){
                    cutStringLead = "(leadGenMatchType != 1" + cutStringLeadIDMVA2;
                    cutStringSub = "(subGenMatchType != 1" + cutStringSubIDMVA2;
                }
                h[j] = new TH1F (hNames[j].c_str(),"",nBins[i],limsLow[i],limsHigh[i]);
                hSub[j] = new TH1F (hNamesSub[j].c_str(),"",nBins[i],limsLow[i],limsHigh[i]);
                allTrees[j]->Project(hNames[j].c_str(),leadVarStr.c_str(),cutStringLead.c_str());
                allTrees[j]->Project(hNamesSub[j].c_str(),subVarStr.c_str(),cutStringSub.c_str());
                h[j]->Add(hSub[j]);
                
                string label = allLabels[j];
                
                char buff1[100], buff2[100];
                
                snprintf(buff1, sizeof(buff1), "(%0.0f)", h[j]->GetEntries());
                string nEvents = buff1;
                label += nEvents;
                if (j == 1 || j == 2){
                    snprintf(buff2, sizeof(buff2), "(%0.2f", 100*(h[j]->GetSumOfWeights()/h[0]->GetSumOfWeights()));
                    string prop = buff2;
                    label += (prop + "%)");
                }
                if (j == 4 || j == 5){
                    snprintf(buff2, sizeof(buff2), "(%0.2f", 100*(h[j]->GetSumOfWeights()/h[3]->GetSumOfWeights()));
                    string prop = buff2;
                    label += (prop + "%)");
                }
            
                legend->AddEntry(h[j],label.c_str(),"pl");
                if (j == 0 || j == 1 || j == 2)h[j]->SetLineColor(1);
                if (j == 3 || j == 4 || j == 5)h[j]->SetLineColor(2);
                if (j == 1 || j == 4) h[j]->SetLineStyle(9);
                if (j == 2 || j == 5) h[j]->SetLineStyle(3);
                
                //if(j == 3)h[j]->Scale(3.9/26.0);
                
                hStack->Add(h[j]);
            }
            hStack->Draw("nostackhist");
            legend->Draw("same");
            can->SetGrid();
            if (varNames[i] == "IDMVA" || varNames[i] == "ChgIsoWrtChosenVtx" || varNames[i] == "HadTowOverEm" || varNames[i] == "HadronicOverEm" || varNames[i] == "EsEffSigmaRR" || varNames[i] == "EsEnergyOverRawE") can->SetLogy(1);
            else can->SetLogy(0);
            
            string outName;
            if (varNames[i] == "Pt/hggMass") outName = outNameGen + "_PtOvrM" + genMatchFLabels[e];
            else outName = outNameGen + "_" + varNames[i] + genMatchFLabels[e];
            can->SaveAs((outName+".png").c_str());
            can->SaveAs((outName+".root").c_str());
            
            if (i != nVars-1)can->Clear();
            
            for(int j = 0; j < nTrees; j++){
                h[j]->Delete();
                hSub[j]->Delete();
            }
        }
    }
}


