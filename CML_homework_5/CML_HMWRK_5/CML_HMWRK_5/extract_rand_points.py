# -*- coding: utf-8 -*-
"""
Created on Sun Nov 29 02:11:45 2015

@author: subhankari
"""

import matplotlib.pyplot as plt
import numpy as np
import os,sys
from PIL import Image
from os import listdir
from os.path import isfile, join
import time
import cProfile
import matplotlib.image as mpimg
from sklearn import preprocessing

norm1_path = '/work/sm6202/ML/data/norm_features'
norm2_path = '/work/sm6202/ML/data/norm_features2'
f10_features = '/work/sm6202/ML/data/20_features_200'

i = 0
j = 0

with open(f10_features,'a+') as f_handle:

    for filename in listdir(norm1_path):
        data = np.zeros((1,128)) 
        data = np.matrix(data)
        if i > 200:
            break
        i = i + 1
        print j
        j = 0
        print filename
        #if i > 3:
        #    break
        with open(join(norm1_path,filename)) as f:
            for line in f:
                j = j + 1
                #if j > 1:
                #    break
                #print line
                if not line.isspace():
                    floats = map(float, line.split())
                    #print floats
                    floats = np.asarray(floats)
                    float1 = np.matrix(floats)
                    float1 = np.asarray(float1)
                    #sign = np.sign(float1)
                    #abs_val = np.absolute(float1)
                    #print abs_val
                    #abs_val = abs_val**(0.5)
                    #float1 = np.multiply(abs_val,sign)

                    #print float1
                    if j == 1:
                        data = float1
                    else:
                        data = np.concatenate((data,float1),axis = 0)
        #data = preprocessing.normalize(data, norm='l2')
        
        random_10 = np.random.randint(data.shape[0], size = 20)
        random_number = 11212015
        rng = np.random.RandomState(random_number)
        permutation1 = rng.permutation(len(random_10))
        random_10 = random_10[permutation1]
        data_10 = data[random_10]
	print data_10
        np.savetxt(f_handle,data_10,fmt='%.4f')
