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
    // int width=cap.get(CV_CAP_PROP_FRAME_WIDTH);
    // int height=cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    // VideoWriter video("output.avi",CV_FOURCC('M','J','P','G'),30, Size(width,height));


    Mat source;

    for (;;) {
        source= imread("/home/hoaiphuong/Desktop/photos/image/Image"+ to_string(successes) + ".jpg");
     //   resize(source,source,Size(480))
        cvtColor(source, gray, CV_BGR2GRAY);

        bool patternfound = findChessboardCorners(gray, patternsize, corners,
                                                  CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE +
                                                  CALIB_CB_FAST_CHECK);

        if (patternfound) {
            cornerSubPix(gray, corners, Size(11,11 ), Size(-1, -1),
                         TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, 0.1));
            drawChessboardCorners(source, patternsize, corners, patternfound);
            // image_points.push_back(corners);
            // object_points.push_back(obj);
            // printf("Snap stored!\n");

            image_points.push_back(corners);
            object_points.push_back(obj);
        }


        imshow("hinh", source);
            successes++;
            waitKey(0);
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


    Mat view, rview, map1, map2;
       initUndistortRectifyMap(intrinsic, distCoeffs, Mat(),
                         getOptimalNewCameraMatrix(intrinsic, distCoeffs, source.size(), 1,  source.size()),
                          source.size(), CV_16SC2, map1, map2);
    while (1) {


        cap2 >> source;

            remap(source, rview, map1, map2, INTER_LINEAR);
            imshow("Image View", rview);
        cout << rview.size()<< endl;
            Rect Roi(23,54,48,72);
            Mat a= rview(Roi);
            Mat b;
            resize(a,b,Size(480,320));
            imshow("b",b);
            waitKey(0);
           // char c = waitKey();
          //  if( c  == ESC_KEY || c == 'q' || c == 'Q' )
          //      break;


/*
        cap2 >> view;
        undistort(view, imageUndistorted, intrinsic, distCoeffs,
                  getOptimalNewCameraMatrix(intrinsic, distCoeffs, view.size(), 1, view.size()));
        //  video.write(imageUndistorted);
        //    cout << imageUndistorted.size()<< endl;
        imshow("win1", view);
        imshow("win2", imageUndistorted);
*/
        if (waitKey(30) >= 0) break;
    }

    return 0;
}