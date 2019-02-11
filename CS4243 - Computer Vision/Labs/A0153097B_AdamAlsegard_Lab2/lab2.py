#Author: Adam Alsegard, A0153097B, Lab session: Tue 4.00pm

import numpy as np
import cv2
import math

################################################


def RGBtoHSV (R, G, B):
	nR = np.float64(R)/255
	nG = np.float64(G)/255
	nB = np.float64(B)/255

	Cmax = max(nR, nG, nB)
	Cmin = min(R, nG, nB)
	delta = Cmax - Cmin

	V = Cmax

	if Cmax == 0: 
		S = 0.0
	else:
		S = delta/Cmax

	if delta == 0:
		H = 0.0
	elif  Cmax == nR:
		H = 60*(((nG-nB)/delta)%6)
	elif Cmax == nG:
		H = 60*(((nB-nR)/delta)+2)
	else:
		H = 60*(((nR-nG)/delta)+4)

	return [H, S, V]

def HSVtoRGB(H, S, V):
	H = np.float64(H)*360/255
	S = np.float64(S)/255
	V = np.float64(V)/255

	C = V*S
	X = C*(1-abs((H/60)%2 - 1))
	m = V-C

	if H<60:
		[nR, nG, nB] = [C, X, 0]
	elif H<120:
		[nR, nG, nB] = [X, C, 0]
	elif H<180:
		[nR, nG, nB] = [0, C, X]
	elif H<240:
		[nR, nG, nB] = [0, X, C]
	elif H<300:
		[nR, nG, nB] = [X, 0, C]
	else:
		[nR, nG, nB] = [C, 0, X]

	R = (nR+m)*255
	G = (nG+m)*255
	B = (nB+m)*255


	return [R, G, B]

def histEQ (V, num_values):
	
	hist = np.zeros(num_values)

	for value in V.flatten():
		hist[value] += 1

	cumHist = np.cumsum(hist)

	#mapping = np.floor_divide(np.true_divide(cumHist, num_values), num_values)
	#newValues = np.interp(V.flatten(), mapping, cumHist)
	
	rows = len(cV[:,0])
	cols = len(cV[0,:])
	newV = np.matrix(np.zeros([rows, cols]))

	for i in range(rows):
		for j in range(cols):
			newV[i, j] = cumHist[V[i, j]]*255/(rows*cols)

	return newV


# 1a - Perform RGB->HSV

image1 = cv2.imread('pics/concert.jpg')

rows = len(image1[:,0,0])
cols = len(image1[0,:,0])
cH = np.matrix(np.zeros([rows, cols]))
cS = np.matrix(np.zeros([rows, cols]))
cV = np.matrix(np.zeros([rows, cols]))


for i in range(rows):
	for j in range(cols):
		#print [image1[i,j,2], image1[i,j,1], image1[i,j,0]]
		[cH[i, j], cS[i, j], cV[i, j]]  = RGBtoHSV(image1[i,j,2], image1[i,j,1], image1[i,j,0])
		#print [cH[i, j], cS[i, j], cV[i, j]] 

cH = (cH/360)*255 
cS = cS*255
cV = cV*255

cv2.imwrite('results/concert_hue.jpg', cH)
cv2.imwrite('results/concert_saturation.jpg', cS)
cv2.imwrite('results/concert_brightness.jpg', cV)


#1b
image2 = cv2.imread('pics/sea1.jpg')

rows = len(image2[:,0,0])
cols = len(image2[0,:,0])
sea1H = np.matrix(np.zeros([rows, cols]))
sea1S = np.matrix(np.zeros([rows, cols]))
sea1V = np.matrix(np.zeros([rows, cols]))

for i in range(rows):
	for j in range(cols):
		[sea1H[i, j], sea1S[i, j], sea1V[i, j]]  = RGBtoHSV(image2[i,j,2], image2[i,j,1], image2[i,j,0])

sea1H = (sea1H/360)*255 
sea1S = sea1S*255
sea1V = sea1V*255

cv2.imwrite('results/sea1_hue.jpg', sea1H)
cv2.imwrite('results/sea1_saturation.jpg', sea1S)
cv2.imwrite('results/sea1_brightness.jpg', sea1V)


#1c
image3 = cv2.imread('pics/sea2.jpg')

rows = len(image3[:,0,0])
cols = len(image3[0,:,0])
sea2H = np.matrix(np.zeros([rows, cols]))
sea2S = np.matrix(np.zeros([rows, cols]))
sea2V = np.matrix(np.zeros([rows, cols]))

for i in range(rows):
	for j in range(cols):
		[sea2H[i, j], sea2S[i, j], sea2V[i, j]]  = RGBtoHSV(image3[i,j,2], image3[i,j,1], image3[i,j,0])

sea2H = (sea2H/360)*255 
sea2S = sea2S*255
sea2V = sea2V*255

cv2.imwrite('results/sea2_hue.jpg', sea2H)
cv2.imwrite('results/sea2_saturation.jpg', sea2S)
cv2.imwrite('results/sea2_brightness.jpg', sea2V)


# 2a - Perform HSV->RGB
cH = cv2.imread('results/concert_hue.jpg')
cS = cv2.imread('results/concert_saturation.jpg')
cV = cv2.imread('results/concert_brightness.jpg')

rows = len(cH[:,0])
cols = len(cH[0,:])
newR = np.matrix(np.zeros([rows, cols]))
newG = np.matrix(np.zeros([rows, cols]))
newB = np.matrix(np.zeros([rows, cols]))
concert_hsv2rgb = np.zeros((rows, cols, 3))

for i in range(rows):
	for j in range(cols):
		[newR[i, j], newG[i, j], newB[i, j]] = HSVtoRGB(cH[i,j, 0], cS[i, j, 1], cV[i, j, 2])

concert_hsv2rgb[:,:,2] = newR
concert_hsv2rgb[:,:,1] = newG
concert_hsv2rgb[:,:,0] = newB

cv2.imwrite('results/concert_hsv2rgb.jpg', concert_hsv2rgb)


# 2b
sea1H = cv2.imread('results/sea1_hue.jpg')
sea1S = cv2.imread('results/sea1_saturation.jpg')
sea1V = cv2.imread('results/sea1_brightness.jpg')

rows = len(sea1H[:,0])
cols = len(sea1H[0,:])
newR = np.matrix(np.zeros([rows, cols]))
newG = np.matrix(np.zeros([rows, cols]))
newB = np.matrix(np.zeros([rows, cols]))
sea1_hsv2rgb = np.zeros((rows, cols, 3, ))

for i in range(rows):
	for j in range(cols):
		[newR[i, j], newG[i, j], newB[i, j]] = HSVtoRGB(sea1H[i,j, 0], sea1S[i, j, 1], sea1V[i, j, 2])

sea1_hsv2rgb[:,:,2] = newR
sea1_hsv2rgb[:,:,1] = newG
sea1_hsv2rgb[:,:,0] = newB

cv2.imwrite('results/sea1_hsv2rgb.jpg', sea1_hsv2rgb)


# 2c
sea2H = cv2.imread('results/sea2_hue.jpg')
sea2S = cv2.imread('results/sea2_saturation.jpg')
sea2V = cv2.imread('results/sea2_brightness.jpg')

rows = len(sea2H[:,0])
cols = len(sea2H[0,:])
newR = np.matrix(np.zeros([rows, cols]))
newG = np.matrix(np.zeros([rows, cols]))
newB = np.matrix(np.zeros([rows, cols]))
sea2_hsv2rgb = np.zeros((rows, cols, 3))

for i in range(rows):
	for j in range(cols):
		[newR[i, j], newG[i, j], newB[i, j]] = HSVtoRGB(sea2H[i,j, 0], sea2S[i, j, 1], sea2V[i, j, 2])

sea2_hsv2rgb[:,:,2] = newR
sea2_hsv2rgb[:,:,1] = newG
sea2_hsv2rgb[:,:,0] = newB

cv2.imwrite('results/sea2_hsv2rgb.jpg', sea2_hsv2rgb)


# 3a - Histogram equaization

cH = cv2.imread('results/concert_hue.jpg')
cS = cv2.imread('results/concert_saturation.jpg')
cV = cv2.imread('results/concert_brightness.jpg')

rows = len(cV[:,0])
cols = len(cV[0,:])

newValues = histEQ(cV[:,:,0], 256)

newR = np.matrix(np.zeros([rows, cols]))
newG = np.matrix(np.zeros([rows, cols]))
newB = np.matrix(np.zeros([rows, cols]))
concert_histeq = np.zeros((rows, cols, 3))

for i in range(rows):
	for j in range(cols):
		[newR[i, j], newG[i, j], newB[i, j]] = HSVtoRGB(cH[i,j, 0], cS[i, j, 0], newValues[i, j])

concert_histeq[:,:,2] = newR
concert_histeq[:,:,1] = newG
concert_histeq[:,:,0] = newB

cv2.imwrite('results/concert_histeq.jpg', concert_histeq)

# 3b

sea1H = cv2.imread('results/sea1_hue.jpg')
sea1S = cv2.imread('results/sea1_saturation.jpg')
sea1V = cv2.imread('results/sea1_brightness.jpg')

rows = len(sea1V[:,0])
cols = len(sea1V[0,:])

newValues = histEQ(sea1V[:,:,0], 256)

newR = np.matrix(np.zeros([rows, cols]))
newG = np.matrix(np.zeros([rows, cols]))
newB = np.matrix(np.zeros([rows, cols]))
sea1_histeq = np.zeros((rows, cols, 3))

for i in range(rows):
	for j in range(cols):
		[newR[i, j], newG[i, j], newB[i, j]] = HSVtoRGB(sea1H[i,j, 0], sea1S[i, j, 0], newValues[i, j])

sea1_histeq[:,:,2] = newR
sea1_histeq[:,:,1] = newG
sea1_histeq[:,:,0] = newB

cv2.imwrite('results/sea1_histeq.jpg', sea1_histeq)

# 3c

sea2H = cv2.imread('results/sea2_hue.jpg')
sea2S = cv2.imread('results/sea2_saturation.jpg')
sea2V = cv2.imread('results/sea2_brightness.jpg')

rows = len(cV[:,0])
cols = len(cV[0,:])

newValues = histEQ(sea2V[:,:,0], 256)

newR = np.matrix(np.zeros([rows, cols]))
newG = np.matrix(np.zeros([rows, cols]))
newB = np.matrix(np.zeros([rows, cols]))
sea2_histeq = np.zeros((rows, cols, 3))

for i in range(rows):
	for j in range(cols):
		[newR[i, j], newG[i, j], newB[i, j]] = HSVtoRGB(sea2H[i,j, 0], sea2S[i, j, 0], newValues[i, j])

sea2_histeq[:,:,2] = newR
sea2_histeq[:,:,1] = newG
sea2_histeq[:,:,0] = newB

cv2.imwrite('results/sea2_histeq.jpg', sea2_histeq)