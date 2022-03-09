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

void scoreCutCompBkg(bool etaSplit, bool diphotonCuts){
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
    string gjpLabelIDMVA = "GJet Prompt W/ idMVA > -0.99";
    string gjfLabelUncut = "GJet Fake";
    string gjfLabelCut = "GJet Fake W/ Presel";
    string gjfLabelIDMVA = "GJet Fake W/ idMVA > -0.99";
    
    string fileNameDiphoton = "../NTuples/GGH_And_Diphoton_M95PTM25_HovrE_DPT075_1129.root";
    TFile *fDiph = new TFile(fileNameDiphoton.c_str());
    TTree *tDiph = (TTree*)fDiph->Get("diphotons");
    string diphPLabelUncut = "Diphoton Prompt No Presel";
    string diphPLabelCut = "Diphoton Prompt W/ Presel";
    string diphPLabelIDMVA = "Diphoton Prompt W/ idMVA > -0.99";
    string diphFLabelUncut = "100*Diphoton Fake No Presel";
    string diphFLabelCut = "100*Diphoton Fake W/ Presel";
    string diphFLabelIDMVA = "100*Diphoton Fake W/ idMVA > -0.99";
    
    
    string dirStr ="plots/1229/BkgOnlyPromptFake/";
    
    
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

    int nTrees = 4;
    TTree *allTrees[4] = {tGJet,tGJet,tDiph,tDiph};
    string allLabelsUncut[4] = {gjpLabelUncut,gjfLabelUncut,diphPLabelUncut,diphFLabelUncut};
    string allLabelsCut[4] = {gjpLabelCut,gjfLabelCut,diphPLabelCut,diphFLabelCut};
    string allLabelsIDMVA[4] = {gjpLabelIDMVA,gjfLabelIDMVA,diphPLabelIDMVA,diphFLabelIDMVA};
    
    string idMVACutLead = " && leadIDMVA > - 0.99";
    string idMVACutSub = " && subIDMVA > - 0.99";
    
    if(etaSplit == true) dirStr += "EtaSplit/";
    string outNameGen = dirStr + "Background_idMVACut";
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
    
        string cutStringLeadLoose,cutStringSubLoose,cutStringLeadTight,cutStringSubTight,cutStringLeadIDMVA,cutStringSubIDMVA;
        if(diphotonCuts == true){
            cutStringLeadLoose = etaCuts[e] + massCutLoose + kinCutLeadLoose + kinCutSubLoose + ")*weight";
            cutStringSubLoose =  etaCuts[e] + massCutLoose + kinCutSubLoose + kinCutLeadLoose + ")*weight";
            
            cutStringLeadTight = etaCuts[e] + massCutLoose + kinCutLeadTight + preselLead + kinCutSubTight + preselSub + ")*weight";
            cutStringSubTight = etaCuts[e] + massCutLoose + kinCutSubTight + preselSub + kinCutLeadTight + preselLead + ")*weight";
            
            cutStringLeadIDMVA = etaCuts[e] + massCutLoose + kinCutLeadLoose + idMVACutLead + kinCutSubLoose + idMVACutSub + ")*weight";
            cutStringSubIDMVA = etaCuts[e] + massCutLoose + kinCutSubLoose + idMVACutSub + kinCutLeadLoose + idMVACutLead + ")*weight";
            
        }
        if(diphotonCuts == false){
            cutStringLeadLoose = etaCuts[e] + massCutLoose + kinCutLeadLoose + ")*weight";
            cutStringSubLoose = etaCuts[e] + massCutLoose + kinCutSubLoose + ")*weight";
            
            cutStringLeadTight = etaCuts[e] + massCutLoose + kinCutLeadTight + preselLead + ")*weight";
            cutStringSubTight = etaCuts[e] + massCutLoose + kinCutSubTight + preselSub + ")*weight";
            
            cutStringLeadIDMVA = etaCuts[e] + massCutLoose + kinCutLeadLoose + idMVACutLead + ")*weight";
            cutStringSubIDMVA = etaCuts[e] + massCutLoose + kinCutSubLoose + idMVACutSub + ")*weight";
            
        }
        
        int nVars = 20;
        string varNames[20] = {"Pt","Pt/hggMass","hggMass","SigmaIetaIeta","IDMVA","SCRawE","R9","EtaWidth","PhiWidth","CovIEtaIPhi","S4","PhoIso03","ChgIsoWrtChosenVtx","ChgIsoWrtWorstVtx","ScEta","rho","HadTowOverEm","HadronicOverEm","EsEffSigmaRR","EsEnergyOverRawE"};
        double limsLow[20] = {0.0,0.0,80.0,0.0,-1.1,0.0,0.0,0.0,0.0,-0.002,0.0,0.0,0.0,0.0,-5.0,0.0,0.0,0.0,0.0,0.0};
        double limsHigh[20] = {200,1.3,180,0.1,1.1,650,1.1,0.2,0.2,0.002,1.0,25.0,25.0,25.0,5.0,60.0,0.04,0.4,8.0,0.2};
        int nBins[20] = {400,520,400,400,440,650,440,400,400,400,400,500,500,500,400,600,400,400,400,400};
        int nEventsLoose, nEventsTight;
        
        TCanvas *can = new TCanvas ("can","can",10,10,1600,900);
        
        for(int i = 0; i < nVars; i++){
        //for(int i = 9; i < 10; i++){
        //for(int i = 16; i < 20; i++){
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
            
            if(varNames[i] == "hggMass" || varNames[i] == "rho"){
                leadVarStr = varNames[i];
                subVarStr = varNames[i];
            }
        
            
            TH1F *hLoose[4];
            TH1F *hLooseSub[4];
            TH1F *hTight[4];
            TH1F *hTightSub[4];
            TH1F *hIDMVA[4];
            TH1F *hIDMVASub[4];
            string hNamesLoose[4] = {"hLoose1","hLoose2","hLoose3","hLoose4"};
            string hNamesLooseSub[4] = {"hLooseSub1","hLooseSub2","hLooseSub3","hLooseSub4"};
            string hNamesTight[4] = {"hTight1","hTight2","hTight3","hTight4"};
            string hNamesTightSub[4] = {"hTightSub1","hTightSub2","hTightSub3","hTightSub4"};
            string hNamesIDMVA[4] = {"hidMVA1","hidMVA2","hidMVA3","hidMVA4"};
            string hNamesIDMVASub[4] = {"hidMVASub1","hidMVASub2","hidMVASub3","hidMVASub4"};
            for(int j = 0; j < nTrees; j++){
                string cutStringLeadLooseInside;
                string cutStringLeadTightInside;
                string cutStringLeadIDMVAInside;
                
                string cutStringSubLooseInside;
                string cutStringSubTightInside;
                string cutStringSubIDMVAInside;
                
                if(j == 0){
                    cutStringLeadLooseInside = "(leadGenMatchType == 1 && subGenMatchType != 1" + cutStringLeadLoose;
                    cutStringLeadTightInside = "(leadGenMatchType == 1 && subGenMatchType != 1" + cutStringLeadTight;
                    cutStringLeadIDMVAInside = "(leadGenMatchType == 1 && subGenMatchType != 1" + cutStringLeadIDMVA;
                    
                    cutStringSubLooseInside = "(subGenMatchType == 1 && leadGenMatchType != 1" + cutStringSubLoose;
                    cutStringSubTightInside = "(subGenMatchType == 1 && leadGenMatchType != 1" + cutStringSubTight;
                    cutStringSubIDMVAInside = "(subGenMatchType == 1 && leadGenMatchType != 1" + cutStringSubIDMVA;
                    
                    cutStringLeadLooseInside += "*1";
                    cutStringLeadTightInside += "*1";
                    cutStringLeadIDMVAInside += "*1";
                    
                    cutStringSubLooseInside += "*1";
                    cutStringSubTightInside += "*1";
                    cutStringSubIDMVAInside += "*1";
                }
                if(j == 1){
                    cutStringLeadLooseInside = "(leadGenMatchType != 1 && subGenMatchType == 1" + cutStringLeadLoose;
                    cutStringLeadTightInside = "(leadGenMatchType != 1 && subGenMatchType == 1" + cutStringLeadTight;
                    cutStringLeadIDMVAInside = "(leadGenMatchType != 1 && subGenMatchType == 1" + cutStringLeadIDMVA;
                    
                    cutStringSubLooseInside = "(subGenMatchType != 1 && leadGenMatchType == 1" + cutStringSubLoose;
                    cutStringSubTightInside = "(subGenMatchType != 1 && leadGenMatchType == 1" + cutStringSubTight;
                    cutStringSubIDMVAInside = "(subGenMatchType != 1 && leadGenMatchType == 1" + cutStringSubIDMVA;
                    
                    cutStringLeadLooseInside += "*1";
                    cutStringLeadTightInside += "*1";
                    cutStringLeadIDMVAInside += "*1";
                    
                    cutStringSubLooseInside += "*1";
                    cutStringSubTightInside += "*1";
                    cutStringSubIDMVAInside += "*1";
                }
                if(j == 2){
                    cutStringLeadLooseInside = "(leadGenMatchType == 1" + cutStringLeadLoose;
                    cutStringLeadTightInside = "(leadGenMatchType == 1" + cutStringLeadTight;
                    cutStringLeadIDMVAInside = "(leadGenMatchType == 1" + cutStringLeadIDMVA;
                    
                    cutStringSubLooseInside = "(subGenMatchType == 1" + cutStringSubLoose;
                    cutStringSubTightInside = "(subGenMatchType == 1" + cutStringSubTight;
                    cutStringSubIDMVAInside = "(subGenMatchType == 1" + cutStringSubIDMVA;
                    
                    cutStringLeadLooseInside += "*1";
                    cutStringLeadTightInside += "*1";
                    cutStringLeadIDMVAInside += "*1";
                    
                    cutStringSubLooseInside += "*1";
                    cutStringSubTightInside += "*1";
                    cutStringSubIDMVAInside += "*1";
                }
                if(j == 3){
                    cutStringLeadLooseInside = "(leadGenMatchType != 1" + cutStringLeadLoose;
                    cutStringLeadTightInside = "(leadGenMatchType != 1" + cutStringLeadTight;
                    cutStringLeadIDMVAInside = "(leadGenMatchType != 1" + cutStringLeadIDMVA;
                    
                    cutStringSubLooseInside = "(subGenMatchType != 1" + cutStringSubLoose;
                    cutStringSubTightInside = "(subGenMatchType != 1" + cutStringSubTight;
                    cutStringSubIDMVAInside = "(subGenMatchType != 1" + cutStringSubIDMVA;
                    
                    cutStringLeadLooseInside += "*100";
                    cutStringLeadTightInside += "*100";
                    cutStringLeadIDMVAInside += "*100";
                    
                    cutStringSubLooseInside += "*100";
                    cutStringSubTightInside += "*100";
                    cutStringSubIDMVAInside += "*100";
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
            if (varNames[i] == "IDMVA" || varNames[i] == "ChgIsoWrtChosenVtx" || varNames[i] == "HadTowOverEm" || varNames[i] == "HadronicOverEm" || varNames[i] == "EsEffSigmaRR" || varNames[i] == "EsEnergyOverRawE") can->SetLogy(1);
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
                hIDMVA[j]->Delete();
                hIDMVASub[j]->Delete();
            }
        }
    }
}


