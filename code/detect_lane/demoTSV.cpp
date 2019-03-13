//#include "Controller.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;
double TinhGoc(Point xe, Point center);
using namespace std;
using namespace cv;
int positionL=110 ,positionR= 545, positionCenter=330;

double TinhGoc(Point xe, Point center);
int main( int argc, char** argv ) {
    int positionXe = 330;
    //   Controller *_Controller = new Controller();
// VideoWriter video("lane2.avi",CV_FOURCC('M','J','P','G'),15, Size(640,480));
    VideoCapture cap(0);// open the default camera
    // VideoCapture cap("/home/hoaiphuong/Desktop/photos/hinh.avi");
    if(!cap.isOpened())  // check if we succeeded

    {
        cout <<"loi" << endl;
        return -1;
    }

    float intrinsicL[9] = { 4021.647192987246, 0, 289.2681406534879,0, 2063.840795697641, 296.9477514262573,0, 0, 1 };
    float distCoeffsL[5]= {-20.85217395672828, 496.0780489644387, -0.1519992457109527, 0.007373718783583586, -5847.008198395703};

    float intrinsicR[9] = { 418.4535213265403, 0, 317.2840861380859,0, 420.3090290275157, 213.8242139124298,0, 0, 1 };
    float distCoeffsR[5]= {-0.6785932084546785, 0.5380945237244258, 0.02055055818394564, 0.01534725027174448, -0.1741468280123287};

    Mat matrixcameraL  =Mat(3, 3, CV_32F, intrinsicL);
    Mat DistortionCoefficientsL=Mat(1, 5, CV_32F, distCoeffsL);
    Mat matrixcameraR  =Mat(3, 3, CV_32F, intrinsicL);
    Mat DistortionCoefficientsR=Mat(1, 5, CV_32F, distCoeffsL);






    Mat view, map1, map2,map3, map4;
    Mat rview;
    Size kichthuoc(1280,720);





    initUndistortRectifyMap(matrixcameraL, DistortionCoefficientsL, Mat(),
                            getOptimalNewCameraMatrix(matrixcameraL, DistortionCoefficientsL,  kichthuoc, 0,  Size(640,480),0, true),
                            Size(640,480), CV_16SC2, map1, map2);

    initUndistortRectifyMap(matrixcameraR, DistortionCoefficientsR, Mat(),
                            getOptimalNewCameraMatrix(matrixcameraR, DistortionCoefficientsR,  kichthuoc, 0,  Size(640,480),0, true),
                            Size(640,480), CV_16SC2, map3, map4);

    Rect ROI1(0,0,640,480);
    Rect ROI2(0,250,640,150);

    for(;;) {

        Mat frame;
        cap >> frame; // get a new frame from camera
        double prevTickCount = getTickCount();


        Mat ImageLeft = frame(ROI1);
        remap(ImageLeft, ImageLeft, map1, map2, INTER_LINEAR);


        Point2f pc(ImageLeft.cols / 2., ImageLeft.rows / 2.);
        Mat r = getRotationMatrix2D(pc, -180, 1.0);

        //  warpAffine(ImageRight, ImageRight, r, ImageRight.size()); // what size I should use?
        warpAffine(ImageLeft, ImageLeft, r, ImageLeft.size());

        //  imshow("left",ImageLeft);
        ImageLeft = ImageLeft(ROI2);
        //Rect ROI(0,180,source.cols,200);

        // Mat source = imread("/home/hoaiphuong/Desktop/photos/10.png",CV_LOAD_IMAGE_COLOR);

        //   source=source(ROI);

        Mat imgHSV;
        GaussianBlur(ImageLeft, ImageLeft, Size(5, 5), 5);
        //Convert the captured frame from BGR to HSV
        cvtColor(ImageLeft, imgHSV, COLOR_RGB2HSV);
        // cvtColor(source, imBRG, COLOR_RGB2BGR);
        // threshold( imGray, imBinary, 194, 255,0 );
        Mat imgThresholded;


        //    inRange(imgHSV, Scalar(11, 39, 166), Scalar(20, 74, 210), imgThresholded1);
        inRange(imgHSV, Scalar(0, 0, 200), Scalar(179, 255, 255), imgThresholded);



        //   erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        //    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        //morphological closing (fill small holes in the foreground)
        //   dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        //   erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        // bitwise_and(imgThresholded,mask,imgThresholded);




        Point2f inputQuad[4];
        Point2f outputQuad[4];

        inputQuad[0] = Point2f(235, 0);
        inputQuad[1] = Point2f(100, 149);
        inputQuad[2] = Point2f(639, 149);
        inputQuad[3] = Point2f(500, 0);
        // The 4 points where the mapping is to be done , from top-left in clockwise order
        outputQuad[0] = Point2f(87, 0);
        outputQuad[1] = Point2f(100, 149);
        outputQuad[2] = Point2f(500, 149);
        outputQuad[3] = Point2f(450, 0);

        Mat transform = getPerspectiveTransform(inputQuad, outputQuad);
        warpPerspective(imgThresholded, imgThresholded, transform, Size(640, 149));


        imshow("imgThresholded", imgThresholded);

        threshold(imgThresholded, imgThresholded, 100, 255, THRESH_BINARY);

        // int positionL=110 ,positionR= 545;



        for (int i = 330; i > 0; i--) {


            if (imgThresholded.at<uchar>(Point(i, 30)) == 255) {
                positionL = i;
                break;
            }
        }
        for (int i = 330; i < imgThresholded.cols; i++) {


            if (imgThresholded.at<uchar>(Point(i, 30)) == 255) {
                positionR = i;
                break;
            }
        }
        cout << "trai " << positionL << " phai " << positionR << endl;

        positionCenter = (positionL + positionR) / 2;

        cout << " diem xe 330, diem lane " << positionCenter << endl;

        double theta = TinhGoc(Point(330, 110), Point(positionCenter, 30));

        //   _Controller->Speed(90,90);
        //    _Controller->Handle(theta);

        if (theta > 0) {
            cout << "cua phai goc " << theta << endl;

        } else
            cout << "cua trai goc " << theta << endl;

        double executionTime = (getTickCount() * 1.0000 - prevTickCount * 1.0000) / (getTickFrequency() * 1.0000);
        cout << "execution Time = " << executionTime << " s" << endl;

        if(waitKey(30)==27)
        {

            break;
        }


    }

    //  _Controller->Speed(0,0);
    //  _Controller->Handle(0);
    return 0;
}


double TinhGoc(Point xe, Point center)
{
    float a= xe.y-center.y;
    float b= abs(center.x-xe.x);
    if (b <10)
        return 0;

    double pi = acos(-1.0);
    if( center.x > xe.x)
        return  (atan(b/a)*180) / pi;
    else
        return  -(atan(b/a)*180) / pi;
}
