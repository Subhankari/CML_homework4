# -*- coding: utf-8 -*-
"""
Created on Sat Nov 28 20:12:02 2015

@author: subhankari
"""

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
from scipy.spatial.distance import cdist
from scipy.cluster.vq import vq
from sklearn import preprocessing
from sklearn import decomposition
import urllib, urlparse
import os
from os import listdir
from os.path import isfile, join
import time
import cProfile

def mykmeansplusplus (datapoints,n_cluster,max_iter):
    data_x_shape = datapoints.shape[0]
    random_index = np.random.randint(data_x_shape, size =1)
    initial_clust_center = datapoints[random_index]
    weight = np.zeros((datapoints.shape[0]))
    index = np.zeros((datapoints.shape[0]))
    for i in range(0,(n_cluster - 1)):
        Y= cdist(datapoints, initial_clust_center, metric='euclidean', p=2, V=None, VI=None, w=None)
        min_dist = np.amin(Y, axis=1) 
        min_center = np.argmin(Y, axis=1)
        min_dist_sum = np.sum(min_dist, axis=0)
        min_squared_dist_sum = np.sum(min_dist**2, axis=0)
        min_dist = min_dist**2
        min_dist_prob = min_dist / min_squared_dist_sum
        next_index = np.random.choice(datapoints.shape[0],1,p=min_dist_prob)
        initial_clust_center = np.concatenate((initial_clust_center,datapoints[next_index]),axis=0)
    initial_class = np.zeros((datapoints.shape[0]))
    if max_iter == 0:
        max_iter = 100000
        
    for j in range(0,max_iter):
        Z= cdist(datapoints, initial_clust_center, metric='euclidean', p=2, V=None, VI=None, w=None)
        target_dist = np.amin(Z, axis=1)  
        target_class = np.argmin(Z, axis=1)
        for i in range(0,n_cluster):
            values = datapoints[target_class == i]
            initial_clust_center[i,:] = np.mean(values, axis = 0)
        if np.array_equal(initial_class,target_class):
            print j
            print 'converged'
            break
        else:
            initial_class = target_class

    return initial_clust_center, target_class  
        
    
    
    
    