import numpy as np
import cv2
import math
import matplotlib.pyplot as plt
import matplotlib.patches as patches


def gauss_kernels(size, sigma=1.0):
	## returns a 2d gaussian kernel
	if size<3:
		size = 3
	m = size/2

	x, y = np.mgrid[-m:m+1, -m:m+1]
	kernel = np.exp(-(x*x + y*y)/(2*sigma*sigma))
	kernel_sum = kernel.sum()

	if not sum==0:
		kernel = kernel/kernel_sum

	return kernel


def MyColvolve (img, kernel):
	result = np.zeros(img.shape)
	rows = img.shape[0]
	cols = img.shape[1]

	# No use right now
	padImg = np.zeros((rows+2, cols+2))
	padImg[1:rows+1, 1:cols+1] = img

	# Flip kernel!
	kernel = np.fliplr(np.flipud(kernel))
	
	# Skip the zero padding so we don't detect the edges later on
	for i in range(2, rows):
		for j in range(2, cols):
			
			value = (padImg[i-1,j-1]*kernel[0,0])+(padImg[i-1,j]*kernel[0,1])+(padImg[i-1,j+1]*kernel[0,2])+ \
					(padImg[i,j-1]*kernel[1,0])+(padImg[i,j]*kernel[1,1])+(padImg[i,j+1]*kernel[1,2])+ \
					(padImg[i+1,j-1]*kernel[2,0])+(padImg[i+1,j]*kernel[2,1])+(padImg[i+1,j+1]*kernel[2,2])
			result[i-1, j-1] = abs(value)

	return result


def HarrisDetec(img, k = 0.06):
	result = np.zeros(img.shape)
	stepsize = 10
	threshold = 1-0.9

	rows = img.shape[0]
	cols = img.shape[1]
	response = np.matrix(np.zeros([rows/stepsize, cols/stepsize]))


	sobelHor = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
	sobelVert = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]])

	gx = MyColvolve(img, sobelHor)
	gy = MyColvolve(img, sobelVert)

	I_xx = gx*gx
	I_xy = gx*gy
	I_yy = gy*gy

	gauss = gauss_kernels(3, 1)

	W_xx = MyColvolve(I_xx, gauss)
	W_xy = MyColvolve(I_xy, gauss)
	W_yy = MyColvolve(I_yy, gauss)


	for i in range(1, rows/stepsize):
		for j in range(1, cols/stepsize):

			W = np.matrix([[W_xx[i*stepsize, j*stepsize], W_xy[i*stepsize, j*stepsize]], [W_xy[i*stepsize, j*stepsize], W_yy[i*stepsize, j*stepsize]]])
			detW = W[0,0]*W[1,1]-W[0,1]*W[1,0]
			#detW = np.linalg.det(W)
			traceW = W[0,0]+W[1,1]
			#traceW = np.trace(W)

			response[i,j] = detW - (k * traceW * traceW)

	maxValue = np.matrix.max(response)

	for i in range(1, rows/stepsize): 
		for j in range(1, cols/stepsize):
			if response[i, j] >= maxValue*threshold:

				#add markings to image 
				result[(i*stepsize)-4:(i*stepsize)+4, (j*stepsize)-4:(j*stepsize)+4] = 255
				result[(i*stepsize)-3:(i*stepsize)+3, (j*stepsize)-3:(j*stepsize)+3] = 0

	return result 
