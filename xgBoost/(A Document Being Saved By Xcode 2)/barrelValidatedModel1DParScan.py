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
import barrelPtCutUtils
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
                Double_t     yPred;\
                };" );

from ROOT import myStruct

# for sklearn, see
np.random.seed(1337)

inFileName ="../NTuples/SinglePhoton_GJet_M95PTM15_DPT075_OnlyPromptFakePairs_0120_Validation.root"

nFiles = 7

modelName1 = "barrel/Model_MD13_LR04_M95PTM15_OnlyPFPairs_UL2017_0120.model"
modelName2 = "barrel/Model_MD14_LR04_M95PTM15_OnlyPFPairs_UL2017_0120.model"
modelName3 = "barrel/Model_MD15_LR04_M95PTM15_OnlyPFPairs_UL2017_0120.model"
modelName4 = "barrel/Model_MD16_LR04_M95PTM15_OnlyPFPairs_UL2017_0120.model"
modelName5 = "barrel/Model_MD17_LR04_M95PTM15_OnlyPFPairs_UL2017_0120.model"
modelName6 = "barrel/Model_MD18_LR0.04_M95PTM15_OnlyPFPairs_UL2017_0113.model"
modelName7 = "barrel/Model_MD19_LR04_M95PTM15_OnlyPFPairs_UL2017_0120.model"

#modelName1 = "barrel/Model_MD18_LR0.01_M95PTM15_OnlyPFPairs_UL2017_0113.model"
#modelName2 = "barrel/Model_MD18_LR0.02_M95PTM15_OnlyPFPairs_UL2017_0113.model"
#modelName3 = "barrel/Model_MD18_LR0.03_M95PTM15_OnlyPFPairs_UL2017_0113.model"
#modelName4 = "barrel/Model_MD18_LR0.04_M95PTM15_OnlyPFPairs_UL2017_0113.model"
#modelName5 = "barrel/Model_MD18_LR0.05_M95PTM15_OnlyPFPairs_UL2017_0113.model"
#modelName6 = "barrel/Model_MD18_LR0.06_M95PTM15_OnlyPFPairs_UL2017_0120.model"
#modelName7 = "barrel/Model_MD18_LR0.07_M95PTM15_OnlyPFPairs_UL2017_0120.model"
modelName = [modelName1,modelName2,modelName3,modelName4,modelName5,modelName6,modelName7]

fName1 = "validationNTuples/0124/MD13_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel_0121"
fName2 = "validationNTuples/0124/MD14_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel_0121"
fName3 = "validationNTuples/0124/MD15_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel_0121"
fName4 = "validationNTuples/0124/MD16_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel_0121"
fName5 = "validationNTuples/0124/MD17_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel_0121"
fName6 = "validationNTuples/0124/MD18_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel_0121"
fName7 = "validationNTuples/0124/MD19_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel_0121"

#fName1 = "validationNTuples/MD18_LR01_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel_0121"
#fName2 = "validationNTuples/MD18_LR02_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel_0121"
#fName3 = "validationNTuples/MD18_LR03_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel_0121"
#fName4 = "validationNTuples/MD18_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel_0121"
#fName5 = "validationNTuples/MD18_LR05_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel_0121"
#fName6 = "validationNTuples/MD18_LR06_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel_0121"
#fName7 = "validationNTuples/MD18_LR07_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel_0121"

fName = [fName1,fName2,fName3,fName4,fName5,fName6,fName7]

fin = uproot.open(inFileName)
prompt = fin['promptPhotons']
fake = fin['fakePhotons']
## for barrel
geometry_selection = lambda tree: np.abs(tree.array('scEta')) < 1.4442
dptCut = lambda tree: abs((tree.array('genPt')-tree.array('pt'))/tree.array('genPt')) < 0.1
ptCut = lambda tree: tree.array('pt') > 17.0

input_values, target_values, orig_weights, train_weights, pt, scEta, input_vars = barrelPtCutUtils.load_file(fin,geometry_selection,ptCut)

print "input_vars", input_vars

# ### split into training and test set
X_train, X_test, w_train, dummy, y_train, y_test, dummy, w_test, pt_train, pt_test, scEta_train, scEta_test = train_test_split(input_values,train_weights,target_values,orig_weights,pt,scEta,test_size=0.99)


for i in range(0,nFiles):
    fNameNPZ = fName[i] + ".npz"
    outFile = open(fNameNPZ,"w")
    fNameRoot = fName[i] + ".root"
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
    
    startTime = time.time()
    
    modelFile = open(modelName[i],'rb')
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

    eval_set = [(X_train, y_train), (X_test, y_test)]

    ###FIRST, EVALUATE AUC
    y_train_pred = model.predict_proba(X_train)[:,1]
    y_test_pred = model.predict_proba(X_test)[:,1]

    mvaValsTest.append(y_test_pred)
    mvaValsTrain.append(y_train_pred)
        
    fpr, tpr, thresholds = roc_curve(y_test, y_test_pred,sample_weight=w_test)
    fprVals.append(fpr)
    tprVals.append(tpr)
    auc_val_test = metrics.auc(fpr, tpr)
    print auc_val_test

    #NOW, write fpr and tpr to a root file
    promptClass = myStruct()
    fakeClass = myStruct()
    tPrompt = TTree("promptPhotons", "GJet Prompt")
    tPrompt.Branch( 'XGBScores', promptClass, 'yVal/D:yPred/D' )
    tFake = TTree("fakePhotons", "GJet Fake")
    tFake.Branch( 'XGBScores', fakeClass, 'yVal/D:yPred/D' )

    sigVals = []
    bkgVals = []

    trainLen = len(y_test)
    for n in range (0,trainLen):
        if y_test[n] == 1:
            sigVals.append(y_test_pred[n])
        if y_test[n] == 0:
            bkgVals.append(y_test_pred[n])

    sigLen = len(sigVals)
    bkgLen = len(bkgVals)
    for n in range(0,sigLen):
        promptClass.yPred = sigVals[n]
        promptClass.yVal = 1
        tPrompt.Fill()

    for n in range(0,bkgLen):
        fakeClass.yPred = bkgVals[n]
        fakeClass.yVal = 0
        tFake.Fill()

    outFileRoot.Write()
    outFileRoot.Close()


    ###NEXT, EVALUATE LOG LOSS
    results = model.evals_result() ##NO TEST REWEIGHTING
    
#    aucTest.append(results['validation_1']['auc'])
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
