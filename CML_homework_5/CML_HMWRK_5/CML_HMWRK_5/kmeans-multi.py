# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
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

def mykmeans_multi(datapoints,n_cluster,max_iter,multi_iter):
    G_kmeans = 0
    final_centroids = np.zeros((n_cluster,datapoints.shape[1]))
    final_labels = np.zeros((datapoints.shape[0]))
    index = np.zeros((multi_iter))
    distortion = np.zeros((multi_iter))
    for i in range(0,multi_iter):
        G_kmeans_prime = 0
        centroids,labels = mykmeans(datapoints,n_cluster,max_iter)
        if i == 0:
            final_centroids = centroids
            for j in range(0,n_cluster):
                values = datapoints[labels == j]
                centroid = np.mat(centroids[j,:])
                G_kmeans_prime = G_kmeans_prime + np.sum(cdist(values,centroid,metric='euclidean', p=2, V=None, VI=None, w=None),axis = 0)
            G_kmeans = G_kmeans_prime
            final_labels = labels
        else:
            for j in range(0,n_cluster):
                values = datapoints[labels == j]
                centroid = np.mat(centroids[j,:])
                G_kmeans_prime = G_kmeans_prime + np.sum(cdist(values,centroid,metric='euclidean', p=2, V=None, VI=None, w=None),axis = 0)
            if G_kmeans_prime < G_kmeans:
                final_centroids = centroids
                final_labels = labels
                G_kmeans = G_kmeans_prime
        index[i] = i
        distortion[i] = G_kmeans
    plt.figure()    
    plt.plot(np.arange(multi_iter),distortion,'c',linewidth=2.5, linestyle="-",label = 'distortion') 
    plt.title("mykmeans_multi distortion plot")
    return final_centroids,final_labels
            
            
        
    