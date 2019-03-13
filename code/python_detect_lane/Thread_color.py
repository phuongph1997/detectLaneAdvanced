import cv2
import numpy as np
import time
from sliding_windown import *
from roi_lane import *
import time
cap = cv2.VideoCapture('/home/hoaiphuong/CodePython/abc.mp4')
while cap.isOpened():
    ret,img = cap.read()
    t= time.time()
    #print (img.shape)
    #img = cv2.imread ("/home/hoaiphuong/CodePython/2.png")
    img_HSV = cv2.cvtColor(img,cv2.COLOR_RGB2HSV)
    low_threshold= (0,0,240)
    high_threshold= (180,255,255)
    frame_threshold = cv2.inRange(img_HSV,low_threshold,high_threshold)
    #cv2.namedWindow('frame_threshold', cv2.WINDOW_NORMAL)
    #cv2.imshow('frame_threshold',frame_threshold)
    #left_fit,right_fit = sliding_window(frame_threshold)
    #left_fit,right_fit,left_lane_inds, right_lane_inds = sliding_window(frame_threshold)
    
    imshape = frame_threshold.shape
    
    vertices = np.array([[(0.55*imshape[1], 0.6*imshape[0]), \
                          (imshape[1],imshape[0]),(0,imshape[0]), \
                          (0.45*imshape[1], 0.6*imshape[0])]], dtype=np.int32)
    #print vertices
    img_roi = region_of_interest(frame_threshold,vertices)
    img_transform = perspective_transform(img_roi)
    img_cut = img_transform[int(imshape[0]/2):imshape[0],0:imshape[1]]
    ret,img_binary = cv2.threshold(img_cut,100,255,cv2.THRESH_BINARY)
    #cv2.imwrite('a.png',img_cut)
    #cv2.imshow('cut',img_cut)
    #cv2.imshow('transform',img_transform)
    left_fit,right_fit,left_lane_inds, right_lane_inds = sliding_window(img_binary)
    array_left_fitx,array_right_fitx = poly_fit(img_binary,left_fit,right_fit,left_lane_inds, right_lane_inds)
    #cv2.imshow('out_put',img_out)
    print (time.time()-t)
    #cv2.waitKey(0)
    if cv2.waitKey(33)& 0xFF == ord('q'):
        break
cap.release()
#cv2.imshow('ROI',img_roi)
#cv2.waitKey(0)
cv2.destroyAllWindows()

