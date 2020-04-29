#!/usr/bin/env python
import csv
import matplotlib
import numpy as np
matplotlib.use('Agg')

import matplotlib.pyplot as plt

gb=1024*1024*1024
x=np.arange(0,gb+1)
y=[0]*(gb+1)

with open('dataTest2.txt', 'r') as csvfile:
    plots = csv.reader(csvfile, delimiter='\n')
    for row in plots:
        j=map(int, row)[0]
        y[j]+=1;
print("0")
plt.figure(1)
print("1")
plt.plot(x,y)
print("2")
plt.title('Zipfian data gen Test')
print("3")
plt.xlabel('address')
print("4")
plt.ylabel('count')
print("5")
plt.savefig('result2.png')
print("6")
plt.close()
print("7")
