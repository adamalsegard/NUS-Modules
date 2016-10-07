#Author: Adam Alsegard, A0153097B
#Lab session: Tue 4.00pm
#Lab 4

import numpy as np
import cv2
import matplotlib.pyplot as plt
from functions4 import HarrisDetec

################################################


# Read color images
imageCheck = cv2.imread('pics/checker.jpg')
imageFlow = cv2.imread('pics/flower.jpg')
image1 = cv2.imread('pics/test1.jpg')
image2 = cv2.imread('pics/test2.jpg')
image3 = cv2.imread('pics/test3.jpg')

# Instead of converting them to grayscale I just read them one more time
imageCheckBW = cv2.imread('pics/checker.jpg', cv2.IMREAD_GRAYSCALE)
imageFlowBW = cv2.imread('pics/flower.jpg', cv2.IMREAD_GRAYSCALE)
image1BW = cv2.imread('pics/test1.jpg', cv2.IMREAD_GRAYSCALE)
image2BW = cv2.imread('pics/test2.jpg', cv2.IMREAD_GRAYSCALE)
image3BW = cv2.imread('pics/test3.jpg', cv2.IMREAD_GRAYSCALE)


# Perform Harris Corner Detection on the grayscale images
imageCheck_corners = HarrisDetec(imageCheckBW)
imageFlow_corners = HarrisDetec(imageFlowBW)
image1_corners = HarrisDetec(image1BW)
image2_corners = HarrisDetec(image2BW)
image3_corners = HarrisDetec(image3BW)

# Draw red squares on the color image
maskCheck = (imageCheck_corners==255)
imageCheck[maskCheck] = [255, 0, 0]
maskFlower = (imageFlow_corners==255)
imageFlow[maskFlower] = [255, 0, 0]
mask1 = (image1_corners==255)
image1[mask1] = [255, 0, 0]
mask2 = (image2_corners==255)
image2[mask2] = [255, 0, 0]
mask3 = (image3_corners==255)
image3[mask3] = [255, 0, 0]


# Write results
cv2.imwrite('result/checker_corners10.jpg', imageCheck)
cv2.imwrite('result/flower_corners10.jpg', imageFlow)
cv2.imwrite('result/test1_corners10.jpg', image1)
cv2.imwrite('result/test2_corners10.jpg', image2)
cv2.imwrite('result/test3_corners10.jpg', image3)

