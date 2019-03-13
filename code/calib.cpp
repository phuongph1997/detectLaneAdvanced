#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/calib3d/calib3d.hpp"
using  namespace std;
using namespace cv;
int main() {

    VideoCapture cap(0);// open the default camera
   // VideoCapture cap("/home/hoaiphuong/Desktop/photos/hinh.avi");
    if(!cap.isOpened())  // check if we succeeded

    {
        cout <<"loi" << endl;
        return -1;
    }

    float intrinsicL[9] = {252.8578156159297, 0, 327.7155720972848,0, 256.238463076993, 253.3375750459992,0, 0, 1 };
    float distCoeffsL[5]= {-0.2616389054889315, 0.06165360001187491, 0.0007497641752947679, -0.002608843059660595, -0.005927874747253872};

    float intrinsicR[9] = { 249.2328012582979, 0, 342.2416053148595, 0, 250.925843791375, 240.1380869179251, 0, 0, 1 };
    float distCoeffsR[5]= {-0.263126234967536, 0.06336631464190885, 0.00319951113422688, -0.0002882083266245706, -0.006085546912196437};

    Mat matrixcameraL  =Mat(3, 3, CV_32F, intrinsicL);
    Mat DistortionCoefficientsL=Mat(1, 5, CV_32F, distCoeffsL);
    Mat matrixcameraR  =Mat(3, 3, CV_32F, intrinsicR);
    Mat DistortionCoefficientsR=Mat(1, 5, CV_32F, distCoeffsR);






    Mat view, map1, map2,map3, map4;
    Mat rview;
    Size kichthuoc(1280,720);





    initUndistortRectifyMap(matrixcameraL, DistortionCoefficientsL, Mat(),
                            getOptimalNewCameraMatrix(matrixcameraL, DistortionCoefficientsL,  kichthuoc, 0,  Size(640,480),0, true),
                            Size(640,480), CV_16SC2, map1, map2);

    initUndistortRectifyMap(matrixcameraR, DistortionCoefficientsR, Mat(),
                            getOptimalNewCameraMatrix(matrixcameraR, DistortionCoefficientsR,  kichthuoc, 0,  Size(640,480),0, true),
                            Size(640,480), CV_16SC2, map3, map4);
    VideoWriter video1("lane1.avi",CV_FOURCC('M','J','P','G'),15, Size(640,480));
    VideoWriter video2("lane2.avi",CV_FOURCC('M','J','P','G'),15, Size(640,480));
 //   VideoWriter video3("lane3.avi",CV_FOURCC('M','J','P','G'),15, Size(1280,480));
    int dem =0;
    for(;;)
    {

        Mat frame;
        cap >> frame; // get a new frame from camera

       // imshow("a",frame);

        Rect ROI1(0,0,frame.cols/2,frame.rows);
        Rect ROI2(frame.cols/2,0,frame.cols- frame.cols/2,frame.rows);

        Mat ImageLeft = frame(ROI1);
        Mat ImageRight = frame(ROI2);
       // imshow("ImageLeft",ImageLeft);
       // imshow("ImageRight",ImageRight);

        remap(ImageLeft, ImageLeft, map1, map2, INTER_LINEAR);
        remap(ImageRight, ImageRight, map3, map4, INTER_LINEAR);


        Point2f pc(ImageLeft.cols/2., ImageLeft.rows/2.);
        Mat r = getRotationMatrix2D(pc, -180, 1.0);

       warpAffine(ImageRight, ImageRight, r, ImageRight.size()); // what size I should use?
       warpAffine(ImageLeft, ImageLeft, r, ImageLeft.size());
        imshow("right",ImageRight);
        imshow("left",ImageLeft);
         video1.write(ImageRight);
          video2.write(ImageLeft);
          /*
        Mat imgHLS;
        Mat imgHSV;
        Mat imGray;
        Mat imBinary;
        Mat imBRG;
        cvtColor(ImageLeft, imgHLS, COLOR_RGB2HLS); //Convert the captured frame from BGR to HSV
        cvtColor(ImageLeft, imgHSV, COLOR_RGB2HSV);
       // cvtColor(source, imBRG, COLOR_RGB2BGR);
       // threshold( imGray, imBinary, 194, 255,0 );
        Mat imgThresholded;
        Mat imgThresholded1;
        inRange(imgHLS, Scalar(86, 101, 49), Scalar(95, 149, 152), imgThresholded); //Threshold the image
        inRange(imgHSV, Scalar(11, 39, 166), Scalar(20, 74, 210), imgThresholded1);
    //    inRange(imBRG, Scalar(168, 183, 171), Scalar(255, 255, 255), imBRG);
      // imshow("imgThresholded1", imgThresholded1);


      //  imshow("imgThresholded", imgThresholded);
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        //morphological closing (fill small holes in the foreground)
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        erode(imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        //morphological closing (fill small holes in the foreground)
        dilate(imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        erode(imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));


        imgThresholded= imgThresholded+ imgThresholded1;
        imshow("imgThresholded2", imgThresholded);


          if (waitKey(30) == ' ')
                 {
                      cout << "Da chup"<< endl;
                     cv::imwrite( cv::format( "/home/ubuntu/Phuong/calib/image/Image%d.jpg", dem ), ImageLeft );
                     dem++;

          }
          */
               if (waitKey(30) == 27) break;
        }
        return 0;
    }
