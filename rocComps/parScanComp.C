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

void parScanComp(bool diphotonCuts, string etaRegion){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);
    string fileNames[10];
    
    int nFiles = 5;
    fileNames[0] = "../NTuples/GGH_And_GJets_M95PTM25_Old0916_0113.root";
    fileNames[1] = "../NTuples/GGH_And_GJets_M95PTM25_DPT075_HovrE_OnlyPFPairs_1223.root";
    fileNames[2] = "../NTuples/GGH_And_GJets_M95PTM15_DPT075_HovrE_OnlyPFPairs_0110.root";
    fileNames[3] = "../NTuples/GGH_And_GJets_M95PTM25_DPT075_HovrE_OnlyPFPairs_1223.root";
    
    string sigStr = "ggh_125";
    //string sigStr = "GJets";
    string bkgStr = "GJets";

    string parName = "LR";
    double parVals[5] = {0.1,0.2,0.3,0.4,0.5};
    string genLabels[10];
    genLabels[0] = " || TRAIN:M95, PTM25, Old";
    genLabels[1] = " || TRAIN:M95, PTM25, DPT075, 1 Prompt 1 Fake, New";
    genLabels[2] = " || TRAIN:M95, PTM15, DPT075, 1 Prompt 1 Fake, New";
    //genLabels[1] = " || TRAIN:M95, PTM25, DPT075, No Pt Reweight, Old ";
    //genLabels[2] = " || TRAIN:M95, PTM25, DPT075, 1 Prompt 1 Fake, No Pt Reweight, New";
    genLabels[3] = " || TRAIN:M95, PTM25, DPT075, 1 Prompt 1 Fake, New";
    
    string mvaOut = "curves/"+ etaRegion + "Photons_NewVsOldTMP_GGH_0117";
    if(diphotonCuts == true) mvaOut += "_Diphoton";
    if(diphotonCuts == false) mvaOut += "_SinglePhoton";
    
    string rocOut = mvaOut + "_ROC";
   
    TCanvas * can_RoC = new TCanvas ("can_RoC","can_RoC",10,10,1600,900);
    
    TLegend *legend_RoC = new TLegend(0.3,0.1,0.9,0.65,"","brNDC");
    
    string etaCutLead;
    string etaCutSub;
    string etaRegionStr;

    if (etaRegion == "Barrel" || etaRegion == "barrel") {
        etaCutLead = " && abs(leadScEta) < 1.4442";
        etaCutSub = " && abs(subScEta) < 1.4442";
        etaRegionStr = "Barrel";
    }
    else if(etaRegion == "Endcap" || etaRegion == "endcap"){
        etaCutLead = " && abs(leadScEta) > 1.556";
        etaCutSub = " && abs(subScEta) > 1.556";
        etaRegionStr = "Endcap";
    }
    else{
        etaCutLead = "";
        etaCutSub = "";
        etaRegionStr = "All";
        cout<<"Plotting for all scEta -- Unrecognized string (use 'Barrel' or 'Endcap')"<<endl;
    }
    //string weightCut = "&& weight > 2.0";
    //string weightCutStr = " Weight > 2.0";
    string weightCut = "";
    string weightCutStr = "";
    
    string massCutSig = " && hggMass > 121 && hggMass < 129";
    //string massCutSig = " && hggMass > 95 " + weightCut;
    string massCutBkg = " && hggMass > 95 " + weightCut;
    
    string kinCutLeadLoose = " && leadPt > 17 && leadPt/hggMass > 0.15";
    string kinCutSubLoose = " && subPt > 17 && subPt/hggMass > 0.15";
    
    string kinCutLeadTight = " && leadPt > 17 && leadPt/hggMass > 0.33";
    string kinCutSubTight = " && subPt > 17 && subPt/hggMass > 0.25";
    
    string preselLead = " && leadR9 > 0.5 && leadHadronicOverEm < 0.08 && !(abs(leadScEta) < 1.5 && leadR9 < 0.85 && (leadSigmaIetaIeta > 0.015 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0)) && !(abs(leadScEta) > 1.5 && leadR9 < 0.9 && (leadSigmaIetaIeta > 0.035 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0))";
    string preselSub =  " && subR9 > 0.5 && subHadronicOverEm < 0.08 && !(abs(subScEta) < 1.5 && subR9 < 0.85 && (subSigmaIetaIeta > 0.015 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0)) && !(abs(subScEta) > 1.5 && subR9 < 0.9 && (subSigmaIetaIeta > 0.035 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0))";
        
    string cutStringLeadSigLoose,cutStringSubSigLoose,cutStringLeadBkgLoose,cutStringSubBkgLoose,cutStringLeadSigTight,cutStringSubSigTight,cutStringLeadBkgTight,cutStringSubBkgTight;
    if(diphotonCuts == true){
        cutStringLeadSigLoose = "(leadGenMatchType == 1" + massCutSig + etaCutLead + kinCutLeadLoose + kinCutSubLoose + ")*weight";
        cutStringSubSigLoose = "(subGenMatchType == 1" + massCutSig + etaCutSub + kinCutSubLoose + kinCutLeadLoose + ")*weight";
        //cutStringLeadSigLoose = "(leadGenMatchType == 1 && subGenMatchType != 1" + massCutSig + etaCutLead + kinCutLeadLoose + kinCutSubLoose + ")*weight";
        //cutStringSubSigLoose = "(subGenMatchType == 1 && leadGenMatchType != 1" + massCutSig + etaCutSub + kinCutSubLoose + kinCutLeadLoose + ")*weight";
        
        cutStringLeadBkgLoose = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutBkg + etaCutLead + kinCutLeadLoose + kinCutSubLoose + ")*weight";
        cutStringSubBkgLoose = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutBkg + etaCutSub +kinCutSubLoose + kinCutLeadLoose + ")*weight";
    
        cutStringLeadSigTight = "(leadGenMatchType == 1" + massCutSig + etaCutLead + kinCutLeadTight + kinCutSubTight + preselLead + preselSub + ")*weight";
        cutStringSubSigTight = "(subGenMatchType == 1" + massCutSig + etaCutSub + kinCutSubTight + kinCutLeadTight + preselSub + preselLead + ")*weight";
        //cutStringLeadSigTight = "(leadGenMatchType == 1 && subGenMatchType != 1" + massCutSig + etaCutLead + kinCutLeadTight + kinCutSubTight + preselLead + preselSub + ")*weight";
        //cutStringSubSigTight = "(subGenMatchType == 1 && leadGenMatchType != 1" + massCutSig + etaCutSub + kinCutSubTight + kinCutLeadTight + preselSub + preselLead + ")*weight";
        
        cutStringLeadBkgTight = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutBkg + etaCutLead + kinCutLeadTight + kinCutSubTight + preselLead + preselSub + ")*weight";
        cutStringSubBkgTight = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutBkg + etaCutSub + kinCutSubTight + kinCutLeadTight + preselSub + preselLead + ")*weight";
    
    }
    if(diphotonCuts == false){
        cutStringLeadSigLoose = "(leadGenMatchType == 1" + massCutSig + etaCutLead + kinCutLeadLoose + ")*weight";
        cutStringSubSigLoose = "(subGenMatchType == 1" + massCutSig + etaCutSub + kinCutSubLoose + ")*weight";
        //cutStringLeadSigLoose = "(leadGenMatchType == 1 && subGenMatchType != 1" + massCutSig + etaCutLead + kinCutLeadLoose + ")*weight";
        //cutStringSubSigLoose = "(subGenMatchType == 1 && leadGenMatchType != 1" + massCutSig + etaCutSub + kinCutSubLoose + ")*weight";
        
        cutStringLeadBkgLoose = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutBkg + etaCutLead + kinCutLeadLoose + ")*weight";
        cutStringSubBkgLoose = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutBkg + etaCutSub + kinCutSubLoose + ")*weight";
        
        cutStringLeadSigTight = "(leadGenMatchType == 1" + massCutSig + etaCutLead + kinCutLeadTight + preselLead + ")*weight";
        cutStringSubSigTight = "(subGenMatchType == 1" + massCutSig + etaCutSub + kinCutSubTight + preselSub + ")*weight";
        //cutStringLeadSigTight = "(leadGenMatchType == 1 && subGenMatchType != 1" + massCutSig + etaCutLead + kinCutLeadTight + preselLead + ")*weight";
        //cutStringSubSigTight = "(subGenMatchType == 1 && leadGenMatchType != 1" + massCutSig + etaCutSub +kinCutSubTight + preselSub + ")*weight";
        
        cutStringLeadBkgTight = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutBkg + etaCutLead + kinCutLeadTight + preselLead + ")*weight";
        cutStringSubBkgTight = "(subGenMatchType != 1  && leadGenMatchType == 1" + massCutBkg + etaCutSub + kinCutSubTight + preselSub + ")*weight";

    }
    
    int nCuts = 320*100; //For development, faster but still pretty close to final (Change in 5th decimal place)
    for(int i = 0; i < nFiles; i++){
    //for(int i = 0; i < 3; i++){
        cout << "file # " << i << endl;
        
        TFile *fIn = new TFile(fileNames[i].c_str());
    
        TTree *sigTree = (TTree*)fIn->Get(sigStr.c_str());
        TH1F *hSigLoose = new TH1F ("hSigLoose","hSigLoose",nCuts,-1,1);
        TH1F *hSigLooseSub = new TH1F ("hSigLooseSub","hSigLooseSub",nCuts,-1,1);
        sigTree->Project("hSigLoose","leadIDMVA",cutStringLeadSigLoose.c_str());
        sigTree->Project("hSigLooseSub","subIDMVA",cutStringSubSigLoose.c_str());
        hSigLoose->Add(hSigLooseSub);
        
        TH1F *hSigTight = new TH1F ("hSigTight","hSigTight",nCuts,-1,1);
        TH1F *hSigTightSub = new TH1F ("hSigTightSub","hSigTightSub",nCuts,-1,1);
        sigTree->Project("hSigTight","leadIDMVA",cutStringLeadSigTight.c_str());
        sigTree->Project("hSigTightSub","subIDMVA",cutStringSubSigTight.c_str());
        hSigTight->Add(hSigTightSub);
        
        
        TTree *bkgTree = (TTree*)fIn->Get(bkgStr.c_str());
        TH1F *hBkgLoose = new TH1F ("hBkgLoose","hBkgLoose",nCuts,-1,1);
        TH1F *hBkgLooseSub = new TH1F ("hBkgLooseSub","hBkgLooseSub",nCuts,-1,1);
        bkgTree->Project("hBkgLoose","leadIDMVA",cutStringLeadBkgLoose.c_str());
        bkgTree->Project("hBkgLooseSub","subIDMVA",cutStringSubBkgLoose.c_str());
        hBkgLoose->Add(hBkgLooseSub);
        
        TH1F *hBkgTight = new TH1F ("hBkgTight","hBkgTight",nCuts,-1,1);
        TH1F *hBkgTightSub = new TH1F ("hBkgTightSub","hBkgTightSub",nCuts,-1,1);
        bkgTree->Project("hBkgTight","leadIDMVA",cutStringLeadBkgTight.c_str());
        bkgTree->Project("hBkgTightSub","subIDMVA",cutStringSubBkgTight.c_str());
        hBkgTight->Add(hBkgTightSub);
     
        
        
        char buff1[100], buff2[100], buff3[100], buff4[100];
        
        snprintf(buff1, sizeof(buff1), "(%0.0f)", hSigLoose->GetEntries());
        string nEventsSigLoose = buff1;
        
        snprintf(buff2, sizeof(buff2), "(%0.0f)", hSigTight->GetEntries());
        string nEventsSigTight = buff2;
        
        snprintf(buff3, sizeof(buff3), "(%0.0f)", hBkgLoose->GetEntries());
        string nEventsBkgLoose = buff3;
        
        snprintf(buff4, sizeof(buff4), "(%0.0f)", hBkgTight->GetEntries());
        string nEventsBkgTight = buff4;
    
        
        float stepVal = 2.0/nCuts;
        
        float NsigLoose[nCuts+1], NbkgLoose[nCuts+1];
        float NsigTight[nCuts+1], NbkgTight[nCuts+1];
        float sigEffLoose[nCuts+1], bkgEffLoose[nCuts+1];
        float sigEffTight[nCuts+1], bkgEffTight[nCuts+1];
        float cutsVal[nCuts+1];
        float mvaResCutVal = -1.0 - stepVal;
        
        int mvaSMaxBin = hSigLoose->GetXaxis()->FindBin(1) + 1;
        int mvaBMaxBin = hBkgLoose->GetXaxis()->FindBin(1) + 1;
        
        for(int k = 0; k < nCuts; k++){
            
            mvaResCutVal+= stepVal;
            cutsVal[k] = mvaResCutVal;
            
            int mvaBinSig = hSigLoose->GetXaxis()->FindBin(mvaResCutVal);
            NsigLoose[k] = hSigLoose->Integral(mvaBinSig,mvaSMaxBin);
            NsigTight[k] = hSigTight->Integral(mvaBinSig,mvaSMaxBin);
            
            int mvaBinBkg = hBkgLoose->GetXaxis()->FindBin(mvaResCutVal);
            NbkgLoose[k] = hBkgLoose->Integral(mvaBinBkg,mvaBMaxBin);
            NbkgTight[k] = hBkgTight->Integral(mvaBinBkg,mvaBMaxBin);
           
            sigEffLoose[k] = NsigLoose[k]/NsigLoose[0];
            sigEffTight[k] = NsigTight[k]/NsigLoose[0];
            
            bkgEffLoose[k] = NbkgLoose[k]/NbkgLoose[0];
            bkgEffTight[k] = NbkgTight[k]/NbkgLoose[0];
            
        }
        
        //TGraph * sigEff_vs_cut = new TGraph (nCuts, cutsVal, sigEff);
        //TGraph * bkgEff_vs_cut = new TGraph (nCuts, cutsVal, bkgEff);

        TGraph * sigEff_vs_bkgEff_Loose = new TGraph (nCuts, bkgEffLoose, sigEffLoose);
        TGraph * sigEff_vs_bkgEff_Tight = new TGraph (nCuts, bkgEffTight, sigEffTight);
        
        can_RoC->cd();

        if(i == 0){
            sigEff_vs_bkgEff_Loose->SetTitle("");
            sigEff_vs_bkgEff_Loose->GetYaxis()->SetTitleOffset(1.5);
            sigEff_vs_bkgEff_Loose->GetYaxis()->SetTitleSize(0.03);
            sigEff_vs_bkgEff_Loose->GetYaxis()->SetLabelSize(0.02);
            sigEff_vs_bkgEff_Loose->GetXaxis()->SetTitleOffset(1.5);
            sigEff_vs_bkgEff_Loose->GetXaxis()->SetTitleSize(0.03);
            sigEff_vs_bkgEff_Loose->GetXaxis()->SetLabelSize(0.02);
            sigEff_vs_bkgEff_Loose->GetYaxis()->SetTitle("signal eff");
            sigEff_vs_bkgEff_Loose->GetXaxis()->SetTitle("bkg eff");
            sigEff_vs_bkgEff_Loose->GetXaxis()->SetRangeUser(0.0,0.32);
        }
        
        sigEff_vs_bkgEff_Loose->SetLineWidth(2);
        sigEff_vs_bkgEff_Tight->SetLineWidth(2);
        sigEff_vs_bkgEff_Tight->SetLineStyle(9);
        
        
        sigEff_vs_bkgEff_Loose->SetLineColor(i+1);
        sigEff_vs_bkgEff_Loose->SetMarkerColor(i+1);
        sigEff_vs_bkgEff_Tight->SetLineColor(i+1);
        sigEff_vs_bkgEff_Tight->SetMarkerColor(i+1);
        
        can_RoC->SetGrid();
        
        string titleString = "GGH RoC Curves For " + etaRegionStr + "Photons" + weightCutStr;
        if(diphotonCuts == true) titleString += "W/ Diphoton Cuts";
        if(diphotonCuts == false) titleString += "W/ Single Photon Cuts";
        if(i != 10000){
            if(i == 0){
                sigEff_vs_bkgEff_Loose->Draw("AC");
                sigEff_vs_bkgEff_Loose->SetTitle(titleString.c_str());
            }
            else sigEff_vs_bkgEff_Loose->Draw("sameC");
            sigEff_vs_bkgEff_Tight->Draw("sameC");
        }
        TF1 f1("f1",[&](double *bkgEffLoose, double *sigEffLoose){ return sigEff_vs_bkgEff_Loose->Eval(bkgEffLoose[0]); },0,1,0);
        double AUCLoose = f1.Integral(0,1);
        stringstream streamLoose;
        streamLoose << fixed << setprecision(6) << AUCLoose;
        string AUCLooseStr = streamLoose.str();
        
        TF1 f2("f2",[&](double *bkgEffTight, double *sigEffTight){ return sigEff_vs_bkgEff_Tight->Eval(bkgEffTight[0]); },0,1,0);
        double AUCTight = f2.Integral(0,1);
        stringstream streamTight;
        streamTight << fixed << setprecision(6) << AUCTight;
        string AUCTightStr = streamTight.str();
        
        string legTitleLoose = "#splitline{VALIDATION: No Presel" + genLabels[i] + ", AUC = " + AUCLooseStr + "}{ NSignal = " + nEventsSigLoose + ", NBkg = " + nEventsBkgLoose + "}";
        string legTitleTight = "#splitline{VALIDATION: Presel" + genLabels[i] + ", AUC = " + AUCTightStr + "}{ NSignal = " + nEventsSigTight + ", NBkg = " + nEventsBkgTight + "}";
        if(i != 10000){
            legend_RoC->AddEntry(sigEff_vs_bkgEff_Loose,legTitleLoose.c_str(),"pl");
            legend_RoC->AddEntry(sigEff_vs_bkgEff_Tight,legTitleTight.c_str(),"pl");
        }
        can_RoC->SetGrid();
        can_RoC->Update();
        can_RoC->Modified();
        //}
    }
    legend_RoC->Draw("same");
    can_RoC->SaveAs((rocOut+".png").c_str());
    can_RoC->SaveAs((rocOut+".root").c_str());
}


