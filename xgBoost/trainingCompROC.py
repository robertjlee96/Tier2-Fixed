import matplotlib.pyplot as plt
import numpy as np

#file1=np.load("./barrel/Model_MD18_LR0.01_M95PTM15_OnlyPFPairs_UL2017_0113.npz", allow_pickle = True)
#file2=np.load("./barrel/Model_MD18_LR0.02_M95PTM15_OnlyPFPairs_UL2017_0113.npz", allow_pickle = True)
#file3=np.load("./barrel/Model_MD18_LR0.03_M95PTM15_OnlyPFPairs_UL2017_0113.npz", allow_pickle = True)
#file4=np.load("./barrel/Model_MD18_LR0.04_M95PTM15_OnlyPFPairs_UL2017_0113.npz", allow_pickle = True)
#file5=np.load("./barrel/Model_MD18_LR0.05_M95PTM15_OnlyPFPairs_UL2017_0113.npz", allow_pickle = True)
#file6=np.load("./barrel/Model_MD18_LR0.06_M95PTM15_OnlyPFPairs_UL2017_0120.npz", allow_pickle = True)
#file7=np.load("./barrel/Model_MD18_LR0.07_M95PTM15_OnlyPFPairs_UL2017_0120.npz", allow_pickle = True)

file1=np.load("./endcap/Model_MD16_LR04_M95PTM15_OnlyPFPairs_UL2017_Endcap_0120.npz", allow_pickle = True)
file2=np.load("./endcap/Model_MD17_LR04_M95PTM15_OnlyPFPairs_UL2017_Endcap_0120.npz", allow_pickle = True)
file3=np.load("./endcap/Model_MD18_LR0.04_M95PTM15_OnlyPFPairs_UL2017_Endcap_0119.npz", allow_pickle = True)
file4=np.load("./endcap/Model_MD19_LR04_M95PTM15_OnlyPFPairs_UL2017_Endcap_0120.npz", allow_pickle = True)

fileArray = [file1,file2,file3,file4]
fpr = []
tpr = []
auc = []
aucVals = []
for file in fileArray:
    fpr.append(file['fprVals'][0])
    tpr.append(file['tprVals'][0])
    auc.append(file['aucTest'][0][-1])
    print file['aucTest'][0][-1]

    
outAll = "outROC/EndcapLRSearch_0120.pdf"
outAllZOOM = "outROC/EndcapLRSearch_0120ZOOM.pdf"

lable1 ="M95PTM15 MD16 LR 0.04 [AUC = %0.6f]" %auc[0]
lable2 ="M95PTM15 MD17 LR 0.04 [AUC = %0.6f]" %auc[1]
lable3 ="M95PTM15 MD18 LR 0.04 [AUC = %0.6f]" %auc[2]
lable4 ="M95PTM15 MD19 LR 0.04 [AUC = %0.6f]" %auc[3]

#lable1 ="M95PTM15 MD18 LR 0.01 [AUC = %0.6f]" %auc[0]
#lable2 ="M95PTM15 MD18 LR 0.02 [AUC = %0.6f]" %auc[1]
#lable3 ="M95PTM15 MD18 LR 0.03 [AUC = %0.6f]" %auc[2]
#lable4 ="M95PTM15 MD18 LR 0.04 [AUC = %0.6f]" %auc[3]
#lable5 ="M95PTM15 MD18 LR 0.05 [AUC = %0.6f]" %auc[4]
#lable6 ="M95PTM15 MD18 LR 0.06 [AUC = %0.6f]" %auc[4]
#lable7 ="M95PTM15 MD18 LR 0.07 [AUC = %0.6f]" %auc[4]

fig = plt.figure()
plt.plot(fpr[0], tpr[0],label = lable1)
plt.plot(fpr[1], tpr[1],label = lable2)
plt.plot(fpr[2], tpr[2],label = lable3)
plt.plot(fpr[3], tpr[3],label = lable4)
#plt.plot(fpr[4], tpr[4],label = lable5)
#plt.plot(fpr[5], tpr[5],label = lable6)
#plt.plot(fpr[6], tpr[6],label = lable7)


plt.legend()
namStr = "Endcap Max Depth Search ROC Curves"
plt.title(namStr)
plt.xlabel("False Positive Rate (Background Efficiency)")
plt.ylabel("True Positive Rate (Signal Efficiency)")

plt.grid()
plt.savefig(outAll,bbox_inches='tight')
plt.xlim(0.0,0.2)
plt.ylim(0.8,1.0)
plt.savefig(outAllZOOM,bbox_inches='tight')

