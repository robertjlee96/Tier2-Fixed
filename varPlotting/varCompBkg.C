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

void varCompBkg(bool etaSplit,bool diphotonCuts){
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
    string gjfLabelUncut = "GJet Fake";
    string gjfLabelCut = "GJet Fake W/ Presel";
    
    string fileNameDiphoton = "../NTuples/GGH_And_Diphoton_M95PTM25_HovrE_DPT075_1129.root";
    TFile *fDiph = new TFile(fileNameDiphoton.c_str());
    TTree *tDiph = (TTree*)fDiph->Get("diphotons");
    string diphLabelUncut = "Diphoton No Presel";
    string diphLabelCut = "Diphoton W/ Presel";
    
    //bool etaSplit == false;
    int nEta;
    string etaCuts[3];
    string etaLabels[3];
    string etaFLabels[3];
    
    if (etaSplit == true){
        nEta = 3;
        
        etaLabels[0] = "Barrel-Barrel";
        etaFLabels[0] = "_BB";
        etaCuts[0] = " && abs(leadScEta) < 1.4442 && abs(subScEta) < 1.4442";
        
        etaLabels[1] = "Endcap-Endcap";
        etaFLabels[1] = "_EE";
        etaCuts[1] = " && abs(leadScEta) > 1.566 && abs(subScEta) > 1.566";
        
        
        etaLabels[2] = "Endcap-Barrel";
        etaFLabels[2] = "_BE";
        etaCuts[2] = " && ((abs(leadScEta) < 1.4442 && abs(subScEta) > 1.566) || (abs(leadScEta) > 1.566 && abs(subScEta) < 1.4442))";
    }
    
    if (etaSplit == false){
        nEta = 1;
        
        etaLabels[0] = "All Eta";
        etaFLabels[0] = "";
        etaCuts[0] = "";
    }

    
    string dirStr ="plots/1223/";


    int nTrees = 3;
    TTree *allTrees[3] = {tGJet,tGJet,tDiph};
    string allLabelsUncut[3] = {gjpLabelUncut,gjfLabelUncut,diphLabelUncut};
    string allLabelsCut[3] = {gjpLabelCut,gjfLabelCut,diphLabelCut};
    
    if(etaSplit == true) dirStr += "EtaSplit/";
    string outNameGen = dirStr + "Background";
    if(diphotonCuts == true)outNameGen += "_DiphotonCuts";
    if(diphotonCuts == false)outNameGen += "_SinglePhotonCuts";
    
    string massCutLoose = " && hggMass > 95";
    
    string kinCutLeadLoose = " && leadPt > 17 && leadPt/hggMass > 0.15";
    string kinCutSubLoose = " && subPt > 17 && subPt/hggMass > 0.15";
    
    string kinCutLeadTight = " && leadPt > 17 && leadPt/hggMass > 0.33";
    string kinCutSubTight = " && subPt > 17 && subPt/hggMass > 0.25";
    
    string preselLead = " && leadR9 > 0.5 && leadHadronicOverEm < 0.08 && !(abs(leadScEta) < 1.5 && leadR9 < 0.85 && (leadSigmaIetaIeta > 0.015 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0)) && !(abs(leadScEta) > 1.5 && leadR9 < 0.9 && (leadSigmaIetaIeta > 0.035 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0))";
    string preselSub =  " && subR9 > 0.5 && subHadronicOverEm < 0.08 && !(abs(subScEta) < 1.5 && subR9 < 0.85 && (subSigmaIetaIeta > 0.015 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0)) && !(abs(subScEta) > 1.5 && subR9 < 0.9 && (subSigmaIetaIeta > 0.035 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0))";
    
    for (int e = 0; e < nEta; e++){
        
        string cutStringLeadLoose,cutStringSubLoose,cutStringLeadTight,cutStringSubTight;
        
        if(diphotonCuts == true){
            cutStringLeadLoose = etaCuts[e] + massCutLoose + kinCutLeadLoose + kinCutSubLoose + ")*weight";
            cutStringSubLoose = etaCuts[e] + massCutLoose + kinCutSubLoose + kinCutLeadLoose + ")*weight";
            
            cutStringLeadTight = etaCuts[e] + massCutLoose + kinCutLeadTight + preselLead + kinCutSubTight + preselSub + ")*weight";
            cutStringSubTight = etaCuts[e] + massCutLoose + kinCutSubTight + preselSub + kinCutLeadTight + preselLead + ")*weight";
                        
        }
        if(diphotonCuts == false){
            cutStringLeadLoose = etaCuts[e] + massCutLoose + kinCutLeadLoose + ")*weight";
            cutStringSubLoose = etaCuts[e] + massCutLoose + kinCutSubLoose + ")*weight";
            
            cutStringLeadTight = etaCuts[e] + massCutLoose + kinCutLeadTight + preselLead + ")*weight";
            cutStringSubTight = etaCuts[e] + massCutLoose + kinCutSubTight + preselSub + ")*weight";
        }
        int nVars = 5;
        string varNames[5] = {"Pt","Pt/hggMass","hggMass","SigmaIetaIeta","IDMVA"};
        double limsLow[5] = {0.0,0.0,80.0,0.0,-1.1};
        double limsHigh[5] = {200,1.3,180,0.1,1.1};
        int nBins[5] = {400,520,400,400,440};
         
        int nEventsLoose, nEventsTight;
        
        TCanvas *can = new TCanvas ("can","can",10,10,1600,900);
        
        for(int i = 0; i < nVars; i++){
            TLegend *legend;
            if (varNames[i] == "IDMVA") legend = new TLegend(0.35,0.6,0.65,0.9,"","brNDC");
            else legend = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
            
            double maxVal = 0.0;
            int maxInt = -1;
            
            string plotTitleStr;
            if(diphotonCuts == true)plotTitleStr = varNames[i] + " Diphoton Cuts ";
            if(diphotonCuts == false)plotTitleStr = varNames[i] + " Single Photon Cuts ";
            string overallTitleStr = plotTitleStr + etaLabels[e]+ ";" + varNames[i];
            THStack *hStack = new THStack("hStack",overallTitleStr.c_str());
            
            string leadVarStr = "lead" + varNames[i];
            string subVarStr = "sub" + varNames[i];
                        
            if(varNames[i] == "hggMass"){
                leadVarStr = varNames[i];
                subVarStr = varNames[i];
            }
                               
            TH1F *hLoose[3];
            TH1F *hLooseSub[3];
            TH1F *hTight[3];
            TH1F *hTightSub[3];
            string hNamesLoose[3] = {"hLoose1","hLoose2","hLoose3"};
            string hNamesLooseSub[3] = {"hLooseSub1","hLooseSub2","hLooseSub3"};
            string hNamesTight[3] = {"hTight1","hTight2","hTight3"};
            string hNamesTightSub[3] = {"hTightSub1","hTightSub2","hTightSub3"};
           
            for(int j = 0; j < nTrees; j++){
                string cutStringLeadLooseInside;
                string cutStringLeadTightInside;
                
                string cutStringSubLooseInside;
                string cutStringSubTightInside;
                
                if(j == 0){
                    cutStringLeadLooseInside = "(leadGenMatchType == 1" + cutStringLeadLoose;
                    cutStringLeadTightInside = "(leadGenMatchType == 1" + cutStringLeadTight;
                    
                    cutStringSubLooseInside = "(subGenMatchType == 1" + cutStringSubLoose;
                    cutStringSubTightInside = "(subGenMatchType == 1" + cutStringSubTight;
                    
                    cutStringLeadLooseInside += "*1";
                    cutStringLeadTightInside += "*1";
                    
                    cutStringSubLooseInside += "*1";
                    cutStringSubTightInside += "*1";
                }
                if(j == 1){
                    cutStringLeadLooseInside = "(leadGenMatchType != 1" + cutStringLeadLooseFake;
                    cutStringLeadTightInside = "(leadGenMatchType != 1" + cutStringLeadTightFake;
                    
                    cutStringSubLooseInside = "(subGenMatchType != 1" + cutStringSubLooseFake;
                    cutStringSubTightInside = "(subGenMatchType != 1" + cutStringSubTightFake;
                    
                    cutStringLeadLooseInside += "*1";
                    cutStringLeadTightInside += "*1";
                    
                    cutStringSubLooseInside += "*1";
                    cutStringSubTightInside += "*1";
                }
                if(j == 2){
                    cutStringLeadLooseInside = "(leadGenMatchType == 1" + cutStringLeadLoose;
                    cutStringLeadTightInside = "(leadGenMatchType == 1" + cutStringLeadTight;
                    
                    cutStringSubLooseInside = "(subGenMatchType == 1" + cutStringSubLoose;
                    cutStringSubTightInside = "(subGenMatchType == 1" + cutStringSubTight;
                    
                    cutStringLeadLooseInside += "*1";
                    cutStringLeadTightInside += "*1";
                    
                    cutStringSubLooseInside += "*1";
                    cutStringSubTightInside += "*1";
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

                hLoose[j]->SetLineColor(j+1);
                hTight[j]->SetLineColor(j+1);
                hTight[j]->SetLineStyle(9);
                
                hStack->Add(hLoose[j]);
                hStack->Add(hTight[j]);
            }
            hStack->Draw("nostackhist");
            legend->Draw("same");
            can->SetGrid();
            if (varNames[i] == "IDMVA") can->SetLogy(1);
            else can->SetLogy(0);
            
            string outName;
            if (varNames[i] == "Pt/hggMass") outName = outNameGen + "_PtOvrM" + etaFLabels[e];
            else outName = outNameGen + "_" + varNames[i] + etaFLabels[e];
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
}
