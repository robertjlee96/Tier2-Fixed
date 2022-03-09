#!/usr/bin/env python                                                                                                                                                  
import numpy as np
import uproot
from sklearn.model_selection import train_test_split
from sklearn import metrics
import xgboost as xgb
from xgboost import XGBClassifier
import matplotlib.pyplot as plt
import utils_barrel
import xgboost2tmva
import time,pickle
from tqdm import tqdm





## feature importances                                                                                                                                                


plt.figure(figsize=(15, 15))                                                                                                                                          
plt.title("Feature importances")                                                                                                                                      
x = np.arange(12)                                                                                                                                                     
values = [0.369642, 0.136580, 0.106511, 0.090591, 0.063466, 0.058499, 0.047548, 0.037177, 0.035417, 0.022406, 0.016443, 0.015720]                                     
plt.bar(x, values, color = 'r', align = 'center')                                                                                                                     
plt.xticks(x,('WorstVtx', 'sigmaIetaIeta', 's4', 'phoIso03', 'r9',  'ChosenVtx', 'covIEtaIPhi', 'etaWidth', 'rho', 'phiWidth',                                        
              'scEta', 'SCRawE'), rotation=90, fontsize=13, fontweight='bold')                                                                                        
plt.savefig('Variable_importance_names_15032019_nTree2000.png')
                                                                                                              

