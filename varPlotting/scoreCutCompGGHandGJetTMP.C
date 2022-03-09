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

void scoreCutCompGGHandGJetTMP(bool genMatchSplit, bool diphotonCuts){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);

    string fileName1 = "../NTuples/GGH_And_GJets_M95PTM25_Old0916_0113.root";
    TFile *f1 = new TFile(fileName1.c_str());
    TTree *tGJet1 = (TTree*)f1->Get("GJets");
    TTree *tGGH1 = (TTree*)f1->Get("ggh_125");
    
    string fileName2 = "../NTuples/GGH_And_GJets_M95PTM25_NoDPT_0113.root";
    TFile *f2 = new TFile(fileName2.c_str());
    TTree *tGJet2 = (TTree*)f2->Get("GJets");
    TTree *tGGH2 = (TTree*)f2->Get("ggh_125");
    
    string gjpLabelUncut = "GJet Prompt";
    string gjpLabelPresel = "GJet Prompt Presel";
    string gjpLabelIDMVA1 = "GJet Prompt W/ Old idMVA > -0.99";
    string gjpLabelIDMVA2 = "GJet Prompt W/ New idMVA > -0.997";
    //string gjpLabelIDMVA2 = "GJet Prompt W/ New idMVABarrel > -0.99581 & idMVAEndcap > -0.97466";
    string gjfLabelUncut = "GJet Fake";
    string gjfLabelPresel = "GJet Fake Presel";
    string gjfLabelIDMVA1 = "GJet Fake W/ Old idMVA > -0.99";
    string gjfLabelIDMVA2 = "GJet Fake W/ New idMVA > -0.997";
    //string gjfLabelIDMVA2 = "GJet Fake W/ New idMVABarrel > -0.99581 & idMVAEndcap > -0.97466";
    string gghLabelUncut = "GGH";
    string gghLabelPresel = "GGH Presel";
    string gghLabelIDMVA1 = "GGH W/ Old idMVA > -0.99";
    string gghLabelIDMVA2 = "GGH W/ New idMVA > -0.997";
    //string gghLabelIDMVA2 = "GGH W/ New idMVABarrel > -0.99581 & idMVAEndcap > -0.97466";
    
//    string gjpLabelUncut = "GJet Prompt";
//    string gjpLabelPresel = "GJet Prompt Presel";
//    string gjpLabelIDMVA1 = "GJet Prompt PtM > 0.25 idMVA > -0.99";
//    string gjpLabelIDMVA2 = "GJet Prompt PtM > 0.15 idMVA > -0.99";
//    //string gjpLabelIDMVA2 = "GJet Prompt W/ New idMVABarrel > -0.99581 & idMVAEndcap > -0.97466";
//    string gjfLabelUncut = "GJet Fake";
//    string gjfLabelPresel = "GJet Fake Presel";
//    string gjfLabelIDMVA1 = "GJet Fake PtM > 0.25 idMVA > -0.99";
//    string gjfLabelIDMVA2 = "GJet Fake PtM > 0.15 idMVA > -0.99";
//    //string gjfLabelIDMVA2 = "GJet Fake W/ New idMVABarrel > -0.99581 & idMVAEndcap > -0.97466";
//    string gghLabelUncut = "GGH";
//    string gghLabelPresel = "GGH Presel";
//    string gghLabelIDMVA1 = "GGH PtM > 0.25 idMVA > -0.99";
//    string gghLabelIDMVA2 = "GGH PtM > 0.15 idMVA > -0.99";
//    //string gghLabelIDMVA2 = "GGH W/ New idMVABarrel > -0.99581 & idMVAEndcap > -0.97466";

    string dirStr ="plots/0119/GGHandGJetPromptFake/";
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
        
        genMatchLabels[0] = "Endcap";
        genMatchFLabels[0] = "";
        genMatchCuts[0] = "";
    }
    int nTrees = 12;
    TTree *allTrees[12] = {tGJet1,tGJet1,tGJet1,tGJet2,tGJet1,tGJet1,tGJet1,tGJet2,tGGH1,tGGH1,tGGH1,tGGH2};
    string allLabels[12] = {gjpLabelUncut,gjpLabelPresel,gjpLabelIDMVA1,gjpLabelIDMVA2,gjfLabelUncut,gjfLabelPresel,gjfLabelIDMVA1,gjfLabelIDMVA2,gghLabelUncut,gghLabelPresel,gghLabelIDMVA1,gghLabelIDMVA2};
    
    
    if(genMatchSplit == true) dirStr += "GenMatchSplit/";
    string outNameGen = dirStr + "Endcap_NewVsOld_idMVACut";
    if(diphotonCuts == true)outNameGen += "_DiphotonCuts";
    if(diphotonCuts == false)outNameGen += "_SinglePhotonCuts";
    
    string etaCutLead = " && abs(leadScEta) > 1.556";
    string etaCutSub = " && abs(subScEta) > 1.556";
    
    //string etaCutLead = " && abs(leadScEta) < 1.4442";
    //string etaCutSub = " && abs(subScEta) < 1.4442";
    
    //string etaCutLead = " ";
    //string etaCutSub = " ";
    
    string massCutTight = " && hggMass > 121 && hggMass < 129" + etaCutLead + etaCutSub;
    string massCutLoose = " && hggMass > 95" + etaCutLead + etaCutSub;
    
    string kinCutLeadLoose = " && leadPt > 17 && leadPt/hggMass > 0.15";
    string kinCutSubLoose = " && subPt > 17 && subPt/hggMass > 0.15";

    string kinCutLeadTight = " && leadPt > 17 && leadPt/hggMass > 0.33";
    string kinCutSubTight = " && subPt > 17 && subPt/hggMass > 0.25";
    
    string idMVACutLead1 = "&& leadIDMVA > -0.99";
    string idMVACutSub1 = "&& subIDMVA > -0.99";
    
    string idMVACutLead2 = "&& leadIDMVA > -0.997";
    string idMVACutSub2 = "&& subIDMVA > -0.997";
    
   // string idMVACutLead2 = " && !(abs(leadScEta) < 1.4442 && leadIDMVA < -0.99581) && !(abs(leadScEta) > 1.556 && leadIDMVA < -0.97466)";
    //string idMVACutSub2 = " && !(abs(subScEta) < 1.4442 && subIDMVA < -0.99581) && !(abs(subScEta) > 1.556 && subIDMVA < -0.97466)";
    
    string preselLead = " && leadR9 > 0.5 && leadHadronicOverEm < 0.08 && !(abs(leadScEta) < 1.5 && leadR9 < 0.85 && (leadSigmaIetaIeta > 0.015 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0)) && !(abs(leadScEta) > 1.5 && leadR9 < 0.9 && (leadSigmaIetaIeta > 0.035 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0))";
    string preselSub =  " && subR9 > 0.5 && subHadronicOverEm < 0.08 && !(abs(subScEta) < 1.5 && subR9 < 0.85 && (subSigmaIetaIeta > 0.015 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0)) && !(abs(subScEta) > 1.5 && subR9 < 0.9 && (subSigmaIetaIeta > 0.035 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0))";
    
    for (int e = 0; e < nGenMatch; e++){
        string cutStringLeadLoose,cutStringSubLoose,cutStringLeadTight,cutStringSubTight,cutStringLeadIDMVA1,cutStringSubIDMVA1,cutStringLeadIDMVA2,cutStringSubIDMVA2;
        if(diphotonCuts == true){
           
            cutStringLeadLoose = genMatchCuts[e] + kinCutLeadLoose + kinCutSubLoose + ")*weight";
            cutStringSubLoose =  genMatchCuts[e] + kinCutSubLoose + kinCutLeadLoose + ")*weight";
            
            cutStringLeadTight = genMatchCuts[e] + kinCutLeadTight + preselLead + kinCutSubTight + preselSub + ")*weight";
            cutStringSubTight =  genMatchCuts[e] + kinCutSubTight + preselSub + kinCutLeadTight + preselLead + ")*weight";
    
            cutStringLeadIDMVA1 = genMatchCuts[e] + kinCutLeadLoose + idMVACutLead1 + kinCutSubLoose + idMVACutSub1 + ")*weight";
            cutStringSubIDMVA1 = genMatchCuts[e] + kinCutSubLoose + idMVACutSub1 + kinCutLeadLoose + idMVACutLead1 + ")*weight";
            
            cutStringLeadIDMVA2 = genMatchCuts[e] + kinCutLeadLoose + idMVACutLead2 + kinCutSubLoose + idMVACutSub2 + ")*weight";
            cutStringSubIDMVA2 = genMatchCuts[e] + kinCutSubLoose + idMVACutSub2 + kinCutLeadLoose + idMVACutLead2 + ")*weight";
        }
        if(diphotonCuts == false){
            cutStringLeadLoose = genMatchCuts[e] + kinCutLeadLoose + ")*weight";
            cutStringSubLoose = genMatchCuts[e] + kinCutSubLoose + ")*weight";
            
            cutStringLeadTight = genMatchCuts[e] + kinCutLeadTight + preselLead + ")*weight";
            cutStringSubTight =  genMatchCuts[e] + kinCutSubTight + preselSub + ")*weight";
            
            cutStringLeadIDMVA1 = genMatchCuts[e] + kinCutLeadLoose;
            cutStringSubIDMVA1 = genMatchCuts[e] + kinCutSubLoose;
            
            cutStringLeadIDMVA2 = genMatchCuts[e] + kinCutLeadLoose;
            cutStringSubIDMVA2 = genMatchCuts[e] + kinCutSubLoose;
        }
        
        int nVars = 22;
        string varNames[22] = {"Pt","Pt/hggMass","ScEta","hggMass","SigmaIetaIeta","IDMVA","IDMVAZoom","weight","SCRawE","R9","EtaWidth","PhiWidth","CovIEtaIPhi","S4","PhoIso03","ChgIsoWrtChosenVtx","ChgIsoWrtWorstVtx","rho","HadTowOverEm","HadronicOverEm","EsEffSigmaRR","EsEnergyOverRawE"};
        double limsLow[22] = {0.0,0.0,-3.0,80.0,0.0,-1.1,-1.0,-1.0,0.0,0.0,0.0,0.0,-0.002,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
        double limsHigh[22] = {200,1.3,3.0,180,0.1,1.1,-0.95,5.0,650,1.1,0.2,0.2,0.002,1.0,25.0,25.0,25.0,60.0,0.4,1.0,8.0,0.2};
        int nBins[22] = {400,520,400,400,400,440,400,500,650,440,400,400,400,400,500,500,500,600,400,400,400,400};
        int nEventsLoose, nEventsTight;
        
        TCanvas *can = new TCanvas ("can","can",10,10,1600,900);
        
        //for(int i = 0; i < nVars; i++){
        for(int i = 6; i < 7; i++){
            string cutStringLeadIDMVA1Inside;
            string cutStringSubIDMVA1Inside;
            string cutStringLeadIDMVA2Inside;
            string cutStringSubIDMVA2Inside;
            
        //for(int i = 16; i < 20; i++){
            string insideIDMVACutLead1;
            string insideIDMVACutSub1;
            string insideIDMVACutLead2;
            string insideIDMVACutSub2;
            if(varNames[i] == "IDMVA" || varNames[i] == "IDMVAZoom"){
                insideIDMVACutLead1 = ")*weight";
                insideIDMVACutSub1 = ")*weight";
                insideIDMVACutLead2 = ")*weight";
                insideIDMVACutSub2 = ")*weight";
            }
            else{
                insideIDMVACutLead1 = idMVACutLead1 + ")*weight";
                insideIDMVACutSub1 = idMVACutSub1 + ")*weight";
                insideIDMVACutLead2 = idMVACutLead2 + ")*weight";
                insideIDMVACutSub2 = idMVACutSub2 + ")*weight";
            }
            cutStringLeadIDMVA1Inside = cutStringLeadIDMVA1 + insideIDMVACutLead1;
            cutStringSubIDMVA1Inside = cutStringSubIDMVA2 + insideIDMVACutSub1;
            cutStringLeadIDMVA2Inside = cutStringLeadIDMVA2 + insideIDMVACutLead2;
            cutStringSubIDMVA2Inside = cutStringSubIDMVA2 + insideIDMVACutSub2;
           
            TLegend *legend;
            if (varNames[i] == "IDMVA") legend = new TLegend(0.35,0.6,0.65,0.9,"","brNDC");
            else legend = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
            
            double maxVal = 0.0;
            int maxInt = -1;
            
            string plotTitleStr;
            if(diphotonCuts == true)plotTitleStr = varNames[i] + " Diphoton Cuts ";
            if(diphotonCuts == false)plotTitleStr = varNames[i] + " Single Photon Cuts ";
            string overallTitleStr = plotTitleStr + genMatchLabels[e] + ";" + varNames[i];
            THStack *hStack = new THStack("hStack",overallTitleStr.c_str());
            
            string leadVarStr = "lead" + varNames[i];
            string subVarStr = "sub" + varNames[i];
            
            if(varNames[i] == "hggMass" || varNames[i] == "rho" || varNames[i] == "weight"){
                leadVarStr = varNames[i];
                subVarStr = varNames[i];
            }
            if(varNames[i] == "IDMVAZoom"){
                leadVarStr = "leadIDMVA";
                subVarStr = "subIDMVA";
            }
            
            TH1F *h[12];
            TH1F *hSub[12];
            string hNames[12] = {"hPUncut","hPPresel","hIDMVA1P","hIDMVA2P","hFUncut","hFPresel","hIDMVA1F","hIDMVA2F","hGGHUncut","hGGHPresel","hGGHIDMVA1","hGGHIDMVA2"};
            string hNamesSub[12] = {"hPUncutSub","hPPreselSub","hIDMVA1PSub","hIDMVA2PSub","hFUncutSub","hFPreselSub","hIDMVA1FSub","hIDMVA2FSub","hGGHUncutSub","hGGHPreselSub","hGGHIDMVA1Sub","hGGHIDMVA2Sub"};
            for(int j = 0; j < nTrees; j++){
        
                string cutStringLead;
                string cutStringSub;
                
                if (j == 0){
                    cutStringLead = "(leadGenMatchType == 1 && subGenMatchType != 1" + massCutLoose + cutStringLeadLoose;
                    cutStringSub = "(subGenMatchType == 1 && leadGenMatchType != 1" + massCutLoose + cutStringSubLoose;
                }
                if (j == 1){
                    cutStringLead = "(leadGenMatchType == 1 && subGenMatchType != 1" + massCutLoose + cutStringLeadTight;
                    cutStringSub = "(subGenMatchType == 1 && leadGenMatchType != 1" + massCutLoose + cutStringSubTight;
                }
                if (j == 2){
                    cutStringLead = "(leadGenMatchType == 1 && subGenMatchType != 1" + massCutLoose + cutStringLeadIDMVA1Inside;
                    cutStringSub = "(subGenMatchType == 1 && leadGenMatchType != 1" + massCutLoose + cutStringSubIDMVA1Inside;
                }
                if (j == 3){
                    cutStringLead = "(leadGenMatchType == 1 && subGenMatchType != 1" + massCutLoose + cutStringLeadIDMVA2Inside;
                    cutStringSub = "(subGenMatchType == 1 && leadGenMatchType != 1" + massCutLoose + cutStringSubIDMVA2Inside;
                }
                
                if (j == 4){
                    cutStringLead = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + cutStringLeadLoose;
                    cutStringSub = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + cutStringSubLoose;
                }
                if (j == 5){
                    cutStringLead = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + cutStringLeadTight;
                    cutStringSub = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + cutStringSubTight;
                }
                if (j == 6){
                    cutStringLead = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + cutStringLeadIDMVA1Inside;
                    cutStringSub = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + cutStringSubIDMVA1Inside;
                }
                if (j == 7){
                    cutStringLead = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + cutStringLeadIDMVA2Inside;
                    cutStringSub = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + cutStringSubIDMVA2Inside;
                }
                
                if (j == 8){
                    cutStringLead = "(leadGenMatchType == 1 " + massCutTight + cutStringLeadLoose + "*400";
                    cutStringSub = "(subGenMatchType == 1" + massCutTight + cutStringSubLoose + "*400";
                }
                if (j == 9){
                    cutStringLead = "(leadGenMatchType == 1 " + massCutTight + cutStringLeadTight + "*400";
                    cutStringSub = "(subGenMatchType == 1" + massCutTight + cutStringSubTight + "*400";
                }
                if (j == 10){
                    cutStringLead = "(leadGenMatchType == 1 " + massCutTight + cutStringLeadIDMVA1Inside + "*400";
                    cutStringSub = "(subGenMatchType == 1" + massCutTight + cutStringSubIDMVA1Inside + "*400";
                }
                if (j == 11){
                    cutStringLead = "(leadGenMatchType == 1 " + massCutTight + cutStringLeadIDMVA2Inside + "*400";
                    cutStringSub = "(subGenMatchType == 1" + massCutTight + cutStringSubIDMVA2Inside + "*400";
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
                if (j == 1 || j == 2 || j == 3){
                    snprintf(buff2, sizeof(buff2), "(%0.2f", 100*(h[j]->GetSumOfWeights()/h[0]->GetSumOfWeights()));
                    string prop = buff2;
                    label += (prop + "%)");
                }
                if (j == 5 || j == 6 || j == 7){
                    snprintf(buff2, sizeof(buff2), "(%0.2f", 100*(h[j]->GetSumOfWeights()/h[4]->GetSumOfWeights()));
                    string prop = buff2;
                    label += (prop + "%)");
                }
                if (j == 9 || j == 10 || j == 11){
                    snprintf(buff2, sizeof(buff2), "(%0.2f", 100*(h[j]->GetSumOfWeights()/h[8]->GetSumOfWeights()));
                    string prop = buff2;
                    label += (prop + "%)");
                }
            
                legend->AddEntry(h[j],label.c_str(),"pl");
                if (j == 0 || j == 1 || j == 2 || j == 3)h[j]->SetLineColor(1);
                if (j == 4 || j == 5 || j == 6 || j == 7)h[j]->SetLineColor(2);
                if (j == 8 || j == 9 || j == 10 || j == 11)h[j]->SetLineColor(3);
                if (j == 1 || j == 5 || j == 9) h[j]->SetLineStyle(9);
                if (j == 2 || j == 6 || j == 10) h[j]->SetLineStyle(3);
                if (j == 3 || j == 7 || j == 11) h[j]->SetLineStyle(6);
                
                //if(j == 3)h[j]->Scale(3.9/26.0);
                
                hStack->Add(h[j]);
            }
            hStack->Draw("nostackhist");
            legend->Draw("same");
            can->SetGrid();
            if (varNames[i] == "IDMVA" || varNames[i] == "IDMVAZoom" || varNames[i] == "ChgIsoWrtChosenVtx" || varNames[i] == "HadTowOverEm" || varNames[i] == "HadronicOverEm" || varNames[i] == "EsEffSigmaRR" || varNames[i] == "EsEnergyOverRawE") can->SetLogy(1);
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


