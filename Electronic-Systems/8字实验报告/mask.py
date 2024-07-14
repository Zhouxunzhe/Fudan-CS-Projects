import cv2

frame = cv2.imread("img.png")

# RGB图像转换到YCrCb空间
YCrCb_orange = cv2.cvtColor(frame,cv2.COLOR_BGR2YCrCb)
# 提取Cr通道并高斯去噪，（3,3）表示高斯核大小
Cr_orange = cv2.GaussianBlur(YCrCb_orange[:,:,1],(3,3),0)
# 二值化，160为阈值
ret,dst = cv2.threshold(Cr_orange,130,255,cv2.THRESH_BINARY)

cv2.imshow("display", dst)

cv2.imwrite("mask.png", dst)