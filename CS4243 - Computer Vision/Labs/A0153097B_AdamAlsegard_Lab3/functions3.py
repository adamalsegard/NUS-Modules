import numpy as np
import cv2
import math

def MyColvolve (img, filterfunc):
	result = np.zeros(img.shape)
	rows = img.shape[0]
	cols = img.shape[1]
	# Zero padding for edges in convolution
	padded_img = np.zeros((rows+2, cols+2))
	padded_img[1:rows+1, 1:cols+1] = img
	
	if filterfunc == "prewitt":
		for i in range(1, rows+1):
			for j in range(1, cols+1):
				
				hor_value = (-padded_img[i-1,j-1]-padded_img[i,j-1]-padded_img[i+1,j-1]+padded_img[i-1,j+1]+padded_img[i,j+1]+padded_img[i+1,j+1])/3
				vert_value = (-padded_img[i-1,j-1]-padded_img[i-1,j]-padded_img[i-1,j+1]+padded_img[i+1,j-1]+padded_img[i+1,j]+padded_img[i+1,j+1])/3
				result[i-1, j-1] = np.sqrt(pow(hor_value, 2) + pow(vert_value, 2))

	elif filterfunc == "sobel" :

		for i in range(1, rows+1):
			for j in range(1, cols+1):
				
				hor_value = (-padded_img[i-1,j-1]-2*padded_img[i,j-1]-padded_img[i+1,j-1]+padded_img[i-1,j+1]+2*padded_img[i,j+1]+padded_img[i+1,j+1])/4
				vert_value = (-padded_img[i-1,j-1]-2*padded_img[i-1,j]-padded_img[i-1,j+1]+padded_img[i+1,j-1]+2*padded_img[i+1,j]+padded_img[i+1,j+1])/4
				result[i-1, j-1] = np.sqrt(pow(hor_value, 2) + pow(vert_value, 2))
	
	elif filterfunc == "gradient": #Based on Sobel
		for i in range(1, rows+1):
			for j in range(1, cols+1):
				
				hor_value = (-padded_img[i-1,j-1]-2*padded_img[i,j-1]-padded_img[i+1,j-1]+padded_img[i-1,j+1]+2*padded_img[i,j+1]+padded_img[i+1,j+1])/4
				vert_value = (-padded_img[i-1,j-1]-2*padded_img[i-1,j]-padded_img[i-1,j+1]+padded_img[i+1,j-1]+2*padded_img[i+1,j]+padded_img[i+1,j+1])/4
				
				if hor_value == 0:
					result[i-1, j-1] = 255
				else:
					result[i-1, j-1] = np.float64(np.arctan(vert_value/hor_value))
	else: 
		print "Wrong kernel"

	return result


def NonMaxSuppr (img, low_threshold):
	result = np.zeros(img.shape)
	rows = img.shape[0]
	cols = img.shape[1]
	gradient = MyColvolve(img, "gradient")

	for i in range(rows):
		maxV = 0
		maxPos = 0

		for j in range(cols):
			
			# Don't bother with noise or other small values
			if img[i, j] < low_threshold :
				result[i, j] = 0
				maxPos = 0
				maxV = 0

			# Found an edge
			elif img[i, j] > maxV:
				# Store the new maximum 
				maxV = result[i, j] = img[i, j]

				# Check if gradient points vertically or horizontally
				# If gradient is horizontal (<pi/4) -> vertical edge (= short detection)
				# If gradient points vertically (>pi/4) -> horizontal edge (= long detection)
				if abs(gradient[i, j]) <= math.pi/4: 
					# Short edge, store only one value
					result[maxPos, j] = 0
					maxPos = i

				else:
					# Long edge, keep the previous result and compare to the value above to restrain from thick edges
					if result[i, j] > result[i-1, j]: # New result is better, throw away the one above
						result[i-1, j] = 0
						maxPos = i
					else: # Maximum is above, disregard the new value
						result[i, j] = 0

			# New value is lower than current maximum
			else:
				result[i, j] = 0

	return result
