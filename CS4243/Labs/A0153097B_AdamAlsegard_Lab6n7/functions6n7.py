import numpy as np
import cv2


# Convert 3D point to quaternion
def quat(v):

	return [0, v[0], v[1], v[2]]


# Returns the conjugate of quaternion Q
def quatconj(q):

	return [q[0], -q[1], -q[2], -q[3]]


# Convert quaternion to 3D point
def point(q):

	return q[1:]


# Quaternion multiplication
def quatmult(q1, q2):

	out = [0, 0, 0, 0]	# output array to hold the result

	out[0] = q1[0]*q2[0] - q1[1]*q2[1] - q1[2]*q2[2] - q1[3]*q2[3]
	out[1] = q1[0]*q2[1] + q1[1]*q2[0] + q1[2]*q2[3] - q1[3]*q2[2]
	out[2] = q1[0]*q2[2] - q1[1]*q2[3] + q1[2]*q2[0] + q1[3]*q2[1]
	out[3] = q1[0]*q2[3] + q1[1]*q2[2] - q1[2]*q2[1] + q1[3]*q2[0]

	return out  


# Extract the rotation matrix
def quat2rot(q):

	rotMat = np.zeros([3,3])
	rotMat[0,:] = [q[0]*q[0]+q[1]*q[1]-q[2]*q[2]-q[3]*q[3], 2*(q[1]*q[2]-q[0]*q[3]), 2*(q[1]*q[3]+q[0]*q[2])]
	rotMat[1,:] = [2*(q[1]*q[2]+q[0]*q[3]), q[0]*q[0]-q[1]*q[1]+q[2]*q[2]-q[3]*q[3], 2*(q[2]*q[3]-q[0]*q[1])]
	rotMat[2,:] = [2*(q[1]*q[3]-q[0]*q[2]), 2*(q[2]*q[3]+q[0]*q[1]), q[0]*q[0]-q[1]*q[1]-q[2]*q[2]+q[3]*q[3]]

	return np.matrix(rotMat)


# Project points with projection matrix
def persProj(Sp, T, R, f):
	
	u0, v0 = 0, 0
	Bu, Bv, ku, kv = 1, 1, 1, 1 # Why do I need ku and kv!?

	i = R[0,:] 	# Horizontal axis
	j = R[1,:] 	# Vertical axis
	k = R[2,:]	# Optical axis

	u = (float(f*np.dot(np.transpose(Sp-T),i)) / np.dot(np.transpose(Sp-T),k) )*Bu + u0
	v = (float(f*np.dot(np.transpose(Sp-T),j)) / np.dot(np.transpose(Sp-T),k) )*Bv + v0

	return [u, v]


# Project points with orthografic matrix
def orthoProj(Sp, T, R):

	u0, v0 = 0, 0
	Bu, Bv = 1, 1

	i = R[0,:] 	# Horizontal axis
	j = R[1,:] 	# Vertical axis
	k = R[2,:]	# Optical axis

	u = np.dot(np.transpose(Sp-T), i)*Bu + u0
	v = np.dot(np.transpose(Sp-T), j)*Bv + v0

	return [u, v]
