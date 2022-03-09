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

void allEtaScoreCutVarCompBkg(){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);

    string fileNameGJet = "../NTuples/GGH_And_GJet_M95PTM25_HovrE_DPT075_1118.root";
    TFile *fGJet = new TFile(fileNameGJet.c_str());
    TTree *tGJet = (TTree*)fGJet->Get("GJets");
    string gjpLabelUncut = "GJet Prompt";
    string gjpLabelCut = "GJet Prompt W/ Presel";
    string gjpLabelIDMVA = "GJet Prompt W/ idMVA > -0.6";
    string gjfLabelUncut = "GJet Fake";
    string gjfLabelCut = "GJet Fake W/ Presel";
    string gjfLabelIDMVA = "GJet Fake W/ idMVA > -0.6";

    string fileNameDiphoton = "../NTuples/GGH_And_Diphoton_M95PTM25_HovrE_DPT075_1129.root";
    TFile *fDiph = new TFile(fileNameDiphoton.c_str());
    TTree *tDiph = (TTree*)fDiph->Get("diphotons");
    string diphLabelUncut = "Diphoton No Presel";
    string diphLabelCut = "Diphoton W/ Presel";
    string diphLabelIDMVA = "Diphoton W/ idMVA > -0.6";

    
    int nTrees = 3;
    TTree *allTrees[3] = {tGJet,tGJet,tDiph};
    string allLabelsUncut[3] = {gjpLabelUncut,gjfLabelUncut,diphLabelUncut};
    string allLabelsCut[3] = {gjpLabelCut,gjfLabelCut,diphLabelCut};
    string allLabelsIDMVA[3] = {gjpLabelIDMVA,gjfLabelIDMVA,diphLabelIDMVA};
    
    string idMVACutLead = " && leadIDMVA > - 0.6";
    string idMVACutSub = " && subIDMVA > - 0.6";
    
    string outNameGen = "plots/1201/BkgVariableDistributions_idMVACut_1201";

    string massCutLoose = " && hggMass > 95";
    
    string kinCutLeadLoose = " && leadPt > 17 && leadPt/hggMass > 0.15";
    string kinCutSubLoose = " && subPt > 17 && subPt/hggMass > 0.15";
    
    string kinCutLeadTight = " && leadPt > 17 && leadPt/hggMass > 0.33";
    string kinCutSubTight = " && subPt > 17 && subPt/hggMass > 0.25";
    
    string preselLead = " && leadR9 > 0.5 && leadHadronicOverEm < 0.08 && !(abs(leadScEta) < 1.5 && leadR9 < 0.85 && (leadSigmaIetaIeta > 0.015 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0)) && !(abs(leadScEta) > 1.5 && leadR9 < 0.9 && (leadSigmaIetaIeta > 0.035 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0))";
    string preselSub =  " && subR9 > 0.5 && subHadronicOverEm < 0.08 && !(abs(subScEta) < 1.5 && subR9 < 0.85 && (subSigmaIetaIeta > 0.015 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0)) && !(abs(subScEta) > 1.5 && subR9 < 0.9 && (subSigmaIetaIeta > 0.035 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0))";
    
    string cutStringLeadLooseGenPrompt = "(leadGenMatchType == 1" + massCutLoose + kinCutLeadLoose + kinCutSubLoose + ")*weight";
    string cutStringSubLooseGenPrompt = "(subGenMatchType == 1" + massCutLoose + kinCutSubLoose + kinCutLeadLoose + ")*weight";

    string cutStringLeadTightGenPrompt = "(leadGenMatchType == 1" + massCutLoose + kinCutLeadTight + preselLead + kinCutSubTight + preselSub + ")*weight";
    string cutStringSubTightGenPrompt = "(subGenMatchType == 1" + massCutLoose + kinCutSubTight + preselSub + kinCutLeadTight + preselLead + ")*weight";
    
    string cutStringLeadIDMVAGenPrompt = "(leadGenMatchType == 1" + massCutLoose + kinCutLeadLoose + idMVACutLead + kinCutSubLoose + idMVACutSub + ")*weight";
    string cutStringSubIDMVAGenPrompt = "(subGenMatchType == 1" + massCutLoose + kinCutSubLoose + idMVACutSub + kinCutLeadLoose + idMVACutLead + ")*weight";
    
    string cutStringLeadLooseMassPrompt = "(leadGenMatchType == 1" + massCutLoose + kinCutLeadLoose + kinCutSubLoose + ")*weight";
    string cutStringSubLooseMassPrompt = "(subGenMatchType == 1" + massCutLoose + kinCutSubLoose + kinCutLeadLoose + ")*weight";
    
    string cutStringLeadTightMassPrompt = "(leadGenMatchType == 1" + massCutLoose + kinCutLeadTight + preselLead + kinCutSubTight + preselSub + ")*weight";
    string cutStringSubTightMassPrompt = "(subGenMatchType == 1" + massCutLoose + kinCutSubTight + preselSub + kinCutLeadTight + preselLead + ")*weight";
    
    string cutStringLeadIDMVAMassPrompt = "(leadGenMatchType == 1" + massCutLoose + kinCutLeadLoose + idMVACutLead + kinCutSubLoose + idMVACutSub + ")*weight";
    string cutStringSubIDMVAMassPrompt = "(subGenMatchType == 1" + massCutLoose + kinCutSubLoose + idMVACutSub + kinCutLeadLoose + idMVACutLead + ")*weight";
    
    string cutStringLeadLooseGenFake = "(leadGenMatchType != 1" + massCutLoose + kinCutLeadLoose + kinCutSubLoose + ")*weight";
    string cutStringSubLooseGenFake = "(subGenMatchType != 1" + massCutLoose + kinCutSubLoose + kinCutLeadLoose + ")*weight";
    
    string cutStringLeadTightGenFake = "(leadGenMatchType != 1" + massCutLoose + kinCutLeadTight + preselLead + kinCutSubTight + preselSub + ")*weight";
    string cutStringSubTightGenFake = "(subGenMatchType != 1" + massCutLoose + kinCutSubTight + preselSub + kinCutLeadTight + preselLead + ")*weight";
    
    string cutStringLeadIDMVAGenFake = "(leadGenMatchType != 1" + massCutLoose + kinCutLeadLoose + idMVACutLead + kinCutSubLoose + idMVACutSub + ")*weight";
    string cutStringSubIDMVAGenFake = "(subGenMatchType != 1" + massCutLoose + kinCutSubLoose + idMVACutSub + kinCutLeadLoose + idMVACutLead + ")*weight";
    
    string cutStringLeadLooseMassFake = "(leadGenMatchType != 1" + massCutLoose + kinCutLeadLoose + kinCutSubLoose + ")*weight";
    string cutStringSubLooseMassFake = "(subGenMatchType != 1" + massCutLoose + kinCutSubLoose + kinCutLeadLoose + ")*weight";
    
    string cutStringLeadTightMassFake = "(leadGenMatchType != 1" + massCutLoose + kinCutLeadTight + preselLead + kinCutSubTight + preselSub + ")*weight";
    string cutStringSubTightMassFake = "(subGenMatchType != 1" + massCutLoose + kinCutSubTight + preselSub + kinCutLeadTight + preselLead + ")*weight";
    
    string cutStringLeadIDMVAMassFake = "(leadGenMatchType != 1" + massCutLoose + kinCutLeadLoose + idMVACutLead + kinCutSubLoose + idMVACutSub + ")*weight";
    string cutStringSubIDMVAMassFake = "(subGenMatchType != 1" + massCutLoose + kinCutSubLoose + idMVACutSub + kinCutLeadLoose + idMVACutLead + ")*weight";
    
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
        
        string cutStringLeadLoosePrompt,cutStringSubLoosePrompt,cutStringLeadTightPrompt,cutStringSubTightPrompt,cutStringLeadIDMVAPrompt,cutStringSubIDMVAPrompt;
        string cutStringLeadLooseFake,cutStringSubLooseFake,cutStringLeadTightFake,cutStringSubTightFake,cutStringLeadIDMVAFake,cutStringSubIDMVAFake;
        
        if(varNames[i] == "hggMass"){
            leadVarStr = varNames[i];
            subVarStr = varNames[i];
            cutStringLeadLoosePrompt = cutStringLeadLooseMassPrompt;
            cutStringSubLoosePrompt = cutStringSubLooseMassPrompt;
            cutStringLeadTightPrompt = cutStringLeadTightMassPrompt;
            cutStringSubTightPrompt = cutStringSubTightMassPrompt;
            cutStringLeadIDMVAPrompt = cutStringLeadIDMVAMassPrompt;
            cutStringSubIDMVAPrompt = cutStringSubIDMVAMassPrompt;
            
            cutStringLeadLooseFake = cutStringLeadLooseMassFake;
            cutStringSubLooseFake = cutStringSubLooseMassFake;
            cutStringLeadTightFake = cutStringLeadTightMassFake;
            cutStringSubTightFake = cutStringSubTightMassFake;
            cutStringLeadIDMVAFake = cutStringLeadIDMVAMassFake;
            cutStringSubIDMVAFake = cutStringSubIDMVAMassFake;
            
        }
        else{
            cutStringLeadLoosePrompt = cutStringLeadLooseGenPrompt;
            cutStringSubLoosePrompt = cutStringSubLooseGenPrompt;
            cutStringLeadTightPrompt = cutStringLeadTightGenPrompt;
            cutStringSubTightPrompt = cutStringSubTightGenPrompt;
            cutStringLeadIDMVAPrompt = cutStringLeadIDMVAGenPrompt;
            cutStringSubIDMVAPrompt = cutStringSubIDMVAGenPrompt;

            cutStringLeadLooseFake = cutStringLeadLooseGenFake;
            cutStringSubLooseFake = cutStringSubLooseGenFake;
            cutStringLeadTightFake = cutStringLeadTightGenFake;
            cutStringSubTightFake = cutStringSubTightGenFake;
            cutStringLeadIDMVAFake = cutStringLeadIDMVAGenFake;
            cutStringSubIDMVAFake = cutStringSubIDMVAGenFake;

        }
        
        TH1F *hLoose[3];
        TH1F *hLooseSub[3];
        TH1F *hTight[3];
        TH1F *hTightSub[3];
        TH1F *hIDMVA[3];
        TH1F *hIDMVASub[3];
        string hNamesLoose[3] = {"hLoose1","hLoose2","hLoose3"};
        string hNamesLooseSub[3] = {"hLooseSub1","hLooseSub2","hLooseSub3"};
        string hNamesTight[3] = {"hTight1","hTight2","hTight3"};
        string hNamesTightSub[3] = {"hTightSub1","hTightSub2","hTightSub3"};
        string hNamesIDMVA[3] = {"hidMVA1","hidMVA2","hidMVA3"};
        string hNamesIDMVASub[3] = {"hidMVASub1","hidMVASub2","hidMVASub3"};
        for(int j = 0; j < nTrees; j++){
            string cutStringLeadLooseInside;
            string cutStringLeadTightInside;
            string cutStringLeadIDMVAInside;
            
            string cutStringSubLooseInside;
            string cutStringSubTightInside;
            string cutStringSubIDMVAInside;
            
            if(j == 0){
                cutStringLeadLooseInside = cutStringLeadLoosePrompt;
                cutStringLeadTightInside = cutStringLeadTightPrompt;
                cutStringLeadIDMVAInside = cutStringLeadIDMVAPrompt;
                
                cutStringSubLooseInside = cutStringSubLoosePrompt;
                cutStringSubTightInside = cutStringSubTightPrompt;
                cutStringSubIDMVAInside = cutStringSubIDMVAPrompt;
                
                cutStringLeadLooseInside += "*1";
                cutStringLeadTightInside += "*1";
                cutStringLeadIDMVAInside += "*1";
                
                cutStringSubLooseInside += "*1";
                cutStringSubTightInside += "*1";
                cutStringSubIDMVAInside += "*1";
            }
            if(j == 1){
                cutStringLeadLooseInside = cutStringLeadLooseFake;
                cutStringLeadTightInside = cutStringLeadTightFake;
                cutStringLeadIDMVAInside = cutStringLeadIDMVAFake;
                
                cutStringSubLooseInside = cutStringSubLooseFake;
                cutStringSubTightInside = cutStringSubTightFake;
                cutStringSubIDMVAInside = cutStringSubIDMVAFake;
                
                cutStringLeadLooseInside += "*1";
                cutStringLeadTightInside += "*1";
                cutStringLeadIDMVAInside += "*1";

                cutStringSubLooseInside += "*1";
                cutStringSubTightInside += "*1";
                cutStringSubIDMVAInside += "*1";
            }
            if(j == 2){
                cutStringLeadLooseInside = cutStringLeadLoosePrompt;
                cutStringLeadTightInside = cutStringLeadTightPrompt;
                cutStringLeadIDMVAInside = cutStringLeadIDMVAPrompt;
                
                cutStringSubLooseInside = cutStringSubLoosePrompt;
                cutStringSubTightInside = cutStringSubTightPrompt;
                cutStringSubIDMVAInside = cutStringSubIDMVAPrompt;
                
                cutStringLeadLooseInside += "*1";
                cutStringLeadTightInside += "*1";
                cutStringLeadIDMVAInside += "*1";
                
                cutStringSubLooseInside += "*1";
                cutStringSubTightInside += "*1";
                cutStringSubIDMVAInside += "*1";
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
            
            hIDMVA[j] = new TH1F (hNamesIDMVA[j].c_str(),"",nBins[i],limsLow[i],limsHigh[i]);
            hIDMVASub[j] = new TH1F (hNamesIDMVASub[j].c_str(),"",nBins[i],limsLow[i],limsHigh[i]);
            allTrees[j]->Project(hNamesIDMVA[j].c_str(),leadVarStr.c_str(),cutStringLeadIDMVAInside.c_str());
            allTrees[j]->Project(hNamesIDMVASub[j].c_str(),subVarStr.c_str(),cutStringSubIDMVAInside.c_str());
            hIDMVA[j]->Add(hIDMVASub[j]);
            
            string labelUncut = allLabelsUncut[j];
            string labelCut = allLabelsCut[j];
            string labelIDMVA = allLabelsIDMVA[j];
            
            char buff1[100], buff2[100], buff3[100], buff4[100];
            
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
            
            snprintf(buff4, sizeof(buff4), "(%0.2f", 100*(hIDMVA[j]->GetSumOfWeights()/hLoose[j]->GetSumOfWeights()));
            string propIDMVA = buff4;
            labelIDMVA += (propIDMVA + "%)");
            legend->AddEntry(hIDMVA[j],labelIDMVA.c_str(),"pl");
            
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
            hIDMVA[j]->SetLineColor(j+1);
            hTight[j]->SetLineStyle(9);
            hIDMVA[j]->SetLineStyle(3);
            
            hStack->Add(hLoose[j]);
            hStack->Add(hTight[j]);
            hStack->Add(hIDMVA[j]);
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
            hIDMVA[j]->Delete();
            hIDMVASub[j]->Delete();

        }

    }
}


