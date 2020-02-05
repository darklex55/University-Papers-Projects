import cv2
import numpy as np

fN7 = "N7.png"
fNF7 = "NF7.png"

N7 = cv2.imread(fN7, cv2.IMREAD_GRAYSCALE)
NF7 = cv2.imread(fNF7, cv2.IMREAD_GRAYSCALE)

cv2.namedWindow('NF7')
cv2.imshow('NF7', NF7)

cv2.namedWindow('N7')
cv2.imshow('N7', N7)

cv2.waitKey(0)


img = cv2.copyMakeBorder(N7,1,1,1,1,cv2.BORDER_REFLECT)

for k in range(0,2):
    imgtmp = img
    print("Please wait. Filter being applied: ", k + 1, "/", 2)
    for i in range(1,imgtmp.shape[1]-1):
        for j in range(1,imgtmp.shape[0]-1):
            kernel = np.array([[imgtmp[j-1][i-1],imgtmp[j-1][i],imgtmp[j-1][i+1]],[imgtmp[j][i-1],imgtmp[j][i],imgtmp[j][i+1]],[imgtmp[j+1][i-1],imgtmp[j+1][i],imgtmp[j+1][i+1]]])
            img[j][i] = np.median(kernel).astype(np.int8)
print("Done!")

img = np.delete(img,img.shape[1]-1,1)
img = np.delete(img,img.shape[0]-1,0) 
img = np.delete(img,0,0)
img = np.delete(img,0,1)

cv2.namedWindow('NF7 Median Blur')
cv2.imshow('NF7 Median Blur', img)

cv2.waitKey(0)
cv2.destroyAllWindows()

sN7 = img.astype('int64')

for i in range(1,sN7.shape[1]):
    sN7[0][i] = sN7[0][i] + sN7[0][i-1]
for i in range(1,sN7.shape[0]):
    sN7[i][0] = sN7[i][0] + sN7[i-1][0]
for j in range(1,sN7.shape[0]):
    for i in range(1,sN7.shape[1]):
        sN7[j][i] = sN7[j][i] + sN7[j][i - 1] + sN7[j-1][i] - sN7[j-1][i-1]

img2 = cv2.medianBlur(NF7,3)

strel = np.zeros((7,7), np.uint8)

for i in range(0,6):
    strel[i,i]=1

img2 = cv2.morphologyEx(img2, cv2.MORPH_OPEN, strel)

ret,bnr = cv2.threshold(img,53,255,0)
CLRret,CLRbnr = cv2.threshold(img2,52,255,0)

cv2.namedWindow('Binary (N7)')
cv2.imshow('Binary (N7)', bnr)
cv2.namedWindow('Binary (NF7)')
cv2.imshow('Binary (NF7)', CLRbnr)

cv2.waitKey(0)
cv2.destroyAllWindows()

'''
bnrc = np.where(bnr>0,0,255)
cmp = NF7*bnrc

cv2.namedWindow('NF7 BINARY COMPARE')
cv2.imshow('NF7 BINARY COMPARE', cmp)

cv2.waitKey(0)
'''

_,cntrs,_ = cv2.findContours(bnr,cv2.RETR_LIST,cv2.CHAIN_APPROX_NONE,)
_,CLRcntrs,_ = cv2.findContours(CLRbnr,cv2.RETR_LIST,cv2.CHAIN_APPROX_NONE)

akr = np.ones((len(cntrs),),dtype=int)
CLRakr = np.ones((len(CLRcntrs),),dtype=int)

ttlclls = 0
CLRttlclls = 0

for i in range(len(cntrs)):
    m=False
    for j in range(len(cntrs[i])):
            if (cntrs[i][j][0][0]==0 or cntrs[i][j][0][1]==0 or cntrs[i][j][0][0]==bnr.shape[1]-1 or cntrs[i][j][0][1]==bnr.shape[0]-1):
                m=True
                break
    if m:
        akr[i]=0
    else:
        if len(cntrs[i])>20:
            ttlclls=ttlclls+1

for i in range(0,akr.size-1):
    k=akr.size-1-i
    if akr[k]==0:
        del cntrs[k]

for i in range(len(CLRcntrs)):
    m=False
    for j in range(len(CLRcntrs[i])):
            if (CLRcntrs[i][j][0][0]==1 or CLRcntrs[i][j][0][1]==1 or CLRcntrs[i][j][0][0]==bnr.shape[1]-2 or CLRcntrs[i][j][0][1]==bnr.shape[0]-2):
                m=True
                break
    if m:
        CLRakr[i]=0
    else:
        if len(CLRcntrs[i])>20:
            CLRttlclls=CLRttlclls+1

for i in range(0,CLRakr.size-1):
    k=CLRakr.size-1-i
    if CLRakr[k]==0:
        del CLRcntrs[k]

cntrsl = np.zeros((len(cntrs),),dtype=int)
CLRcntrsl = np.zeros((len(CLRcntrs),),dtype=int)

for k in range(0,len(cntrs)):
    cntrsl[k] = cv2.contourArea(cntrs[k])

for k in range(0,len(CLRcntrs)):
    CLRcntrsl[k] = cv2.contourArea(CLRcntrs[k])

maxmins = np.zeros((len(cntrs),4),dtype=int)
CLRmaxmins = np.zeros((len(CLRcntrs),4),dtype=int)

for k in range(0,len(cntrs)):
    max=[0,0]
    min=[img.shape[1],img.shape[1]]
    print("[NF7]Calculating area for cell: ", k + 1, "/", len(cntrs))
    for p in range(0,len(cntrs[k]-1)):
        if max[0]<cntrs[k][p][0][0]:
            max[0]=cntrs[k][p][0][0]
            maxmins[k,2]=cntrs[k][p][0][0]
        if max[1]<cntrs[k][p][0][1]:
            max[1]=cntrs[k][p][0][1]
            maxmins[k,3]=cntrs[k][p][0][1]
        if min[0]>cntrs[k][p][0][0]:
            min[0]=cntrs[k][p][0][0]
            maxmins[k,0]=cntrs[k][p][0][0]
        if min[1]>cntrs[k][p][0][1]:
            min[1]=cntrs[k][p][0][1]
            maxmins[k,1]=cntrs[k][p][0][1]

for k in range(0,len(CLRcntrs)):
    max=[0,0]
    min=[img2.shape[1],img2.shape[1]]
    print("[N7]Calculating area for cell: ", k + 1, "/", len(CLRcntrs))
    for p in range(0,len(CLRcntrs[k]-1)):
        if max[0]<CLRcntrs[k][p][0][0]:
            max[0]=CLRcntrs[k][p][0][0]
            CLRmaxmins[k,2]=CLRcntrs[k][p][0][0]
        if max[1]<CLRcntrs[k][p][0][1]:
            max[1]=CLRcntrs[k][p][0][1]
            CLRmaxmins[k,3]=CLRcntrs[k][p][0][1]
        if min[0]>CLRcntrs[k][p][0][0]:
            min[0]=CLRcntrs[k][p][0][0]
            CLRmaxmins[k,0]=CLRcntrs[k][p][0][0]
        if min[1]>CLRcntrs[k][p][0][1]:
            min[1]=CLRcntrs[k][p][0][1]
            CLRmaxmins[k,1]=CLRcntrs[k][p][0][1]

print('Done!')

CNF7 = NF7.copy()
C2NF7 = NF7.copy()

for i in range(0,len(cntrs)):
    x=maxmins[i,2]+int((maxmins[i,0]-maxmins[i,2])/2)
    y=maxmins[i,3]+int((maxmins[i,1]-maxmins[i,3])/2)
    avgGrey=round((sN7[maxmins[i,3]-1,maxmins[i,2]-1]-sN7[maxmins[i,1]+1,maxmins[i,2]-1]-sN7[maxmins[i,3]-1,maxmins[i,0]+1]+sN7[maxmins[i,1]+1,maxmins[i,0]+1])/((maxmins[i,3]-maxmins[i,1]-1)*(maxmins[i,2]-maxmins[i,0]-1)),2)
    cv2.putText(CNF7, str(cntrsl[i]), (x - 10, y - 15), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (245, 0, 0), 1, cv2.LINE_AA)
    cv2.putText(CNF7, str(avgGrey), (x - 10, y + 15), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (245, 0, 0),1, cv2.LINE_AA)
    cv2.putText(CNF7, str(i+1), (maxmins[i, 0]+15, maxmins[i, 3]-15), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (245, 0, 0), 1, cv2.LINE_AA)
    cv2.rectangle(CNF7, (maxmins[i, 2], maxmins[i, 3]), (maxmins[i, 0], maxmins[i, 1]),  (255, 2, 2), 1)

for i in range(0,len(CLRcntrs)):
    x=CLRmaxmins[i,2]+int((CLRmaxmins[i,0]-CLRmaxmins[i,2])/2)
    y=CLRmaxmins[i,3]+int((CLRmaxmins[i,1]-CLRmaxmins[i,3])/2)
    avgGrey=round((sN7[CLRmaxmins[i,3]-1,CLRmaxmins[i,2]-1]-sN7[CLRmaxmins[i,1]+1,CLRmaxmins[i,2]-1]-sN7[CLRmaxmins[i,3]-1,CLRmaxmins[i,0]+1]+sN7[CLRmaxmins[i,1]+1,CLRmaxmins[i,0]+1])/((CLRmaxmins[i,3]-CLRmaxmins[i,1]-1)*(CLRmaxmins[i,2]-CLRmaxmins[i,0]-1)),2)
    cv2.putText(C2NF7, str(CLRcntrsl[i]), (x - 10, y - 15), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (245, 0, 0), 1, cv2.LINE_AA)
    cv2.putText(C2NF7, str(avgGrey), (x - 10, y + 15), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (245, 0, 0),1, cv2.LINE_AA)
    cv2.putText(C2NF7, str(i+1), (CLRmaxmins[i, 0]+15, CLRmaxmins[i, 3]-15), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (245, 0, 0), 1, cv2.LINE_AA)
    cv2.rectangle(C2NF7, (CLRmaxmins[i, 2], CLRmaxmins[i, 3]), (CLRmaxmins[i, 0], CLRmaxmins[i, 1]),  (255, 2, 2), 1)

'''
cv2.drawContours(NF7,cntrs,-1,(245,5,5),1)
cv2.namedWindow('NF7 CNTRS')
cv2.imshow('NF7 CNRS', NF7)
cv2.waitKey(0)
'''

print("Number of objects in NF7 Picture: ", ttlclls)
print("Number of objects in N7 Picture: ", CLRttlclls)

cv2.namedWindow('NF7 WITH BOUNDING BOXES')
cv2.imshow('NF7 WITH BOUNDING BOXES', CNF7)

cv2.namedWindow('N7 WITH BOUNDING BOXES')
cv2.imshow('N7 WITH BOUNDING BOXES', C2NF7)

cv2.waitKey(0)
