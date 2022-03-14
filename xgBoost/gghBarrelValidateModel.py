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
from array import array
#import joblib
#import gc

# for sklearn, see
np.random.seed(1337)

inFileName ="../NTuples/SinglePhoton_GGHandGJet_M95_PTM15_DPT075_Validation_0228.root"

modelNames = []
modelNames.append("barrel/Model_MD20_LR045_NTree359_M95PTM15_OnlyPFPairs_UL2017_0202.model")

fNames = []
fNames.append("validationNTuples/0309/MD20_LR045_NTree359_M95PTM15_GGH_UL2017_Validation_TEST")

fin = uproot.open(inFileName)
prompt = fin['promptPhotons']
fake = fin['fakePhotons']
## for barrel
geometry_selection = lambda tree: np.abs(tree.array('scEta')) < 1.4442
dptCut = lambda tree: abs((tree.array('genPt')-tree.array('pt'))/tree.array('genPt')) < 0.1
ptCut = lambda tree: tree.array('pt') > 17.0

input_values, target_values, orig_weights, train_weights, input_vars, varValsSigIn, varValsBkgIn, varNames = barrelPtCutUtils.load_file(fin,geometry_selection,ptCut)

print "len(varValsSigIn[0]) = ", len(varValsSigIn[0])

for i in range(0,len(fNames)):
    fNameNPZ = fNames[i] + ".npz"
    outFile = open(fNameNPZ,"w")
    fNameRoot = fNames[i] + ".root"
    outFileRoot = TFile(fNameRoot,'recreate')

    eval_set = [(input_values, target_values)]
    
    startTime = time.time()
   
    modelFile = open(modelNames[i],'rb')
    model = pickle.load(modelFile)

    ###FIRST, EVALUATE AUC
    y_pred = model.predict_proba(input_values)[:,1]
        
    auc_val_test = metrics.roc_auc_score(target_values,y_pred,sample_weight=train_weights)
    print auc_val_test
    
    endTime = time.time()
    timeSpent = endTime - startTime
    timeSpentMin = timeSpent/60.0
    print 'Run Time(min) =  {0:.4f}'.format(timeSpentMin)
    
    xgbScoreSig = array('f',[0.0])
    weightSig = array('f',[0.0])
    varValsSig = array('f',21*[0.0])
    
    xgbScoreBkg = array('f',[0.0])
    weightBkg = array('f',[0.0])
    varValsBkg = array('f',21*[0.0])
   
    tPrompt = TTree("promptPhotons", "GGH Prompt")
    tPrompt.Branch( 'xgbScore', xgbScoreSig, 'xgbScore/F' )
    tPrompt.Branch( 'weight', weightSig, 'weight/F' )
    tPrompt.Branch( 'varVals', varValsSig, 'varVals[21]/F' )
    
    tFake = TTree("fakePhotons", "GJet Fake")
    tFake.Branch( 'xgbScore', xgbScoreBkg, 'xgbScore/F' )
    tFake.Branch( 'weight', weightBkg, 'weight/F' )
    tFake.Branch( 'varVals', varValsBkg, 'varVals[21]/F' )

    xgbScoreSigArray = []
    weightSigArray = []
    
    xgbScoreBkgArray = []
    weightBkgArray = []

    trainLen = len(y_pred)
    for n in range (0,trainLen):
        if target_values[n] == 1:
            xgbScoreSigArray.append(y_pred[n])
            weightSigArray.append(orig_weights[n])
        if target_values[n] == 0:
            xgbScoreBkgArray.append(y_pred[n])
            weightBkgArray.append(orig_weights[n])

    sigLen = len(xgbScoreSigArray)
    bkgLen = len(xgbScoreBkgArray)
    for n in range(0,sigLen):
        xgbScoreSig[0] = xgbScoreSigArray[n]
        weightSig[0] = weightSigArray[n]
        for i in range(0,21):
            varValsSig[i] = varValsSigIn[n][i]
        tPrompt.Fill()

    for n in range(0,bkgLen):
        xgbScoreBkg[0] = xgbScoreBkgArray[n]
        weightBkg[0] = weightBkgArray[n]
        for i in range(0,21):
            varValsBkg[i] = varValsBkgIn[n][i]
        tFake.Fill()

    outFileRoot.Write()
    outFileRoot.Close()



    #    xgBoostParams = []
#    aucTest = []
#    aucTrain = []
#    lossTest = []
#    lossTrain = []
#    mvaValsTest = []
#    mvaValsTrain = []
#    bestVals = []
#    fprVals = []
#    tprVals = []
#    timeVals = []

#    boostParamsTmp = []
#    boostParamsTmp.append(model.get_xgb_params()["max_depth"])
#    boostParamsTmp.append(model.get_xgb_params()["learning_rate"])
#    boostParamsTmp.append(model.get_xgb_params()["gamma"])
#    boostParamsTmp.append(model.get_xgb_params()["min_child_weight"])
#    boostParamsTmp.append(model.get_xgb_params()["max_delta_step"])
#    boostParamsTmp.append(model.get_xgb_params()["colsample_bytree"])
#    boostParamsTmp.append(model.get_xgb_params()["subsample"])
#    boostParamsTmp.append(model.get_xgb_params()["reg_alpha"])
#    boostParamsTmp.append(model.get_xgb_params()["reg_lambda"])
#    xgBoostParams.append(boostParamsTmp)
    
#     fpr, tpr, thresholds = roc_curve(target_values,y_pred,sample_weight=train_weights)
#    fprVals.append(fpr)
#    tprVals.append(tpr)

    
#    results = model.evals_result() ##NO TEST REWEIGHTING
    
#    aucTest.append(auc_val_test)
#    aucTrain.append(results['validation_0']['auc'])
#    lossTest.append(results['validation_1']['logloss'])
#    lossTrain.append(results['validation_0']['logloss'])
#
#    bestRound = model.best_iteration
#
#    bestValsTmp = [bestRound,results['validation_0']['auc'][bestRound],results['validation_1']['logloss'][bestRound],results['validation_0']['logloss'][bestRound]]
#
#    bestVals.append(bestValsTmp)
#
#
#    #MoreVarsTmvaUtils.variable_importance(model,input_variables,model_name)
#
#    del results
#    del model
#
#    np.savez(outFile, xgBoostParams = xgBoostParams, aucTest = aucTest, aucTrain = aucTrain, lossTest = lossTest, lossTrain = lossTrain, mvaValsTest = mvaValsTest, mvaValsTrain = mvaValsTrain, bestVals = bestVals, fprVals = fprVals, tprVals = tprVals, timeVals = timeVals)
