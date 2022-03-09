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
import MoreVarsPtCutUtils
import MoreVarsTmvaUtils
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

<<<<<<< HEAD
nEstVals = [300,250,200,150,100,50]
=======
nEst = 1500
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
fin = uproot.open("../../NTuples/SinglePhoton_GJet_M95PTM15_DPT075_0109_OnlyPromptFakePairs_Train_Proc.root")

prompt = fin['promptPhotons']
fake = fin['fakePhotons']
## for barrel
geometry_selection = lambda tree: np.abs(tree.array('scEta')) > 1.556
#dptCut = lambda tree: abs((tree.array('genPt')-tree.array('pt'))/tree.array('genPt')) < 0.1
ptCut = lambda tree: tree.array('pt') > 17.0


input_values, target_values, orig_weights, train_weights, pt, scEta, input_vars = MoreVarsPtCutUtils.load_file(fin,geometry_selection,ptCut)

print "input_vars", input_vars
# ### split into training and test set
X_train, X_test, w_train, dummy, y_train, y_test, dummy, w_test, pt_train, pt_test, scEta_train, scEta_test = train_test_split(input_values,train_weights,target_values,orig_weights,pt,scEta,test_size=0.25)

MD = 20
#mdVals = [13,14,15]
<<<<<<< HEAD

#lrVals = [0.030,0.06]
#lrVals = [0.06,0.07]
LR = 0.045

for nEst in nEstVals:
    nNow = 0
    fNameGen = "Model_MD20_LR045_NTree" + str(nEst) + "_M95PTM15_OnlyPFPairs_UL2017_0202_Endcap"
=======
lrVals = [0.030,0.045,0.06,0.075,0.09]
#lrVals = [0.06,0.07]
#LR = 0.04

for LR in lrVals:
    nNow = 0
    fNameGen = "Model_MD20_LR" + str(LR) + "_M95PTM15_OnlyPFPairs_UL2017_Endcap_0126"
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
    fName = fNameGen + ".npz"
    outFile = open(fName,"w")
    fNameRoot = fNameGen + ".root"
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

    model = XGBClassifier(max_depth = MD, learning_rate = LR, subsample = 0.9, max_delta_step = 0.0, min_child_weight = 0.0, reg_alpha = 0.0, reg_lambda = 4.0, colsample_bytree = 0.65, verbosity = 1, n_estimators=nEst, nthread = 16, feature_names = input_vars)

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
<<<<<<< HEAD
    #model.fit(X_train, y_train, sample_weight = w_train, eval_metric=["auc", "logloss"], eval_set=eval_set, early_stopping_rounds=50, verbose=True)
    model.fit(X_train, y_train, sample_weight = w_train, eval_metric=["auc", "logloss"], eval_set=eval_set, early_stopping_rounds = 1000, verbose=True)#NO EARLY STOPPING
=======
    model.fit(X_train, y_train, sample_weight = w_train, eval_metric=["auc", "logloss"], eval_set=eval_set, early_stopping_rounds=50, verbose=True)
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b

#    model_name = "Model_MD18_LR" + LR + "_M95PTM25_OnlyPFPairs_UL2017_0113"
    model_fname = fNameGen + ".model"
    model_xmlName = fNameGen + ".xml"

    input_variables = []
    for name in input_vars:#model.get_booster().feature_names:
        input_variables.append((name, 'F'))
    #    print "name in config file = {}".format(name)

    MoreVarsTmvaUtils.convert_model(model.get_booster().get_dump(), input_variables = input_variables, output_xml=model_xmlName)

    #gc.collect()
    pickle.dump(model, open(model_fname, "wb"))
    #joblib.dump(model, model_fname)
     
    ###FIRST, EVALUATE AUC
    y_train_pred = model.predict_proba(X_train)[:,1]
    y_test_pred = model.predict_proba(X_test)[:,1]

    mvaValsTest.append(y_test_pred)
    mvaValsTrain.append(y_train_pred)
        
    fpr, tpr, thresholds = roc_curve(y_test, y_test_pred,sample_weight=w_test)
    fprVals.append(fpr)
    tprVals.append(tpr)
    auc_val_test = metrics.auc(fpr, tpr)

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
                
    aucTest.append(results['validation_1']['auc'])
    aucTrain.append(results['validation_0']['auc'])
    lossTest.append(results['validation_1']['logloss'])
    lossTrain.append(results['validation_0']['logloss'])
            
    bestRound = model.best_iteration

    bestValsTmp = [bestRound,results['validation_0']['auc'][bestRound],results['validation_1']['logloss'][bestRound],results['validation_0']['logloss'][bestRound]]
                
    bestVals.append(bestValsTmp)
                
    print 'Done Training {0:.0f}'.format(nNow)
        
    endTime = time.time()
    timeSpent = endTime - startTime
    timeSpentHR = timeSpent/3600.0
    print 'Run Time(hr) =  {0:.4f}'.format(timeSpentHR)
    timeVals.append(timeSpent)

    #MoreVarsTmvaUtils.variable_importance(model,input_variables,model_name)

        
    del results
    del model

    np.savez(outFile, xgBoostParams = xgBoostParams, aucTest = aucTest, aucTrain = aucTrain, lossTest = lossTest, lossTrain = lossTrain, mvaValsTest = mvaValsTest, mvaValsTrain = mvaValsTrain, bestVals = bestVals, fprVals = fprVals, tprVals = tprVals, timeVals = timeVals)


