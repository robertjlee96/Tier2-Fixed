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
void makeFile(){
    string fNameNoLow;
    string fNameLow;
  
    fNameNoLow = "NTuples/GJets_2017UL_NoPTMCut_Robert19_0315.root";
    TFile *inFileNoLow = new TFile(fNameNoLow.c_str());
    
    TTree *sigPhotonsNoLow = (TTree*)inFileNoLow->Get("photonViewDumper/trees/promptPhotons");
    TTree *bkgPhotonsNoLow = (TTree*)inFileNoLow->Get("photonViewDumper/trees/fakePhotons");

    TFile *outFile1 = new TFile("NTuples/GJets_2017UL_NoPTMCut_Robert19_0315_Train.root","RECREATE");
    TTree *sigGJTrain = sigPhotonsNoLow->CloneTree(0);
    sigGJTrain->SetAlias("covIEtaIphi","covIEtaIPhi");
    TTree *bkgGJTrain = bkgPhotonsNoLow->CloneTree(0);
    bkgGJTrain->SetAlias("covIEtaIphi","covIEtaIPhi");
    
    //Int_t n = sigPhotonsGJ->Draw("event","1==1","goff");
    //printf("The arrays' dimension is %d\n",n);
    
    ULong64_t nEventNoLowPrompt;
    sigPhotonsNoLow->SetBranchAddress("event",&nEventNoLowPrompt);
    
    int nEventsNoLowPrompt;
    nEventsNoLowPrompt = sigPhotonsNoLow->GetEntries();
    
    for (int i = 0; i < nEventsNoLowPrompt; i++)
    {
        sigPhotonsNoLow->GetEvent(i);
        if (nEventNoLowPrompt %2 != 0) sigGJTrain->Fill();
    }
    
    sigGJTrain->Print();
    sigGJTrain->AutoSave();
    
    ULong64_t nEventNoLowFake;
    bkgPhotonsNoLow->SetBranchAddress("event",&nEventNoLowFake);
    
    int nEventsNoLowFake;
    nEventsNoLowFake = bkgPhotonsNoLow->GetEntries();
    
    for (int i = 0; i < nEventsNoLowFake; i++)
    {
        bkgPhotonsNoLow->GetEvent(i);
        if (nEventNoLowFake%2 != 0) bkgGJTrain->Fill();
    }
    
    bkgGJTrain->Print();
    bkgGJTrain->AutoSave();
    
    outFile1->Close();
    
    TFile *outFile2 = new TFile("NTuples/GJets_2017UL_NoPTMCut_Robert19_0315_Validation.root","RECREATE");
    TTree *sigGJValidation = sigPhotonsNoLow->CloneTree(0);
    sigGJValidation->SetAlias("covIEtaIPhi","covIEtaIphi");
    TTree *bkgGJValidation = bkgPhotonsNoLow->CloneTree(0);
    bkgGJValidation->SetAlias("covIEtaIPhi","covIEtaIphi");
    

    for (int i = 0; i < nEventsNoLowPrompt; i++)
    {
        sigPhotonsNoLow->GetEvent(i);
        if (nEventNoLowPrompt %2 == 0) sigGJValidation->Fill();
    }
    
    sigGJValidation->Print();
    sigGJValidation->AutoSave();
    
    for (int i = 0; i < nEventsNoLowFake; i++)
    {
        bkgPhotonsNoLow->GetEvent(i);
        if (nEventNoLowFake%2 == 0) bkgGJValidation->Fill();
    }
    bkgGJValidation->Print();
    bkgGJValidation->AutoSave();
    
    outFile2->Close();

    inFileNoLow->Close();
}
