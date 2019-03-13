#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main( int argc, char** argv ) {

       Mat source= imread("/home/hoaiphuong/Pictures/9.png",CV_LOAD_IMAGE_COLOR);
    // get a new frame from camera

    // cout << source.size() << endl;

    Rect ROI(0,180,source.cols,200);

    // Mat source = imread("/home/hoaiphuong/Desktop/photos/10.png",CV_LOAD_IMAGE_COLOR);
    imshow("source", source);
      source=source(ROI);
    Mat imgHLS;
    Mat imgHSV;
    Mat imGray;
    Mat imBinary;
    Mat imBRG;
    cvtColor(source, imgHLS, COLOR_RGB2HLS); //Convert the captured frame from BGR to HSV
    cvtColor(source, imgHSV, COLOR_RGB2HSV);
    // cvtColor(source, imBRG, COLOR_RGB2BGR);
    // threshold( imGray, imBinary, 194, 255,0 );
    Mat imgThresholded;
    Mat imgThresholded1;
    inRange(imgHLS, Scalar(86, 101, 49), Scalar(95, 149, 152), imgThresholded); //Threshold the image
   //    inRange(imgHSV, Scalar(11, 39, 166), Scalar(20, 74, 210), imgThresholded1);
    inRange(imgHSV, Scalar(0, 6, 49), Scalar(44, 62, 255), imgThresholded1);
    imshow("trang", imgThresholded1);


    imshow("vang", imgThresholded);
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

        Point2f inputQuad[4];
        Point2f outputQuad[4];
        /*
        inputQuad[0] = Point2f( 280,190);
        inputQuad[1] = Point2f( 0,300);
        inputQuad[2] = Point2f( 590,300);
        inputQuad[3] = Point2f( 380,190);
        // The 4 points where the mapping is to be done , from top-left in clockwise order
        outputQuad[0] = Point2f(0,0 );
        outputQuad[1] = Point2f( 0,(imgThresholded.rows-1));
        outputQuad[2] = Point2f( imgThresholded.cols-1,(imgThresholded.rows-1));
        outputQuad[3] = Point2f( imgThresholded.cols-1,0);
         */
    inputQuad[0] = Point2f( 245,30);
    inputQuad[1] = Point2f( 50,120);
    inputQuad[2] = Point2f( 590,120);
    inputQuad[3] = Point2f( 420,30);
    // The 4 points where the mapping is to be done , from top-left in clockwise order
    outputQuad[0] = Point2f(0,0 );
    outputQuad[1] = Point2f( 0,(imgThresholded.rows-1));
    outputQuad[2] = Point2f( imgThresholded.cols-1,(imgThresholded.rows-1));
    outputQuad[3] = Point2f( imgThresholded.cols-1,0);

        Mat transform= getPerspectiveTransform(inputQuad,outputQuad);
        warpPerspective(imgThresholded,imgThresholded,transform,imgThresholded.size());
        imshow("warpPerspective", imgThresholded);
        threshold(imgThresholded,imgThresholded,100,255,THRESH_BINARY);

        int maxL=0;
        int maxR=0;
        vector<int> positionL,positionR;

        for(int i =0; i< imgThresholded.cols;i++)
        {
            if( i< imgThresholded.cols/2) {
                if (countNonZero(imgThresholded.col(i)) >= maxL) {
                    maxL = countNonZero(imgThresholded.col(i));
                    positionL.push_back(i);
                }
            } else{
                if (countNonZero(imgThresholded.col(i)) >= maxR) {
                    maxR = countNonZero(imgThresholded.col(i));
                    positionR.push_back(i);
                }
            }
        }
//////============ set slibding window =====================///////////////////////
    int nwindows = 9;
    int window_height =  imgThresholded.rows/nwindows;

    int margin= 35;

    Size imgSize(imgThresholded.size());


///================== store center lane left, right ============///////
    vector<Point> centerLaneLeft;
    vector<Point> centerLaneRight;

    for (int i =0 ; i< nwindows-1; i++)
    {
        int win_y_low =imgThresholded.rows-(i+1)*window_height ;
        int win_y_high = imgThresholded.rows - i*window_height ;
        int win_xleft_low = positionL - margin ;
        int win_xleft_high = positionL + margin ;
        int win_xright_low = positionR - margin ;
        int win_xright_high = positionR + margin ;
        int marginX_low=35;
        int marginX_high=35;
        int win_size= 70;
        bool test= true;
        int minpixel= 50;
        if(win_xleft_low <0 )
            win_xleft_low = 0;

        if(win_xright_high >imgSize.width) {
            win_xright_high = imgSize.width;

        }
        if(win_xright_high+ marginX_high >imgSize.width ) {
            marginX_high = 0;
            win_xright_high = imgSize.width;
        }

        if( win_xleft_low-marginX_low <0) {
            marginX_low = 0;
            win_xleft_low = 0;
        }
        Rect Left1(win_xleft_low-marginX_low,win_y_low,win_size,win_y_high-win_y_low);
        Rect Left(win_xleft_low,win_y_low,win_size,win_y_high-win_y_low);
        Rect Left2(win_xleft_low+35,win_y_low,win_size,win_y_high-win_y_low);
        Rect Rifgt1(win_xright_low-35,win_y_low,win_size,win_y_high-win_y_low);
        Rect Rifgt(win_xright_low,win_y_low,win_size,win_y_high-win_y_low);
        Rect Rifgt2(win_xright_low+marginX_high,win_y_low,win_xright_high-win_xright_low,win_y_high-win_y_low);

        int demL=countNonZero(imgThresholded(Left));
        int demR= countNonZero(imgThresholded(Rifgt));
        int demL1=countNonZero(imgThresholded(Left1));
        int demR1= countNonZero(imgThresholded(Rifgt1));
        int demL2=countNonZero(imgThresholded(Left2));
        int demR2= countNonZero(imgThresholded(Rifgt2));

        if(demL<demL1 && demL1 > minpixel )
        {
            test= false;
            positionL= positionL-35;

        }
        if(demL<demL2  && demL2 > minpixel )
        {
            test= false;
            positionL= positionL+35;

        }
        if(demR<demR1  && demR1 > minpixel )
        {
            test= false;
            positionR= positionR-35;

        }
        if(demR<demR2  && demR2 > minpixel )
        {
            test= false;
            positionR= positionR+35;

        }
        if(test)
        {
            centerLaneLeft.push_back(Point(positionL,(win_y_high+win_y_low)/2)) ;
            centerLaneRight.push_back(Point(positionR,(win_y_high+win_y_low)/2)) ;
        } else
            i=i-1;

    }
    for( int i =0;i< centerLaneLeft.size();i++)
    {
        circle(source,centerLaneLeft[i],5,255);
    }
    for( int i =0;i< centerLaneRight.size();i++)
    {
        circle(source,centerLaneRight[i],5,255);
    }
    imshow("ve tam ",source);


/////////========== Tim phuong trinh duong thang cua  lane trai ==============////////////////
////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

    float x1L = 0, x2L = 0, x3L = 0, x4L = 0, y1L = 0, xyL = 0, xy2L = 0;
    Mat matrixAL = Mat(3, 3, CV_32FC1);
    Mat matrixBL = Mat(3, 1, CV_32FC1);
    Mat matrixCoefficientABCL = cv::Mat(3, 1, CV_32FC1);

    for (int i = 0; i < centerLaneLeft.size(); i++) {
        int x = centerLaneLeft[i].y;
        int y = centerLaneLeft[i].x;
        x1L = x1L + x;
        x2L = x2L + x * x;
        x3L = x3L + x * x * x;
        x4L = x4L + x * x * x * x;
        y1L = y1L + y;
        xyL = xyL + x * y;
        xy2L = xy2L + x * x * y;
    }
/////////========== Tim phuong trinh duong thang cua  lane phai ==============////////////////
////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
    float x1R = 0, x2R = 0, x3R = 0, x4R = 0, y1R = 0, xyR = 0, xy2R = 0;
    Mat matrixAR = Mat(3, 3, CV_32FC1);
    Mat matrixBR = Mat(3, 1, CV_32FC1);
    Mat matrixCoefficientABCR = cv::Mat(3, 1, CV_32FC1);
    for (int i = 0; i < centerLaneRight.size(); i++) {
        int x = centerLaneRight[i].y;
        int y = centerLaneRight[i].x;
        x1R = x1R + x;
        x2R = x2R + x * x;
        x3R = x3R + x * x * x;
        x4R = x4R + x * x * x * x;
        y1R = y1R + y;
        xyR = xyR + x * y;
        xy2R = xy2R + x * x * y;
    }
////////////////===================== Dua cac gia tri vao ma tran ================///////////////////////
///////////================ Left ===========///////////////
    matrixAL.at<float>(0, 0) = centerLaneLeft.size();

    matrixAL.at<float>(0, 1) = x1L;
    matrixAL.at<float>(1, 0) = x1L;

    matrixAL.at<float>(0, 2) = x2L;
    matrixAL.at<float>(1, 1) = x2L;
    matrixAL.at<float>(2, 0) = x2L;

    matrixAL.at<float>(1, 2) = x3L;
    matrixAL.at<float>(2, 1) = x3L;

    matrixAL.at<float>(2, 2) = x4L;

    matrixBL.at<float>(0, 0) = y1L;
    matrixBL.at<float>(1, 0) = xyL;
    matrixBL.at<float>(2, 0) = xy2L;

///////////================ Right ===========///////////////
    matrixAR.at<float>(0, 0) = centerLaneRight.size();

    matrixAR.at<float>(0, 1) = x1R;
    matrixAR.at<float>(1, 0) = x1R;

    matrixAR.at<float>(0, 2) = x2R;
    matrixAR.at<float>(1, 1) = x2R;
    matrixAR.at<float>(2, 0) = x2R;

    matrixAR.at<float>(1, 2) = x3R;
    matrixAR.at<float>(2, 1) = x3R;

    matrixAR.at<float>(2, 2) = x4R;

    matrixBR.at<float>(0, 0) = y1R;
    matrixBR.at<float>(1, 0) = xyR;
    matrixBR.at<float>(2, 0) = xy2R;


////////////=============== Tim phuong trinh duong lane======================//////////////////////
/////////================ Left ================//////////////
    matrixAL = matrixAL.inv();
    matrixCoefficientABCL = matrixAL * matrixBL;
/////////================ Right ================//////////////
    matrixAR = matrixAR.inv();
    matrixCoefficientABCR = matrixAR * matrixBR;
    Mat ImageContours(imgThresholded.size(), CV_8UC1, Scalar(0));
    for (int row = 0; row < imgThresholded.rows; row++) {
        int x = (matrixCoefficientABCL.at<float>(0, 0) + matrixCoefficientABCL.at<float>(1, 0) * row +
                 matrixCoefficientABCL.at<float>(2, 0) * row * row);
        int x2 = (matrixCoefficientABCR.at<float>(0, 0) + matrixCoefficientABCR.at<float>(1, 0) * row +
                 matrixCoefficientABCR.at<float>(2, 0) * row * row);
        ImageContours.at<uchar>(Point(x, row)) = 255;
        ImageContours.at<uchar>(Point(x2, row)) = 255;
        //   int yR= (matrixCoefficientABCR.at<float>(0,0) + matrixCoefficientABCR.at<float>(1,0)*col+ matrixCoefficientABCR.at<float>(2,0)*col*col);
        //   ImageContours.at<uchar>(Point(col,yR))= 255;

    }

  //  cout << matrixCoefficientABCL << endl;
    matrixCoefficientABCL= (matrixCoefficientABCL + matrixCoefficientABCR)/2;
  //  cout << matrixCoefficientABCL << endl;
  //  cout << matrixCoefficientABCR << endl;

    //  int col=1;
    //  int y= (matrixCoefficientABC.at<float>(0,0) + matrixCoefficientABC.at<float>(1,0)*col+ matrixCoefficientABC.at<float>(2,0)*col*col);
    // cout << y << endl;



    for (int row = 0; row < imgThresholded.rows; row++) {
        int x = (matrixCoefficientABCL.at<float>(0, 0) + matrixCoefficientABCL.at<float>(1, 0) * row +
                  matrixCoefficientABCL.at<float>(2, 0) * row * row);

        ImageContours.at<uchar>(Point(x, row)) = 255;

        //   int yR= (matrixCoefficientABCR.at<float>(0,0) + matrixCoefficientABCR.at<float>(1,0)*col+ matrixCoefficientABCR.at<float>(2,0)*col*col);
        //   ImageContours.at<uchar>(Point(col,yR))= 255;

    }


    imshow("ve imgThresholded ",ImageContours);






/*
        for (int i = 0; i < centerLane.size(); i++) {
            ImageContours.at<uchar>(Point(centerLane[i].x, centerLane[i].y)) = 255;
        }


        imshow("ve tam duong", ImageContours);
*/


    waitKey(0);
    return 0;
}
