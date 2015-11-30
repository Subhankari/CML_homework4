# -*- coding: utf-8 -*-
"""
Created on Sun Nov 29 01:45:11 2015

@author: subhankari
"""

import matplotlib.pyplot as plt
import numpy as np
import time
import cProfile
from os import listdir
from os.path import isfile, join
from sklearn import preprocessing
from sklearn import decomposition
from sklearn.cross_validation import train_test_split
from scipy.spatial.distance import cdist
from my_kmeans_new import my_kmeans_new

def VLAD(C,x):
    Y = cdist(x,C,metric='euclidean', p=2, V=None, VI=None, w=None)
    clust_index = np.argmin(Y,axis = 1)
    res_vec = np.zeros(x.shape[1])
    for i in range(Y.shape[1]):
        x_i = x[clust_index == i]
        x_i = np.add(x_i,np.multiply(-1,C[i]))
        if i == 0:
            res_vec = x_i
        else:
            res_vec = np.concatenate((res_vec,x_i),axis = 0)
    return np.sum((res_vec),axis = 0)
    
def build_bof():
    
    f50_features = '/work/sm6202/ML/data/features_10_100_pca'
    with open(f50_features) as f:
        data = np.zeros((1,65))
        data = np.matrix(data)
        i = 0
        for line in f:
            floats = map(float, line.split())
            floats = np.asarray(floats)
            float1 = np.matrix(floats)
            float1 = np.asarray(float1)
            if(i == 0):
                data = float1
            else:
                data = np.concatenate((data,float1),axis = 0)
            i = i + 1
    centroids,l = my_kmeans_new(20,400,data,200)
    print centroids 
    vlad_features = '/work/sm6202/ML/data/vlad_features_10_100'
    pca_features = '/work/sm6202/ML/data/pca_features_10_100'
    
    i = 0
    j = 0

    #print data
    with open(vlad_features,'a+') as f_handle:

        for filename in listdir(pca_features):
            data = np.zeros((1,65)) 
            data = np.matrix(data)
            i = i + 1
            print i
            j = 0
            print filename
            if i > 100:
                break
            with open(join(pca_features,filename)) as f:
                for line in f:
                    j = j + 1
                    #if j > 1:
                    #    break
                    #print line
                    if not line.isspace():
                        floats = map(float, line.split())
                        floats = np.asarray(floats)
                        float1 = np.matrix(floats)
                        float1 = np.asarray(float1)
                        sign = np.sign(float1)
                        abs_val = np.absolute(float1)
                        #print abs_val
                        abs_val = abs_val**(0.5)
                        float1 = np.multiply(abs_val,sign)

                        #print float1
                        if j == 1:
                            data = float1
                        else:
                            data = np.concatenate((data,float1),axis = 0)
            vlad_vec = VLAD(centroids,data)
            file_n = np.zeros((1,1));
            file_n = np.matrix(file_n)
            file_n[0,0] = filename
	    vlad_vec = np.matrix(vlad_vec)
#	    print file_n.shape
#	    print vlad_vec.shape
            vlad_vec = preprocessing.normalize(vlad_vec,norm='l2')
	    vlad_vec = np.concatenate((file_n,vlad_vec),axis = 1)
#            print vlad_vec
            np.savetxt(f_handle,vlad_vec,fmt='%.4f')
    
    
    
build_bof()    
    
    
    
    
