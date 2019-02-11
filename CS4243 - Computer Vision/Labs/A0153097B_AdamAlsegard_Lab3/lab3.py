#Author: Adam Alsegard, A0153097B, Lab session: Tue 4.00pm

import numpy as np
import cv2
from functions3 import MyColvolve
from functions3 import NonMaxSuppr

################################################

# Read images
imageEx = cv2.imread('pics/example.jpg', cv2.IMREAD_GRAYSCALE)
image1 = cv2.imread('pics/test1.jpg', cv2.IMREAD_GRAYSCALE)
image2 = cv2.imread('pics/test2.jpg', cv2.IMREAD_GRAYSCALE)
image3 = cv2.imread('pics/test3.jpg', cv2.IMREAD_GRAYSCALE)

# Convolve and do non-maximal suppression
imageEx_prewitt = MyColvolve(imageEx, "prewitt")
imageEx_sobel = MyColvolve(imageEx, "sobel")
imageEx_thinSobel = NonMaxSuppr(imageEx_sobel, 40)

image1_prewitt = MyColvolve(image1, "prewitt")
image1_sobel = MyColvolve(image1, "sobel")
image1_thinSobel = NonMaxSuppr(image1_sobel, 40)

image2_prewitt = MyColvolve(image2, "prewitt")
image2_sobel = MyColvolve(image2, "sobel")
image2_thinSobel = NonMaxSuppr(image2_sobel, 40)

image3_prewitt = MyColvolve(image3, "prewitt")
image3_sobel = MyColvolve(image3, "sobel")
image3_thinSobel = NonMaxSuppr(image3_sobel, 40)

'''
imageEx_gradient = MyColvolve(imageEx, "gradient")
cv2.imwrite('result/imageEx_gradient.jpg', imageEx_gradient)

imageEx_sobel_cvX = cv2.Sobel(imageEx, cv2.CV_64F,1,0,ksize=3)
imageEx_sobel_cvY = cv2.Sobel(imageEx, cv2.CV_64F,0,1,ksize=3)
imageEx_sobel_cv = np.sqrt(pow(imageEx_sobel_cvX, 2) + pow(imageEx_sobel_cvY, 2))
cv2.imwrite('result/imageEx_opencv.jpg', imageEx_sobel_cv)
'''
# Write results
cv2.imwrite('result/imageEx.jpg', imageEx)
cv2.imwrite('result/imageEx_prewitt.jpg', imageEx_prewitt)
cv2.imwrite('result/imageEx_sobel.jpg', imageEx_sobel)
cv2.imwrite('result/imageEx_thinSobel.jpg', imageEx_thinSobel)

cv2.imwrite('result/image1.jpg', image1)
cv2.imwrite('result/image1_prewitt.jpg', image1_prewitt)
cv2.imwrite('result/image1_sobel.jpg', image1_sobel)
cv2.imwrite('result/image1_thinSobel.jpg', image1_thinSobel)

cv2.imwrite('result/image2.jpg', image2)
cv2.imwrite('result/image2_prewitt.jpg', image2_prewitt)
cv2.imwrite('result/image2_sobel.jpg', image2_sobel)
cv2.imwrite('result/image2_thinSobel.jpg', image2_thinSobel)

cv2.imwrite('result/image3.jpg', image3)
cv2.imwrite('result/image3_prewitt.jpg', image3_prewitt)
cv2.imwrite('result/image3_sobel.jpg', image3_sobel)
cv2.imwrite('result/image3_thinSobel.jpg', image3_thinSobel)
