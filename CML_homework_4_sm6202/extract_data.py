# -*- coding: utf-8 -*-
"""
Created on Fri Oct 23 18:41:01 2015

@author: Subhankari
"""

# This function reads a siftgeo binary file
#
"""
% Usage: [v, meta] = siftgeo_read (filename, maxdes)
%   filename    the input filename
%   maxdes      maximum number of descriptors to be loaded
%   (default=unlimited)
%
% Returned values
%   v           the sift descriptors (1 descriptor per line)
%   meta        meta data for each descriptor, i.e., per line:
%               x, y, scale, angle, mi11, mi12, mi21, mi22, cornerness
"""
import numpy as np
#from struct import *

def load_flickr60K_data():
    filename = 'C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\flickr60K_desc_100';
    #content = [line.rstrip('\n') for line in open(filename)]
    data = np.zeros((100,128)) 
    data = np.matrix(data)
    i = 0
    with open(filename) as f:
        for line in f:
            if not line.isspace():
                floats = map(float, line.split())
                floats = np.asarray(floats)
                float1 = np.matrix(floats)
                float1 = np.asarray(float1)
                data[i,:] = float1
                i += 1
    
    return data
        
    
    """ *
    with open(filename) as f:
        content = f.readlines()
    """
    """
    fid = open(filename, 'r')
    fid.seek(0,2)
    n = fid.tell()
    n = fid.tell()
    """
    #return line

"""
def siftgeo_Read(filename,maxdes):
    print filename
    if maxdes == 0:
            #maxdes = 100000000;
            maxdes = 100
    fid = open(filename, 'r')
    fid.seek(0,2)
    n = fid.tell()
    print n
    n = fid.tell() / (9 * 4 + 1 * 4 + 128)
    print n
    fid.seek(0 , 0)
    
    if n > maxdes:
        n = maxdes
    
    meta = np.zeros((n,9),dtype = np.float32)
    v = np.zeros((n,128),dtype = np.float32)

 
    for i in range(n):
        print 'read', fid.read(9)
        data_str = fid.read(9);
        meta[1,:] = unpack('f', data_str)
        meta[1,:] = float(fid.read(9))
      #  d = fid.read(1)
       # v[i,:] = fid.read(d)
        
    fid.close()
   # print v
   # print meta
    
    return v, meta   
    
v,meta = siftgeo_Read('C:\\Users\\Subhankari\\Desktop\\desktop_as_on_13th_april\\text books\\ML\\project\\ImageRetrieval\\Notebook\\dataset\\flickr60K.siftgeo\\flickr60K.siftgeo',0)
print v
print meta
    
"""

"""
function [v, meta] = siftgeo_read (filename, maxdes)

if nargin < 2
  maxdes = 100000000;
end
  
% open the file and count the number of descriptors
fid = fopen (filename, 'r');
 
fseek (fid, 0, 1);
n = ftell (fid) / (9 * 4 + 1 * 4 + 128);
fseek (fid, 0, -1);


if n > maxdes
  n = maxdes;
end;

% first read the meta information associated with the descriptor
meta = zeros (n, 9, 'single');
v = zeros (n, 128, 'single');
d = 0;

% read the elements
for i = 1:n
  meta(i,:) = fread (fid, 9, 'float');
  d = fread (fid, 1, 'int');
  v(i,:) = fread (fid, d, 'uint8=>single');
end

fclose (fid);
"""