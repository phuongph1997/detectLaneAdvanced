import cv2
import numpy as np
import os
import matplotlib.pyplot as plt
def hog(img):
    winSize = (64,64)
    blockSize = (16,16)
    blockStride = (8,8)
    cellSize = (8,8)
    nbins = 9
    derivAperture = 1
    winSigma = 4.
    histogramNormType = 0
    L2HysThreshold = 2.0000000000000001e-01
    gammaCorrection = 0
    nlevels = 64
    hog = cv2.HOGDescriptor(winSize,blockSize,blockStride,cellSize,nbins)
    #compute(img[, winStride[, padding[, locations]]]) -> descriptors
    winStride = (8,8)
    padding = (0,0)
    locations = ((10,20),)
    
    hist = hog.compute(img,winStride,padding)

    return hist
samples = []
labels = []

DATADIR = "/home/hoaiphuong/PycharmProjects/untitled1/Pet"
CATEGORIES= ["Cat","Dog"]
IMG_SIZE = 64
def create_training_data():
    for category in CATEGORIES:  # do dogs and cats

        path = os.path.join(DATADIR,category)  # create path to dogs and cats
        class_num = CATEGORIES.index(category)  # get the classification  (0 or a 1). 0=dog 1=cat

        for img in os.listdir(path):  # iterate over each image per dogs and cats
            try:
                img_array = cv2.imread(os.path.join(path,img) ,cv2.IMREAD_GRAYSCALE)  # convert to array
                new_array = cv2.resize(img_array, (64, 64))  # resize to normalize data size
                #training_data.append([new_array, class_num])  # add this to our training_data
                #plt.imshow(img_array, cmap='gray')  # graph it
                #plt.show()
                hist = hog(new_array)

                samples.append(hist)
                labels.append(class_num)
            except Exception as e:  # in the interest in keeping the output clean...
                pass
            #except OSError as e:
            #    print("OSErrroBad img most likely", e, os.path.join(path,img))
            #except Exception as e:
            #    print("general exception", e, os.path.join(path,img))






create_training_data()
# Get positive samples

# Convert objects to Numpy Objects
samples= np.array(samples, dtype = np.float32)
print(samples.shape)

labels = np.array(labels)

# Shuffle Samples
#rand = np.random.RandomState(321)
#shuffle = rand.permutation(len(samples))
#samples = samples[shuffle]
#labels = labels[shuffle]

# Create SVM classifier
svm = cv2.ml.SVM_create()

# Set up SVM for OpenCV 3
# Set SVM type
svm.setType(cv2.ml.SVM_C_SVC)
# Set SVM Kernel to Radial Basis Function (RBF)
svm.setKernel(cv2.ml.SVM_RBF)
# Set parameter C
svm.setC(1)
# Set parameter Gamma
svm.setGamma(1)

# Train
svm.train(samples, cv2.ml.ROW_SAMPLE, labels)
svm.save('svm_data.dat')

svm = cv2.ml.SVM_load('trainsvm.xml')

img = cv2.imread("/home/hoaiphuong/PycharmProjects/untitled1/6.ppm" ,cv2.IMREAD_GRAYSCALE)  # convert to array
cv2.imshow("anh test",img)
#cv2.waitKey(0)
new_array = cv2.resize(img, (64, 64))  # resize to normalize data size
hist = hog(new_array)
hist =np.reshape(hist,(1,-1))
print(hist.shape)
resp,result = svm.predict(hist)

print(resp)
print(result)
cv2.waitKey(0)

