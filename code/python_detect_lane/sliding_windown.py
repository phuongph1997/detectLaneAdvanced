import cv2
import numpy as np
import matplotlib.pyplot as plt
from global_lane import *
#img = cv2.imread('/home/hoaiphuong/CodePython/a.png',0)
#img_HSV = cv2.cvtColor(img,cv2.COLOR_RGB2HSV)
#img_threshold = cv2.inRange(img_HSV,(0,0,240),(180,255,255))
#thresh, im_bw = cv2.threshold(img, 200, 255, cv2.THRESH_BINARY)
#print im_bw.shape
def sliding_window(binary_warped):
    histogram = np.sum(binary_warped, axis=0)
    #print img_threshold[img_threshold.shape[0]/2:,:]
    #plt.plot(histogram)
    #plt.show()
    #print ('histogram %s'%(histogram.shape))
    midpoint = np.int(histogram.shape[0]/2)
    #print midpoint
    #print histogram.shape
    leftx_base = np.argmax(histogram[:midpoint])
    rightx_base = np.argmax(histogram[midpoint:]) + midpoint
    #print leftx_base
    #print rightx_base
    #print leftx_base , rightx_base
    #binary_warped = np.copy(img_threshold)
    nwindows = 9
    # Set height of windows
    window_height = np.int(binary_warped.shape[0]/nwindows)
    # Identify the x and y positions of all nonzero pixels in the image
    nonzero = binary_warped.nonzero()
    nonzeroy = np.array(nonzero[0])
    nonzerox = np.array(nonzero[1])
    # Current positions to be updated for each window
    leftx_current = leftx_base
    rightx_current = rightx_base
    # Set the width of the windows +/- margin
    margin = 20
    # Set minimum number of pixels found to recenter window
    minpix = 10
    # Create empty lists to receive left and right lane pixel indices
    left_lane_inds = []
    right_lane_inds = []
    # Step through the windows one by one
    for window in range(nwindows):
        # Identify window boundaries in x and y (and right and left)
        win_y_low = binary_warped.shape[0] - (window+1)*window_height
        win_y_high = binary_warped.shape[0] - window*window_height
        win_xleft_low = leftx_current - margin
        win_xleft_high = leftx_current + margin
        win_xright_low = rightx_current - margin
        win_xright_high = rightx_current + margin
        #print ((nonzeroy >= win_y_low).nonzero())
        #Draw the windows on the visualization image
        #cv2.rectangle(binary_warped,(win_xleft_low,win_y_low),(win_xleft_high,win_y_high),255, 2) 
        #cv2.rectangle(binary_warped,(win_xright_low,win_y_low),(win_xright_high,win_y_high),255, 2) 
        #Identify the nonzero pixels in x and y within the window
        #trich xuat cac vi tru x = 1
        good_left_inds = ((nonzeroy >= win_y_low) & (nonzeroy < win_y_high) & (nonzerox >= win_xleft_low) & (nonzerox < win_xleft_high)).nonzero()[0]
        good_right_inds = ((nonzeroy >= win_y_low) & (nonzeroy < win_y_high) & (nonzerox >= win_xright_low) & (nonzerox < win_xright_high)).nonzero()[0]
        #Append these indices to the lists
        left_lane_inds.append(good_left_inds)
        right_lane_inds.append(good_right_inds)
        # If you found > minpix pixels, recenter next window on their mean position
        # cal adventage lane x
        if len(good_left_inds) > minpix:
            leftx_current = np.int(np.mean(nonzerox[good_left_inds]))
            #print leftx_current
        if len(good_right_inds) > minpix:        
            rightx_current = np.int(np.mean(nonzerox[good_right_inds]))
    # Concatenate the arrays of indices
    # noi cac mang lai voi nhau
    #print len(left_lane_inds)
    left_lane_inds = np.concatenate(left_lane_inds)
    right_lane_inds = np.concatenate(right_lane_inds)
    #print len(left_lane_inds)
    # Extract left and right line pixel positions
    leftx = nonzerox[left_lane_inds]
    lefty = nonzeroy[left_lane_inds] 
    rightx = nonzerox[right_lane_inds]
    righty = nonzeroy[right_lane_inds]
    if (leftx.size < 5):
        left_lane.detected = False
        #print ("Left lane deteceted - False")
    else:
        left_lane.detected = True
        #print ("Left lane detected - true")                                                    
    if (rightx.size < 5):
        right_lane.detected = False
        #print ("Right lane detected False")
    else:
        right_lane.detected = True
        #print ("Right lane detected True")
    if left_lane.detected == True & right_lane.detected == True:
        # Fit a second order polynomial to each
        left_fit = np.polyfit(lefty, leftx, 2)
        right_fit = np.polyfit(righty, rightx, 2)
        left_lane.best_fit = np.vstack([left_lane.best_fit,left_fit])
        left_lane.best_fit[0] = left_fit
        right_lane.best_fit = np.vstack([right_lane.best_fit,right_fit])
        right_lane.best_fit[0] = right_fit
        left_lane.best_fit = np.average(left_lane.best_fit[-left_lane.smoothen_nframes:], axis = 0)
        right_lane.best_fit = np.average(right_lane.best_fit[-right_lane.smoothen_nframes:], axis = 0)
        #print ("saved best fit")
    else: 
        #use the history avg values 
        left_fit = left_lane.best_fit
        right_fit = right_lane.best_fit
    left_lane.detected == False
    right_lane.detected == False
    #cv2.imshow('sliding',binary_warped)
    return left_fit,right_fit,left_lane_inds, right_lane_inds
def poly_fit(binary_warped,left_fit,right_fit,left_lane_inds, right_lane_inds, plot=False):

    # Generate x and y values for plotting
    ploty = np.linspace(0, binary_warped.shape[0]-1, binary_warped.shape[0] )
    left_fitx = left_fit[0]*ploty**2 + left_fit[1]*ploty + left_fit[2]
    right_fitx = right_fit[0]*ploty**2 + right_fit[1]*ploty + right_fit[2]
    #k = np.int(left_fitx)
    # Identify the x and y positions of all nonzero pixels in the image
    nonzero = binary_warped.nonzero()
    nonzeroy = np.array(nonzero[0])
    nonzerox = np.array(nonzero[1])
    #print nonzeroy.shape
    #print left_lane_inds.shape
    out_img = np.dstack((binary_warped, binary_warped, binary_warped))*255
    out_img[nonzeroy[left_lane_inds], nonzerox[left_lane_inds]] = [255, 0, 0]
    out_img[nonzeroy[right_lane_inds], nonzerox[right_lane_inds]] = [0, 0, 255]
    out_img[ploty.astype('int'),left_fitx.astype('int')] = [0, 255, 255]
    out_img[ploty.astype('int'),right_fitx.astype('int')] = [0, 255, 255]
    #out_img[nonzeroy[left_fitx], nonzerox[left_fitx]] = [0, 0, 255]
    #k = np.array (left_fitx,dtype=int)
    #out_img[ploty,k] = [0, 255,0 ]
    cv2.imshow('out_img',out_img)
    if(plot):
        plt.imshow(out_img)
        plt.plot(left_fitx, ploty, color='yellow')
        plt.plot(right_fitx, ploty, color='yellow')
        plt.xlim(0,640)
        plt.ylim(480, 0)
        plt.show()

    return left_fitx,right_fitx
#left_fit,right_fit,left_lane_inds, right_lane_inds=sliding_window(im_bw)
#left_fitx,right_fitx=poly_fit(im_bw,left_fit,right_fit,left_lane_inds, right_lane_inds, plot=True)
#polynomial to each lane left and right
#left_fit = np.polyfit(lefty,leftx,2)
#right_fit = np.polyfit()
#return leftx, lefty, rightx, righty, left_lane_inds, right_lane_inds
#cv2.imshow('img_threshold',img_threshold)
#cv2.waitKey(0)


