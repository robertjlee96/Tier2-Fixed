import matplotlib.pyplot as plt
import numpy as np

#file1=np.load("./endcap/Model_MD18_LR0.01_M95PTM15_OnlyPFPairs_UL2017_Endcap_0119.npz", allow_pickle = True)
#file2=np.load("./endcap/Model_MD18_LR0.02_M95PTM15_OnlyPFPairs_UL2017_Endcap_0119.npz", allow_pickle = True)
#file3=np.load("./endcap/Model_MD18_LR0.03_M95PTM15_OnlyPFPairs_UL2017_Endcap_0119.npz", allow_pickle = True)
#file4=np.load("./endcap/Model_MD18_LR0.04_M95PTM15_OnlyPFPairs_UL2017_Endcap_0119.npz", allow_pickle = True)
#file5=np.load("./endcap/Model_MD18_LR0.05_M95PTM15_OnlyPFPairs_UL2017_Endcap_0119.npz", allow_pickle = True)
#file6=np.load("./endcap/Model_MD18_LR0.06_M95PTM15_OnlyPFPairs_UL2017_Endcap_0120.npz", allow_pickle = True)
#file7=np.load("./endcap/Model_MD18_LR0.07_M95PTM15_OnlyPFPairs_UL2017_Endcap_0120.npz", allow_pickle = True)

#file1=np.load("./barrel/Model_MD18_LR0.01_M95PTM15_OnlyPFPairs_UL2017_0113.npz", allow_pickle = True)
#file2=np.load("./barrel/Model_MD18_LR0.02_M95PTM15_OnlyPFPairs_UL2017_0113.npz", allow_pickle = True)
#file3=np.load("./barrel/Model_MD18_LR0.03_M95PTM15_OnlyPFPairs_UL2017_0113.npz", allow_pickle = True)
#file4=np.load("./barrel/Model_MD18_LR0.04_M95PTM15_OnlyPFPairs_UL2017_0113.npz", allow_pickle = True)
#file5=np.load("./barrel/Model_MD18_LR0.05_M95PTM15_OnlyPFPairs_UL2017_0113.npz", allow_pickle = True)
#file6=np.load("./barrel/Model_MD18_LR0.06_M95PTM15_OnlyPFPairs_UL2017_0120.npz", allow_pickle = True)
#file7=np.load("./barrel/Model_MD18_LR0.07_M95PTM15_OnlyPFPairs_UL2017_0120.npz", allow_pickle = True)
<<<<<<< HEAD
fileArray = []

fileArray.append(np.load("./validationNTuples/0202/GGH/MD20_LR045_NTree300_M95PTM15_GGH_UL2017_Validation_Endcap.npz", allow_pickle = True))
fileArray.append(np.load("./validationNTuples/0202/GGH/MD20_LR045_NTree250_M95PTM15_GGH_UL2017_Validation_Endcap.npz", allow_pickle = True))
fileArray.append(np.load("./validationNTuples/0202/GGH/MD20_LR045_NTree200_M95PTM15_GGH_UL2017_Validation_Endcap.npz", allow_pickle = True))
fileArray.append(np.load("./validationNTuples/0202/GGH/MD20_LR045_NTree150_M95PTM15_GGH_UL2017_Validation_Endcap.npz", allow_pickle = True))
fileArray.append(np.load("./validationNTuples/0202/GGH/MD20_LR045_NTree100_M95PTM15_GGH_UL2017_Validation_Endcap.npz", allow_pickle = True))
fileArray.append(np.load("./validationNTuples/0202/GGH/MD20_LR045_NTree50_M95PTM15_GGH_UL2017_Validation_Endcap.npz", allow_pickle = True))

###
#fileArray.append(np.load("./validationNTuples/0131/GJet/MD13_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation.npz", allow_pickle = True))
#fileArray.append(np.load("./validationNTuples/0131/GJet/MD14_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation.npz", allow_pickle = True))
#fileArray.append(np.load("./validationNTuples/0131/GJet/MD15_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation.npz", allow_pickle = True))
#fileArray.append(np.load("./validationNTuples/0131/GJet/MD16_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation.npz", allow_pickle = True))
#fileArray.append(np.load("./validationNTuples/0131/GJet/MD17_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation.npz", allow_pickle = True))
#fileArray.append(np.load("./validationNTuples/0131/GJet/MD18_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation.npz", allow_pickle = True))
#fileArray.append(np.load("./validationNTuples/0131/GJet/MD19_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation.npz", allow_pickle = True))

outSize = "aucPlots/Endcap_GGH_AUC_MD20LR045_NTreeSearch_0203.pdf"
outTime = "aucPlots/Endcap_GGH_AUC_MD20LR045_NTreeSearch_Time_0203.pdf"
nameStrAUC = "Endcap GHH AUC vs Model Size"
nameStrTime = "Endcap GGH AUC vs Validation Time"

#lrArray = ['LR=0.02','LR=0.03','LR=0.04','LR=0.05','LR=0.06','LR=0.07']
#lrArray = ['LR=0.03','LR=0.045','LR=0.06','LR=0.075','LR=0.09']
#sizeArray = [1770.00,938.84,602.95,464.52,385.43,320.00,283.82] #Endcap LR18 Seach xgbSize
#sizeArray = [256.05,229.08,216.58,194.44,153.86,138.06] #Endcap LR13 Search xgbSize !!! ONLY 6 !!!
#sizeArray = [1541.09,947.14,658.45,494.90,414.02,336.44,288.72] #Barrel LR18 Search xgbSize
#sizeArray = [277.81,228.83,208.18,195.58,187.98,184.47,163.20] #Barrel LR13 Search xgbSize
#sizeArray = [834.53,606.36,439.71,378.19,325.65] #Barrel LR20 Search xgbSize
#sizeArray = [796.14,567.30,445.92,355.86]#,300.79] #Endcap LR20 Search xgbSize

#mdArray = ['MD=13','MD=14','MD=15','MD=16','MD=17','MD=18','MD=19']
#sizeArray = [216.58,268.18,295.45,342.90,387.70,464.52,563.30] #Endcap MD Search xgbSize
#sizeArray = [195.58,282.92,343.38,403.00,443.54,494.90,570.44] #Barrel MD Search xgbSize

nTreeArray = ['NTrees=359','NTrees=300','NTrees=250','NTrees=200','NTrees=150','NTrees=100']
#sizeArray = [606.36,527.12,463.54,393.27,319.92,223.97] #Barrel NTree Search
sizeArray = [582.78,505.03,422.04,332.46,232.44,107.94] #Endcap NTree Search

usedArray = nTreeArray
=======

file1=np.load("./validationNTuples/0125/MD13_LR01_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel.npz", allow_pickle = True)
file2=np.load("./validationNTuples/0125/MD13_LR02_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel.npz", allow_pickle = True)
file3=np.load("./validationNTuples/0125/MD13_LR03_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel.npz", allow_pickle = True)
file4=np.load("./validationNTuples/0125/MD13_LR04_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel.npz", allow_pickle = True)
file5=np.load("./validationNTuples/0125/MD13_LR05_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel.npz", allow_pickle = True)
file6=np.load("./validationNTuples/0125/MD13_LR06_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel.npz", allow_pickle = True)
file7=np.load("./validationNTuples/0125/MD13_LR07_M95PTM15_OnlyPFPairs_UL2017_Validation_Barrel.npz", allow_pickle = True)

outSize = "aucPlots/Barrel_AUC_LRSearch18_0127.pdf"
outTime = "aucPlots/Barrel_AUC_LRSearch18_Time_0127.pdf"

fileArray = [file1,file2,file3,file4,file5,file6,file7]

lrArray = ['LR=0.01','LR=0.02','LR=0.03','LR=0.04','LR=0.05','LR=0.06','LR=0.07']
#sizeArray = [1770.00,938.84,602.95,464.52,385.43,320.00,283.82] #Endcap LR18 Seach xgbSize
#sizeArray = [256.05,229.08,216.58,194.44,153.86,138.06] #Endcap LR13 Search xgbSize !!! ONLY 6 !!!
#sizeArray = [1541.09,947.14,658.45,494.90,414.02,336.44,288.72] #Barrel LR18 Search xgbSize
sizeArray = [277.81,228.83,208.18,195.58,187.98,184.47,163.20] #Barrel LR13 Search xgbSize

mdArray = ['MD=13','MD=14','MD=15','MD=16','MD=17','MD=18','MD=19']
#sizeArray = [216.58,268.18,295.45,342.90,387.70,464.52,563.30] #Endcap MD Search xgbSize
#sizeArray = [195.58,282.92,343.38,403.00,443.54,494.90,570.44] #Barrel MD Search xgbSize

usedArray = lrArray
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b

auc = []
valTime = []
for file in fileArray:

    auc.append(file['aucTest'][0])
    valTime.append((file['timeVals'][0]))
    print (file['timeVals'][0])
    print file['aucTest'][0]

fig = plt.figure()
plt.plot(sizeArray,auc)

for i in range(0,len(fileArray)):
    plt.annotate(usedArray[i], (sizeArray[i], auc[i]))

<<<<<<< HEAD
plt.title(nameStrAUC)
=======
namStr = "Barrel GJet AUC vs Model Size"
plt.title(namStr)
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
plt.xlabel("XGB Model Size (MB)")
plt.ylabel("AUC")
plt.grid()
plt.savefig(outSize,bbox_inches='tight')

plt.clf()
fig = plt.figure()
plt.plot(valTime,auc)

for i in range(0,len(fileArray)):
    plt.annotate(usedArray[i], (valTime[i], auc[i]))
<<<<<<< HEAD
    
    
plt.title(nameStrTime)
=======

namStr = "Barrel GJet AUC vs GJet Validation Time"
plt.title(namStr)
>>>>>>> 27f7c4344d75d61327757beea580fc85cb3f969b
plt.xlabel("Validation Time (sec)")
plt.ylabel("AUC")
plt.grid()
plt.savefig(outTime,bbox_inches='tight')
plt.show()
