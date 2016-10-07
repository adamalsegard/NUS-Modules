#Author: Adam Alsegard, A0153097B
#Lab session: Tue 4.00pm
#Lab 5

import numpy as np
import cv2

###################################33

# 1 - Read video
vidObj = cv2.VideoCapture('traffic.mp4')

# 2 - Print frame w, h, fps and count
print "Frame width: " + str(vidObj.get(cv2.CAP_PROP_FRAME_WIDTH))
print "Frame height: " + str(vidObj.get(cv2.CAP_PROP_FRAME_HEIGHT))
print "Video fps: " + str(vidObj.get(cv2.CAP_PROP_FPS))
print "Frame count: " + str(vidObj.get(cv2.CAP_PROP_FRAME_COUNT))

# 3 - convert the above into ints
frWidth = int(vidObj.get(cv2.CAP_PROP_FRAME_WIDTH))
frHeight = int(vidObj.get(cv2.CAP_PROP_FRAME_HEIGHT))
frFPS = int(vidObj.get(cv2.CAP_PROP_FPS))
frCount = int(vidObj.get(cv2.CAP_PROP_FRAME_COUNT))

# 4 - Get background object

_,img = vidObj.read() # Returns the first frame
avgImg = np.float32(img)
for fr in range(1, frCount):

	# Read next frame
	_,nextFr = vidObj.read()

	# Calculate the new avarage
	## Version 1
	alpha = 1/float(fr+1)
	#cv2.accumulateWeighted(nextFr, avgImg, alpha)
	
	## Version 2
	R_new = 1/float(fr+1)
	R_old = fr / float(fr+1)
	avgImg = avgImg * R_old + nextFr * R_new 


	# Scale -> Abs -> Convert to uint8
	normImg = cv2.convertScaleAbs(avgImg)

	coll = np.zeros([frHeight, frWidth*2, 3])
	coll[:,0:frWidth, :] = nextFr
	coll[:, frWidth:, :] = normImg
	coll = cv2.convertScaleAbs(coll)
	
	#cv2.imshow('img', nextFr)
	#cv2.imshow('normImg', normImg)

	cv2.imshow('collated', coll)

	print "fr = ", fr, "alpha = ", alpha
	cv2.waitKey(1)
	

cv2.waitKey(0)
cv2.destroyAllWindows()
vidObj.release()

# Create a snapshot of the background and save that image
cv2.imwrite('result/Background.jpg', normImg)
