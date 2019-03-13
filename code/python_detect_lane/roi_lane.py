import cv2
import numpy as np
# top right, bot righ, bot left, top left
def perspective_transform(img):
    imshape = img.shape
    #print (imshape)
    
    src = np.float32([[(0.55*imshape[1], 0.63*imshape[0]), \
                        (imshape[1],imshape[0]), \
                        (0,imshape[0]), \
                        (.45*imshape[1], 0.63*imshape[0])]])
    #print (vertices)
    #src= np.float32(vertices)
    dst = np.float32([[0.75*img.shape[1],0], \
                    [0.75*img.shape[1],img.shape[0]], \
                    [0.25*img.shape[1],img.shape[0]], \
                    [0.25*img.shape[1],0]])
    #print ('dst : %s'%(dst))
    M = cv2.getPerspectiveTransform(src, dst)
    #Minv = cv2.getPerspectiveTransform(dst, src)
    img_size = (imshape[1], imshape[0]) 
    perspective_img = cv2.warpPerspective(img, M, img_size, flags = cv2.INTER_LINEAR)    
    return perspective_img
def region_of_interest(img, vertices):
    #defining a blank mask to start with
    ignore_mask_color=255
    mask = np.zeros_like(img, dtype=np.uint8)
    #filling pixels inside the polygon defined by "vertices" with the fill color    
    #print ('src ',vertices)    
    #cv2.namedWindow('mask', cv2.WINDOW_NORMAL)    
    cv2.fillPoly(mask, vertices, ignore_mask_color)
    #cv2.imshow('mask',mask)    
    #returning the image only where mask pixels are nonzero
    masked_image = cv2.bitwise_and(img, mask)
    return masked_image
