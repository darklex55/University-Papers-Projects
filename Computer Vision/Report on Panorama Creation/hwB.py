import cv2 as cv
import numpy as np
import time

Images = []

for i in range(0,9):
    Images.append(cv.imread("HWBimgs/yard-0"+str(i)+".png"))

def findBest4(Images):

    mrkr=8
    BestScoreBy=8
    BestScore=0
    scores=[]

    while mrkr-3>=0:

        score=0

        tmpsift = cv.xfeatures2d_SIFT.create()

        kp1 = tmpsift.detect(Images[mrkr])
        desc1 = tmpsift.compute(Images[mrkr], kp1)

        kp2 = tmpsift.detect(Images[mrkr-1])
        desc2 = tmpsift.compute(Images[mrkr-1], kp2)

        kp3 = tmpsift.detect(Images[mrkr-2])
        desc3 = tmpsift.compute(Images[mrkr-2], kp3)

        kp4 = tmpsift.detect(Images[mrkr-3])
        desc4 = tmpsift.compute(Images[mrkr-3], kp4)

        bf = cv.BFMatcher(cv.NORM_L2, crossCheck=True)
        score = score + len(bf.match(desc1[1], desc2[1]))

        bf = cv.BFMatcher(cv.NORM_L2, crossCheck=True)
        score = score + len(bf.match(desc2[1], desc3[1]))

        bf = cv.BFMatcher(cv.NORM_L2, crossCheck=True)
        score = score + len(bf.match(desc3[1], desc4[1]))

        tmpsurf = cv.xfeatures2d_SURF.create()

        kp1 = tmpsurf.detect(Images[mrkr])
        desc1 = tmpsurf.compute(Images[mrkr], kp1)

        kp2 = tmpsurf.detect(Images[mrkr-1])
        desc2 = tmpsurf.compute(Images[mrkr-1], kp2)

        kp3 = tmpsurf.detect(Images[mrkr-2])
        desc3 = tmpsurf.compute(Images[mrkr-2], kp3)

        kp4 = tmpsurf.detect(Images[mrkr-3])
        desc4 = tmpsurf.compute(Images[mrkr-3], kp4)

        bf = cv.BFMatcher(cv.NORM_L2, crossCheck=True)
        score = score + len(bf.match(desc1[1], desc2[1]))

        bf = cv.BFMatcher(cv.NORM_L2, crossCheck=True)
        score = score + len(bf.match(desc2[1], desc3[1]))

        bf = cv.BFMatcher(cv.NORM_L2, crossCheck=True)
        score = score + len(bf.match(desc3[1], desc4[1]))

        scores.append(score)

        if score>BestScore:
            BestScore=score
            BestScoreBy=mrkr

        mrkr=mrkr-1

    print(scores)
    print(BestScoreBy)
    return BestScoreBy

def matcher(desc1, desc2):
    bf = cv.BFMatcher(cv.NORM_L2, crossCheck=False)
    matchesa = bf.match(desc1, desc2)
    matchesb = bf.match(desc2, desc1)
    matches = []

    for i in range(0, len(matchesa) - 1):
        for j in range(0, len(matchesb) - 1):
            if matchesa[i].distance == matchesb[j].distance:
                matches.append(matchesa[i])
                break
    return matches

def siftAl(imgA,imgB,TranslateX):

    sift = cv.xfeatures2d_SIFT.create()

    kp1 = sift.detect(imgA)
    desc1 = sift.compute(imgA, kp1)

    kp2 = sift.detect(imgB)
    desc2 = sift.compute(imgB, kp2)

    matches = matcher(desc1[1], desc2[1])

    img_pt1 = []
    img_pt2 = []

    for x in matches:
        img_pt1.append(kp1[x.queryIdx].pt)
        img_pt2.append(kp2[x.trainIdx].pt)

    img_pt1 = np.array(img_pt1)
    img_pt2 = np.array(img_pt2)

    M, mask = cv.findHomography(img_pt2, img_pt1, cv.RANSAC)

    M = np.matmul(np.array([[1,0,TranslateX],[0,1,0],[0,0,1]]),M)

    img3 = cv.warpPerspective(imgB, M, (imgA.shape[1] + imgB.shape[1], imgA.shape[0] + 1000))

    img3[0: imgA.shape[0], 0+TranslateX: imgA.shape[1]+TranslateX] = imgA

    dimg = cv.drawMatches(imgA, desc1[0], imgB, desc2[0], matches, None)

    return img3,dimg

def surfAl(imgA,imgB,TransformX):
    surf = cv.xfeatures2d_SURF.create()

    kp1 = surf.detect(imgA)
    desc1 = surf.compute(imgA, kp1)

    kp2 = surf.detect(imgB)
    desc2 = surf.compute(imgB, kp2)

    matches = matcher(desc1[1], desc2[1])

    img_pt1 = []
    img_pt2 = []

    for x in matches:
        img_pt1.append(kp1[x.queryIdx].pt)
        img_pt2.append(kp2[x.trainIdx].pt)

    img_pt1 = np.array(img_pt1)
    img_pt2 = np.array(img_pt2)

    M, mask = cv.findHomography(img_pt2, img_pt1, cv.RANSAC)

    M = np.matmul(np.array([[1,0,TransformX],[0,1,0],[0,0,1]]),M)

    img3 = cv.warpPerspective(imgB, M, (imgA.shape[1] + imgB.shape[1], imgA.shape[0] + 1000))

    img3[0: imgA.shape[0], 0+TransformX: imgA.shape[1]+TransformX] = imgA

    dimg = cv.drawMatches(imgA, desc1[0], imgB, desc2[0], matches, None)

    return img3,dimg

def cleanImg(img):

    while True:
        if (np.sum(img[:,0,0])!=0) and (np.sum(img[:,0,1])!=0) and (np.sum(img[:,0,2])!=0):
            break
        img = np.delete(img,0,1)

    while True:
        if (np.sum(img[:,img.shape[1]-1,0])!=0) and (np.sum(img[:,img.shape[1]-1,0])!=0) and (np.sum(img[:,img.shape[1]-1,0])!=0):
            break
        img = np.delete(img,img.shape[1]-1,1)

    while True:
        if (np.sum(img[img.shape[0]-1,:,0])!=0) and (np.sum(img[img.shape[0]-1,:,1])!=0) and (np.sum(img[img.shape[0]-1,:,2])!=0):
            break
        img = np.delete(img,img.shape[0]-1,0)

    return img

def MakePanoramaSIFT(Images,i):
    starttime = time.time()
    A,mtchA = siftAl(Images[i-1],Images[i],Images[i-1].shape[1])
    A = cleanImg(A)
    print("1/3 Done [",round(time.time() - starttime),"sec]")

    starttime = time.time()
    B,mtchB = siftAl(Images[i-2],Images[i-3],0)
    B = cleanImg(B)
    print("2/3 Done [",round(time.time() - starttime),"sec]")

    starttime = time.time()
    C,mtchC = siftAl(A,B,0)
    C = cleanImg(C)
    print("3/3 Done [",round(time.time() - starttime),"sec]")

    return A,B,C,mtchA,mtchB,mtchC

def MakePanoramaSURF(Images,i):

    starttime = time.time()
    A,mtchA = surfAl(Images[i-1],Images[i],Images[i-1].shape[1])
    A = cleanImg(A)
    print("1/3 Done [",round(time.time() - starttime),"sec]")

    starttime = time.time()
    B,mtchB = surfAl(Images[i-2],Images[i-3],0)
    B = cleanImg(B)
    print("2/3 Done [",round(time.time() - starttime),"sec]")

    starttime = time.time()
    C,mtchC = surfAl(A,B,0)
    C = cleanImg(C)
    print("3/3 Done [",round(time.time() - starttime),"sec]")

    return A,B,C,mtchA,mtchB,mtchC

ti=8

print("Calculating matches and transforming images acordingly using SIFT:")
finalSIFT = MakePanoramaSIFT(Images,ti)

print("Calculating matches and transforming images acordingly using SURF:")
finalSURF = MakePanoramaSURF(Images,ti)

final = [finalSIFT,finalSURF]

pntr=0

for name in ["SIFT", "SURF"]:
    for i in range(0,3):
        cv.namedWindow("[" + name + "] Matches of Images " + str(ti-i) + " , " + str(ti-i-1),cv.WINDOW_NORMAL)
        cv.imshow("[" + name + "] Matches of Images " + str(ti-i) + " , " + str(ti-i-1), final[pntr][i+3])

        cv.namedWindow("[" + name + "] Panorama "+ str(i+1),cv.WINDOW_NORMAL)
        cv.imshow("[" + name + "] Panorama "+ str(i+1), final[pntr][i])

        cv.waitKey(0)

        cv.destroyWindow("[" + name + "] Matches of Images " + str(ti-i) + " , " + str(ti-i-1))
        cv.destroyWindow("[" + name + "] Panorama "+ str(i+1))
    pntr=pntr+1

ICEpanorama = cv.imread("HWBimgs/ICEpanorama.jpg")

cv.namedWindow("SIFT Panorama",cv.WINDOW_NORMAL)
cv.namedWindow("SURF Panorama",cv.WINDOW_NORMAL)
cv.namedWindow("ICE Panorama",cv.WINDOW_NORMAL)
cv.imshow("SIFT Panorama",final[0][2])
cv.imshow("SURF Panorama",final[1][2])
cv.imshow("ICE Panorama",ICEpanorama)
cv.waitKey(0)


myImages=[]

for i in [0,1,2,3]:
    myImages.append(cv.imread("HWBimgs/vcg-0"+str(i)+".jpg"))

ti=3

print("For my Images:")

print("Calculating matches and transforming images acordingly using SIFT:")
myImagesSIFT = MakePanoramaSIFT(myImages,ti)

print("Calculating matches and transforming images acordingly using SURF:")
myImagesSURF = MakePanoramaSURF(myImages,ti)

myfinal = [myImagesSIFT,myImagesSURF]

pntr=0

for name in ["SIFT", "SURF"]:
    for i in range(0,3):
        cv.namedWindow("[" + name + "] Matches of my Images " + str(ti-i) + " , " + str(ti-i-1),cv.WINDOW_NORMAL)
        cv.imshow("[" + name + "] Matches of my Images " + str(ti-i) + " , " + str(ti-i-1), myfinal[pntr][i+3])

        cv.namedWindow("[" + name + "] My panorama "+ str(i+1),cv.WINDOW_NORMAL)
        cv.imshow("[" + name + "] My panorama "+ str(i+1), myfinal[pntr][i])

        cv.waitKey(0)

        cv.destroyWindow("[" + name + "] Matches of my Images " + str(ti-i) + " , " + str(ti-i-1))
        cv.destroyWindow("[" + name + "] My panorama "+ str(i+1))
    pntr=pntr+1

myICEpanorama = cv.imread("HWBimgs/myICEpanorama.jpg")

cv.namedWindow("SIFT Panorama",cv.WINDOW_NORMAL)
cv.namedWindow("SURF Panorama",cv.WINDOW_NORMAL)
cv.namedWindow("ICE Panorama",cv.WINDOW_NORMAL)
cv.imshow("SIFT Panorama",myfinal[0][2])
cv.imshow("SURF Panorama",myfinal[1][2])
cv.imshow("ICE Panorama",myICEpanorama)
cv.waitKey(0)

