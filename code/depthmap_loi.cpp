#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main() {

    Size patternsize(7, 7);
    int numBoards = 30;

    vector<vector<Point3f>> object_points;
    vector<vector<Point2f>> image_points1;
    vector<Point2f> corners1;


    vector<vector<Point2f>> image_points2;
    vector<Point2f> corners2;

    vector<vector<Point3f>> object_points3;
    vector< vector< Point2f > > left_img_points, right_img_points;
    int successes = 0;




    vector<Point3f> obj;


    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            obj.push_back(Point3f((float) j * 25, (float) i * 25, 0));

    bool patternfound1= false;
    bool patternfound2= false;

    Mat source1, source2;
    Mat gray1,gray2;
    for (;;) {
        source1= imread("/home/hoaiphuong/Desktop/photos/image/ImageLeft"+ to_string(successes) + ".jpg");
        source2= imread("/home/hoaiphuong/Desktop/photos/image/ImageRight"+ to_string(successes) + ".jpg");
     //   resize(source,source,Size(480))
        cvtColor(source1, gray1, CV_BGR2GRAY);
        cvtColor(source2, gray2, CV_BGR2GRAY);
         patternfound1 = findChessboardCorners(gray1, patternsize, corners1,
                                                  CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE +
                                                  CALIB_CB_FAST_CHECK);
         patternfound2 = findChessboardCorners(gray2, patternsize, corners2,
                                                  CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE +
                                                  CALIB_CB_FAST_CHECK);
        if (patternfound1==true) {
            cornerSubPix(gray1, corners1, Size(5,5 ), Size(-1, -1),
                         TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, 0.1));
            drawChessboardCorners(source1, patternsize, corners1, patternfound1);


        }
        if (patternfound2== true) {
            cornerSubPix(gray2, corners2, Size(5,5 ), Size(-1, -1),
                         TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, 0.1));
            drawChessboardCorners(source2, patternsize, corners2, patternfound2);


        }



        if (patternfound1 == true  && patternfound2  == true) {


            cout <<"da them corners"<< endl;
            image_points1.push_back(corners1);
            image_points2.push_back(corners2);
            object_points.push_back(obj);
        }







     //   imshow("source1", source1);
     //   imshow("source2", source2);
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


    for (int i = 0; i < image_points1.size(); i++) {
        vector< Point2f > v1, v2;
        for (int j = 0; j < image_points1[i].size(); j++) {
            v1.push_back(Point2f((double)image_points1[i][j].x, (double)image_points1[i][j].y));
            v2.push_back(Point2f((double)image_points2[i][j].x, (double)image_points2[i][j].y));
        }
        left_img_points.push_back(v1);
        right_img_points.push_back(v2);
    }


///////////////===========  Left ==================///////////////////
    Mat intrinsic = Mat(3, 3, CV_32FC1);
    Mat distCoeffs;
    vector<Mat> rvecs;
    vector<Mat> tvecs;

    intrinsic.ptr<float>(0)[0] = 1;
    intrinsic.ptr<float>(1)[1] = 1;

////////////============== Right ===================///////////////
    Mat intrinsic2 = Mat(3, 3, CV_32FC1);
    Mat distCoeffs2;
    vector<Mat> rvecs2;
    vector<Mat> tvecs2;

    intrinsic2.ptr<float>(0)[0] = 1;
    intrinsic2.ptr<float>(1)[1] = 1;

////////////============================////////////////////
    calibrateCamera(object_points, left_img_points, source1.size(), intrinsic, distCoeffs, rvecs, tvecs);
    calibrateCamera(object_points, right_img_points, source2.size(), intrinsic2, distCoeffs2, rvecs2, tvecs2);
   // cout <<"da chay den day "<< endl;

///////////////////=======================///////////////////////

    Mat R,E,F;
    Vec3d T;

    stereoCalibrate(object_points,left_img_points,right_img_points,intrinsic,distCoeffs,intrinsic2,distCoeffs2,source1.size(),R,T,E,F);
    cout <<"da chay den day "<< endl;
    Mat R1,R2,P1,P2,Q;
    stereoRectify(intrinsic,distCoeffs,intrinsic2,distCoeffs2,source1.size(),R,T,R1,R2,P1,P2,Q);
////////////////================////////////////////////////



    Mat imageUndistorted;
    VideoCapture cap2("/home/hoaiphuong/Desktop/photos/hinh2.avi");

    Size kichthuoc(1280,720);
    Mat view, map1, map2;
    Mat rview1,rview2;
    Mat  map3, map4;


////////////////////////////////////============///////////////
    initUndistortRectifyMap(intrinsic, distCoeffs,R1,P1,source1.size(),CV_16SC2,map1,map2);
    initUndistortRectifyMap(intrinsic2, distCoeffs2,R2,P2,source2.size(),CV_16SC2,map3,map4);

///////////////////////==========================////////////////////////////////////


 //   int width=cap2.get(CV_CAP_PROP_FRAME_WIDTH);
 //   int height=cap2.get(CV_CAP_PROP_FRAME_HEIGHT);


//    Mat ImageLeft =imread("/home/hoaiphuong/Desktop/photos/image/ImageLeft13.jpg");
 //   Mat ImageRight =imread("/home/hoaiphuong/Desktop/photos/image/ImageRight13.jpg");
    Mat ImageLeft;
for(;;) {

    cap2 >> ImageLeft;
    remap(ImageLeft, rview1, map1, map2, INTER_LINEAR);
  //  remap(ImageRight, rview2, map3, map4, INTER_LINEAR);


    imshow("rview1", rview1);
   // imshow("rview2", rview2);
    if(waitKey(30) >= 0) break;
}


/*
    Mat grayImageLeft, grayImageRight,results;
    cvtColor(rview1, grayImageLeft, CV_BGR2GRAY);
    cvtColor(rview2, grayImageRight, CV_BGR2GRAY);

    cv::Ptr<cv::StereoBM> sbm = cv::StereoBM::create();

    sbm->setMinDisparity(4);
    sbm ->setNumDisparities(128);
    sbm ->setBlockSize(21);
    sbm->setSpeckleRange(16);
    sbm ->setSpeckleWindowSize(45);

    sbm->compute(grayImageLeft,grayImageRight,results);
    normalize(results, results, 0, 255, CV_MINMAX, CV_8U);

    imshow("results",results);

*/


        //cout << rview.size()<< endl;
    /*
            Rect Roi(93,86,451,279);

        Mat c;
        Point2f pc(rview.cols/2., rview.rows/2.);
        Mat r = getRotationMatrix2D(pc, -180, 1.0);

        warpAffine(rview, c, r, rview.size()); // what size I should use?
        imshow("b",c);
      //   c= c(Roi);

        imshow("source",source);
     //   video.write(c);
       // imshow("c",c);

           // waitKey(0);
           // char c = waitKey();
          //  if( c  == ESC_KEY || c == 'q' || c == 'Q' )
          //      break;
*/

/*
        cap2 >> view;
        undistort(view, imageUndistorted, intrinsic, distCoeffs,
                  getOptimalNewCameraMatrix(intrinsic, distCoeffs, kichthuoc, 0, view.size(),0, false));
        //  video.write(imageUndistorted);
        //    cout << imageUndistorted.size()<< endl;
        imshow("win1", view);
        imshow("win2", imageUndistorted);
*/

    waitKey(0);

    return 0;
}

