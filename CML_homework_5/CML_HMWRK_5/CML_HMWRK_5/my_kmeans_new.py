# -*- coding: utf-8 -*-
"""
Created on Sun Nov 29 00:13:42 2015

@author: subhankari
"""

import matplotlib.pyplot as plt
import numpy as np
import time
import cProfile
from sklearn import preprocessing
from sklearn import decomposition
from sklearn.cross_validation import train_test_split
from scipy.spatial.distance import cdist
from kmeansplusplus import mykmeansplusplus

def online_k_means(k,b,t,X_in):
    random_number = 11232015
    random_num = np.random.randint(X_in.shape[0], size =300 )
    rng = np.random.RandomState(random_number)
    permutation1 = rng.permutation(len(random_num))
    random_num = random_num[permutation1]
    x_input = X_in[random_num]
    c,l = mykmeansplusplus(x_input,k,t)
    v = np.zeros((k))
    for i in range(t):
        random_num = np.random.randint(X_in.shape[0], size = b)
        rng = np.random.RandomState(random_number)
        permutation1 = rng.permutation(len(random_num))
        random_num = random_num[permutation1]
        M = X_in[random_num]
        Y = cdist(M,c,metric='euclidean', p=2, V=None, VI=None, w=None)
        clust_index = np.argmin(Y,axis = 1)
        for i in range(M.shape[0]):
            c_in = clust_index[i]
            v[c_in] += 1
            ita = 1 / v[c_in]
            c[c_in] = np.add(np.multiply((1 - ita),c[c_in]),np.multiply(ita,M[i]))
    Y_l = cdist(X_in,c,metric='euclidean', p=2, V=None, VI=None, w=None)
    l = np.argmin(Y_l,axis = 1)        
    return c,l
    
def my_kmeans_new(k,t,x,b):
    print x.shape[0]
    if x.shape[0] > 1000:
        return online_k_means(k,b,t,x)
    else:
        return mykmeansplusplus(x,k,t)