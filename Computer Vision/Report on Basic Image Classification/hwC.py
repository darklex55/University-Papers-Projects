import numpy as np
import cv2 as cv
import os

sift = cv.xfeatures2d_SIFT.create()

def extract_local_desc(path):
    img = cv.imread(path)

    kp = sift.detect(img)
    desc = sift.compute(img, kp)
    return desc[1]

def Desc_Extractor(path,voc):

    desc = extract_local_desc(path)

    bf = cv.BFMatcher(cv.NORM_L2SQR)

    matches = bf.match(desc, voc)

    hist = np.zeros(len(voc), dtype=np.float32)

    for i in matches:
        hist[i.trainIdx]+=1

    sm = np.sum(hist)
    for i in range(len(hist)):
        hist[i]=hist[i]/sm

    hist = np.array([hist])
    return hist

def norm(data):
    for i in range(data.shape[0]):
        data[i] = data[i]/np.sqrt(np.sum(np.square(data[i])))
    return data

def encoder(dir,voc):
    b_desc = np.zeros((0, len(voc)))
    lsf = os.listdir(dir)
    print("Creating Bag of Words: ")
    total = len(lsf)
    cntr=0
    for folders in lsf:
        files = os.listdir(dir+"/"+folders)
        for imgs in files:
            path = dir+"/"+folders+"/"+imgs

            hist = Desc_Extractor(path,voc)

            b_desc = np.concatenate((b_desc, hist), axis=0)

        cntr+=1
        print((cntr/total)*100,' % Done')
    return b_desc

def import_train_imgs_desc(dir):
    train_descs = np.zeros((0, 128),dtype=np.float32)
    lsf = os.listdir(dir)
    print("Geting Descriptors: ")
    total = len(lsf)
    cntr=0
    for folders in lsf:
        files = os.listdir(dir+"/"+folders)
        for imgs in files:
            path = dir+"/"+folders+"/"+imgs
            desc = extract_local_desc(path)
            train_descs = np.concatenate((train_descs, desc), axis=0)
        cntr+=1
        print((cntr/total)*100,' % Done')
    return train_descs

def vocabulary(train_descs,ftr):
    print('Creating Vocabulary...')
    term_crit = (cv.TERM_CRITERIA_EPS, 30, 0.1)
    trainer = cv.BOWKMeansTrainer(ftr, term_crit, 2, cv.KMEANS_PP_CENTERS)
    vocabulary = trainer.cluster(train_descs.astype(np.float32))
    return vocabulary

def create_voc(words,dir):
    train_descs = import_train_imgs_desc(dir)
    voc = vocabulary(train_descs, words)
    np.save('vocabulary.npy', voc)
    return voc

def load_voc():
    return np.load('vocabulary.npy')

def create_bow(voc,dir):
    bow_d = encoder(dir, voc)
    np.save('bow_descs.npy', bow_d)
    return bow_d

def load_bow():
    return np.load('bow_descs.npy')

def image_paths(dir):
    lsf = os.listdir(dir)
    paths = []
    for folders in lsf:
        files = os.listdir(dir + "/" + folders)
        for imgs in files:
            path = dir + "/" + folders + "/" + imgs
            paths.append(path)

    return paths

def indexes(dir):
    folders = os.listdir(dir)
    cntr=0
    lst = []
    for i in folders:
        for j in os.listdir(dir+'/'+i):
            lst.append(cntr)
        cntr = cntr + 1
    return np.array(lst)

def create_SVM(img_ind,bow_d,name,kernel):

    svm = cv.ml.SVM_create()
    svm.setType(cv.ml.SVM_C_SVC)
    svm.setKernel(kernel)
    svm.setTermCriteria((cv.TERM_CRITERIA_COUNT, 100, 1.e-06))
    svm.trainAuto(bow_d.astype(np.float32), cv.ml.ROW_SAMPLE, img_ind)
    svm.save(name)

def create_SVM_one_vs_all(bow_d,kernel):
    paths = image_paths('imagedb_train')

    Fighters = np.array(['jet' in a for a in paths], np.int32)
    Motorbikes = np.array(['motorbikes' in a for a in paths], np.int32)
    Busses = np.array(['bus' in a for a in paths], np.int32)
    Bikes = np.array(['touring' in a for a in paths], np.int32)
    Planes = np.array(['airplanes' in a for a in paths], np.int32)
    Cars = np.array(['car' in a for a in paths], np.int32)

    create_SVM(Fighters, bow_d, 'svm_fighters',kernel)
    create_SVM(Motorbikes, bow_d, 'svm_motorbikes',kernel)
    create_SVM(Busses, bow_d, 'svm_busses',kernel)
    create_SVM(Bikes, bow_d, 'svm_bikes',kernel)
    create_SVM(Planes, bow_d, 'svm_planes',kernel)
    create_SVM(Cars, bow_d, 'svm_cars',kernel)

def guess_with_SVMs(test_img_path,voc, normalize):
    names=['fighters','motorbikes','busses','bikes','planes','cars']

    bow_desc = Desc_Extractor(test_img_path,voc)

    if normalize:
        bow_desc = norm(bow_desc)

    matching=[0,0,0,0,0,0]

    for i in range(6):
        svm = cv.ml.SVM_create()
        svm = svm.load('svm_'+names[i])
        matching[i] = svm.predict(bow_desc.astype(np.float32), flags=cv.ml.STAT_MODEL_RAW_OUTPUT)[1][0][0]

    return matching

def kNN(img_data,bow_d,NN):
    train_indexes = indexes('imagedb_train')
    distances = np.sqrt(np.sum(np.square(img_data-bow_d),axis=1))
    tst = img_data-bow_d
    sorted_distances = np.argsort(distances)
    lst = []
    for i in range(NN):
        lst.append(train_indexes[sorted_distances[i]])
    a = np.argmax(np.bincount(np.array(lst)))
    return np.argmax(np.bincount(np.array(lst)))

def check_KNN(dir,NN,voc,bow_d,normalize):

    file = os.listdir(dir)
    results = []
    cntr = 0
    for folders in file:
        imgs = os.listdir(dir + '/' + folders)
        for i in imgs:
            path = dir + '/' + folders + '/' + i
            img_desc = Desc_Extractor(path, voc)
            if normalize:
                img_desc = norm(img_desc)
            result = kNN(img_desc, bow_d, NN)
            if result == cntr:
                results.append(1)
            else:
                results.append(0)
        cntr = cntr + 1

    return sum(results) / len(results)

def check_SVM(dir,voc,normalize):

    lsf = os.listdir(dir)
    success = []
    cntr = 0
    for folders in lsf:
        img_name = os.listdir(dir+ '/' + folders)
        for i in img_name:
            path = dir + '/' + folders + '/' + i
            tmp = np.asarray(guess_with_SVMs(path, voc, normalize))
            if cntr == np.argmin(tmp):
                success.append(1)
            else:
                success.append(0)
        cntr = cntr + 1
    return sum(success) / len(success)

def full_all(voc,bow_d,normalize):
    test_dir = 'imagedb_test'
    NN = [1,3,6,12,18,36]
    knnR = []
    for i in range(len(NN)):
        print('Classifying with ' + str(NN[i]) +'-NN...')
        knnR.append('kNN - '+ str(NN[i]) + ' Neighbours Rate: ' + str(check_KNN(test_dir,NN[i],voc,bow_d,normalize)))

    kernels = [cv.ml.SVM_LINEAR,cv.ml.SVM_INTER,cv.ml.SVM_RBF]
    kernel_name = ['Linear','Histogram Intersection', 'RBF']
    svmR = []
    for i in range(len(kernels)):
        print('Classifying with SVM and ' + kernel_name[i] + ' kernel...')
        create_SVM_one_vs_all(bow_d, kernels[i])
        svmR.append('SVM [' + kernel_name[i] + '] Success Rate: ' + str(check_SVM(test_dir,voc,normalize)))

    return knnR, svmR

def multi_check(dir,normalize = True):

    vocsize = [25,50,100,250,500,800]
    results = []

    for loop in range(len(vocsize)):
        print('Vocabulary size ', vocsize[loop], ' :')
        voc = create_voc(vocsize[loop], dir)
        bow_d = create_bow(voc, dir)

        if normalize:
            bow_d = norm(bow_d)

        knns, svms = full_all(voc, bow_d, normalize)
        results.append('Voc_Size = ' + str(vocsize[loop]))
        for i in knns:
            results.append(i)
        for i in svms:
            results.append(i)

    return results


dir = 'imagedb_train'

#=============================================#
#Full Check:


'''
Full_Results = multi_check(dir,True)

for i in Full_Results:
    print(i)
'''

#=============================================#
#Manual Checking:


#Create vocabulary:
#voc = create_voc(100,dir)

#Or load vocabulary:
voc = load_voc()

#Create Bag of Visual Words
#bow_d=create_bow(voc,dir)

#Or load Bag of Visual Words
bow_d = load_bow()

normalize = True

if normalize:
    bow_d = norm(bow_d)

knns, svms = full_all(voc,bow_d,normalize)
for i in knns:
    print(i)
for i in svms:
    print(i)
#=============================================#