import cv2
import numpy as np
from primesense import openni2
from primesense import _openni2 as c_api
openni2.initialize("/home/hoaiphuong/Downloads/OpenNI_2.3.0.43/Linux/OpenNI-Linux-x64-2.3/Redist")
dev = openni2.Device.open_any()
#depth_stream = dev.create_depth_stream()
rgb_stream = dev.create_color_stream()
#depth_stream.start()

#depth_stream.set_video_mode(c_api.OniVideoMode(pixelFormat = c_api.OniPixelFormat.ONI_PIXEL_FORMAT_DEPTH_100_UM, resolutionX = 640, resolutionY = 480, fps = 30))
rgb_stream.set_video_mode(c_api.OniVideoMode(pixelFormat=c_api.OniPixelFormat.ONI_PIXEL_FORMAT_RGB888, resolutionX=640, resolutionY=480, fps=30))
rgb_stream.start()
while True:
	#frame = depth_stream.read_frame()
	#frame_data = frame.get_buffer_as_uint16()
	#img = np.frombuffer(frame_data, dtype=np.uint16)
	#img.shape = (1, 480, 640)
	#img = np.concatenate((img, img, img), axis=0)
	#img = np.swapaxes(img, 0, 2)
	#img = np.swapaxes(img, 0, 1)
	#cv2.imshow("image", img)
	#cv2.waitKey(34)
    bgr   = np.fromstring(rgb_stream.read_frame().get_buffer_as_uint8(),dtype=np.uint8).reshape(480,640,3)
    rgb = cv2.cvtColor(bgr,cv2.COLOR_BGR2RGB)
    cv2.imshow('rgb', rgb)
    if cv2.waitKey(33) & 0xFF == ord('q'):
        break
cv2.destroyAllWindows()
rgb_stream.stop()
openni2.unload()
