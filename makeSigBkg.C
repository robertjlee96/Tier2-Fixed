#include "TROOT.h"
#include "TKey.h"
#include "TFile.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
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
#include "../test/Event.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
//ROBERT'S SPLITTING -- Even->Validation   Odd->Training
void makeSigBkg(){
    string fNameSig;
    string fNameBkg;
  
<<<<<<< HEAD
    fNameSig= "./NTuples/GGH_Prasant_0210.root";
    fNameBkg= "./NTuples/GJet_Prasant_0210.root";
=======
    fNameSig= "./NTuples/GGH_M95PTM25_NoDPT_0113.root";
    fNameBkg= "./NTuples/GJet_M95PTM25_NoDPT_0113.root";
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
    TFile *inFileSig = new TFile(fNameSig.c_str());
    TFile *inFileBkg = new TFile(fNameBkg.c_str());
    
    TTree *sigPhotons = (TTree*)inFileSig->Get("diphotonDumper/trees/ggh_125");
//    TTree *sigPhotons = (TTree*)inFileSig->Get("diphotonDumper/trees/VBF_125");
//    TTree *sigPhotons = (TTree*)inFileSig->Get("diphotonDumper/trees/TTH_125");
//    TTree *sigPhotons = (TTree*)inFileSig->Get("diphotonDumper/trees/glugluH_HH_2B2G");
    TTree *bkgPhotons = (TTree*)inFileBkg->Get("diphotonDumper/trees/GJets");

<<<<<<< HEAD
    TFile *outFile = new TFile("NTuples/GGH_And_GJets_Prasant_0211.root","RECREATE");
=======
    TFile *outFile = new TFile("NTuples/GGH_And_GJets_M95PTM25_NoDPT_0113.root","RECREATE");
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
    TTree *sigPhotonsOut = sigPhotons->CloneTree(0);
//    sigPhotonsOut->SetName("dihiggs_125");
//    sigPhotonsOut->SetName("tth_125");
    sigPhotonsOut->SetAlias("covIEtaIphi","covIEtaIPhi");
    TTree *bkgPhotonsOut = bkgPhotons->CloneTree(0);
    //bkgPhotonsOut->SetName("diphotons");
    bkgPhotonsOut->SetAlias("covIEtaIphi","covIEtaIPhi");
    
    //Int_t n = sigPhotonsGJ->Draw("event","1==1","goff");
    //printf("The arrays' dimension is %d\n",n);
    
    ULong64_t nEventSig;
    sigPhotons->SetBranchAddress("event",&nEventSig);
    
    int nEventsSig = sigPhotons->GetEntries();
    
    for (int i = 0; i < nEventsSig; i++)
    {
        sigPhotons->GetEvent(i);
        sigPhotonsOut->Fill();
    }
    
    sigPhotonsOut->Print();
    sigPhotonsOut->AutoSave();
    
    ULong64_t nEventBkg;
    bkgPhotons->SetBranchAddress("event",&nEventBkg);
    
    int nEventsBkg = bkgPhotons->GetEntries();
    
    for (int i = 0; i < nEventsBkg; i++)
    {
        bkgPhotons->GetEvent(i);
        if (nEventBkg %2 == 0)bkgPhotonsOut->Fill();
    }
    
    bkgPhotonsOut->Print();
    bkgPhotonsOut->AutoSave();
    
    outFile->Close();
}
