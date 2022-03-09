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

void tmvaROCComp(){
    gROOT->Reset();
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);
    string fileNames[10];

    /*
     fileNames[0] = "/home/users/rlee/NTuples/GJets_SinglePhoton_StdMVA_0120_Validation.root";
         //fileNames[0] = "/home/users/rlee/NTuples/GJet_SACuts_20LowMass_Validation_1118.root";
         fileNames[1] = "/home/users/rlee/NTuples/GJets_SinglePhoton_StdMVA_0120_LowMass.root";
         fileNames[2] = "/home/users/rlee/NTuples/GJets_SinglePhoton_StdMVA_0120_HighMass.root";
*/
    /*
     fileNames[0] = "/home/users/rlee/NTuples/GJets_SinglePhoton_StdMVA_0204_Validation.root";
         fileNames[1] = "/home/users/rlee/NTuples/GJets_SinglePhoton_StdMVA_0204_LowMass.root";
         fileNames[2] = "/home/users/rlee/NTuples/GJets_SinglePhoton_StdMVA_0204_HighMass.root";
    */
    /*
        fileNames[0] = "/home/users/rlee/NTuples/GJets_SinglePhoton_XGBMD15LR03_0118_Validation.root";
        fileNames[1] = "/home/users/rlee/NTuples/GJets_SinglePhoton_XGBMD15LR03_0118_LowMass.root";
        fileNames[2] = "/home/users/rlee/NTuples/GJets_SinglePhoton_XGBMD15LR03_0118_HighMass.root";
        */
    

    /*
    fileNames[0] = "./tmpNTuples/XGB_NewAdditionModel_NewAdditionSample_0212_Validation.root";
    fileNames[1] = "./tmpNTuples/XGB_NoLowMassModel_0209_NewAddition_Validation.root";
    fileNames[2] = "./tmpNTuples/XGB_NoLowMassModel_HighMass_0207_Validation.root";
    fileNames[3] = "./tmpNTuples/XGB_NewAdditionModel_HighMass_0212_Validation.root";
    fileNames[4] = "./tmpNTuples/XGB_NoLowMassModel_LowMass_0207_Validation.root";
    fileNames[5] = "./tmpNTuples/XGB_NewAdditionModel_LowMass_0212_Validation.root";
    */
    
    fileNames[0] = "../NTuples/GJets_2017UL_NoPTMCut_TMVA_0312_Validation.root";
    fileNames[1] = "../NTuples/GJets_2017UL_NoPTMCut_Prasant_0312_Validation.root";
    fileNames[2] = "../NTuples/GJets_2017UL_NoPTMCut_Robert18_0315_Validation.root";
    fileNames[3] = "../NTuples/GJets_2017UL_NoPTMCut_Robert19_0315_Validation.root";

    
    TCanvas * can = new TCanvas("can_mvares","can_mvares",10,10,1500,1000);
    string label_mvares = "mva output";

    TLegend *legend = new TLegend(0.35,0.55,0.8,0.85,"","brNDC");
  //legend->SetHeader("PhoId, EB :");
  //legend->SetBorderSize(0);
 // legend->SetFillStyle(0);
  //legend->SetTextFont(42);

    string labelLeg_s[10];

    labelLeg_s[0] = "Sig: Standard TMVA";
    labelLeg_s[1] = "Sig: Prasant -- MD19 LR0.05";
    labelLeg_s[2] = "Sig: Robert  -- MD18 LR0.03";
    labelLeg_s[3] = "Sig: Robert  -- MD19 LR0.05";


/*
    labelLeg_s[0] = "Sig: XGB, Both Files";
    labelLeg_s[1] = "Sig: TMVA, Both Files";
*/
    string labelLeg_b[10];

    labelLeg_b[0] = "Bkg: Standard TMVA";
    labelLeg_b[1] = "Bkg: Prasant -- MD19 LR0.05";
    labelLeg_b[2] = "Bkg: Robert  -- MD18 LR0.03";
    labelLeg_b[3] = "Bkg: Robert  -- MD19 LR0.05";


    TCanvas * can_RoC = new TCanvas ("can_RoC","can_RoC",10,10,1500,1000);

    TLegend *legend_RoC = new TLegend(0.6,0.35,0.85,0.6,"","brNDC");
  //legend_RoC->SetHeader("PhoId, EB :");
  //legend_RoC->SetBorderSize(0);
  //legend_RoC->SetFillStyle(0);
  //legend_RoC->SetTextFont(42);

    string labelLeg_RoC[10];
    labelLeg_RoC[0] = "Standard TMVA";
    labelLeg_RoC[1] = "Prasant -- MD19 LR0.05";
    labelLeg_RoC[2] = "Robert  -- MD18 LR0.03";
    labelLeg_RoC[3] = "Robert  -- MD19 LR0.05";
    


    for(int i = 0; i < 4; i++){

        cout << "file # " << i << endl;

        TFile *mvaResFile = new TFile(fileNames[i].c_str());
        
    
        char promptStr[100], fakeStr[100];
        //if (i == 0 || i == 3){
        sprintf(promptStr, "promptPhotons");
        sprintf(fakeStr, "fakePhotons");
       // }
       /*
        else {
            sprintf(promptStr,"photonViewDumper/trees/promptPhotons");
            sprintf(fakeStr, "photonViewDumper/trees/fakePhotons");
        }
        */
        TTree *t_output_s = (TTree*)mvaResFile->Get(promptStr);
        TTree *t_output_b = (TTree*)mvaResFile->Get(fakeStr);
        //int nCuts = 1000*100; //For production, takes long time but most accurate
        int nCuts = 320*100; //For development, faster but still pretty close to final (Change in 5th decimal place)
      
        TH1F * histo_s = new TH1F ("histo_s","histo_s",nCuts,-1,1);
        TH1F * histo_b = new TH1F ("histo_b","histo_b",nCuts,-1,1);

        
        TString tmp_s = "";
        TString tmp_b  = "";

        tmp_s = "idMVA";
        tmp_s+=">>histo_s";
    
        tmp_b = "idMVA";
        tmp_b+=">>histo_b";
        
        t_output_s->Draw(tmp_s,"(abs(scEta)<1.5)*weight","goff");
        t_output_b->Draw(tmp_b,"(abs(scEta)<1.5)*weight","goff");


       // if (i == 1 || i == 2){
       //     t_output_s->Draw(tmp_s,"(abs(scEta)<1.5)*weight*(event %2 != 0)","goff");
       //     t_output_b->Draw(tmp_b,"(abs(scEta)<1.5)*weight*(event %2 != 0)","goff");
       // }

        char buff1[100], buff2[100];

        snprintf(buff1, sizeof(buff1), ", NEvents = %0.0f", histo_s->GetEntries());
        string nEvS = buff1;
        
        snprintf(buff2, sizeof(buff2), ", NEvents = %0.0f", histo_b->GetEntries());
        string nEvB = buff2;
      
        labelLeg_s[i] = labelLeg_s[i] + nEvS;
        labelLeg_b[i] = labelLeg_b[i] + nEvB;
    
        
        float stepVal = 2.0/nCuts;

        float Nsig[nCuts+1], Nbkg[nCuts+1];
        float sigEff[nCuts+1], bkgEff[nCuts+1];
        float cutsVal[nCuts+1];
        float mvaResCutVal = -1.0 - stepVal;

        int mvaSMaxBin = histo_s->GetXaxis()->FindBin(1) + 1;
        int mvaBMaxBin = histo_b->GetXaxis()->FindBin(1) + 1;

        for(int k = 0; k < nCuts; k++){

            mvaResCutVal+= stepVal;
            cutsVal[k] = mvaResCutVal;

            int mvaBin = histo_s->GetXaxis()->FindBin(mvaResCutVal);
            Nsig[k] = histo_s->Integral(mvaBin,mvaSMaxBin);

            int mvaBin_b = histo_b->GetXaxis()->FindBin(mvaResCutVal);
            Nbkg[k] = histo_b->Integral(mvaBin_b,mvaBMaxBin);
            sigEff[k] = Nsig[k]/Nsig[0];
            bkgEff[k] = Nbkg[k]/Nbkg[0];

      // if(sigEff[k] > 0.98) cout << " sigEff[k] = " << sigEff[k] <<  " bkgEff[k] = " << bkgEff[k] << " with a cut at " << mvaResCutVal << endl;
      //if(sigEff[k] > 0.895 && sigEff[k] < 0.905) cout << " sigEff[k] = " << sigEff[k] <<  " bkgEff[k] = " << bkgEff[k] << " with a cut at " << mvaResCutVal << endl;


        }

        TGraph * sigEff_vs_cut = new TGraph (nCuts, cutsVal, sigEff);
        TGraph * bkgEff_vs_cut = new TGraph (nCuts, cutsVal, bkgEff);
        // TGraph * sigEff_vs_bkgEff = new TGraph (nCuts, sigEff, bkgEff);
        TGraph * sigEff_vs_bkgEff = new TGraph (nCuts, bkgEff, sigEff);
        TGraph * bkgEff_vs_sigEff = new TGraph (nCuts, sigEff, bkgEff);

        // draw mvares
        can->cd();
        can->SetLogy();

        histo_s->SetTitle("");
        histo_s->SetStats(0);
        histo_s->GetXaxis()->SetTitle(label_mvares.c_str());
        histo_s->SetMaximum(histo_s->GetBinContent(histo_s->GetMaximumBin())*1000);
       
        // histo_s->SetMarkerColor(i+2);
        // histo_s->SetLineColor(i+2);
        histo_s->SetLineWidth(2);

        // histo_b->SetMarkerColor(i+2);
        // histo_b->SetLineColor(i+2);
        histo_b->SetLineWidth(2);

        if(i == 0) {
            histo_s->SetLineColor(kRed);
            histo_s->SetMarkerColor(kRed);
            histo_b->SetLineColor(kRed);
            histo_b->SetMarkerColor(kRed);
            histo_b->SetLineStyle(2);
        }
        if(i == 2){
            histo_s->SetLineColor(kBlue);
            histo_s->SetMarkerColor(kBlue);
            histo_b->SetLineColor(kBlue);
            histo_b->SetMarkerColor(kBlue);
            histo_b->SetLineStyle(2);
        }
        if(i == 1){
            histo_s->SetLineColor(kGreen+2);
            histo_s->SetMarkerColor(kGreen+2);
            histo_b->SetLineColor(kGreen+2);
            histo_b->SetMarkerColor(kGreen+2);
            histo_b->SetLineStyle(2);
        }
        else{
            histo_s->SetLineColor(i+2);
            histo_s->SetMarkerColor(i+2);
            histo_b->SetLineColor(i+2);
            histo_b->SetMarkerColor(i+2);
        }

        legend->AddEntry(histo_s,labelLeg_s[i].c_str(),"lm");
        legend->AddEntry(histo_b,labelLeg_b[i].c_str(),"lm");

       
        histo_s->Rebin(200);
        histo_b->Rebin(200);
        
        if(i == 0){
            histo_s->Draw("hist");
        }
        else histo_s->Draw("histsame");
        legend->Draw("histsame");
        histo_b->Draw("histsame");

        can->SetGrid();
        can->Update();
        can->Modified();

        // draw RoC curves
        
        can_RoC->cd();

        if(i == 0){
            sigEff_vs_bkgEff->SetTitle("");
            sigEff_vs_bkgEff->GetYaxis()->SetTitleOffset(1.5);
            sigEff_vs_bkgEff->GetYaxis()->SetTitleSize(0.03);
            sigEff_vs_bkgEff->GetYaxis()->SetLabelSize(0.02);
            sigEff_vs_bkgEff->GetXaxis()->SetTitleOffset(1.5);
            sigEff_vs_bkgEff->GetXaxis()->SetTitleSize(0.03);
            sigEff_vs_bkgEff->GetXaxis()->SetLabelSize(0.02);

      
        
            //sigEff_vs_bkgEff->GetYaxis()->SetRangeUser(0.4,1.);
            //sigEff_vs_bkgEff->GetXaxis()->SetRangeUser(0.,0.4);
      


            sigEff_vs_bkgEff->GetYaxis()->SetTitle("signal eff");
            sigEff_vs_bkgEff->GetXaxis()->SetTitle("bkg eff");
        }


        sigEff_vs_bkgEff->SetLineWidth(2);

        if(i == 0) {
            sigEff_vs_bkgEff->SetLineColor(kRed);
            sigEff_vs_bkgEff->SetMarkerColor(kRed);
        }
        if(i == 2){
            sigEff_vs_bkgEff->SetLineColor(kBlue);
            sigEff_vs_bkgEff->SetMarkerColor(kBlue);
        }
        if(i == 1){
            sigEff_vs_bkgEff->SetLineColor(kGreen+2);
            sigEff_vs_bkgEff->SetMarkerColor(kGreen+2);
        }

        else{
            sigEff_vs_bkgEff->SetLineColor(i+2);
            sigEff_vs_bkgEff->SetMarkerColor(i+2);
        }

        can_RoC->SetGrid();

        if(i == 0)  sigEff_vs_bkgEff->Draw("AC");
        else sigEff_vs_bkgEff->Draw("sameC");
      
        TF1 f1("f",[&](double *bkgEff, double *sigEff){ return sigEff_vs_bkgEff->Eval(bkgEff[0]); },0,1,0);
        float AUC = f1.Integral(0,1);

      
      /*
    float AUC = 0.0;
    for(int n = 0; n < nCuts; n++){
        double xValLow = histo_s->GetXaxis()->GetBinContent(n);
        double xValHigh = histo_s->GetXaxis()->GetBinContent(n+1);
        double yValLow = histo_s->GetXaxis()->GetBinContent(n);
        double yValHigh = histo_s->GetXaxis()->GetBinContent(n+1);
    }
  */
      
        char buff[100];
        snprintf(buff, sizeof(buff), ", AUC = %f", AUC);
        string aucStr = buff;
      
        string labelAUC = labelLeg_RoC[i] + aucStr;
      
        labelLeg_b[0] = "Bkg: XGB MD 18";
      
        legend_RoC->AddEntry(sigEff_vs_bkgEff,labelAUC.c_str(),"pl");
        legend_RoC->Draw("same");

        can_RoC->SetGrid();
        can_RoC->Update();
        can_RoC->Modified();
    }

    string mvaRes = "";
    mvaRes = "XGB_UL17_VsPrasant_NoPTM_0311";

    can->SaveAs((mvaRes+".pdf").c_str());
    can->SaveAs((mvaRes+".root").c_str());

    string canName_RoC = "";
    canName_RoC = "XGB_UL17_VsPrasant_NoPTM_0311_ROC";

    can_RoC->SaveAs((canName_RoC+".pdf").c_str());
    can_RoC->SaveAs((canName_RoC+".root").c_str());
}
