#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main() {

    Size patternsize(7, 7);
    int numBoards = 30;

    vector<vector<Point3f>> object_points;
    vector<vector<Point2f>> image_points;

    vector<Point2f> corners;
    //  int numSquares=3;
    int successes = 0;


    Mat gray;
    //  Mat source = imread("/home/hoaiphuong/Pictures/3.png");
    vector<Point3f> obj;


    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            obj.push_back(Point3f((float) j * 25, (float) i * 25, 0));


    //   for(int j=0;j<49;j++)
    //     obj.push_back(Point3f(j/7, j%7, 0.0f));


    Mat source;

    for (;;) {
        source= imread("/home/hoaiphuong/Desktop/photos/image/Image"+ to_string(successes) + ".jpg");
     //   resize(source,source,Size(480))
        cvtColor(source, gray, CV_BGR2GRAY);

        bool patternfound = findChessboardCorners(gray, patternsize, corners,
                                                  CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE +
                                                  CALIB_CB_FAST_CHECK);

        if (patternfound) {
            cornerSubPix(gray, corners, Size(7,7 ), Size(-1, -1),
                         TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, 0.1));
            drawChessboardCorners(source, patternsize, corners, patternfound);
            // image_points.push_back(corners);
            // object_points.push_back(obj);
            // printf("Snap stored!\n");

            image_points.push_back(corners);
            object_points.push_back(obj);
        }


        //imshow("hinh", source);
            successes++;
          //  waitKey(0);
            if (successes >= numBoards)
                break;


        // cap >> source;
        // if(waitKey(30) >= 0) break;
        //   if(successes>=numBoards) {
        //       break;
        //      }
//


    }

    Mat intrinsic = Mat(3, 3, CV_32FC1);
    Mat distCoeffs;
    vector<Mat> rvecs;
    vector<Mat> tvecs;

    intrinsic.ptr<float>(0)[0] = 1;
    intrinsic.ptr<float>(1)[1] = 1;

    calibrateCamera(object_points, image_points, source.size(), intrinsic, distCoeffs, rvecs, tvecs);

    cout << intrinsic << endl;
    cout << distCoeffs << endl;
    Mat imageUndistorted;
    VideoCapture cap2("/home/hoaiphuong/Desktop/photos/hinh2.avi");

    Size kichthuoc(1280,720);
    Mat view, map1, map2;
    Mat rview;
      initUndistortRectifyMap(intrinsic, distCoeffs, Mat(),
                        getOptimalNewCameraMatrix(intrinsic, distCoeffs,  kichthuoc, 0,  source.size(),0,true),
                              source.size(), CV_16SC2, map1, map2);


 //   int width=cap2.get(CV_CAP_PROP_FRAME_WIDTH);
 //   int height=cap2.get(CV_CAP_PROP_FRAME_HEIGHT);
     VideoWriter video("output.avi",CV_FOURCC('M','J','P','G'),15, Size(431,297));

    while (1) {


        cap2 >> source;

            remap(source, rview, map1, map2, INTER_LINEAR);
            imshow("Image View", rview);
        //cout << rview.size()<< endl;
            Rect Roi(93,86,451,279);

        Mat c;
        Point2f pc(rview.cols/2., rview.rows/2.);
        Mat r = getRotationMatrix2D(pc, -180, 1.0);

        warpAffine(rview, c, r, rview.size()); // what size I should use?
        imshow("b",c);
         c= c(Roi);

        imshow("source",source);
     //   video.write(c);
        imshow("c",c);

           // waitKey(0);
           // char c = waitKey();
          //  if( c  == ESC_KEY || c == 'q' || c == 'Q' )
          //      break;


/*
        cap2 >> view;
        undistort(view, imageUndistorted, intrinsic, distCoeffs,
                  getOptimalNewCameraMatrix(intrinsic, distCoeffs, kichthuoc, 0, view.size(),0, false));
        //  video.write(imageUndistorted);
        //    cout << imageUndistorted.size()<< endl;
        imshow("win1", view);
        imshow("win2", imageUndistorted);
*/
        if (waitKey(30) >= 0) break;
    }

    return 0;
}