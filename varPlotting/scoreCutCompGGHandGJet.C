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

<<<<<<< HEAD
void scoreCutCompGGHandGJetTMP(bool etaSplit, bool diphotonCuts){
=======
void scoreCutCompGGHandGJet(bool etaSplit, bool diphotonCuts){
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);

<<<<<<< HEAD
    string fileName1 = "../NTuples/GGH_And_GJets_M95PTM25_Old0916_0113.root";
    TFile *f1 = new TFile(fileName1.c_str());
    TTree *tGJet1 = (TTree*)f1->Get("GJets");
    TTree *tGGH1 = (TTree*)f1->Get("ggh_125");
    
    string fileName2 = "../NTuples/GGH_And_GJets_M95PTM15_MD20LR045_DPT075_0202.root";
    TFile *f2 = new TFile(fileName2.c_str());
    TTree *tGJet2 = (TTree*)f2->Get("GJets");
    TTree *tGGH2 = (TTree*)f2->Get("ggh_125");
    
    string gjpLabelUncut = "GJet Prompt";
    string gjpLabelPresel = "GJet Prompt Presel";
    string gjpLabelIDMVA1 = "GJet Prompt W/ Old idMVA > -0.99";
//    string gjpLabelIDMVA2 = "GJet Prompt W/ New idMVA > -0.997";
    string gjpLabelIDMVA2 = "GJet Prompt W/ New idMVABarrel > -0.99964 & idMVAEndcap > -0.998";
    
    string gjfLabelUncut = "GJet Fake";
    string gjfLabelPresel = "GJet Fake Presel";
    string gjfLabelIDMVA1 = "GJet Fake W/ Old idMVA > -0.99";
//    string gjfLabelIDMVA2 = "GJet Fake W/ New idMVA > -0.997";
    string gjfLabelIDMVA2 = "GJet Fake W/ New idMVABarrel > -0.99964 & idMVAEndcap > -0.998";
    
    string gghLabelUncut = "GGH";
    string gghLabelPresel = "GGH Presel";
    string gghLabelIDMVA1 = "GGH W/ Old idMVA > -0.99";
//    string gghLabelIDMVA2 = "GGH W/ New idMVA > -0.997";
    string gghLabelIDMVA2 = "GGH W/ New idMVABarrel > -0.99964 & idMVAEndcap > -0.998";

    string dirStr ="plots/0207/";
=======
    string fileName = "../NTuples/GGH_And_GJets_M95PTM25_DPT075_HovrE_OnlyPFPairs_1223.root";
    TFile *f1 = new TFile(fileName.c_str());
    TTree *tGJet = (TTree*)f1->Get("GJets");
    string gjpLabelUncut = "GJet Prompt";
    string gjpLabelCut = "GJet Prompt W/ Presel";
    string gjpLabelIDMVA = "GJet Prompt W/ New idMVABarrel > -0.95386 & idMVAEndcap > -0.85947";
    string gjfLabelUncut = "GJet Fake";
    string gjfLabelCut = "GJet Fake W/ Presel";
    string gjfLabelIDMVA = "GJet Fake W/ New idMVABarrel > -0.95386 & idMVAEndcap > -0.85947";
    
    TTree *tGGH = (TTree*)f1->Get("ggh_125");
    string gghLabelUncut = "400*GluGluH";
    string gghLabelCut = "400*GluGluH W/ Presel";
    string gghLabelIDMVA = "400*GluGluH Fake W/ New idMVABarrel > -0.95386 & idMVAEndcap > -0.85947";
    
    
    string fileName2 = "../NTuples/GGH_And_GJets_M95PTM25_DPT075_HHovrE_Test25Percent_FixSplit_1222.root";
    TFile *f2 = new TFile(fileName2.c_str());
    TTree *tGJet2 = (TTree*)f2->Get("GJets");
    TTree *tGGH2 = (TTree*)f2->Get("ggh_125");
    string gjpLabelIDMVA2 = "GJet Prompt W/ Old idMVA > -0.99";
    string gjfLabelIDMVA2 = "GJet Fake W/ Old idMVA > -0.99";
    string gghLabelIDMV2 = "400*GluGluH Fake W/ Old idMVA > -0.99";


    
    string dirStr ="plots/0110/GGHandGJetPromptFake/";
    
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
    int nEta;
    string etaCuts[3];
    string etaLabels[3];
    string etaFLabels[3];
    
    if (etaSplit == true){
        nEta = 3;
        
<<<<<<< HEAD
        etaLabels[0] = "Both Barrel";
        etaFLabels[0] = "_BB";
        etaCuts[0] = " && abs(leadScEta) < 1.4442 && abs(subScEta) < 1.4442";
        
        etaLabels[1] = "One or Both Endcap";
        etaFLabels[1] = "_EE";
        etaCuts[1] = " && (abs(leadScEta) > 1.556 || abs(subScEta) > 1.556)";
        
        etaLabels[2] = "All Eta";
        etaFLabels[2] = "_All";
        etaCuts[2] = "";
=======
        etaLabels[0] = "Barrel-Barrel";
        etaFLabels[0] = "_BB";
        etaCuts[0] = " && abs(leadScEta) < 1.4442 && abs(subScEta) < 1.4442";
        
        etaLabels[1] = "Endcap-Endcap";
        etaFLabels[1] = "_EE";
        etaCuts[1] = " && abs(leadScEta) > 1.566 && abs(subScEta) > 1.566";
        
        
        etaLabels[2] = "Endcap-Barrel";
        etaFLabels[2] = "_BE";
        etaCuts[2] = " && ((abs(leadScEta) < 1.4442 && abs(subScEta) > 1.566) || (abs(leadScEta) > 1.566 && abs(subScEta) < 1.4442))";
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
    }
    
    if (etaSplit == false){
        nEta = 1;
        
        etaLabels[0] = "All Eta";
        etaFLabels[0] = "";
        etaCuts[0] = "";
    }
<<<<<<< HEAD
    int nTrees = 12;
    TTree *allTrees[12] = {tGJet1,tGJet1,tGJet1,tGJet2,tGJet1,tGJet1,tGJet1,tGJet2,tGGH1,tGGH1,tGGH1,tGGH2};
    string allLabels[12] = {gjpLabelUncut,gjpLabelPresel,gjpLabelIDMVA1,gjpLabelIDMVA2,gjfLabelUncut,gjfLabelPresel,gjfLabelIDMVA1,gjfLabelIDMVA2,gghLabelUncut,gghLabelPresel,gghLabelIDMVA1,gghLabelIDMVA2};
    
    
    string outNameGen = dirStr + "GGHandGJet_NewVsOld_idMVACut";
    if(diphotonCuts == true)outNameGen += "_DiphotonCuts";
    if(diphotonCuts == false)outNameGen += "_SinglePhotonCuts";
    
    
    string massCutTight = " && hggMass > 121 && hggMass < 129";
    string massCutLoose = " && hggMass > 95";
    
    string kinCutLeadLoose = " && leadPt > 17 && leadPt/hggMass > 0.15";
    string kinCutSubLoose = " && subPt > 17 && subPt/hggMass > 0.15";

    string kinCutLeadTight = " && leadPt > 17 && leadPt/hggMass > 0.33";
    string kinCutSubTight = " && subPt > 17 && subPt/hggMass > 0.25";
    
    string idMVACutLead1 = "&& leadIDMVA > -0.99";
    string idMVACutSub1 = "&& subIDMVA > -0.99";
    
//    string idMVACutLead2 = "&& leadIDMVA > -0.997";
//    string idMVACutSub2 = "&& subIDMVA > -0.997";
    
    string idMVACutLead2 = " && !(abs(leadScEta) < 1.4442 && leadIDMVA < -0.99964) && !(abs(leadScEta) > 1.556 && leadIDMVA < -0.998)";
    string idMVACutSub2 = " && !(abs(subScEta) < 1.4442 && subIDMVA < -0.99964) && !(abs(subScEta) > 1.556 && subIDMVA < -0.998)";
    
=======

    int nTrees = 6;
    TTree *allTrees[6] = {tGJet,tGJet,tGJet2,tGJet2,tGGH,tGGH2};
    string allLabelsUncut[3] = {gjpLabelUncut,gjfLabelUncut,gghLabelUncut};
    string allLabelsCut[3] = {gjpLabelCut,gjfLabelCut,gghLabelCut};
    string allLabelsIDMVA[3] = {gjpLabelIDMVA,gjfLabelIDMVA,gghLabelIDMVA};
    string allLabelsIDMVA2[3] = {gjpLabelIDMVA2,gjfLabelIDMVA2,gghLabelIDMVA2};
    
    string idMVACutLead1 = " && !(abs(leadScEta) < 1.4442 && leadIDMVA < -0.95386) && !(abs(leadScEta) > 1.556 && leadIDMVA < -0.85947)";
    string idMVACutSub1 = " && !(abs(subScEta) < 1.4442 && subIDMVA < -0.95386) && !(abs(subScEta) > 1.556 && subIDMVA < -0.85947)";
   
    string idMVACutLead2 = "&& leadIDMVA > -0.99";
    string idMVACutSub2 = "&& subIDMVA > -0.99";
    
    if(etaSplit == true) dirStr += "EtaSplit/";
    string outNameGen = dirStr + "LowWeightGJet_GGHandGJet_idMVACut";
    if(diphotonCuts == true)outNameGen += "_DiphotonCuts";
    if(diphotonCuts == false)outNameGen += "_SinglePhotonCuts";
    
    string massCutTight = " && hggMass > 121 && hggMass < 129";
    string massCutLoose = " && hggMass > 95 && weight < 2.0";
    
    string kinCutLeadLoose = " && leadPt > 17 && leadPt/hggMass > 0.15";
    string kinCutSubLoose = " && subPt > 17 && subPt/hggMass > 0.15";
    
    string kinCutLeadTight = " && leadPt > 17 && leadPt/hggMass > 0.33";
    string kinCutSubTight = " && subPt > 17 && subPt/hggMass > 0.25";
    
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
    string preselLead = " && leadR9 > 0.5 && leadHadronicOverEm < 0.08 && !(abs(leadScEta) < 1.5 && leadR9 < 0.85 && (leadSigmaIetaIeta > 0.015 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0)) && !(abs(leadScEta) > 1.5 && leadR9 < 0.9 && (leadSigmaIetaIeta > 0.035 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0))";
    string preselSub =  " && subR9 > 0.5 && subHadronicOverEm < 0.08 && !(abs(subScEta) < 1.5 && subR9 < 0.85 && (subSigmaIetaIeta > 0.015 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0)) && !(abs(subScEta) > 1.5 && subR9 < 0.9 && (subSigmaIetaIeta > 0.035 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0))";
    
    for (int e = 0; e < nEta; e++){
<<<<<<< HEAD
        string cutStringLeadLoose,cutStringSubLoose,cutStringLeadTight,cutStringSubTight,cutStringLeadIDMVA1,cutStringSubIDMVA1,cutStringLeadIDMVA2,cutStringSubIDMVA2;
        if(diphotonCuts == true){
           
=======
    
        string cutStringLeadLoose,cutStringSubLoose,cutStringLeadTight,cutStringSubTight,cutStringLeadIDMVA,cutStringSubIDMVA,cutStringLeadIDMVA2,cutStringSubIDMVA2;
        if(diphotonCuts == true){
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
            cutStringLeadLoose = etaCuts[e] + kinCutLeadLoose + kinCutSubLoose + ")*weight";
            cutStringSubLoose =  etaCuts[e] + kinCutSubLoose + kinCutLeadLoose + ")*weight";
            
            cutStringLeadTight = etaCuts[e] + kinCutLeadTight + preselLead + kinCutSubTight + preselSub + ")*weight";
<<<<<<< HEAD
            cutStringSubTight =  etaCuts[e] + kinCutSubTight + preselSub + kinCutLeadTight + preselLead + ")*weight";
    
            cutStringLeadIDMVA1 = etaCuts[e] + kinCutLeadLoose + kinCutSubLoose;
            cutStringSubIDMVA1 = etaCuts[e] + kinCutSubLoose + kinCutLeadLoose;
            
            cutStringLeadIDMVA2 = etaCuts[e] + kinCutLeadLoose + kinCutSubLoose;
            cutStringSubIDMVA2 = etaCuts[e] + kinCutSubLoose + kinCutLeadLoose;
=======
            cutStringSubTight = etaCuts[e] + kinCutSubTight + preselSub + kinCutLeadTight + preselLead + ")*weight";
            
            cutStringLeadIDMVA = etaCuts[e] + kinCutLeadLoose + idMVACutLead + kinCutSubLoose + idMVACutSub1 + ")*weight";
            cutStringSubIDMVA = etaCuts[e] + kinCutSubLoose + idMVACutSub + kinCutLeadLoose + idMVACutLead1 + ")*weight";

            cutStringLeadIDMVA2 = etaCuts[e] + kinCutLeadLoose + idMVACutLead + kinCutSubLoose + idMVACutSub2 + ")*weight";
            cutStringSubIDMVA2 = etaCuts[e] + kinCutSubLoose + idMVACutSub + kinCutLeadLoose + idMVACutLead2 + ")*weight";
            
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
        }
        if(diphotonCuts == false){
            cutStringLeadLoose = etaCuts[e] + kinCutLeadLoose + ")*weight";
            cutStringSubLoose = etaCuts[e] + kinCutSubLoose + ")*weight";
            
            cutStringLeadTight = etaCuts[e] + kinCutLeadTight + preselLead + ")*weight";
<<<<<<< HEAD
            cutStringSubTight =  etaCuts[e] + kinCutSubTight + preselSub + ")*weight";
            
            cutStringLeadIDMVA1 = etaCuts[e] + kinCutLeadLoose;
            cutStringSubIDMVA1 = etaCuts[e] + kinCutSubLoose;
            
            cutStringLeadIDMVA2 = etaCuts[e] + kinCutLeadLoose;
            cutStringSubIDMVA2 = etaCuts[e] + kinCutSubLoose;
        }
        
        int nVars = 22;
        string varNames[22] = {"Pt","Pt/hggMass","ScEta","hggMass","SigmaIetaIeta","IDMVA","IDMVAZoom","weight","SCRawE","R9","EtaWidth","PhiWidth","CovIEtaIPhi","S4","PhoIso03","ChgIsoWrtChosenVtx","ChgIsoWrtWorstVtx","rho","HadTowOverEm","HadronicOverEm","EsEffSigmaRR","EsEnergyOverRawE"};
        double limsLow[22] = {0.0,0.0,-3.0,80.0,0.0,-1.1,-1.0,-1.0,0.0,0.0,0.0,0.0,-0.002,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
        double limsHigh[22] = {200,1.3,3.0,180,0.1,1.1,-0.95,5.0,650,1.1,0.2,0.2,0.002,1.0,25.0,25.0,25.0,60.0,0.4,1.0,8.0,0.2};
        int nBins[22] = {400,520,400,400,400,440,400,500,650,440,400,400,400,400,500,500,500,600,400,400,400,400};
=======
            cutStringSubTight = etaCuts[e] + kinCutSubTight + preselSub + ")*weight";
            
            cutStringLeadIDMVA = etaCuts[e] + kinCutLeadLoose + idMVACutLead1 + ")*weight";
            cutStringSubIDMVA = etaCuts[e] + kinCutSubLoose + idMVACutSub1 + ")*weight";
            
            cutStringLeadIDMVA2 = etaCuts[e] + kinCutLeadLoose + idMVACutLead2 + ")*weight";
            cutStringSubIDMVA2 = etaCuts[e] + kinCutSubLoose + idMVACutSub2 + ")*weight";
            
        }
        
        int nVars = 20;
        string varNames[20] = {"Pt","Pt/hggMass","hggMass","SigmaIetaIeta","IDMVA","SCRawE","R9","EtaWidth","PhiWidth","CovIEtaIPhi","S4","PhoIso03","ChgIsoWrtChosenVtx","ChgIsoWrtWorstVtx","ScEta","rho","HadTowOverEm","HadronicOverEm","EsEffSigmaRR","EsEnergyOverRawE"};
        double limsLow[20] = {0.0,0.0,80.0,0.0,-1.1,0.0,0.0,0.0,0.0,-0.002,0.0,0.0,0.0,0.0,-5.0,0.0,0.0,0.0,0.0,0.0};
        double limsHigh[20] = {200,1.3,180,0.1,1.1,650,1.1,0.2,0.2,0.002,1.0,25.0,25.0,25.0,5.0,60.0,0.04,0.4,8.0,0.2};
        int nBins[20] = {400,520,400,400,440,650,440,400,400,400,400,500,500,500,400,600,400,400,400,400};
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
        int nEventsLoose, nEventsTight;
        
        TCanvas *can = new TCanvas ("can","can",10,10,1600,900);
        
        for(int i = 0; i < nVars; i++){
<<<<<<< HEAD
//        for(int i = 6; i < 7; i++){
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
                if (diphotonCuts == false){
                    insideIDMVACutLead1 = idMVACutLead1 + ")*weight";
                    insideIDMVACutSub1 = idMVACutSub1 + ")*weight";
                    insideIDMVACutLead2 = idMVACutLead2 + ")*weight";
                    insideIDMVACutSub2 = idMVACutSub2 + ")*weight";
                }
                if (diphotonCuts == true){
                    insideIDMVACutLead1 = idMVACutLead1 + idMVACutSub1 + ")*weight";
                    insideIDMVACutSub1 = idMVACutSub1 + idMVACutLead1 + ")*weight";
                    insideIDMVACutLead2 = idMVACutLead2 + idMVACutSub2 + ")*weight";
                    insideIDMVACutSub2 = idMVACutSub2 + idMVACutLead2 + ")*weight";
                    
                }
            }
            cutStringLeadIDMVA1Inside = cutStringLeadIDMVA1 + insideIDMVACutLead1;
            cutStringSubIDMVA1Inside = cutStringSubIDMVA2 + insideIDMVACutSub1;
            cutStringLeadIDMVA2Inside = cutStringLeadIDMVA2 + insideIDMVACutLead2;
            cutStringSubIDMVA2Inside = cutStringSubIDMVA2 + insideIDMVACutSub2;
           
=======
        //for(int i = 9; i < 10; i++){
        //for(int i = 16; i < 20; i++){
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
            TLegend *legend;
            if (varNames[i] == "IDMVA") legend = new TLegend(0.35,0.6,0.65,0.9,"","brNDC");
            else legend = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
            
            double maxVal = 0.0;
            int maxInt = -1;
            
            string plotTitleStr;
            if(diphotonCuts == true)plotTitleStr = varNames[i] + " Diphoton Cuts ";
            if(diphotonCuts == false)plotTitleStr = varNames[i] + " Single Photon Cuts ";
<<<<<<< HEAD
            string overallTitleStr = plotTitleStr + etaLabels[e] + ";" + varNames[i];
=======
            string overallTitleStr = plotTitleStr + etaLabels[e]+ " Weight < 2.0;" + varNames[i];
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
            THStack *hStack = new THStack("hStack",overallTitleStr.c_str());
            
            string leadVarStr = "lead" + varNames[i];
            string subVarStr = "sub" + varNames[i];
            
<<<<<<< HEAD
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
                if (varNames[i] == "hggMass"){
                    if (j == 8){
                        cutStringLead = "(leadGenMatchType == 1 " + massCutLoose + cutStringLeadLoose + "*400";
                        cutStringSub = "(subGenMatchType == 1" + massCutLoose + cutStringSubLoose + "*400";
                    }
                    if (j == 9){
                        cutStringLead = "(leadGenMatchType == 1 " + massCutLoose + cutStringLeadTight + "*400";
                        cutStringSub = "(subGenMatchType == 1" + massCutLoose + cutStringSubTight + "*400";
                    }
                    if (j == 10){
                        cutStringLead = "(leadGenMatchType == 1 " + massCutLoose + cutStringLeadIDMVA1Inside + "*400";
                        cutStringSub = "(subGenMatchType == 1" + massCutLoose + cutStringSubIDMVA1Inside + "*400";
                    }
                    if (j == 11){
                        cutStringLead = "(leadGenMatchType == 1 " + massCutLoose + cutStringLeadIDMVA2Inside + "*400";
                        cutStringSub = "(subGenMatchType == 1" + massCutLoose + cutStringSubIDMVA2Inside + "*400";
                    }
                }
                else {
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
=======
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
            TH1F *hIDMVA2[4];
            TH1F *hIDMVASub2[4];
            string hNamesLoose[4] = {"hLoose1","hLoose2","hLoose3","hLoose4"};
            string hNamesLooseSub[4] = {"hLooseSub1","hLooseSub2","hLooseSub3","hLooseSub4"};
            string hNamesTight[4] = {"hTight1","hTight2","hTight3","hTight4"};
            string hNamesTightSub[4] = {"hTightSub1","hTightSub2","hTightSub3","hTightSub4"};
            string hNamesIDMVA[4] = {"hidMVA1","hidMVA2","hidMVA3","hidMVA4"};
            string hNamesIDMVASub[4] = {"hidMVASub1","hidMVASub2","hidMVASub3","hidMVASub4"};
            string hNamesIDMVA2[4] = {"hidMVA21","hidMVA22","hidMVA23","hidMVA24"};
            string hNamesIDMVASub2[4] = {"hidMVASub21","hidMVASub22","hidMVASub23","hidMVASub24"};
            for(int j = 0; j < nTrees; j++){
                string cutStringLeadLooseInside;
                string cutStringLeadTightInside;
                string cutStringLeadIDMVAInside;
                
                string cutStringSubLooseInside;
                string cutStringSubTightInside;
                string cutStringSubIDMVAInside;
                
                if(j == 0){
                    cutStringLeadLooseInside = "(leadGenMatchType == 1 && subGenMatchType != 1" + massCutLoose + cutStringLeadLoose;
                    cutStringLeadTightInside = "(leadGenMatchType == 1 && subGenMatchType != 1" + massCutLoose +cutStringLeadTight;
                    cutStringLeadIDMVAInside = "(leadGenMatchType == 1 && subGenMatchType != 1" + massCutLoose + cutStringLeadIDMVA;
                    
                    cutStringSubLooseInside = "(subGenMatchType == 1 && leadGenMatchType != 1" + massCutLoose + cutStringSubLoose;
                    cutStringSubTightInside = "(subGenMatchType == 1 && leadGenMatchType != 1" + massCutLoose + cutStringSubTight;
                    cutStringSubIDMVAInside = "(subGenMatchType == 1 && leadGenMatchType != 1" + massCutLoose + cutStringSubIDMVA;
                    
                    cutStringLeadLooseInside += "*1";
                    cutStringLeadTightInside += "*1";
                    cutStringLeadIDMVAInside += "*1";
                    
                    cutStringSubLooseInside += "*1";
                    cutStringSubTightInside += "*1";
                    cutStringSubIDMVAInside += "*1";
                }
                if(j == 1){
                    cutStringLeadLooseInside = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + cutStringLeadLoose;
                    cutStringLeadTightInside = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + cutStringLeadTight;
                    cutStringLeadIDMVAInside = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + cutStringLeadIDMVA;
                    
                    cutStringSubLooseInside = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + cutStringSubLoose;
                    cutStringSubTightInside = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + cutStringSubTight;
                    cutStringSubIDMVAInside = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + cutStringSubIDMVA;
                    
                    cutStringLeadLooseInside += "*1";
                    cutStringLeadTightInside += "*1";
                    cutStringLeadIDMVAInside += "*1";
                    
                    cutStringSubLooseInside += "*1";
                    cutStringSubTightInside += "*1";
                    cutStringSubIDMVAInside += "*1";
                if(j == 2){
                    cutStringLeadLooseInside = "(leadGenMatchType == 1 && subGenMatchType != 1" + massCutLoose + cutStringLeadLoose;
                    cutStringLeadTightInside = "(leadGenMatchType == 1 && subGenMatchType != 1" + massCutLoose +cutStringLeadTight;
                    cutStringLeadIDMVAInside = "(leadGenMatchType == 1 && subGenMatchType != 1" + massCutLoose + cutStringLeadIDMVA2;
                    
                    cutStringSubLooseInside = "(subGenMatchType == 1 && leadGenMatchType != 1" + massCutLoose + cutStringSubLoose;
                    cutStringSubTightInside = "(subGenMatchType == 1 && leadGenMatchType != 1" + massCutLoose + cutStringSubTight;
                    cutStringSubIDMVAInside = "(subGenMatchType == 1 && leadGenMatchType != 1" + massCutLoose + cutStringSubIDMVA2;
                    
                    cutStringLeadLooseInside += "*1";
                    cutStringLeadTightInside += "*1";
                    cutStringLeadIDMVAInside += "*1";
                    
                    cutStringSubLooseInside += "*1";
                    cutStringSubTightInside += "*1";
                    cutStringSubIDMVAInside += "*1";
                }
                if(j == 3){
                    cutStringLeadLooseInside = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + cutStringLeadLoose;
                    cutStringLeadTightInside = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + cutStringLeadTight;
                    cutStringLeadIDMVAInside = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + cutStringLeadIDMVA;
                    
                    cutStringSubLooseInside = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + cutStringSubLoose;
                    cutStringSubTightInside = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + cutStringSubTight;
                    cutStringSubIDMVAInside = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + cutStringSubIDMVA;
                    
                    cutStringLeadLooseInside += "*1";
                    cutStringLeadTightInside += "*1";
                    cutStringLeadIDMVAInside += "*1";
                    
                    cutStringSubLooseInside += "*1";
                    cutStringSubTightInside += "*1";
                    cutStringSubIDMVAInside += "*1";
                }
                if(j == 4){
                    cutStringLeadLooseInside = "(leadGenMatchType == 1" + massCutTight + cutStringLeadLoose;
                    cutStringLeadTightInside = "(leadGenMatchType == 1" + massCutTight + cutStringLeadTight;
                    cutStringLeadIDMVAInside = "(leadGenMatchType == 1" + massCutTight + cutStringLeadIDMVA;
                    
                    cutStringSubLooseInside = "(subGenMatchType == 1" + massCutTight + cutStringSubLoose;
                    cutStringSubTightInside = "(subGenMatchType == 1" + massCutTight + cutStringSubTight;
                    cutStringSubIDMVAInside = "(subGenMatchType == 1" + massCutTight + cutStringSubIDMVA;
                    
                    cutStringLeadLooseInside += "*400";
                    cutStringLeadTightInside += "*400";
                    cutStringLeadIDMVAInside += "*400";
                    
                    cutStringSubLooseInside += "*400";
                    cutStringSubTightInside += "*400";
                    cutStringSubIDMVAInside += "*400";
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
                
                hLoose[j]->SetLineColor(j+1);
                hTight[j]->SetLineColor(j+1);
                hIDMVA[j]->SetLineColor(j+1);
                hTight[j]->SetLineStyle(9);
                hIDMVA[j]->SetLineStyle(3);
                
                hStack->Add(hLoose[j]);
                hStack->Add(hTight[j]);
                hStack->Add(hIDMVA[j]);
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
            }
            hStack->Draw("nostackhist");
            legend->Draw("same");
            can->SetGrid();
<<<<<<< HEAD
            if (varNames[i] == "IDMVA" || varNames[i] == "IDMVAZoom" || varNames[i] == "ChgIsoWrtChosenVtx" || varNames[i] == "HadTowOverEm" || varNames[i] == "HadronicOverEm" || varNames[i] == "EsEffSigmaRR" || varNames[i] == "EsEnergyOverRawE") can->SetLogy(1);
=======
            if (varNames[i] == "IDMVA" || varNames[i] == "ChgIsoWrtChosenVtx" || varNames[i] == "HadTowOverEm" || varNames[i] == "HadronicOverEm" || varNames[i] == "EsEffSigmaRR" || varNames[i] == "EsEnergyOverRawE") can->SetLogy(1);
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
            else can->SetLogy(0);
            
            string outName;
            if (varNames[i] == "Pt/hggMass") outName = outNameGen + "_PtOvrM" + etaFLabels[e];
            else outName = outNameGen + "_" + varNames[i] + etaFLabels[e];
            can->SaveAs((outName+".png").c_str());
            can->SaveAs((outName+".root").c_str());
            
            if (i != nVars-1)can->Clear();
            
            for(int j = 0; j < nTrees; j++){
<<<<<<< HEAD
                h[j]->Delete();
                hSub[j]->Delete();
=======
                hLoose[j]->Delete();
                hLooseSub[j]->Delete();
                hTight[j]->Delete();
                hTightSub[j]->Delete();
                hIDMVA[j]->Delete();
                hIDMVASub[j]->Delete();
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
            }
        }
    }
}


