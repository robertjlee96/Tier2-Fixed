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

void preselCompGGHandGJet(bool etaSplit, bool diphotonCuts){
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
    string gjpLabelIDMVA = "GJet Prompt W/ New idMVABarrel > -0.98 & idMVAEndcap > -0.98";
    string gjpLabelIDMVA2 = "GJet Prompt Passing idMVA Failing Presel";
    
    string gjfLabelUncut = "GJet Fake";
    string gjfLabelPresel = "GJet Fake Presel";
    string gjfLabelIDMVA = "GJet Fake W/ New idMVABarrel > -0.98 & idMVAEndcap > -0.98";
    string gjfLabelIDMVA2 = "GJet Fake Passing idMVA Failing Presel";
    
    string gghLabelUncut = "400*GGH";
    string gghLabelPresel = "400*GGH Presel";
    string gghLabelIDMVA = "400*GGH W/ New idMVABarrel > -0.98 & idMVAEndcap > -0.98";
    string gghLabelIDMVA2 = "400*GGH Passing idMVA but Failing Presel";

    string dirStr ="plots/0215/";
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
    int nTrees = 12;
    TTree *allTrees[12] = {tGJet,tGJet,tGJet,tGJet,tGJet,tGJet,tGJet,tGJet,tGGH,tGGH,tGGH,tGGH};
    string allLabels[12] = {gjpLabelUncut,gjpLabelPresel,gjpLabelIDMVA,gjpLabelIDMVA2,gjfLabelUncut,gjfLabelPresel,gjfLabelIDMVA,gjfLabelIDMVA2,gghLabelUncut,gghLabelPresel,gghLabelIDMVA,gghLabelIDMVA2};
    
    
    string outNameGen = dirStr + "GGHandGJet_PreselComp_idMVACut";
    if(diphotonCuts == true)outNameGen += "_DiphotonCuts";
    if(diphotonCuts == false)outNameGen += "_SinglePhotonCuts";
    
    
    string massCutTight = " && hggMass > 120 && hggMass < 128.5";
    string massCutLoose = " && hggMass > 95";
    
    string kinCutLeadLoose = " && leadPt > 17 && leadPt/hggMass > 0.15";
    string kinCutSubLoose = " && subPt > 17 && subPt/hggMass > 0.15";

    string kinCutLeadTight = " && leadPt > 17 && leadPt/hggMass > 0.33";
    string kinCutSubTight = " && subPt > 17 && subPt/hggMass > 0.25";
    
    //string idMVACutLead = " && !(abs(leadScEta) < 1.4442 && leadIDMVA < -0.9999) && !(abs(leadScEta) > 1.556 && leadIDMVA < -0.95343)";
    //string idMVACutSub = " && !(abs(subScEta) < 1.4442 && subIDMVA < -0.9999) && !(abs(subScEta) > 1.556 && subIDMVA < -0.95343)";

    string idMVACutLead = " && !(abs(leadScEta) < 1.4442 && leadIDMVA < -0.98) && !(abs(leadScEta) > 1.556 && leadIDMVA < -0.98)";
    string idMVACutSub = " && !(abs(subScEta) < 1.4442 && subIDMVA < -0.98) && !(abs(subScEta) > 1.556 && subIDMVA < -0.98)";

    string preselLead = " && leadR9 > 0.5 && leadHadronicOverEm < 0.08 && !(abs(leadScEta) > 1.5 && leadR9 < 0.8) && !(abs(leadScEta) < 1.5 && leadR9 < 0.85 && (leadSigmaIetaIeta > 0.015 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0)) && !(abs(leadScEta) > 1.5 && leadR9 < 0.9 && (leadSigmaIetaIeta > 0.035 || leadPfPhoIso03 > 4.0 || leadTrkSumPtHollowConeDR03 > 6.0))";
    string preselSub =  " && subR9 > 0.5 && subHadronicOverEm < 0.08 && !(abs(subScEta) > 1.5 && subR9 < 0.8) && !(abs(subScEta) < 1.5 && subR9 < 0.85 && (subSigmaIetaIeta > 0.015 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0)) && !(abs(subScEta) > 1.5 && subR9 < 0.9 && (subSigmaIetaIeta > 0.035 || subPfPhoIso03 > 4.0 || subTrkSumPtHollowConeDR03 > 6.0))";
    
    for (int e = 0; e < nEta; e++){
        string cutStringLeadLoose,cutStringSubLoose,cutStringLeadTight,cutStringSubTight,cutStringLeadIDMVA,cutStringSubIDMVA,cutStringLeadIDMVA2,cutStringSubIDMVA2;
        if(diphotonCuts == true){
           
            cutStringLeadLoose = etaCuts[e] + kinCutLeadLoose + kinCutSubLoose + ")*weight";
            cutStringSubLoose =  etaCuts[e] + kinCutSubLoose + kinCutLeadLoose + ")*weight";
            
            cutStringLeadTight = etaCuts[e] + kinCutLeadTight + preselLead + kinCutSubTight + preselSub + ")*weight";
            cutStringSubTight =  etaCuts[e] + kinCutSubTight + preselSub + kinCutLeadTight + preselLead + ")*weight";
    
            cutStringLeadIDMVA = etaCuts[e] + kinCutLeadLoose + kinCutSubLoose;
            cutStringSubIDMVA = etaCuts[e] + kinCutSubLoose + kinCutLeadLoose;
            
            cutStringLeadIDMVA2 = etaCuts[e] + kinCutLeadLoose + kinCutSubLoose + " && !( leadPt > 17" + preselLead + preselSub + kinCutLeadTight + kinCutSubTight + ")";
            cutStringSubIDMVA2 = etaCuts[e] + kinCutSubLoose + kinCutLeadLoose + "&& !( subPt > 17 " + preselSub + preselLead + kinCutSubTight + kinCutLeadTight +")";

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
        
        int nVars = 23;
        string varNames[23] = {"Pt","Pt/hggMass","ScEta","hggMass","hggMassCut","SigmaIetaIeta","IDMVA","IDMVAZoom","weight","SCRawE","R9","EtaWidth","PhiWidth","CovIEtaIPhi","S4","PhoIso03","ChgIsoWrtChosenVtx","ChgIsoWrtWorstVtx","rho","HadTowOverEm","HadronicOverEm","EsEffSigmaRR","EsEnergyOverRawE"};
        double limsLow[23] = {0.0,0.0,-3.0,90.0,90.0,0.0,-1.1,-1.0,-1.0,0.0,0.0,0.0,0.0,-0.002,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
        double limsHigh[23] = {200,1.3,3.0,180,180,0.1,1.1,-0.95,5.0,650,1.1,0.2,0.2,0.002,1.0,25.0,25.0,25.0,60.0,0.4,1.0,8.0,0.2};
        int nBins[23] = {400,520,400,4500,4500,400,440,400,500,650,440,400,400,400,400,500,500,500,600,400,400,400,400};
        int nEventsLoose, nEventsTight;
        
        TCanvas *can = new TCanvas ("can","can",10,10,1600,900);
        
        for(int i = 0; i < nVars; i++){
//        for(int i = 3; i < 5; i++){
//        for(int i = 0; i < 8; i++){
            string cutStringLeadIDMVAInside;
            string cutStringSubIDMVAInside;
            string cutStringLeadIDMVAInside2;
            string cutStringSubIDMVAInside2;
            
            string insideIDMVACutLead;
            string insideIDMVACutSub;
            if(varNames[i] == "IDMVA" || varNames[i] == "IDMVAZoom"){
                insideIDMVACutLead = ")*weight";
                insideIDMVACutSub = ")*weight";
            }
            else{
                if (diphotonCuts == false){
                    insideIDMVACutLead = idMVACutLead + ")*weight";
                    insideIDMVACutSub = idMVACutSub + ")*weight";
                }
                if (diphotonCuts == true){
                    insideIDMVACutLead = idMVACutLead + idMVACutSub + ")*weight";
                    insideIDMVACutSub = idMVACutSub + idMVACutLead + ")*weight";
                }
            }
            cutStringLeadIDMVAInside = cutStringLeadIDMVA + insideIDMVACutLead;
            cutStringSubIDMVAInside = cutStringSubIDMVA + insideIDMVACutSub;
            cutStringLeadIDMVAInside2 = cutStringLeadIDMVA2 + insideIDMVACutLead;
            cutStringSubIDMVAInside2 = cutStringSubIDMVA2 + insideIDMVACutSub;
           
            TLegend *legend;
            if (varNames[i] == "IDMVA") legend = new TLegend(0.35,0.6,0.65,0.9,"","brNDC");
            else legend = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
            
            double maxVal = 0.0;
            int maxInt = -1;
            
            string plotTitleStr;
            if(diphotonCuts == true)plotTitleStr = varNames[i] + " Diphoton Cuts ";
            if(diphotonCuts == false)plotTitleStr = varNames[i] + " Single Photon Cuts ";
            string overallTitleStr = plotTitleStr + etaLabels[e] + ";" + varNames[i];
            THStack *hStack = new THStack("hStack",overallTitleStr.c_str());
            
            string leadVarStr = "lead" + varNames[i];
            string subVarStr = "sub" + varNames[i];
            
            if(varNames[i] == "hggMass" || varNames[i] == "rho" || varNames[i] == "weight"){
                leadVarStr = varNames[i];
                subVarStr = varNames[i];
            }
            if(varNames[i] == "hggMassCut"){
                leadVarStr = "hggMass";
                subVarStr = "hggMass";
            }
            if(varNames[i] == "IDMVAZoom"){
                leadVarStr = "leadIDMVA";
                subVarStr = "subIDMVA";
            }
            
            TH1F *h[12];
            TH1F *hSub[12];
            string hNames[12] = {"hPUncut","hPPresel","hIDMVAP","hIDMVAP2","hFUncut","hFPresel","hIDMVAF","hIDMVAF2","hGGHUncut","hGGHPresel","hGGHIDMVA","hGGHIDMVA2"};
            string hNamesSub[12] = {"hPUncutSub","hPPreselSub","hIDMVAPSub","hIDMVAPSub2","hFUncutSub","hFPreselSub","hIDMVAFSub","hIDMVAFSub2","hGGHUncutSub","hGGHPreselSub","hGGHIDMVASub","hGGHIDMVASub2"};
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
                    cutStringLead = "(leadGenMatchType == 1 && subGenMatchType != 1" + massCutLoose + cutStringLeadIDMVAInside;
                    cutStringSub = "(subGenMatchType == 1 && leadGenMatchType != 1" + massCutLoose + cutStringSubIDMVAInside;
                }
                if (j == 3){
                    cutStringLead = "(leadGenMatchType == 1 && subGenMatchType != 1" + massCutLoose + cutStringLeadIDMVAInside2;
                    cutStringSub = "(subGenMatchType == 1 && leadGenMatchType != 1" + massCutLoose + cutStringSubIDMVAInside2;
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
                    cutStringLead = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + cutStringLeadIDMVAInside;
                    cutStringSub = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + cutStringSubIDMVAInside;
                }
                if (j == 7){
                    cutStringLead = "(leadGenMatchType != 1 && subGenMatchType == 1" + massCutLoose + cutStringLeadIDMVAInside2;
                    cutStringSub = "(subGenMatchType != 1 && leadGenMatchType == 1" + massCutLoose + cutStringSubIDMVAInside2;
                }
                if (varNames[i] == "hggMass"){
                    if (j == 8){
                        cutStringLead = "(leadGenMatchType == 1 " + massCutLoose + cutStringLeadLoose;
                        cutStringSub = "(subGenMatchType == 1" + massCutLoose + cutStringSubLoose;
                    }
                    if (j == 9){
                        cutStringLead = "(leadGenMatchType == 1 " + massCutLoose + cutStringLeadTight;
                        cutStringSub = "(subGenMatchType == 1" + massCutLoose + cutStringSubTight;
                    }
                    if (j == 10){
                        cutStringLead = "(leadGenMatchType == 1 " + massCutLoose + cutStringLeadIDMVAInside;
                        cutStringSub = "(subGenMatchType == 1" + massCutLoose + cutStringSubIDMVAInside;
                    }
                    if (j == 11){
                        cutStringLead = "(leadGenMatchType == 1 " + massCutLoose + cutStringLeadIDMVAInside2;
                        cutStringSub = "(subGenMatchType == 1" + massCutLoose + cutStringSubIDMVAInside2;
                    }
                }
                else {
                    if (j == 8){
                        cutStringLead = "(leadGenMatchType == 1 " + massCutTight + cutStringLeadLoose;
                        cutStringSub = "(subGenMatchType == 1" + massCutTight + cutStringSubLoose;
                    }
                    if (j == 9){
                        cutStringLead = "(leadGenMatchType == 1 " + massCutTight + cutStringLeadTight;
                        cutStringSub = "(subGenMatchType == 1" + massCutTight + cutStringSubTight;
                    }
                    if (j == 10){
                        cutStringLead = "(leadGenMatchType == 1 " + massCutTight + cutStringLeadIDMVAInside;
                        cutStringSub = "(subGenMatchType == 1" + massCutTight + cutStringSubIDMVAInside;
                    }
                    if (j == 11){
                        cutStringLead = "(leadGenMatchType == 1 " + massCutTight + cutStringLeadIDMVAInside2;
                        cutStringSub = "(subGenMatchType == 1" + massCutTight + cutStringSubIDMVAInside2;
                    }
                }
               
                h[j] = new TH1F (hNames[j].c_str(),"",nBins[i],limsLow[i],limsHigh[i]);
                hSub[j] = new TH1F (hNamesSub[j].c_str(),"",nBins[i],limsLow[i],limsHigh[i]);
                allTrees[j]->Project(hNames[j].c_str(),leadVarStr.c_str(),cutStringLead.c_str());
                allTrees[j]->Project(hNamesSub[j].c_str(),subVarStr.c_str(),cutStringSub.c_str());
                h[j]->Add(hSub[j]);
                            
                string label = allLabels[j];
                
                char buff1[100], buff2[100], buff3[100];
                
                snprintf(buff1, sizeof(buff1), "(%0.2f)", h[j]->GetSumOfWeights());
                string nEvents = buff1;
                label += nEvents;
                
                if(j == 8 || j == 9 || j == 10 || j == 11){
                    if(varNames[i] == "hggMass" || varNames[i] == "hggMassCut"){
                        snprintf(buff3, sizeof(buff3), "(Width = %0.2f)",getExactWidth(h[j]));
                        string Width = buff3;
                        label += Width;
                    }
                }
                
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
                if (j == 9|| j == 10 || j == 11){
                    snprintf(buff2, sizeof(buff2), "(%0.2f", 100*(h[j]->GetSumOfWeights()/(h[8]->GetSumOfWeights()/400)));
                    string prop = buff2;
                    label += (prop + "%)");
                }
            
                legend->AddEntry(h[j],label.c_str(),"pl");
//                if (j == 0 || j == 1 || j == 2) h[j]->SetLineColor(1);
//                if (j == 3 || j == 4 || j == 5) h[j]->SetLineColor(2);
//                if (j == 6 || j == 7 || j == 8) h[j]->SetLineColor(3);
//                if (j == 1 || j == 4 || j == 7) h[j]->SetLineStyle(9);
//                if (j == 2 || j == 5 || j == 8) h[j]->SetLineStyle(3);
                
                if (j == 0 || j == 4 || j == 8) h[j]->SetLineColor(4);
                if (j == 1 || j == 5 || j == 9) h[j]->SetLineColor(3);
                if (j == 2 || j == 6 || j == 10) h[j]->SetLineColor(2);
                if (j == 3 || j == 7 || j == 11) h[j]->SetLineColor(106);
                if (j == 0 || j == 1 || j == 2 || j == 3) h[j]->SetLineStyle(3);
                if (j == 4 || j == 5 || j == 6 || j == 7) h[j]->SetLineStyle(9);
                
                if(j == 8 || j == 9 || j == 10 || j == 11)h[j]->Scale(400);
                if(varNames[i] == "hggMass" || varNames[i] == "hggMassCut")h[j]->Rebin(10);
                hStack->Add(h[j]);
            }
            hStack->SetHistogram(new TH1F("hstot","",nBins[i],limsLow[i],limsHigh[i]));
            hStack->GetHistogram()->GetXaxis()->SetNdivisions(-520);
            hStack->GetHistogram()->GetXaxis()->SetLabelSize(0.02);
            hStack->Draw("nostackhist");
            legend->Draw("same");
            can->SetGrid();
            if (varNames[i] == "IDMVA" || varNames[i] == "IDMVAZoom" || varNames[i] == "ChgIsoWrtChosenVtx" || varNames[i] == "HadTowOverEm" || varNames[i] == "HadronicOverEm" || varNames[i] == "EsEffSigmaRR" || varNames[i] == "EsEnergyOverRawE") can->SetLogy(1);
            else can->SetLogy(0);
            
            string outName;
            if (varNames[i] == "Pt/hggMass") outName = outNameGen + "_PtOvrM" + etaFLabels[e];
            else outName = outNameGen + "_" + varNames[i] + etaFLabels[e];
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

