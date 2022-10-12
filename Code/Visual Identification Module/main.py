# -*- coding:utf-8 -*-
# coding=gbk
import cv2 as cv
import cv2
import numpy as np
import os
def copy3(img):
    width = img.shape[1]
    height = img.shape[0]
    result = np.zeros((height, width,3), np.uint8)
    for i0 in range(0, height):
        for i1 in range(0, width):
            result[i0,i1,0]=img[i0,i1,0]
            result[i0,i1,1]=img[i0,i1,1]
            result[i0,i1,2]=img[i0,i1,2]
    return result

'''
# 调用usb摄像头
cap = cv2.VideoCapture(1) #0pc,1usb
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1440)
cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter.fourcc("M", "J", "P", "G"))
# 拍照
flag = cap.isOpened()
index = 10
while (flag):
    ret, frame = cap.read()
    cv2.imshow("Capture", frame)
    k = cv2.waitKey(1) & 0xFF
    if k == ord('s'):  # 按s保存
        cv2.imwrite(str(index) + ".jpg", frame)
        print(cap.get(3))
        print(cap.get(4))
        print("save" + str(index)  + ".jpg successfuly!")
        print("-------------------------")
        index += 1
    elif k == ord('q'):  # 按q退出
        break
cap.release()
cv2.destroyAllWindows()
'''

#窗口命名
cv.namedWindow("image")
#载入图像
filename="10.jpg"
img=cv.imread(filename)
#图像拉伸
img= cv.resize(img, (0, 0), fx=0.5, fy=0.5)
#图像宽度
width=img.shape[1]
#图像高度
height=img.shape[0]
#cv.imshow("img",img)
#创建空图
img1 = np.zeros((height, width), np.uint8)
#选取黑色区域
for i0 in range(0,height):
    for i1 in range(0,width):
        a,b,c=img[i0,i1]
        if(a<50 and b<50 and c<50):
            img1[i0,i1]=255
        if(i0<3 or i0>height-3 or i1<3 or i1>width-3):
            img1[i0,i1]=0
#cv.imshow("img1",img1)
#轮廓提取
contours, hierarchy = cv.findContours(img1, cv.RETR_TREE, cv.CHAIN_APPROX_NONE)
#选取最大轮廓
max_contour=len(contours[0])
id=0
for i0 in range(len(contours)):
    if(len(contours[i0])>max_contour):
        max_contour=len(contours[i0])
        id=i0
        #cv.drawContours(img1, contours, i0, 0, -1)
img2 = np.zeros((height, width), np.uint8)
#绘制轮廓
cv.drawContours(img2, contours, id, 255, -1)
#cv.imshow("img2",img2)
img3 = np.zeros((height, width), np.uint8)
for i0 in range(0,height):
    for i1 in range(0,width):
        if(img2[i0,i1]==255):
            if(img1[i0,i1]==0):
                img3[i0,i1]=255
#cv.imshow("img3",img3)

contours, hierarchy = cv.findContours(img3, cv.RETR_TREE, cv.CHAIN_APPROX_NONE)
max_contour=len(contours[0])
id=0
for i0 in range(len(contours)):
    if(len(contours[i0])>max_contour):
        max_contour=len(contours[i0])
        id=i0
        #cv.drawContours(img1, contours, i0, 0, -1)
img4 = np.zeros((height, width), np.uint8)
cv.drawContours(img4, contours, id, 255, -1)
#cv.drawContours(img4,contours,id,100,2)
cirimg=np.zeros((height, width), np.uint8)
cirimg3=np.zeros((height, width,3), np.uint8)
cv.drawContours(cirimg, contours, id, 255, 1)
circles = cv.HoughCircles(cirimg,cv.HOUGH_GRADIENT,1,minDist=100,param1=50,param2=30,minRadius=0,maxRadius=0)
print('circles:', circles)
circles = np.uint16(np.around(circles))
for i in circles[0,:]:
    # draw the outer circle
    cv.circle(cirimg3,(i[0],i[1]),i[2],(0,255,0),2)
cv.circle(img4,(i[0],i[1]),i[2]-35,100,-1)    #去除边界范围
#cv.imshow("img4",img4)
#cv.imshow("circles",cirimg3)
img5 = np.zeros((height, width,3), np.uint8)
for i0 in range(0,height):
    for i1 in range(0,width):
        if(img4[i0,i1]==100):
            img5[i0,i1]=img[i0,i1]

#载入菌种模板
src1=cv.imread("data//1.png");
src2=cv.imread("data//2.png");
#拉伸模板到统一尺寸
src1=cv.resize(src1,(30,30))
src2=cv.resize(src2,(30,30))
#模板均值
src1_color=(0,0,0)
src2_color=(0,0,0)
for i0 in range(0,30):
    for i1 in range(0,30):
        src1_color=src1[i0,i1]+src1_color
        src2_color=src2[i0,i1]+src2_color
src1_color=src1_color/900.0
src2_color=src2_color/900.0
print("color1:"+str(src1_color))
print("color2:"+str(src2_color))
#cv.imshow("img5",img5)
img6 = np.zeros((height, width), np.uint8)
#分类匹配阈值
th=30
#匹配菌种
for i0 in range(0,height):
    for i1 in range(0,width):
        a=src1_color[0]
        b=src1_color[1]
        c=src1_color[2]
        a1=img5[i0,i1,0]
        b1=img5[i0,i1,1]
        c1=img5[i0,i1,2]
        if(abs(a-a1)<th and abs(b-b1)<th and abs(c-c1)<th):
            img6[i0,i1]=255
        a=src2_color[0]
        b=src2_color[1]
        c=src2_color[2]
        a1=img5[i0,i1,0]
        b1=img5[i0,i1,1]
        c1=img5[i0,i1,2]
        if(abs(a-a1)<th and abs(b-b1)<th and abs(c-c1)<th):
            img6[i0,i1]=100

#cv.imshow("img6",img6)
result = copy3(img)

#图像染色
for i0 in range(0,height):
    for i1 in range(0,width):
        if(img6[i0,i1]==255):      #红
            result[i0,i1,0]=71
            result[i0,i1,1]=99
            result[i0,i1,2]=255
        if(img6[i0,i1]==100):      #白
            result[i0,i1,0]=140
            result[i0,i1,1]=230
            result[i0,i1,2]=240

#打印结果图像
cv.imshow("result",result)
img7 = np.zeros((height, width), np.uint8)
img8 = np.zeros((height, width), np.uint8)

for i0 in range(0,height):
    for i1 in range(0,width):
        if(img6[i0,i1]==255):
            img7[i0,i1]=255
        if(img6[i0,i1]==100):
            img8[i0,i1]=255


#用于消除重叠区域
kernel = cv.getStructuringElement(cv.MORPH_RECT,(5, 5))
img81 = cv.dilate(img8,kernel)
for i0 in range(0,height):
    for i1 in range(0,width):
        if(img81[i0,i1]==255):
            img7[i0,i1]=0
img81 = cv.erode(img8,kernel)
#cv.imshow("img7", img7)
#cv.imshow("img8", img8)
#cv.imshow("img81",img81)

#记录圆心坐标
a=[]
b=[]
contours, hierarchy = cv.findContours(img8, cv.RETR_TREE, cv.CHAIN_APPROX_NONE) # hierarchy 层次结构
#White
result1 = copy3(img)
for i0 in range(len(contours)):
    if(len(contours[i0]) > 5):
         (x,y,w,h) = cv.boundingRect(contours[i0])
         a.append(((x+w/2),(y+h/2)))
         cv.circle(result1, (int(x+w/2), int(y+h/2)), 2, (140, 230, 240), 2)
         # print(((x+w/2),(y+h/2)))

contours, hierarchy = cv.findContours(img7, cv.RETR_TREE, cv.CHAIN_APPROX_NONE)
#Red
for i0 in range(len(contours)):
    if (len(contours[i0]) > 10):
         (x,y,w,h) = cv.boundingRect(contours[i0])
         b.append(((x+w/2),(y+h/2)))
         cv.circle(result1, (int(x + w / 2), int(y + h / 2)), 2, (71, 99, 255), 2)

#显示菌落
print('White')
print(a)
print('Red')
print(b)
cv.imshow("result1",result1)
cv.waitKey(0)


