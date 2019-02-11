#Author: Adam Alsegard, A0153097B
#Lab session: Tue 4.00pm
#Lab 6n7

import numpy as np
import cv2
import math
import numpy.linalg as la
import matplotlib.pyplot as plt
from functions6n7 import *

##########################################################


## 1.1 Define points
pts = np.zeros([11, 3])
pts[0, :] = [-1, -1, -1]
pts[1, :] = [1, -1, -1]
pts[2, :] = [1, 1, -1]
pts[3, :] = [-1, 1, -1]
pts[4, :] = [-1, -1, 1]
pts[5, :] = [1, -1, 1]
pts[6, :] = [1, 1, 1]
pts[7, :] = [-1, 1, 1]
pts[8, :] = [-0.5, -0.5, -1]
pts[9, :] = [0.5, -0.5, -1]
pts[10,:] = [0, 0.5, -1]



## 1.2 Calc camera positions for the first 4 frames
radius = 5
camPos = np.zeros([4, 3])
camPos[0, :] = [0, 0, -radius]

# Use quaternions
startQ = quat(camPos[0, :])
print "CameraPos 0: " + str(camPos[0, :])
	
for i in range(1,len(camPos)):
	rad = -float(math.pi*i)/6							# Rotation in radians (-30 degrees)
	rotQ = [math.cos(rad/2), 0, math.sin(rad/2), 0]		# Rotation around Y-axis
	camPos[i, :] = point(quatmult(rotQ, quatmult(startQ, quatconj(rotQ))))
	camPos[i, :] = np.around(camPos[i, :], 6)
	print "CameraPos " + str(i) + ": " + str(camPos[i, :])



## 1.3 Calc camera orientation
quatmat = np.zeros([3, 3, 4])
quatmat[:,:, 0] = np.matrix(np.identity(3, float))
print "quatmat_0: \n" + str(quatmat[:,:,0])
	
for i in range(1,len(quatmat[0,0,:])):
	rad = float(math.pi*i)/6							# Rotation in radians (+30 degrees)
	rotQ = [math.cos(rad/2), 0, math.sin(rad/2), 0]		# Quaternion rotation around Y-axis
	quatmat[:,:,i] = quat2rot(rotQ)*quatmat[:,:, 0]		# Convert to rotation matrix
	quatmat[:,:,i] = np.around(quatmat[:,:,i], 6)
	print "quatmat_" + str(i) + ": \n" + str(quatmat[:,:,i])



## 2.0 Plot projections
focal_length = 1

# Set up plots
fig1, ((im11, im12), (im13, im14)) = plt.subplots(2, 2)
fig2, ((im21, im22), (im23, im24)) = plt.subplots(2, 2)
perspPlots = [im11, im12, im13, im14]
orthoPlots = [im21, im22, im23, im24]
pAxis = 0.5
oAxis = 2

for i in range(0,4):
	
	# Set up subplots
	subplt1 = perspPlots[i]
	subplt1.set_xlim([-pAxis, pAxis])
	subplt1.set_ylim([-pAxis, pAxis])
	subplt1.set_title("Frame: " + str(i))

	subplt2 = orthoPlots[i]
	subplt2.set_xlim([-oAxis, oAxis])
	subplt2.set_ylim([-oAxis, oAxis])
	subplt2.set_title("Frame: " + str(i))

	for point in pts:

		# Plot perspective projection
		perspPts = persProj(point, camPos[i, :], quatmat[:,:,i], focal_length)
		subplt1.plot(perspPts[0], perspPts[1], 'rx')

		# Plot orthografic projection
		orthoPts = orthoProj(point, camPos[i, :], quatmat[:,:,i])
		subplt2.plot(orthoPts[0], orthoPts[1], 'gx')

	plt.tight_layout()

#Draw plots
fig1.savefig("results/perspective.png")
fig2.savefig("results/orthgrafic.png")
plt.show()



# 3.0 Homography

ptIdx = [0, 1, 2, 3, 8]
frIdx = 2
zero = 10e-16
fullRank = 9

A = np.zeros([2*len(ptIdx), 9])
H = np.zeros([3,3])

for i in range(len(ptIdx)):
	id = ptIdx[i]
	[x, y, z] = pts[id, :]
	[u, v] = persProj(pts[id, :], camPos[frIdx, :], quatmat[:,:,frIdx], focal_length)

	up = x/z
	vp = y/z
	uc = u/focal_length
	vc = v/focal_length

	# Set up matrix A
	A[i*2, :] = [up, vp, 1, 0, 0, 0, -uc*up, -uc*vp, -uc]
	A[i*2+1, :] = [0, 0, 0, up, vp, 1, -vc*up, -vc*vp, -vc]


# Find out rank of matrix A
rankA = la.matrix_rank(A)

if rankA < fullRank: 	# Otherwise H = 0
	U, S, VT = la.svd(A)

	# Find the zero value. If there are more then we need to form linear combinations
	for i in range(fullRank):
		if abs(S[i]) < zero:
			H = VT[i]


#print np.dot(A, H) # Should be 0 (or very very small)

# Normalize so h33 = 1
H.resize([3,3])
H = H /H[2,2]

H = np.around(H, 7)
print H
#np.savetxt("results/H.txt", H)

'''
# test
for i in range(len(ptIdx)):
	id = ptIdx[i]
	[x, y, z] = pts[id, :]
	XYZ = np.transpose(np.matrix([x/z, y/z, 1]))
	H = np.matrix(H)
	UV = H*XYZ
	print UV/UV[2]
	[uc, vc] = persProj(pts[id, :], camPos[frIdx, :], quatmat[:,:,frIdx], focal_length)
 	print [uc, vc]
 '''