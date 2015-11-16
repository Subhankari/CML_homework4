#********************************************************************************
#CML Homework 1
#Author: Subhankari Mishra
#Feedback:
#       time taken: approx 20 hours(being first hand experience on python programming,
#                   usage of numpy and scikit learn as well as with Machine Learning)
#       Programming: Python programming was not challenging but I believe in case of
#                    data with more than two axis the numpy operations might be a bit confusing.
#       other feedback: It is a great start with Machine Learning, got to learn a lot from this homework.   
#***********************************************************************************

#************************************************************************************
# import libraries and initialize data
#************************************************************************************
import matplotlib.pyplot as plt
import numpy as np
from sklearn import preprocessing

min_max_scaler = preprocessing.MinMaxScaler((-1,1))
max_iter = 10000
random_number = 900
n = 900
#*************************************************************************************

#*************************************************************************************
# Function Declarations and Definitions
#*************************************************************************************
def re_center(data):
    data = preprocessing.scale(data,axis = 0, with_mean=True, with_std=True)    
    return data

#************************************************************************************   

def batch_perceptron(data_X,data_y,data_test_X,data_test_y,w,max_iter):        
    data_err = np.zeros(max_iter)
    data_test_err = np.zeros(max_iter)
    data_y = np.mat(data_y).T
    data_test_y = np.mat(data_test_y).T
    c = 0
    for i in range(max_iter):
            #w.x
            data_wx = np.dot(w,np.transpose(data_X))
            #y*w.x
            data_ywx = np.zeros(data_y.shape)
            data_y_arr = np.squeeze(np.asarray(data_y))
            data_wx = np.squeeze(np.asarray(data_wx))
            for k in range(data_y_arr.shape[0]):
                data_ywx[k] = np.multiply(data_y_arr[k],data_wx[k])   
            wrong_mask = data_ywx <= 0
            score = float(np.sum(wrong_mask))/float(data_X.shape[0])
            data_err[i] = score
           
            if(score > 0):            
                arr_mask_value = np.arange(data_X.shape[0])
                arr_mask_value = np.mat(arr_mask_value).T
                arr_mask_value = arr_mask_value[wrong_mask]
                arr_mask_value = np.squeeze(np.asarray(arr_mask_value),axis = 0)
                for j in range(arr_mask_value.shape[0]):
                    val = arr_mask_value[j]
                    xy = np.multiply(data_y[val], data_X[val])
                    break;
                w = w + xy
                c = i + 1
            
            data_test_wx = np.dot(w,np.transpose(data_test_X))
            #y*w.x
            data_test_ywx = np.zeros(data_test_y.shape)
            data_test_y_arr = np.squeeze(np.asarray(data_test_y))
            data_test_wx = np.squeeze(np.asarray(data_test_wx))
            for k in range(data_test_y_arr.shape[0]):
                data_test_ywx[k] = np.multiply(data_test_y_arr[k],data_test_wx[k])   
            wrong_mask_test = data_ywx <= 0
            score = float(np.sum(wrong_mask_test))/float(data_test_X.shape[0])
            data_test_err[i] = score
               
    print "Batch Perceptron converged at iteration: ", c                
    return w, data_err,data_test_err

#**************************************************************************************    

def modif_perceptron(data_X,data_y,data_test_X,data_test_y,w,max_iter,n):        
    data_err = np.zeros(max_iter)
    data_test_err = np.zeros(max_iter)
    data_y = np.mat(data_y).T
    data_test_y = np.mat(data_test_y).T 
    c = 0
    for i in range(max_iter):
            #w.x
            data_wx = np.dot(w,np.transpose(data_X))
            #y*w.x
            data_ywx = np.zeros(data_y.shape)
            data_y_arr = np.squeeze(np.asarray(data_y))
            data_wx = np.squeeze(np.asarray(data_wx))
            for k in range(data_y_arr.shape[0]):
                data_ywx[k] = np.multiply(data_y_arr[k],data_wx[k])   
            wrong_mask = data_ywx <= 0

            score = float(np.sum(wrong_mask))/float(data_X.shape[0])
            data_err[i] = score
            
            if(score > 0):            
                arr_mask_value = np.arange(data_X.shape[0])
                arr_mask_value = np.mat(arr_mask_value).T
                arr_mask_value = arr_mask_value[wrong_mask]
                arr_mask_value = np.squeeze(np.asarray(arr_mask_value),axis = 0)
                for j in range(arr_mask_value.shape[0]):
                    val = arr_mask_value[j]
                    xy = np.multiply(data_y[val], data_X[val])
                    break;
                w = w + np.multiply(n,xy)
                c = i + 1
            
            data_test_wx = np.dot(w,np.transpose(data_test_X))
            #y*w.x
            data_test_ywx = np.zeros(data_test_y.shape)
            data_test_y_arr = np.squeeze(np.asarray(data_test_y))
            data_test_wx = np.squeeze(np.asarray(data_test_wx))
            for k in range(data_test_y_arr.shape[0]):
                data_test_ywx[k] = np.multiply(data_test_y_arr[k],data_test_wx[k])   
            wrong_mask_test = data_ywx <= 0
            score = float(np.sum(wrong_mask_test))/float(data_test_X.shape[0])
            data_test_err[i] = score
    
    
    print "Modified Batch Perceptron converged at iteration: ", c  
    return w, data_err,data_test_err
    
#***************************************************************************************
    
#**************************************************************************************
#Load Iris dataset and make three datasets with each different pairs of classes    
#**********************************************************************************

from sklearn.datasets import load_iris
iris = load_iris()
X,y = iris.data , iris.target

Xy_tmp = np.column_stack((X,y))
Xy_tmp1 = Xy_tmp[(Xy_tmp[:,4] != 1)]
Xy_tmp2 = Xy_tmp[(Xy_tmp[:,4] != 0)]
Xy_tmp3 = Xy_tmp[(Xy_tmp[:,4] != 2)]

X1 = Xy_tmp1[:,0:4]
y1 = Xy_tmp1[:,4]

X2 = Xy_tmp2[:,0:4]
y2 = Xy_tmp2[:,4]

X3 = Xy_tmp3[:,0:4]
y3 = Xy_tmp3[:,4]

rng = np.random.RandomState(random_number)


permutation1 = rng.permutation(len(X1))
permutation2 = rng.permutation(len(X2))
permutation3 = rng.permutation(len(X3))

X1, y1 = X1[permutation1], y1[permutation1]
X2, y2 = X2[permutation2], y2[permutation2]
X3, y3 = X3[permutation3], y3[permutation3]


from sklearn.cross_validation import train_test_split
train_X1, test_X1, train_y1, test_y1 = train_test_split(X1, y1, train_size=0.5, random_state=random_number)
train_X2, test_X2, train_y2, test_y2 = train_test_split(X2, y2, train_size=0.5, random_state=random_number)
train_X3, test_X3, train_y3, test_y3 = train_test_split(X3, y3, train_size=0.5, random_state=random_number)

#*********************************************************************************


#*********************************************************************************
#processing and analysing data set1
#**********************************************************************************
print "********************** Dataset 1 (Iris class 0 & 2) ******************"
print ""
train_X1 = re_center(train_X1)
test_X1 = re_center(test_X1)
# the mean after re-centering is close to zero and standard deviation is 1 but
#feature values range from 3 to -3.
print "Mean on train data after re-centering: ", train_X1.mean(axis = 0)
print "Standard deviation on train data after re-centering: ", train_X1.std(axis = 0)
print "Mean on test data after re-centering: ", test_X1.mean(axis = 0)
print "Standard deviation on test data after re-centering: ", test_X1.std(axis = 0)

plt.scatter(train_X1[:,0], train_X1[:,1], c= ['c','g'])
plt.scatter(train_X1[:,2], train_X1[:,3], c=['b','r'])
plt.title("Feature plot after re-centering: Dataset1 (Iris dataset class 0 & 2)")
plt.show()

train_X_minmax1 = min_max_scaler.fit_transform(train_X1)
test_X_minmax1 = min_max_scaler.fit_transform(test_X1)
train_y_minmax1 = min_max_scaler.fit_transform(train_y1)
test_y_minmax1 = min_max_scaler.fit_transform(test_y1)
#after using re-scaling using Min_Max_Scaler the mean is no more zero for the 
#data and the standard variation also changes although the data lies in the range of [1,-1]
print "Mean on train data after re-scaling: ", train_X_minmax1.mean(axis = 0)
print "Standard deviation on train data after re-scaling: ", train_X_minmax1.std(axis = 0)
print "Mean on test data after re-scaling: ", test_X_minmax1.mean(axis = 0)
print "Standard deviation on test data after re-scaling: ", test_X_minmax1.std(axis = 0)

plt.scatter(train_X_minmax1[:,0], train_X_minmax1[:,1], c= ['c','g'])
plt.scatter(train_X_minmax1[:,2], train_X_minmax1[:,3], c=['b','r'])
plt.title("Feature plot after re-scaling: Dataset1 (Iris dataset class 0 & 2)")
plt.show()

d = train_X1.shape[1]
w1 = np.zeros(d)
w1,outbtr1,outbtt1 = batch_perceptron(train_X1,train_y_minmax1,test_X1,test_y_minmax1,w1,max_iter,)

plt.plot(np.arange(outbtr1.shape[0]),outbtr1,'c',linewidth=2.5, linestyle="-",label = 'train error')
plt.plot(np.arange(outbtt1.shape[0]),outbtt1,'g',linewidth=2.5, linestyle="-",label = 'test error')

plt.xlabel("Iterations")
plt.ylabel("% Misclassification Error")
plt.title("Batch Perceptron: Dataset1 (Iris dataset class 0 & 2)")
plt.legend(loc='upper left')
plt.show()


w1 = np.zeros(d)
w1,outbtr1,outbtt1 = batch_perceptron(train_X_minmax1,train_y_minmax1,test_X_minmax1,test_y_minmax1,w1,max_iter)


plt.plot(np.arange(outbtr1.shape[0]),outbtr1,'c',linewidth=2.5, linestyle="-",label = 'train error')
plt.plot(np.arange(outbtt1.shape[0]),outbtt1,'g',linewidth=2.5, linestyle="-",label = 'test error')

plt.xlabel("Iterations")
plt.ylabel("% Misclassification Error")
plt.title("Batch Perceptron with re-scaled input using Min_Max_Scaler: Dataset1 (Iris dataset class 0 & 2)")
plt.legend(loc='upper left')
plt.show()


w1 = np.zeros(d)
w1,outmtr1,outmtt1 = modif_perceptron(train_X1,train_y_minmax1,test_X1,test_y_minmax1,w1,max_iter,n)
#w1,outmtt1 = modif_perceptron(test_X1,test_y_minmax1,w1,max_iter,n,0)


plt.plot(np.arange(outmtr1.shape[0]),outmtr1,'c',linewidth=2.5, linestyle="-",label = 'train error')
plt.plot(np.arange(outmtt1.shape[0]),outmtt1,'g',linewidth=2.5, linestyle="-",label = 'test error')

plt.xlabel("Iterations")
plt.ylabel("% Misclassification Error")
plt.title("Modified Batch Perceptron: Dataset1 (Iris dataset class 0 & 2)")
plt.legend(loc='upper left')
plt.show()

#**************************************************************************************************

#*********************************************************************************
#processing and analysing data set2
#**********************************************************************************
print "********************** Dataset 2 (Iris class 1 & 2) ******************"
print ""
train_X2 = re_center(train_X2)
test_X2 = re_center(test_X2)

# the mean after re-centering is close to zero and standard deviation is 1 but
#feature values range from 3 to -3.
print "Mean on train data after re-centering: ", train_X2.mean(axis = 0)
print "Standard deviation on train data after re-centering: ", train_X2.std(axis = 0)
print "Mean on test data after re-centering: ", test_X2.mean(axis = 0)
print "Standard deviation on test data after re-centering: ", test_X2.std(axis = 0)

plt.scatter(train_X2[:,0],train_X2[:,1], c=['c','g'])
plt.scatter(train_X2[:,2],train_X2[:,3], c=['b','r'])
plt.title("Feature Plot after re-centering: Dataset2 (Iris dataset class 1 & 2)")
plt.show()

train_y_minmax2 = min_max_scaler.fit_transform(train_y2)
train_X_minmax2 = min_max_scaler.fit_transform(train_X2)
test_X_minmax2 = min_max_scaler.fit_transform(test_X2)
test_y_minmax2 = min_max_scaler.fit_transform(test_y2)

#after using re-scaling using Min_Max_Scaler the mean is no more zero for the 
#data and the standard variation also changes although the data lies in the
#range of [1,-1] and also converges earlier than the only re-centered data
print "Mean on train data after re-scaling: ", train_X_minmax2.mean(axis = 0)
print "Standard deviation on train data after re-scaling: ", train_X_minmax2.std(axis = 0)
print "Mean on test data after re-scaling: ", test_X_minmax2.mean(axis = 0)
print "Standard deviation on test data after re-scaling: ", test_X_minmax2.std(axis = 0)

plt.scatter(train_X_minmax2[:,0], train_X_minmax2[:,1], c= ['c','g'])
plt.scatter(train_X_minmax2[:,2], train_X_minmax2[:,3], c=['b','r'])
plt.title("Feature plot after re-scaling: Dataset2 (Iris dataset class 1 & 2)")
plt.show()


d = train_X2.shape[1]

w2 = np.zeros(d)
w2,outbtr2,outbtt2 = batch_perceptron(train_X2,train_y_minmax2,test_X2,test_y_minmax2,w2,max_iter)


plt.plot(np.arange(outbtr2.shape[0]),outbtr2,'c',linewidth=2.5, linestyle="-",label = 'train error')
plt.plot(np.arange(outbtt2.shape[0]),outbtt2,'g',linewidth=2.5, linestyle="-",label = 'test error')

plt.xlabel("Iterations")
plt.ylabel("% Misclassification Error")
plt.title("Batch Perceptron: Dataset2 (Iris dataset class 1 & 2)")
plt.legend(loc='upper left')
plt.show()

w2 = np.zeros(d)
w2,outbtr2,outbtt2 = batch_perceptron(train_X_minmax2,train_y_minmax2,test_X_minmax2,test_y_minmax2,w2,max_iter)


plt.plot(np.arange(outbtr2.shape[0]),outbtr2,'c',linewidth=2.5, linestyle="-",label = 'train error')
plt.plot(np.arange(outbtt2.shape[0]),outbtt2,'g',linewidth=2.5, linestyle="-",label = 'test error')

plt.xlabel("Iterations")
plt.ylabel("% Misclassification Error")
plt.title("Batch Perceptron with re-scaled input using Min_Max_Scaler: Dataset2 (Iris dataset class 1 & 2)")
plt.legend(loc='upper left')
plt.show()

w2 = np.zeros(d)
w2,outmtr2,outmtt2 = modif_perceptron(train_X2,train_y_minmax2,test_X2,test_y_minmax2,w2,max_iter,n)
#w2,outmtt2 = modif_perceptron(test_X2,test_y_minmax2,w2,max_iter,n,0)

plt.plot(np.arange(outmtr2.shape[0]),outmtr2,'c',linewidth=2.5, linestyle="-",label = 'train error')
plt.plot(np.arange(outmtt2.shape[0]),outmtt2,'g',linewidth=2.5, linestyle="-",label = 'test error')

plt.xlabel("Iterations")
plt.ylabel("% Misclassification Error")
plt.title("Modified Batch Perceptron: Dataset2 (Iris dataset class 1 & 2)")
plt.legend(loc='upper left')
plt.show()

#**************************************************************************************************

#*********************************************************************************
#processing and analysing data set3
#**********************************************************************************
print "********************** Dataset 2 (Iris class 0 & 1) ******************"
print ""
train_X3 = re_center(train_X3)
test_X3 = re_center(test_X3)
# the mean after re-centering is close to zero and standard deviation is 1 but
#feature values range from 3 to -3.
print "Mean on train data after re-centering: ", train_X3.mean(axis = 0)
print "Standard deviation on train data after re-centering: ", train_X3.std(axis = 0)
print "Mean on test data after re-centering: ", test_X3.mean(axis = 0)
print "Standard deviation on test data after re-centering: ", test_X3.std(axis = 0)

plt.scatter(train_X3[:,0], train_X3[:,1], c= ['c','g'])
plt.scatter(train_X3[:,2], train_X3[:,3], c=['b','r'])
plt.title("Feature plot after re-centering: Dataset1 (Iris dataset class 0 & 1)")
plt.show()

train_y_minmax3 = min_max_scaler.fit_transform(train_y3)
train_X_minmax3 = min_max_scaler.fit_transform(train_X3)
test_X_minmax3 = min_max_scaler.fit_transform(test_X3)
test_y_minmax3 = min_max_scaler.fit_transform(test_y3)

#after using re-scaling using Min_Max_Scaler the mean is no more zero for the 
#data and the standard variation also changes although the data lies in the
#range of [1,-1] and also converges after the same number of iterations as the only re-centered data
print "Mean on train data after re-scaling: ", train_X_minmax3.mean(axis = 0)
print "Standard deviation on train data after re-scaling: ", train_X_minmax3.std(axis = 0)
print "Mean on test data after re-scaling: ", test_X_minmax3.mean(axis = 0)
print "Standard deviation on test data after re-scaling: ", test_X_minmax3.std(axis = 0)

plt.scatter(train_X_minmax3[:,0], train_X_minmax3[:,1], c= ['c','g'])
plt.scatter(train_X_minmax3[:,2], train_X_minmax3[:,3], c=['b','r'])
plt.title("Feature plot after re-scaling: Dataset2 (Iris dataset class 0 & 1)")
plt.show()

d = train_X3.shape[1]
w3 = np.zeros(d)
w3,outbtr3,outbtt3 = batch_perceptron(train_X3,train_y_minmax3,test_X3,test_y_minmax3,w3,max_iter)
#w3,outbtt3 = batch_perceptron(test_X3,test_y_minmax3,w3,max_iter,0)

plt.plot(np.arange(outbtr3.shape[0]),outbtr3,'c',linewidth=2.5, linestyle="-",label = 'train error')
plt.plot(np.arange(outbtt3.shape[0]),outbtt3,'g',linewidth=2.5, linestyle="-",label = 'test error')

plt.xlabel("Iterations")
plt.ylabel("% Misclassification Error")
plt.title("Batch Perceptron: Dataset3 (Iris dataset class 0 & 1)")
plt.legend(loc='upper left')
plt.show()

w3 = np.zeros(d)
w3,outbtr3,outbtt3 = batch_perceptron(train_X_minmax3,train_y_minmax3,test_X_minmax3,test_y_minmax3,w3,max_iter)


plt.plot(np.arange(outbtr2.shape[0]),outbtr2,'c',linewidth=2.5, linestyle="-",label = 'train error')
plt.plot(np.arange(outbtt2.shape[0]),outbtt2,'g',linewidth=2.5, linestyle="-",label = 'test error')

plt.xlabel("Iterations")
plt.ylabel("% Misclassification Error")
plt.title("Batch Perceptron with re-scaled input using Min_Max_Scaler: Dataset2 (Iris dataset class 1 & 2)")
plt.legend(loc='upper left')
plt.show()

w3 = np.zeros(d)
w3,outmtr3,outmtt3 = modif_perceptron(train_X3,train_y_minmax3,test_X3,test_y_minmax3,w3,max_iter,n)
#w3,outmtt3 = modif_perceptron(test_X3,test_y_minmax3,w3,max_iter,n,0)


plt.plot(np.arange(outmtr3.shape[0]),outmtr3,'c',linewidth=2.5, linestyle="-",label = 'train error')
plt.plot(np.arange(outmtt3.shape[0]),outmtt3,'g',linewidth=2.5, linestyle="-",label = 'test error')

plt.xlabel("Iterations")
plt.ylabel("% Misclassification Error")
plt.title("Modified Batch Perceptron: Dataset3 (Iris dataset class 0 & 1)")
plt.legend(loc='upper left')
plt.show()

#**********************************************************************************
#Extra Dataset: Titanic
#**********************************************************************************
#load data
from helpers import load_titanic
keys, train_data, test_data, train_labels, test_labels = load_titanic(
    test_size=0.5, feature_skip_tuple=(), random_state=random_number)
    
#*************************************************************************************   
#process dataset and plot
#*************************************************************************************
print "********************** Dataset 4 (Titanic) ***************************"
print ""
train_data = re_center(train_data)
train_data_minmax = min_max_scaler.fit_transform(train_data)
train_labels_minmax = min_max_scaler.fit_transform(train_labels.astype(float))
test_data = re_center(test_data)
test_data_minmax = min_max_scaler.fit_transform(test_data)
test_labels_minmax = min_max_scaler.fit_transform(test_labels.astype(float))

print "Mean on train data after re-centering: ", train_data.mean(axis = 0)
print "Standard deviation on train data after re-centering: ", train_data.std(axis = 0)
print "Mean on test data after re-centering: ", test_data.mean(axis = 0)
print "Standard deviation on test data after re-centering: ", test_data.std(axis = 0)

#after using re-scaling using Min_Max_Scaler the mean is no more zero for the 
#data and the standard variation also changes and also but algorithm never converges
print "Mean on train data after re-scaling: ", train_data_minmax.mean(axis = 0)
print "Standard deviation on train data after re-scaling: ", train_data_minmax.std(axis = 0)
print "Mean on test data after re-scaling: ", test_data_minmax.mean(axis = 0)
print "Standard deviation on test data after re-scaling: ", test_data_minmax.std(axis = 0)

d = train_data.shape[1]
w4 = np.zeros(d)
w4,outbtr4,outbtt4 = batch_perceptron(train_data,train_labels_minmax,test_data,test_labels_minmax,w4,max_iter)
#w4,outbtt4 = batch_perceptron(test_data,test_labels_minmax,w4,max_iter,0)

plt.plot(np.arange(outbtr4.shape[0]),outbtr4,'c',linewidth=2.5, linestyle="-",label = 'train error')
plt.plot(np.arange(outbtt4.shape[0]),outbtt4,'g',linewidth=2.5, linestyle="-",label = 'test error')

plt.xlabel("Iterations")
plt.ylabel("% Misclassification Error")
plt.title("Batch Perceptron: Dataset4 (Titanic)")
plt.legend(loc='upper left')
plt.show()

w4 = np.zeros(d)
w4,outbtr4,outbtt4 = batch_perceptron(train_data_minmax,train_labels_minmax,test_data_minmax,test_labels_minmax,w4,max_iter)


plt.plot(np.arange(outbtr2.shape[0]),outbtr2,'c',linewidth=2.5, linestyle="-",label = 'train error')
plt.plot(np.arange(outbtt2.shape[0]),outbtt2,'g',linewidth=2.5, linestyle="-",label = 'test error')

plt.xlabel("Iterations")
plt.ylabel("% Misclassification Error")
plt.title("Batch Perceptron with re-scaled input using Min_Max_Scaler: Dataset4 (Titanic)")
plt.legend(loc='upper left')
plt.show()


w4 = np.zeros(d)
w4,outmtr4,outmtt4 = modif_perceptron(train_data,train_labels_minmax,test_data,test_labels_minmax,w4,max_iter,n)
#w4,outmtt4 = modif_perceptron(test_data,test_labels_minmax,w4,max_iter,n,0)

plt.plot(np.arange(outmtr4.shape[0]),outmtr4,'c',linewidth=2.5, linestyle="-",label = 'train error')
plt.plot(np.arange(outmtt4.shape[0]),outmtt4,'g',linewidth=2.5, linestyle="-",label = 'test error')

plt.xlabel("Iterations")
plt.ylabel("% Misclassification Error")
plt.title("Modified Batch Perceptron: Dataset4 (Titanic)")
plt.legend(loc='upper left')
plt.show()

#***************************************************************************************
#Question 2:
#  Training Data vs Iterations:
#    On plotting the misclassfication error against the number of iterations
#    it is observed that the error decreases as we update the weight vector
#    over each outer iteration and finally the weight vector achieves a value
#    which is able to classify the data with any errors.
#   
#   The curve starts at some maximum i.e. the max numbers of misclassified 
#   examples for a given initial weight vector. In our case it is initialized
#   to zero i.e. w = (0,0,..,0) of d dimensions so the curve starts with 
#   100% misclassification error and then gradually decreases to zero as 
#   the weight vectors gets udated.
    
#    For some datasets which are clearly linearly separable as we can see in 
#    the feature plot in our case Dataset1 and Dataset3 the curve descents to
#    zero very fast i.e. in one or two iterations with mostly the local
#    minimum values same as the global minimum values.
#    And it also converges everytime irrespective of the randon seed used for
#    data shuffling while selecting the test and train split.
    
#    Where as for some datasets which are not clearly linearly separable
#    for example Dataset2 the curve takes time to reach zero and also has
#    multiple local minima. Sometimes the curve never descents to zero
#    depending on random value used for the data shuffling.
#    for example in our case for random seed  4171990,1211986,1286 and 250 
#     the curve never reached zero.
#    
#    Whereas for random seed 100,900 and 189, the algorithm converged i.e.
#    the curve reached 0 after 814, 251 and 119 iterations respectively.
#    Similarly, for random seed 1, 10, 550, 850, 1000 ,2000, 20000 and 15642 the 
#    algorithm converged after 19, 6, 11, 13, 6, 18, 15 and 7 respectively.
#
#   For Dataset4 it was observed that it always convered after 343 iterations
#   irrespective of the random seed value.
    
#    During the last iterations it is observed that the misclassification error
#    become constant for some easily linearly separable datasets like Dataset1
#    Dataset3 and for Dataset2 for some specific shuffling order it becomes
#    constant whereas of some others it keeps on moving up and down and never
#    becomes constant.
    
#  Testing Vs Iterations
    
#    The curve ploted with test data while working on the training data
#    behaves the exactly the same way as the curve on training data behaves.
     
#  Max-iterations:
#     For all the datasets it is not necessary to run the algorithm 10000
#     times. Few datasets converge very soon for most of the shuffling
#     orders which would hardly need 10 iterations to converge where as
#     there are other datasets which somethimes doesnot even converge after 
#     10000 iterations given few specific shuffling order, which might
#     might converge early for some other shuffling order.
#     So it completely depends on the data how many iteration it needs to be
#     run to get the optimal weight vector to achieve the least
#     misclassification error.
#         
#     If we stop the iteration very early for some datasets we might not get
#     the most optimal weight vector value as result might get more
#     misclassification errors on the test data.
     
     
# Question 3:   
#    As per the observations on these 4 datasets, there was no impact of the
#    parameter 'Eta' irrespective of its value wheter increased or decreased
#    or set to 1 the curves remain same as the curve obtained by Batch
#    perceptron and convergence is attained after same number of iterations
#    for the respective datasets.     
#
#