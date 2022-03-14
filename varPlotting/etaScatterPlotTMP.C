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

int findWidth(TH1F *histo, int centerBin);
int findMinWindow(TH1F *histo,int &centerBin);
double getExactWidth(TH1F *histo);

void etaScatterPlotTMP(bool etaSplit, bool diphotonCuts){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);
    
    string fileName = "../NTuples/GGH_And_GJets_M95PTM15_MD20LR045_DPT075_0202.root";
    TFile *f = new TFile(fileName.c_str());
    TTree *tGJet = (TTree*)f->Get("GJets");
    TTree *tGGH = (TTree*)f->Get("ggh_125");
    
    string gjpLabelUncut = "GJet Prompt";
    string gjpLabelPresel = "GJet Prompt Presel";
//    string gjpLabelIDMVA = "GJet Prompt W/ New idMVABarrel > -0.97735 & idMVAEndcap > 0.63704";
    string gjpLabelIDMVA = "GJet Prompt idMVA Cuts Matching Barrel Presel";
    string gjpLabelIDMVA2 = "GJet Prompt idMVA Cuts Matching 1/2 Barrel Presel";
    
    string gjfLabelUncut = "GJet Fake";
    string gjfLabelPresel = "GJet Fake Presel";
//    string gjfLabelIDMVA = "GJet Fake W/ New idMVABarrel > -0.97735 & idMVAEndcap > 0.63704";
    string gjfLabelIDMVA = "GJet Fake idMVA Cuts Matching Barrel Presel";
    string gjfLabelIDMVA2 = "GJet Fake idMVA Cuts Matching 1/2 Barrel Presel";
    
    string gghLabelUncut = "GGH";
    string gghLabelPresel = "GGH Presel";
//    string gghLabelIDMVA = "400*GGH W/ New idMVABarrel > -0.97735 & idMVAEndcap > 0.63704";
    string gghLabelIDMVA = "GGH idMVA Cuts Matching Barrel Presel";
    string gghLabelIDMVA2 = "GGH idMVA Cuts Matching 1/2 Barrel Presel";
    
    string diLabelUncut = "Diphoton Background";
    string diLabelPresel = "Diphoton Background Presel";
//    string diLabelIDMVA = "Diphoton Background W/ New idMVABarrel > -0.97735 & idMVAEndcap > 0.63704";
    string diLabelIDMVA = "Diphoton Background idMVA Cuts Matching Barrel Presel";
    string diLabelIDMVA2 = "Diphoton Background idMVA Cuts 1/2 Matching Barrel Presel";

    string dirStr ="plots/0309/";
    int nEta;
    string etaCuts[4];
    string etaLabels[4];
    string etaFLabels[4];
    
    if (etaSplit == true){
        nEta = 4;
        
        etaLabels[0] = "Barrel-Barrel";
        etaFLabels[0] = "_BB";
        etaCuts[0] = " && abs(leadScEta) < 1.4442 && abs(subScEta) < 1.4442";

        etaLabels[1] = "Barrel-Endcap";
        etaFLabels[1] = "_BE";
        etaCuts[1] = " && ((abs(leadScEta) > 1.556 && abs(subScEta) < 1.4442) || (abs(subScEta) > 1.556 && abs(leadScEta) < 1.4442))";
        
        etaLabels[2] = "Endcap-Endcap";
        etaFLabels[2] = "_EE";
        etaCuts[2] = " && abs(leadScEta) > 1.556 && abs(subScEta) > 1.556";
        
        etaLabels[3] = "All Eta";
        etaFLabels[3] = "_All";
        etaCuts[3] = "";
    }
    
    if (etaSplit == false){
        nEta = 1;
        
        etaLabels[0] = "All Eta";
        etaFLabels[0] = "";
        etaCuts[0] = "";
    }
    int nTrees = 8;
    TTree *allTrees[8] = {tGJet,tGJet,tGJet,tGJet,tGGH,tGGH,tGGH,tGGH};
    string allLabels[8] = {gjfLabelUncut,gjfLabelPresel,gjfLabelIDMVA,gjfLabelIDMVA2,gghLabelUncut,gghLabelPresel,gghLabelIDMVA,gghLabelIDMVA2};
    
    
    string outNameGen = dirStr + "EtaScatterPlot_GGHandGJet";
    if(diphotonCuts == true)outNameGen += "_DiphotonCuts";
    if(diphotonCuts == false)outNameGen += "_SinglePhotonCuts";
    
    
    string massCutTight = " && hggMass > 120 && hggMass < 128.5";
    string massCutLoose = " && hggMass > 95";
    
    string kinCutLeadLoose = " && leadPt > 17 && leadPt/hggMass > 0.15";
    string kinCutSubLoose = " && subPt > 17 && subPt/hggMass > 0.15";
    
//    string kinCutLeadTight = " && leadPt > 17 && leadPt/hggMass > 0.15";
//    string kinCutSubTight = " && subPt > 17 && subPt/hggMass > 0.15";
    

    string kinCutLeadTight = " && leadPt > 17 && leadPt/hggMass > 0.33";
    string kinCutSubTight = " && subPt > 17 && subPt/hggMass > 0.25";
//
    string idMVACutLead = " && !(abs(leadScEta) < 1.4442 && leadIDMVA < -0.99898) && !(abs(leadScEta) > 1.556 && leadIDMVA < -0.99898)";
    string idMVACutSub = " && !(abs(subScEta) < 1.4442 && subIDMVA < -0.99898) && !(abs(subScEta) > 1.556 && subIDMVA < -0.99898)";

    string idMVACutLead2 = " && !(abs(leadScEta) < 1.4442 && leadIDMVA < -0.75701) && !(abs(leadScEta) > 1.556 && leadIDMVA < -0.75701)";
    string idMVACutSub2 = " && !(abs(subScEta) < 1.4442 && subIDMVA < -0.75701) && !(abs(subScEta) > 1.556 && subIDMVA < -0.75701)";

    string preselLead = " && leadR9 > 0.5 && leadHadronicOverEm < 0.08 && !(abs(leadScEta) > 1.5 && leadR9 < 0.8) && !(abs(leadScEta) < 1.5 && leadR9 < 0.85 && (leadSigmaIetaIeta > 0.015 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0)) && !(abs(leadScEta) > 1.5 && leadR9 < 0.9 && (leadSigmaIetaIeta > 0.035 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0))";
    string preselSub =  " && subR9 > 0.5 && subHadronicOverEm < 0.08 && !(abs(subScEta) > 1.5 && subR9 < 0.8) && !(abs(subScEta) < 1.5 && subR9 < 0.85 && (subSigmaIetaIeta > 0.015 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0)) && !(abs(subScEta) > 1.5 && subR9 < 0.9 && (subSigmaIetaIeta > 0.035 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0))";
//    string preselLead = "";
//    string preselSub =  "";
    
    for (int e = 0; e < nEta; e++){
//    for (int e = nEta-1; e < nEta; e++){
        string cutStringLeadLoose,cutStringSubLoose,cutStringLeadTight,cutStringSubTight,cutStringLeadIDMVA,cutStringSubIDMVA,cutStringLeadIDMVA2,cutStringSubIDMVA2;
        if(diphotonCuts == true){
           
            cutStringLeadLoose = etaCuts[e] + kinCutLeadLoose + kinCutSubLoose + ")*weight";
            cutStringSubLoose =  etaCuts[e] + kinCutSubLoose + kinCutLeadLoose + ")*weight";
            
            cutStringLeadTight = etaCuts[e] + kinCutLeadTight + preselLead + kinCutSubTight + preselSub + ")*weight";
            cutStringSubTight =  etaCuts[e] + kinCutSubTight + preselSub + kinCutLeadTight + preselLead + ")*weight";
    
            cutStringLeadIDMVA = etaCuts[e] + kinCutLeadLoose + kinCutSubLoose;
            cutStringSubIDMVA = etaCuts[e] + kinCutSubLoose + kinCutLeadLoose;
            
            cutStringLeadIDMVA2 = etaCuts[e] + kinCutLeadLoose + kinCutSubLoose;
            cutStringSubIDMVA2 = etaCuts[e] + kinCutSubLoose + kinCutLeadLoose;

            
//            cutStringLeadIDMVA2 = etaCuts[e] + kinCutLeadLoose + kinCutSubLoose + " && !( leadPt > 17" + preselLead + preselSub + kinCutLeadTight + kinCutSubTight + ")";
//            cutStringSubIDMVA2 = etaCuts[e] + kinCutSubLoose + kinCutLeadLoose + "&& !( subPt > 17 " + preselSub + preselLead + kinCutSubTight + kinCutLeadTight +")";

        }
        if(diphotonCuts == false){
            cutStringLeadLoose = etaCuts[e] + kinCutLeadLoose + ")*weight";
            cutStringSubLoose = etaCuts[e] + kinCutSubLoose + ")*weight";
            
            cutStringLeadTight = etaCuts[e] + kinCutLeadTight + preselLead + ")*weight";
            cutStringSubTight =  etaCuts[e] + kinCutSubTight + preselSub + ")*weight";
            
            cutStringLeadIDMVA = etaCuts[e] + kinCutLeadLoose;
            cutStringSubIDMVA = etaCuts[e] + kinCutSubLoose;
            
            cutStringLeadIDMVA2 = etaCuts[e] + kinCutLeadLoose + "!( leadPt > 17 " + preselLead + kinCutLeadTight + ")";
            cutStringSubIDMVA2 = etaCuts[e] + kinCutSubLoose + "!( subPt > 17 " + preselSub + kinCutSubTight + ")";
            
        }
        
        int nEventsLoose, nEventsTight;
        
        TCanvas *can = new TCanvas ("can","can",10,10,1600,900);
        
//        for(int i = 0; i < nVars; i++){
//        for(int i = 0; i < 11; i++){
        string cutStringLeadIDMVAInside;
        string cutStringSubIDMVAInside;
        string cutStringLeadIDMVAInside2;
        string cutStringSubIDMVAInside2;
        
        string insideIDMVACutLead;
        string insideIDMVACutSub;
        string insideIDMVACutLead2;
        string insideIDMVACutSub2;
        
        if (diphotonCuts == false){
            insideIDMVACutLead = idMVACutLead + ")*weight";
            insideIDMVACutSub = idMVACutSub + ")*weight";
            insideIDMVACutLead2 = idMVACutLead2 + ")*weight";
            insideIDMVACutSub2 = idMVACutSub2 + ")*weight";
        }
        if (diphotonCuts == true){
            insideIDMVACutLead = idMVACutLead + idMVACutSub + ")*weight";
            insideIDMVACutSub = idMVACutSub + idMVACutLead + ")*weight";
            insideIDMVACutLead2 = idMVACutLead2 + idMVACutSub2 + ")*weight";
            insideIDMVACutSub2 = idMVACutSub2 + idMVACutLead2 + ")*weight";
        }
        
        cutStringLeadIDMVAInside = cutStringLeadIDMVA + insideIDMVACutLead;
        cutStringSubIDMVAInside = cutStringSubIDMVA + insideIDMVACutSub;
        cutStringLeadIDMVAInside2 = cutStringLeadIDMVA2 + insideIDMVACutLead2;
        cutStringSubIDMVAInside2 = cutStringSubIDMVA2 + insideIDMVACutSub2;
       
                
        double maxVal = 0.0;
        int maxInt = -1;
        
        string plotTitleStr;
        if(diphotonCuts == true)plotTitleStr = "Lead vs Sub Eta Diphoton Cuts ";
        string leadVarStr = "leadScEta";
        string subVarStr = "subScEta";
        
        string varStr = subVarStr + ":" + leadVarStr;
        
        TH2F *h[8];
        TH2F *hSub[8];
        string hNames[8] = {"hFUncut","hFPresel","hIDMVAF","hIDMVAF2","hGGHUncut","hGGHPresel","hGGHIDMVA","hGGHIDMVA2"};
        string hNamesSub[8] = {"hFUncutSub","hFPreselSub","hIDMVAFSub","hIDMVAFSub2","hGGHUncutSub","hGGHPreselSub","hGGHIDMVASub","hGGHIDMVASub2"};
        string outSuffix[8] = {"GJetUncut","GJetPresel","GJetIDMVA1","GJetIDMVA2","GGHUncut","GGHIDMVA1,GGHIDMVA2"};
        for(int j = 0; j < nTrees; j++){
            if(j == 0 || j == 4){
                string outName = outNameGen + outSuffix[j] + etaFLabels[e] + ".root";
                string cutStringLead;
                string cutStringSub;
                
                string plotTitleStrIn;
                if (j < 4)plotTitleStrIn = plotTitleStr + " GJet";
                if (j > 4)plotTitleStrIn = plotTitleStr + " GGH";
                if (j == 0){
                    cutStringLead = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + cutStringLeadLoose;
                    cutStringSub = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + cutStringSubLoose;
                }
                if (j == 1){
                    cutStringLead = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + cutStringLeadTight;
                    cutStringSub = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + cutStringSubTight;
                }
                if (j == 2){
                    cutStringLead = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + cutStringLeadIDMVAInside;
                    cutStringSub = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + cutStringSubIDMVAInside;
                }
                if (j == 3){
                    cutStringLead = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + cutStringLeadIDMVAInside2;
                    cutStringSub = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + cutStringSubIDMVAInside2;
                }
                
                if (j == 4){
                    cutStringLead = "(leadGenMatchType == 1 " + massCutLoose + cutStringLeadLoose;
                    cutStringSub = "(subGenMatchType == 1" + massCutLoose + cutStringSubLoose;
                }
                if (j == 5){
                    cutStringLead = "(leadGenMatchType == 1 " + massCutLoose + cutStringLeadTight;
                    cutStringSub = "(subGenMatchType == 1" + massCutLoose + cutStringSubTight;
                }
                if (j == 6){
                    cutStringLead = "(leadGenMatchType == 1 " + massCutLoose + cutStringLeadIDMVAInside;
                    cutStringSub = "(subGenMatchType == 1" + massCutLoose + cutStringSubIDMVAInside;
                }
                if (j == 7){
                    cutStringLead = "(leadGenMatchType == 1 " + massCutLoose + cutStringLeadIDMVAInside2;
                    cutStringSub = "(subGenMatchType == 1" + massCutLoose + cutStringSubIDMVAInside2;
                }
               
                h[j] = new TH2F (hNames[j].c_str(),"",300,-3.0,3.0,300,-3.0,3.0);
                hSub[j] = new TH2F (hNamesSub[j].c_str(),"",300,-3.0,3.0,300,-3.0,3.0);
                allTrees[j]->Project(hNames[j].c_str(),varStr.c_str(),cutStringLead.c_str());
                allTrees[j]->Project(hNamesSub[j].c_str(),varStr.c_str(),cutStringSub.c_str());
                h[j]->Add(hSub[j]);
                
                h[j]->SetTitle(plotTitleStrIn.c_str());
                h[j]->GetXaxis()->SetTitle("#eta_{lead}");
                h[j]->GetYaxis()->SetTitle("#eta_{sub}");
                h[j]->Draw("colz");
                can->Print(outName.c_str());
                can->Clear();
            }
        }

        for(int j = 0; j < nTrees; j++){
            if(j == 0 || j == 4){
                h[j]->Delete();
                hSub[j]->Delete();
            }
        }
    }
}
int findWidth(TH1F *histo, int centerBin){
    //    int maxBin = histo->GetMaximumBin();
    //    cout<<"Center = "<<histo->GetXaxis()->GetBinCenter(maxBin)<<endl;;
    //    cout<<"Center = "<<histo->GetXaxis()->GetBinCenter(centerBin)<<endl;;
    double integral = 0.0;
    double total = histo->Integral();
    int nBinsFromCenter = 0;
    while (integral < 0.68*total){
        nBinsFromCenter += 1;
        integral = histo->Integral(centerBin-nBinsFromCenter,centerBin+nBinsFromCenter);
    }
    int widthInBins = (nBinsFromCenter*2)+1;
    //    cout<<"widthInBins = "<<widthInBins<<endl;
    double widthInGeV = (histo->GetBinWidth(0))*widthInBins;
    //    cout<<"widthInGeV = "<<widthInGeV<<endl;
    
    //return widthInGeV;
    return widthInBins;
}
int findMinWindow(TH1F *histo,int &centerBin){
    int maxBin = histo->GetMaximumBin(); //Seed with maximum bin
    
    int nBinsShift[101];// = {-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10};
    for (int i = 0; i < 101; i++)nBinsShift[i] = -50 + i;
    int minNumBins = 100000;
    double minCenter = 0.0;
    double minWidth = 0.0;
    double percent = 0.0;
    
    for(int i = 0; i < 41; i++){
        int centerBinTmp = maxBin + nBinsShift[i];
        int nBinsTmp = findWidth(histo,centerBinTmp);
        if(nBinsTmp < minNumBins){
            minNumBins = nBinsTmp;
            minCenter = histo->GetXaxis()->GetBinCenter(centerBin);
            minWidth = (histo->GetBinWidth(0))*minNumBins;
            centerBin = centerBinTmp;
        }
        
    }
    //    cout<<"Min Width = "<<minWidth<<" at Center = "<<minCenter<<endl;
    
    //return minWidth;
    return minNumBins;
}
double getExactWidth(TH1F *histo){
    
    int centralBin;
    int nBins = findMinWindow(histo,centralBin);
    //int centralBin = histo->GetXaxis()->GetBinCenter(bestCenter);
    double total = histo->Integral();
    
    double propGoal = 0.68;
    int nFromCenter = (nBins-1)/2;
    //cout<<"nFromCenter = "<<nFromCenter<<endl;
    double currentProp = (histo->Integral(centralBin-nFromCenter,centralBin+nFromCenter))/total;
    //    cout<<"nBins = "<<nBins<<" currentProp = "<<currentProp<<endl;
    double diffProp = currentProp - propGoal;
    double diffSum = diffProp*total; //Total sum of weights that must be removed to get to 68%
    
    int higherBin;
    
    if(histo->GetBinContent(centralBin-(nBins/2)) > histo->GetBinContent(centralBin+(nBins/2))) higherBin = centralBin-(nBins/2);
    if(histo->GetBinContent(centralBin-(nBins/2)) < histo->GetBinContent(centralBin+(nBins/2))) higherBin = centralBin+(nBins/2);
    
    double contentOfLargerBin = histo->GetBinContent(higherBin);
    
    double propToDiscard = (diffSum/contentOfLargerBin);
    //    cout<<"prop of larger bin to discard = "<<propToDiscard<<endl;
    
    double differenceInWidth = propToDiscard*(histo->GetBinWidth(0)); //ASSUME CONSTANT DISTRIBUTION THROUGH BIN. ONLY INCLUDE FRACTION OF BIN WIDTH IN OVERALL WIDTH CALCULATION
                                                                      //    cout<<"differenceInWidth = "<<differenceInWidth<<endl;
    
    double widthInGeV = (histo->GetBinWidth(0))*nBins - differenceInWidth;
    double finalProp = (histo->Integral(centralBin-nFromCenter,centralBin+nFromCenter))/total - (propToDiscard*(histo->GetBinContent(higherBin)))/total;
    
    cout<<"Final Width = "<<widthInGeV<<" With proportion of events = "<<finalProp<<endl;
    
    return widthInGeV;
    
}

