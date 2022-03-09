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

void findIDMVACut(bool etaSplit, bool diphotonCuts){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);
    
    int nBins = 2000;

    string fileNameGJet = "../NTuples/GGH_And_GJet_M95PTM25_HovrE_DPT075_1118.root";
    TFile *fGJet = new TFile(fileNameGJet.c_str());
    TTree *tGJet = (TTree*)fGJet->Get("GJets");
    
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
   
    string outNameGen = "plots/idMVACutLocation/1215";
    if(diphotonCuts == true)outNameGen += "_DiphotonCuts";
    if(diphotonCuts == false)outNameGen += "_SinglePhotonCuts";
    
    string massCut = " && hggMass > 95";
    
    string kinCutLeadLoose = " && leadPt > 17 && leadPt/hggMass > 0.15";
    string kinCutSubLoose = " && subPt > 17 && subPt/hggMass > 0.15";
    
    string kinCutLeadTight = " && leadPt > 17 && leadPt/hggMass > 0.33";
    string kinCutSubTight = " && subPt > 17 && subPt/hggMass > 0.25";
    
    string preselLead = " && leadR9 > 0.5 && leadHadronicOverEm < 0.08 && !(abs(leadScEta) < 1.5 && leadR9 < 0.85 && (leadSigmaIetaIeta > 0.015 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0)) && !(abs(leadScEta) > 1.5 && leadR9 < 0.9 && (leadSigmaIetaIeta > 0.035 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0))";
    string preselSub =  " && subR9 > 0.5 && subHadronicOverEm < 0.08 && !(abs(subScEta) < 1.5 && subR9 < 0.85 && (subSigmaIetaIeta > 0.015 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0)) && !(abs(subScEta) > 1.5 && subR9 < 0.9 && (subSigmaIetaIeta > 0.035 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0))";
    
    for (int e = 0; e < nEta; e++){
        
        string cutStringLeadUncut,cutStringSubUncut,cutStringLeadPresel,cutStringSubPresel,cutStringLeadIDMVA,cutStringSubIDMVA;
        
        if(diphotonCuts == true){
            cutStringLeadPresel = "(leadGenMatchType != 1" + etaCuts[e] + massCut + kinCutLeadTight + preselLead + kinCutSubTight + preselSub + ")*weight";
            cutStringSubPresel = "(subGenMatchType != 1" + etaCuts[e] + massCut + kinCutSubTight + preselSub + kinCutLeadTight + preselLead + ")*weight";
            cutStringLeadUncut = "(leadGenMatchType != 1" + etaCuts[e] + massCut + kinCutLeadLoose + kinCutSubLoose + ")*weight";
            cutStringSubUncut = "(subGenMatchType != 1" + etaCuts[e] + massCut + kinCutSubLoose + kinCutLeadLoose + ")*weight";
            cutStringLeadIDMVA = "(leadGenMatchType != 1" + etaCuts[e] + massCut + kinCutLeadLoose + kinCutSubLoose;
            cutStringSubIDMVA = "(subGenMatchType != 1" + etaCuts[e] + massCut + kinCutSubLoose + kinCutLeadLoose;
        }
        if(diphotonCuts == false){
            cutStringLeadPresel = "(leadGenMatchType != 1" + etaCuts[e] + massCut + kinCutLeadTight + preselLead + ")*weight";
            cutStringSubPresel = "(subGenMatchType != 1" + etaCuts[e] + massCut + kinCutSubTight + preselSub + ")*weight";
            cutStringLeadUncut = "(leadGenMatchType != 1" + etaCuts[e] + massCut + kinCutLeadLoose + ")*weight";
            cutStringSubUncut = "(subGenMatchType != 1" + etaCuts[e] + massCut + kinCutSubLoose + ")*weight";
            cutStringLeadIDMVA = "(leadGenMatchType != 1" + etaCuts[e] + massCut + kinCutLeadLoose;
            cutStringSubIDMVA = "(subGenMatchType != 1" + etaCuts[e] + massCut + kinCutSubLoose;// + ")*weight";
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
        tGJet->Project("hUncut","leadIDMVA",cutStringLeadUncut.c_str());
        tGJet->Project("hUncutSub","subIDMVA",cutStringSubUncut.c_str());
        hUncut->Add(hUncutSub);
            
        int nEventsUncut = hUncut->GetEntries();
        double totalInt = hUncut->Integral();
        
        char buff1[100];
        snprintf(buff1, sizeof(buff1), "(%0.2d", nEventsUncut);
        string nUncutStr = buff1;
        string labelUncut = "GJet Fake Uncut" + (nUncutStr + ")");
        legend->AddEntry(hUncut,labelUncut.c_str(),"pl");
        
        TH1F *hPresel = new TH1F ("hPresel","",nBins,-1.0,1.0);
        TH1F *hPreselSub = new TH1F ("hPreselSub","",nBins,-1.0,1.0);
        tGJet->Project("hPresel","leadIDMVA",cutStringLeadPresel.c_str());
        tGJet->Project("hPreselSub","subIDMVA",cutStringSubPresel.c_str());
        hPresel->Add(hPreselSub);
                
        int nEventsPresel = hPresel->GetEntries();
        double propPresel = hPresel->Integral()/totalInt;
        
        char buff2[100],buff3[100];
        snprintf(buff2, sizeof(buff2), "(%0.2d", nEventsPresel);
        snprintf(buff3, sizeof(buff3), "(%0.2f", 100*propPresel);
        string nPreselStr = buff2;
        string propPreselStr = buff3;
        string labelCut = "GJet Fake Presel Cut" + (nPreselStr + ")") + (propPreselStr + "%)");
        legend->AddEntry(hPresel,labelCut.c_str(),"pl");

        int nCuts = nBins;
        double cutStep = 2.0/nCuts;
        double cutVal = -1.0 - cutStep;
        
        double propIDMVACut;
        double equivalentIDMVACut;
        
        int mvaMaxBin = hUncut->GetXaxis()->FindBin(1) + 1;
        
        for(int i = 0; i < nCuts; i++){
            cutVal+= cutStep;
            
            int mvaBin = hUncut->GetXaxis()->FindBin(cutVal);
            double tmpIDMVAInt = hUncut->Integral(mvaBin,mvaMaxBin);
            
            propIDMVACut = tmpIDMVAInt/totalInt;
            if(propIDMVACut <= propPresel)break;
        }
        cout<<"PropPresel = "<<propPresel<<endl;
        cout<<"PropIDMVA = "<<propIDMVACut<<endl;
        cout<<"cutVal = "<<cutVal<<endl;
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
//        hIDMVA->Delete();
//        hIDMVASub->Delete();
//        hPresel->Delete();
//        hPreselSub->Delete();
    }
}
