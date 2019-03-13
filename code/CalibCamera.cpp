#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/calib3d/calib3d.hpp"
using  namespace std;
using namespace cv;
int main() {

    VideoCapture cap("/home/hoaiphuong/Desktop/depthVideo.avi");// open the default camera
   // VideoCapture cap("/home/hoaiphuong/Desktop/photos/hinh.avi");
    if(!cap.isOpened())  // check if we succeeded

    {
        cout <<"loi" << endl;
        return -1;
    }

    float intrinsicL[9] = {252.8578156159297, 0, 327.7155720972848,
 0, 256.238463076993, 253.3375750459992,0, 0, 1 };
    float distCoeffsL[5]= {-0.2616389054889315, 0.06165360001187491, 0.0007497641752947679, -0.002608843059660595, -0.005927874747253872};

 //   float intrinsicR[9] = { 418.4535213265403, 0, 317.2840861380859,0, 420.3090290275157, 213.8242139124298,0, 0, 1 };
 //   float distCoeffsR[5]= {-0.6785932084546785, 0.5380945237244258, 0.02055055818394564, 0.01534725027174448, -0.1741468280123287};

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
    for(;;)
    {

        Mat frame;
        cap >> frame; // get a new frame from camera

        imshow("a",frame);

        Rect ROI1(0,0,frame.cols/2,frame.rows);
        Rect ROI2(frame.cols/2,0,frame.cols- frame.cols/2,frame.rows);

        Mat ImageLeft = frame(ROI1);
        Mat ImageRight = frame(ROI2);
        imshow("ImageLeft",ImageLeft);
        imshow("ImageRight",ImageRight);

        remap(ImageLeft, ImageLeft, map1, map2, INTER_LINEAR);
        remap(ImageRight, ImageRight, map3, map4, INTER_LINEAR);


        Point2f pc(ImageLeft.cols/2., ImageLeft.rows/2.);
        Mat r = getRotationMatrix2D(pc, -180, 1.0);

       warpAffine(ImageRight, ImageRight, r, ImageRight.size()); // what size I should use?
       warpAffine(ImageLeft, ImageLeft, r, ImageLeft.size());
        imshow("right",ImageRight);
        imshow("left",ImageLeft);
        Mat grayImageLeft, grayImageRight,results;
        /*

        Mat ImageLeft ;
        Mat ImageRight ;
        Mat grayImageLeft, grayImageRight,results;
        ImageLeft= imread("/home/hoaiphuong/Pictures/Tsukuba_L.png");
        ImageRight= imread("/home/hoaiphuong/Pictures/Tsukuba_R.png");
*/
      //  Mat grayImageLeft, grayImageRight,results;
        cvtColor(ImageLeft, grayImageLeft, CV_BGR2GRAY);
        cvtColor(ImageRight, grayImageRight, CV_BGR2GRAY);

        cv::Ptr<cv::StereoBM> sbm = cv::StereoBM::create();
/*
        sbm->setMinDisparity(4);
        sbm ->setNumDisparities(128);
        sbm ->setBlockSize(21);
        sbm->setSpeckleRange(16);
        sbm ->setSpeckleWindowSize(45);
*/


        sbm->compute(grayImageLeft,grayImageRight,results);
        normalize(results, results, 0, 255, CV_MINMAX, CV_8U);

        imshow("results",results);


      /*  //
        int numBoards = 0;
        int numCornersHor;
        int numCornersVer;
        int numSquares = numCornersHor * numCornersVer;
        Size board_sz = Size(numCornersHor, numCornersVer);

        vector<vector<Point3f>> object_points;
        vector<vector<Point2f>> image_points;
        vector<Point2f> corners;
        int successes=0;


        //Mat image;
        // Mat gray_image;
        //capture >> image;
        vector<Point3f> obj;
        for(int j=0;j<numSquares;j++)
            obj.push_back(Point3f(j/numCornersHor, j%numCornersHor, 0.0f));





        while(successes<numBoards)
        {
            cvtColor(image, gray_image, CV_BGR2GRAY);

            bool found = findChessboardCorners(image, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

            if(found)
            {
                cornerSubPix(gray_image, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
                drawChessboardCorners(gray_image, board_sz, corners, found);
            }

            imshow("win1", image);
            imshow("win2", gray_image);

            capture >> image;

            int key = waitKey(1);

            if(key==27)
                return 0;

            if(key==' ' && found!=0)
            {
                image_points.push_back(corners);
                object_points.push_back(obj);
                printf("Snap stored!\n");

                successes++;

                if(successes>=numBoards)
                    break;
            }
        }

        Mat intrinsic = Mat(3, 3, CV_32FC1);
        Mat distCoeffs;
        vector<Mat> rvecs;
        vector<Mat> tvecs;

        intrinsic.ptr<float>(0)[0] = 1;
        intrinsic.ptr<float>(1)[1] = 1;

        calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);

        Mat imageUndistorted;
        while(1)
        {
            capture >> image;
            undistort(image, imageUndistorted, intrinsic, distCoeffs);

            imshow("win1", image);
            imshow("win2", imageUndistorted);

*/
        waitKey(0);
            if(waitKey(30) >= 0) break;
        }
        return 0;
    }
