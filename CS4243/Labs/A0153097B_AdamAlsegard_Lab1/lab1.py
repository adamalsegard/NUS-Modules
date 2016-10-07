#Author: Adam Alsegard, A0153097B, Lab session: Tue 4.00pm

#Write a prgram to solve for the affine transformation parameters

import numpy as np
import numpy.linalg as la

file = open("data.txt")
data = np.genfromtxt(file, delimiter=",")
file.close()

#print "Data: \n", data

rows = len(data[:,0])
cols = len(data[0,:])

#print "rows + cols", rows, cols

M = np.matrix(np.zeros([rows*2, cols+2]))
b = np.matrix(np.zeros([rows*2, 1]))

#print "M = \n", M
#print "b = \n", b

for i in range(rows):
	print "i=", i
	b[i*2] = data[i, 0]
	b[(i*2)+1] = data[i, 1]
	M[(i*2), 0] = M[(i*2)+1, 3] = data[i, 2]
	M[(i*2), 1] = M[(i*2)+1, 4] = data[i, 3]
	M[(i*2), 2] = M[(i*2)+1, 5] = 1


print "M = \n", M
print "b = \n", b

a, e, r, s = la.lstsq(M, b)

print "a = \n", a

ans = M*a

print "ans = \n", ans

sqErr = la.norm(M*a-b)

print "Sum-squared error: ", sqErr
print "Residue error: ", e