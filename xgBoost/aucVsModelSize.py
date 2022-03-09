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

file1=np.load("./barrel/Model_MD13_LR04_M95PTM15_OnlyPFPairs_UL2017_0120.npz", allow_pickle = True)
file2=np.load("./barrel/Model_MD14_LR04_M95PTM15_OnlyPFPairs_UL2017_0120.npz", allow_pickle = True)
file3=np.load("./barrel/Model_MD15_LR04_M95PTM15_OnlyPFPairs_UL2017_0120.npz", allow_pickle = True)
file4=np.load("./barrel/Model_MD16_LR04_M95PTM15_OnlyPFPairs_UL2017_0120.npz", allow_pickle = True)
file5=np.load("./barrel/Model_MD17_LR04_M95PTM15_OnlyPFPairs_UL2017_0120.npz", allow_pickle = True)
file6=np.load("./barrel/Model_MD18_LR0.04_M95PTM15_OnlyPFPairs_UL2017_0113.npz", allow_pickle = True)
file7=np.load("./barrel/Model_MD19_LR04_M95PTM15_OnlyPFPairs_UL2017_0120.npz", allow_pickle = True)

fileArray = [file1,file2,file3,file4,file5,fil6,file7]
#sizeArray = [6471.6,3429.4,2201.6,1695.3,1406.5,1167.5,1035.2]
#sizeArray = [5629.2,3456.4,2401.7,1804.3,1509.3,1226.2,1052.0]
#lrArray = ['LR=0.01','LR=0.02','LR=0.03','LR=0.04','LR=0.05','LR=0.06','LR=0.07']
#sizeArray = [1248.3,1413.8,1695.3,2057.6]
sizeArray = [1464.2,1615.0,1804.3,2081.1]
mdArray = ['MD=16','MD=17','MD=18','MD=19']
auc = []
for file in fileArray:

    auc.append(file['aucTest'][0][-1])
    print file['aucTest'][0][-1]

outAll = "aucPlots/Barrel_MDSearch_0120.pdf"
fig = plt.figure()
plt.plot(sizeArray,auc)

for i in range(0,len(fileArray)):
    plt.annotate(mdArray[i], (sizeArray[i], auc[i]))

namStr = "Barrel AUC vs Model Size"
plt.title(namStr)
plt.xlabel("Model Size (MB)")
plt.ylabel("AUC")
plt.grid()
plt.savefig(outAll,bbox_inches='tight')
plt.show()

