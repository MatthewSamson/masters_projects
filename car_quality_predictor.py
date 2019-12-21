

'''
Neural network based on the car evaluation dataset in the UCI repository to classify acceptability based on car traits

author: matthew
'''

#import section
import pandas as pd
import numpy as np
import datetime

import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from keras.models import Sequential
from keras.layers import Dense
import keras

#%matplotlib inline



# Load the data from the CSV file
carEvalDataS = pd.read_csv('carEvalExtra.csv',names=['buyPrice','maintainPrice','noOfDoors','noOfPersons','bootCapacity','carSafety','carAcc'])

#one-hot encoding of output label
carEvalDataS['unacc'] = (carEvalDataS['carAcc']=='unacc').astype(int) 
carEvalDataS['acc'] = (carEvalDataS['carAcc']=='acc').astype(int)     
carEvalDataS['good'] = (carEvalDataS['carAcc']=='good').astype(int)   
carEvalDataS['vgood'] = (carEvalDataS['carAcc']=='vgood').astype(int) 



#split to train and test set
trainSet, testSet = train_test_split(carEvalDataS, train_size = 0.8, test_size = 0.2)
'''
print(trainSet, end='\n')
print('--------------------------------')
print(testSet, end='\n')
print('--------------------------------')
'''

xtrain = trainSet.drop(['unacc','acc','good','vgood','carAcc'], axis=1).values
#print(xtrain)

ytrain = trainSet.drop(['buyPrice','maintainPrice','noOfDoors','noOfPersons','bootCapacity','carSafety','carAcc'], axis=1).values
#print(ytrain)

xtest = testSet.drop(['unacc','acc','good','vgood','carAcc'], axis=1).values
#print(xtest)

ytest = testSet.drop(['buyPrice','maintainPrice','noOfDoors','noOfPersons','bootCapacity','carSafety','carAcc'], axis=1).values
#print(ytest)

#creating the nn architecture
carEvalModel = Sequential([Dense(21, input_dim=6, activation='relu'), Dense(45, activation='relu'), Dense(4)])

#compiling the nn
carEvalModel.compile(optimizer='adam',loss='mse', metrics=['accuracy'])

#logging data
tb = keras.callbacks.TensorBoard(log_dir='./logs', histogram_freq=0, write_graph=True, write_images=False)

#testing parameters
resultAnalysis = carEvalModel.fit(xtrain, ytrain, epochs=100, verbose=1, validation_data=(xtest, ytest), callbacks=[tb])
#print(resultAnalysis)
print('#################################\n')
finalRes = carEvalModel.evaluate(xtest, ytest)


print("\nAccuracy: {}%".format(finalRes[1]*100))
print("\nLoss: {}".format(finalRes[0]))
#print(carEvalDataS)

print(carEvalModel.summary())


'''
test 0 minimal vs extra datasets
88.439, 0.043
97.3, 0.0107

test 1 split of dataset
96.645, 0.017
97.749, 0.011
95.726, 0.019

test 2 softmax vs relu
86.164, 0.0535
97.638, 0.0117

test 3 rmsprop vs adam
96.625, 0.0186
96.962, 0.0164

test 4 no of epochs
95.723, 0.0255
96.623, 0.0174
97.693, 0.0099

test 5 use of validation vs no use
97.073, 0.0104
97.411, 0.012

'''

'''
change 0 old testTrain set vs new testTrain set
change 1 parameters in dataset.drop
change 2 model layers
change 3 compilation parameters, mse=l2norm?
change 4 epochs and validation set
change 5 print accuracy and loss
'''

