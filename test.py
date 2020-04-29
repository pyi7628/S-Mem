#!/usr/bin/env python
import csv
import numpy as np
import matplotlib

matplotlib.use('Agg')

import matplotlib.pyplot as plt

gb=1024*1024*1024
x=np.arange(0,gb+1)
y=[0]*(gb+1)

with open('/share/pyi/zipfian_distribution/dataTest.txt', 'r') as csvfile:
    plots = csv.reader(csvfile, delimiter='\n')
    for row in plots:
        j=map(int, row)[0]
        y[j]+=1;

point20=0
cx=[]
cy=[]
s=0
flag=0
for i in range(0,2000000):
    cy.append(y[i])
    if flag==0:
        s=s+y[i]
        if s>=(gb/5):
            point20=i
            flag=1
    cx.append(i)
print("point:")
print(point20)

print("0")
plt.figure()
print("1")
#plt.plot(x,y)
plt.plot(cx,cy)
print("2")
plt.title('Zipfian data gen Test')
print("3")
plt.xlabel('address')
print("4")
plt.ylabel('count')
print("5")
plt.savefig('subresult1.png')
print("6")
plt.close()
print("7")
