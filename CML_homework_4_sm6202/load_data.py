# -*- coding: utf-8 -*-
"""
Created on Sun Nov 15 05:59:23 2015

@author: Subhankari
"""

import numpy as np
import urllib, urlparse
import os
from os import listdir
from os.path import isfile, join

def load_data():
    mypath = 'C:/Users/Subhankari/Desktop/desktop_as_on_13th_april/text books/ML/project/ImageRetrieval/Notebook/dataset/unnormalized_tab2';

   # onlyfiles = [ f for f in listdir(mypath) if isfile(join(mypath,f)) ]
    #onlyfiles = [ f for f in listdir(mypath)]
    #print onlyfiles
    data = np.zeros((1,128)) 
    data = np.matrix(data)
    i = 0
    j = 0
    for f in listdir(mypath):
        j = j + 1
        if j % 10  == 0:
            if isfile(join(mypath,f)):
                with open(join(mypath,f)) as f1:
                    for line in f1:
                        if not line.isspace():
                            floats = map(float, line.split())
                            #print floats.shape
                            floats = np.asarray(floats)
                           # print floats.shape
                            float1 = np.matrix(floats)
                            #print float1.shape
                            float1 = np.asarray(float1)
                            #print f
                            #print float1.shape
                            if i > 0:
                                data = np.concatenate((data,float1),axis=0)
                            else:
                                data[i,:] = float1
                            #print i
                            i += 1
    return data
    
#data_load = load_data()
#load_data()
#print data_load