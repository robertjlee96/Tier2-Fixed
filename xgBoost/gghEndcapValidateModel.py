#!/usr/bin/env python -W ignore::DeprecationWarning
import numpy as np
import uproot
from sklearn.model_selection import train_test_split
from sklearn.utils.fixes import _Iterable as Iterable, _Sized as Sized
from sklearn import metrics
from sklearn.metrics import roc_curve, auc
from sklearn.metrics import accuracy_score
from ROOT import TFile, TTree, TGraph
from ROOT import gROOT, AddressOf
import xgboost as xgb
from xgboost import XGBClassifier
import matplotlib 
matplotlib.use('agg') #Sketchy way of using alternate backend for problematic Tkinter package (FIX?)
import matplotlib.pyplot as plt
import endcapPtCutUtils
#import oldXGBoost2tmva
import time,pickle
from tqdm import tqdm
import sys
from tempfile import TemporaryFile
import time
#import joblib
#import gc
gROOT.ProcessLine(
                "struct myStruct {\
                Double_t     yVal;\
                Double_t     passSPresel;\
                Double_t     passDPresel;\
                Double_t     secondScEta;\
                Double_t     yPred;\
                Double_t     hggMass;\
                Double_t     weight;\
                Double_t     pt;\
                Double_t     ptOvrM;\
                Double_t     scEta;\
                };" );

from ROOT import myStruct

# for sklearn, see
np.random.seed(1337)

inFileName ="../NTuples/SinglePhoton_GGHandGJet_M95_PTM15_DPT075_Validation_0228.root"

modelNames = []
#FIRST DO MD SEARCH
#modelNames.append("endcap/Model_MD13_LR04_M95PTM15_OnlyPFPairs_UL2017_Endcap_0120.model")
#modelNames.append("endcap/Model_MD14_LR04_M95PTM15_OnlyPFPairs_UL2017_Endcap_0120.model")
#modelNames.append("endcap/Model_MD15_LR04_M95PTM15_OnlyPFPairs_UL2017_Endcap_0120.model")
#modelNames.append("endcap/Model_MD16_LR04_M95PTM15_OnlyPFPairs_UL2017_Endcap_0120.model")
#modelNames.append("endcap/Model_MD17_LR04_M95PTM15_OnlyPFPairs_UL2017_Endcap_0120.model")
#modelNames.append("endcap/Model_MD18_LR0.04_M95PTM15_OnlyPFPairs_UL2017_Endcap_0119.model")
#modelNames.append("endcap/Model_MD19_LR04_M95PTM15_OnlyPFPairs_UL2017_Endcap_0120.model")

#THEN LR SEARCH MD 13
#modelNames.append("endcap/Model_MD13_LR0.02_M95PTM15_OnlyPFPairs_UL2017_Endcap_0126.model")
#modelNames.append("endcap/Model_MD13_LR0.03_M95PTM15_OnlyPFPairs_UL2017_Endcap_0126.model")
#modelNames.append("endcap/Model_MD13_LR0.04_M95PTM15_OnlyPFPairs_UL2017_Endcap_0126.model")
#modelNames.append("endcap/Model_MD13_LR0.05_M95PTM15_OnlyPFPairs_UL2017_Endcap_0126.model")
#modelNames.append("endcap/Model_MD13_LR0.06_M95PTM15_OnlyPFPairs_UL2017_Endcap_0126.model")
#modelNames.append("endcap/Model_MD13_LR0.07_M95PTM15_OnlyPFPairs_UL2017_Endcap_0126.model")
#THEN LR SEARCH MD 18
#modelNames.append("endcap/Model_MD18_LR0.01_M95PTM15_OnlyPFPairs_UL2017_Endcap_0119.model")
#modelNames.append("endcap/Model_MD18_LR0.02_M95PTM15_OnlyPFPairs_UL2017_Endcap_0119.model")
#modelNames.append("endcap/Model_MD18_LR0.03_M95PTM15_OnlyPFPairs_UL2017_Endcap_0119.model")
#modelNames.append("endcap/Model_MD18_LR0.04_M95PTM15_OnlyPFPairs_UL2017_Endcap_0119.model")
#modelNames.append("endcap/Model_MD18_LR0.05_M95PTM15_OnlyPFPairs_UL2017_Endcap_0119.model")
#modelNames.append("endcap/Model_MD18_LR0.06_M95PTM15_OnlyPFPairs_UL2017_Endcap_0120.model")
#modelNames.append("endcap/Model_MD18_LR0.07_M95PTM15_OnlyPFPairs_UL2017_Endcap_0120.model")
#THEN LR SEARCH MD 20
#modelNames.append("endcap/Model_MD20_LR0.03_M95PTM15_OnlyPFPairs_UL2017_Endcap_0126.model")
#modelNames.append("endcap/Model_MD20_LR0.045_M95PTM15_OnlyPFPairs_UL2017_Endcap_0126.model")
#modelNames.append("endcap/Model_MD20_LR0.06_M95PTM15_OnlyPFPairs_UL2017_Endcap_0126.model")
#modelNames.append("endcap/Model_MD20_LR0.075_M95PTM15_OnlyPFPairs_UL2017_Endcap_0126.model")
#modelNames.append("endcap/Model_MD20_LR0.09_M95PTM15_OnlyPFPairs_UL2017_Endcap_0126.model")

modelNames.append("endcap/Model_MD20_LR045_NTree300_M95PTM15_OnlyPFPairs_UL2017_0202_Endcap.model")
#modelNames.append("endcap/Model_MD20_LR045_NTree250_M95PTM15_OnlyPFPairs_UL2017_0202_Endcap.model")
#modelNames.append("endcap/Model_MD20_LR045_NTree200_M95PTM15_OnlyPFPairs_UL2017_0202_Endcap.model")
#modelNames.append("endcap/Model_MD20_LR045_NTree150_M95PTM15_OnlyPFPairs_UL2017_0202_Endcap.model")
#modelNames.append("endcap/Model_MD20_LR045_NTree100_M95PTM15_OnlyPFPairs_UL2017_0202_Endcap.model")
#modelNames.append("endcap/Model_MD20_LR045_NTree50_M95PTM15_OnlyPFPairs_UL2017_0202_Endcap.model")

fNames = []

#FIRST DO MD SEARCH
#fNames.append("validationNTuples/0128/MD13_LR04_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0201/GGH/MD14_LR04_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD15_LR04_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD16_LR04_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD17_LR04_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0201/GGH/MD18_LR04_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD19_LR04_M95PTM15_GGH_UL2017_Validation_Endcap")

#THEN DO LR SEARCH MD 13
#fNames.append("validationNTuples/0128/MD13_LR02_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD13_LR03_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD13_LR04_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD13_LR05_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD13_LR06_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD13_LR07_M95PTM15_GGH_UL2017_Validation_Endcap")

#THEN DO LR SEARCH MD 18
#fNames.append("validationNTuples/0128/MD18_LR01_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD18_LR02_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD18_LR03_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD18_LR04_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD18_LR05_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD18_LR06_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD18_LR07_M95PTM15_GGH_UL2017_Validation_Endcap")

#THEN DO LR SEARCH MD 20
#fNames.append("validationNTuples/0128/MD20_LR03_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0201/GGH/MD20_LR045_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0201/GGH/MD20_LR06_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD20_LR075_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0128/MD20_LR09_M95PTM15_GGH_UL2017_Validation_Endcap")

fNames.append("validationNTuples/0228/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_OrigWeights_Validation_Endcap")
#fNames.append("validationNTuples/0202/GGH/MD20_LR045_NTree250_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0202/GGH/MD20_LR045_NTree200_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0202/GGH/MD20_LR045_NTree150_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0202/GGH/MD20_LR045_NTree100_M95PTM15_GGH_UL2017_Validation_Endcap")
#fNames.append("validationNTuples/0202/GGH/MD20_LR045_NTree50_M95PTM15_GGH_UL2017_Validation_Endcap")

fin = uproot.open(inFileName)
prompt = fin['promptPhotons']
fake = fin['fakePhotons']
## for barrel
geometry_selection = lambda tree: np.abs(tree.array('scEta')) > 1.5556
dptCut = lambda tree: abs((tree.array('genPt')-tree.array('pt'))/tree.array('genPt')) < 0.1
ptCut = lambda tree: tree.array('pt') > 17.0

input_values, target_values, orig_weights, train_weights, pt, scEta, secondEta, hggMass, passS, passD, input_vars = endcapPtCutUtils.load_file(fin,geometry_selection,ptCut)

print "input_vars", input_vars

# ### split into training and test set
#X_train, X_test, w_train, dummy, y_train, y_test, dummy, w_test, pt_train, pt_test, scEta_train, scEta_test, hggMass_train, hggMass_test, passS_train, passS_test, passD_train,passD_test = train_test_split(input_values,train_weights,target_values,orig_weights,pt,scEta,hggMass,passS,passD,test_size=0.99)


for i in range(0,len(fNames)):
    fNameNPZ = fNames[i] + ".npz"
    outFile = open(fNameNPZ,"w")
    fNameRoot = fNames[i] + ".root"
    outFileRoot = TFile(fNameRoot,'recreate')

    xgBoostParams = []
    aucTest = []
    aucTrain = []
    lossTest = []
    lossTrain = []
    mvaValsTest = []
    mvaValsTrain = []
    bestVals = []
    fprVals = []
    tprVals = []
    timeVals = []
    
    modelFile = open(modelNames[i],'rb')
    model = pickle.load(modelFile)

    boostParamsTmp = []
    boostParamsTmp.append(model.get_xgb_params()["max_depth"])
    boostParamsTmp.append(model.get_xgb_params()["learning_rate"])
    boostParamsTmp.append(model.get_xgb_params()["gamma"])
    boostParamsTmp.append(model.get_xgb_params()["min_child_weight"])
    boostParamsTmp.append(model.get_xgb_params()["max_delta_step"])
    boostParamsTmp.append(model.get_xgb_params()["colsample_bytree"])
    boostParamsTmp.append(model.get_xgb_params()["subsample"])
    boostParamsTmp.append(model.get_xgb_params()["reg_alpha"])
    boostParamsTmp.append(model.get_xgb_params()["reg_lambda"])
    xgBoostParams.append(boostParamsTmp)

    eval_set = [(input_values, target_values)]
    
    startTime = time.time()
    ###FIRST, EVALUATE AUC
    y_pred = model.predict_proba(input_values)[:,1]

    #mvaValsTest.append(y_test_pred)
    #mvaValsTrain.append(y_train_pred)
        
          
    fpr, tpr, thresholds = roc_curve(target_values,y_pred,sample_weight=train_weights)
    fprVals.append(fpr)
    tprVals.append(tpr)
#    auc_val_test = metrics.auc(fpr, tpr)
    auc_val_test = metrics.roc_auc_score(target_values,y_pred,sample_weight=train_weights)
    print auc_val_test
    

    #NOW, write fpr and tpr to a root file
    promptClass = myStruct()
    fakeClass = myStruct()
    tPrompt = TTree("promptPhotons", "GGH Prompt")
    tPrompt.Branch( 'XGBScores', promptClass, 'yVal/D:passSPresel/D:passDPresel/D:secondScEta/D:yPred/D:hggMass/D:weight/D:pt/D:ptOvrM/D:scEta/D' )
    tFake = TTree("fakePhotons", "GJet Fake")
    tFake.Branch( 'XGBScores', fakeClass, 'yVal/D:passSPresel/D:passDPresel/D:secondScEta/D:yPred/D:hggMass/D:weight/D:pt/D:ptOvrM/D:scEta/D' )

    sigVals = []
    mValsSig = []
    weightSig = []
    secondScEtaSig = []
    passSSig = []
    passDSig = []
    ptSig = []
    ptOvrMSig = []
    scEtaSig = []

    bkgVals = []
    mValsBkg = []
    weightBkg = []
    secondScEtaBkg = []
    passSBkg = []
    passDBkg = []
    ptBkg = []
    ptOvrMBkg = []
    scEtaBkg = []

    trainLen = len(y_pred)
    for n in range (0,trainLen):
        if target_values[n] == 1:
            sigVals.append(y_pred[n])
            mValsSig.append(hggMass[n])
            weightSig.append(orig_weights[n])
            secondScEtaSig.append(secondEta[n])
            passSSig.append(passS[n])
            passDSig.append(passD[n])
            ptSig.append(pt[n])
            ptOvrMSig.append(pt[n]/hggMass[n])
            scEtaSig.append(scEta[n])
        if target_values[n] == 0:
            bkgVals.append(y_pred[n])
            mValsBkg.append(hggMass[n])
            weightBkg.append(orig_weights[n])
            secondScEtaBkg.append(secondEta[n])
            passSBkg.append(passS[n])
            passDBkg.append(passD[n])
            ptBkg.append(pt[n])
            ptOvrMBkg.append(pt[n]/hggMass[n])
            scEtaBkg.append(scEta[n])

    sigLen = len(sigVals)
    bkgLen = len(bkgVals)
    for n in range(0,sigLen):
        promptClass.yPred = sigVals[n]
        promptClass.yVal = 1
        promptClass.hggMass = mValsSig[n]
        promptClass.weight = weightSig[n]
        promptClass.secondScEta = secondScEtaSig[n]
        promptClass.passSPresel = passSSig[n]
        promptClass.passDPresel = passDSig[n]
        promptClass.pt = ptSig[n]
        promptClass.ptOvrM = ptOvrMSig[n]
        promptClass.scEta = scEtaSig[n]
        tPrompt.Fill()

    for n in range(0,bkgLen):
        fakeClass.yPred = bkgVals[n]
        fakeClass.yVal = 0
        fakeClass.hggMass = mValsBkg[n]
        fakeClass.weight = weightBkg[n]
        fakeClass.secondScEta = secondScEtaBkg[n]
        fakeClass.passSPresel = passSBkg[n]
        fakeClass.passDPresel = passDBkg[n]
        fakeClass.pt = ptBkg[n]
        fakeClass.ptOvrM = ptOvrMBkg[n]
        fakeClass.scEta = scEtaBkg[n]
        tFake.Fill()

    outFileRoot.Write()
    outFileRoot.Close()

    ###NEXT, EVALUATE LOG LOSS
    results = model.evals_result() ##NO TEST REWEIGHTING
                            
    #aucTest.append(results['validation_1']['auc'])
    aucTest.append(auc_val_test)
    aucTrain.append(results['validation_0']['auc'])
    lossTest.append(results['validation_1']['logloss'])
    lossTrain.append(results['validation_0']['logloss'])
            
    bestRound = model.best_iteration

    bestValsTmp = [bestRound,results['validation_0']['auc'][bestRound],results['validation_1']['logloss'][bestRound],results['validation_0']['logloss'][bestRound]]
                
    bestVals.append(bestValsTmp)
        
    endTime = time.time()
    timeSpent = endTime - startTime
    timeSpentMin = timeSpent/60.0
    print 'Run Time(min) =  {0:.4f}'.format(timeSpentMin)
    timeVals.append(timeSpent)

    #MoreVarsTmvaUtils.variable_importance(model,input_variables,model_name)

    del results
    del model

    np.savez(outFile, xgBoostParams = xgBoostParams, aucTest = aucTest, aucTrain = aucTrain, lossTest = lossTest, lossTrain = lossTrain, mvaValsTest = mvaValsTest, mvaValsTrain = mvaValsTrain, bestVals = bestVals, fprVals = fprVals, tprVals = tprVals, timeVals = timeVals)

