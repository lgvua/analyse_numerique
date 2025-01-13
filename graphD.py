import re
import math
import matplotlib.pyplot as plt
import numpy as np
# lib utilisé scikit-learn (python -m pip install numpy scikit-learn statsmodels)

j=input("j=")

with open('sd1.txt') as f:
	lines = f.readlines();

N=[]
Xn=[]

print(len(lines))
numeric_const_pattern = '[-+]? (?: (?: \d* \. \d+ ) | (?: \d+ \.? ) )(?: [Ee] [+-]? \d+ ) ?'
rx = re.compile(numeric_const_pattern, re.VERBOSE)
tmp = rx.findall(lines[0])

for i in range(len(tmp)):
	N.append(i)
	Xn.append(math.log(float(tmp[i])))
	print(tmp[i])


with open('gpf'+str(j)+".txt") as q:
	qlines = q.readlines();

qN=[]
qXn=[]

print(len(qlines))
qrx = re.compile(numeric_const_pattern, re.VERBOSE)
qtmp = qrx.findall(qlines[0])


for i in range(len(qtmp)):
	qN.append(i)
	qXn.append(math.log(float(qtmp[i])))
	print(qtmp[i])

courbe="pas fixe pour $\\alpha=\\frac{"+str(j)+"\\times \\alpha_{opt}}{5}$"

plt.ylabel("$ln (  \\frac{||r_k||}{||b||} )$")
plt.xlabel("k")
plt.plot(qN,qXn,color="blue",label=courbe)
plt.plot(N,Xn,color="red",label="pas optimal")
plt.title("Méthode du gradient à pas optimale et à pas fixe")
plt.legend()
plt.show();