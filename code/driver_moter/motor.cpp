#include "Controller.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;
double TinhGoc(Point xe, Point center);
int main()
{
    //VideoCapture cap("/home/ubuntu/Phuong/motor/lane2.avi");

    Controller *_Controller = new Controller();
// VideoWriter video("lane2.avi",CV_FOURCC('M','J','P','G'),15, Size(640,480));
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


    int positionL=145 ,positionR= 430;
    int positionXe=300;
     Rect ROI1(0,0,640,480);
    Rect ROI2(0,250,640,180);
    for(;;)
    {

        Mat frame;
        cap >> frame; // get a new frame from camera
        double prevTickCount = getTickCount();



        Mat ImageLeft = frame(ROI1);
        remap(ImageLeft, ImageLeft, map1, map2, INTER_LINEAR);



        Point2f pc(ImageLeft.cols/2., ImageLeft.rows/2.);
        Mat r = getRotationMatrix2D(pc, -180, 1.0);

        //  warpAffine(ImageRight, ImageRight, r, ImageRight.size()); // what size I should use?
        warpAffine(ImageLeft, ImageLeft, r, ImageLeft.size());

      //  imshow("left",ImageLeft);
        ImageLeft = ImageLeft(ROI2);
        Mat mask(ImageLeft.size(), CV_8UC1, Scalar(0));

        vector< vector<Point> >  co_ordinates;
        Point P1(220,0);
        Point P2(420,0);
        Point P3(630,179);
        Point P4(10,179);

        co_ordinates.push_back(vector<Point>());
        co_ordinates[0].push_back(P1);
        co_ordinates[0].push_back(P2);
        co_ordinates[0].push_back(P3);
        co_ordinates[0].push_back(P4);
        drawContours( mask,co_ordinates,0, Scalar(255),CV_FILLED );

        //Rect ROI(0,180,source.cols,200);

        // Mat source = imread("/home/hoaiphuong/Desktop/photos/10.png",CV_LOAD_IMAGE_COLOR);
        //  imshow("source", source);
        //   source=source(ROI);

        Mat imgHSV;
        GaussianBlur(ImageLeft,ImageLeft,Size(5,5),5);
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

      //  bitwise_and(imgThresholded,mask,imgThresholded);

      //  imshow("imgThresholded",imgThresholded);


        Point2f inputQuad[4];
        Point2f outputQuad[4];


        inputQuad[0] = Point2f( 220,0);
        inputQuad[1] = Point2f( 10,179);
        inputQuad[2] = Point2f( 630,179);
        inputQuad[3] = Point2f( 420,0);
        // The 4 points where the mapping is to be done , from top-left in clockwise order
        outputQuad[0] = Point2f(100,0 );
        outputQuad[1] = Point2f( 100,179);
        outputQuad[2] = Point2f( 500,179);
        outputQuad[3] = Point2f( 500,0);

        Mat transform= getPerspectiveTransform(inputQuad,outputQuad);
        warpPerspective(imgThresholded,imgThresholded,transform,Size(640,180));





        threshold(imgThresholded,imgThresholded,100,255,THRESH_BINARY);

        int maxL=70;
        int maxR=70;



        for(int i =positionXe; i>0;i--)
        {
            int k= countNonZero(imgThresholded.col(i));
            if(k>= maxL) {
                positionL = i;
                // break;
                maxL= k;
            }

        }

        for(int i =positionXe; i<imgThresholded.cols;i++)
        {
            //  cout<< countNonZero(imgThresholded.col(i))<< endl;
            int k= countNonZero(imgThresholded.col(i));
            if(countNonZero(imgThresholded.col(i))>= maxR) {
                positionR = i;
                // break;
                maxR= k;
            }

        }







        //////============ set slibding window =====================///////////////////////
        int nwindows = 9;
        int window_height =  imgThresholded.rows/nwindows;

        int margin= 10;

        Size imgSize(imgThresholded.size());


        ///================== store center lane left, right ============///////
        vector<Point> centerLaneLeft;
        vector<Point> centerLaneRight;
        bool magin_left= false;
        bool have_magin_left= false;
        bool have_magin_right= false;
        bool magin_right= false;
        bool magin_start= false;
        for (int i =0 ; i< nwindows; i++)
        {
            int win_y_low =imgThresholded.rows-(i+1)*window_height ;
            int win_y_high = imgThresholded.rows - i*window_height ;
            int win_xleft_low = positionL - margin ;
            int win_xleft_high = positionL + margin ;
            int win_xright_low = positionR - margin ;
            int win_xright_high = positionR + margin ;
            int marginX_low=10;
            int marginX_high=10;
            int win_size=20;
            bool test= true;
            int minpixel= 30;
            if(win_xleft_low <0 )
                win_xleft_low = 0;

            if(win_xright_high >imgSize.width) {
                win_xright_high = imgSize.width;

            }

            if(win_xright_low+ marginX_high >imgSize.width ) {
                marginX_high = 0;

            }
            Rect Right(win_xright_low,win_y_low,win_xright_high-win_xright_low,win_y_high-win_y_low);
            Rect Left(win_xleft_low,win_y_low,win_xleft_high-win_xleft_low,win_y_high-win_y_low);
            Rect Left2(win_xleft_low + 10, win_y_low, win_xleft_high - win_xleft_low, win_y_high - win_y_low);
            Rect Right1(win_xright_low - 10, win_y_low, win_xright_high - win_xright_low, win_y_high - win_y_low);

            if (win_xleft_low - marginX_low < 0) {
                marginX_low = 0;
                win_xleft_low = 0;
            }
            if (win_xright_high + marginX_high > imgSize.width) {
                win_size = imgSize.width - (win_xright_low + marginX_high);
            }
            Rect Left1(win_xleft_low - marginX_low, win_y_low, win_xleft_high - win_xleft_low, win_y_high - win_y_low);
            Rect Right2(win_xright_low + marginX_high, win_y_low, win_size, win_y_high - win_y_low);
            int demL=countNonZero(imgThresholded(Left));
            int demR= countNonZero(imgThresholded(Right));
            if(magin_start == false)
            {
                int demL1=countNonZero(imgThresholded(Left1));
                int demR1= countNonZero(imgThresholded(Right1));
                int demL2=countNonZero(imgThresholded(Left2));
                int demR2= countNonZero(imgThresholded(Right2));
                if( demL1 > demL2) {
                    if (demL < demL1 && demL1 > minpixel) {
                        test = false;
                        positionL = positionL - 5;
                        magin_left= true;
                        have_magin_left= true;
                    }

                }
                else {
                    if (demL < demL2 && demL2 > minpixel) {
                        test = false;
                        positionL = positionL + 5;
                        magin_left= false;
                        have_magin_left= true;
                    }
                }
                if( demR1 > demR2) {
                    if (demR < demR1 && demR1 > minpixel) {
                        test = false;
                        positionR = positionR - 5;
                        magin_right= false;
                        have_magin_right= true;
                    }
                }
                else
                {
                    if (demR < demR2 && demR2 > minpixel) {
                        test = false;
                        positionR = positionR + 5;
                        magin_right= true;
                        have_magin_right= true;
                    }
                }

            } else
            {
                if(have_magin_left ) // false  dich lane trai dich
                {
                    if (magin_left)  // true dich sang trai
                    {
                        int demL1=countNonZero(imgThresholded(Left1));
                        if (demL < demL1 && demL1 > minpixel)
                        {
                            test = false;
                            positionL = positionL - 5;

                        }
                    } else { // dich sang phai
                        int demL2=countNonZero(imgThresholded(Left2));
                        if (demL < demL2 && demL2 > minpixel) {
                            test = false;
                            positionL = positionL + 5;

                        }
                    }
                }
                if(have_magin_right)
                {
                    if(!magin_right) // dich sang trai
                    {
                        int demR1= countNonZero(imgThresholded(Right1));
                        if (demR < demR1 && demR1 > minpixel)
                        {
                            test = false;
                            positionR = positionR - 5;

                        }
                    } else { // dich sang phai
                        int demR2= countNonZero(imgThresholded(Right2));
                        if (demR < demR2 && demR2 > minpixel) {
                            test = false;
                            positionR = positionR + 5;

                        }
                    }
                }
            }

            if(test)
            {
                rectangle(imgThresholded,Left.tl(),Left.br(),Scalar(255,0,0),1);
                rectangle(imgThresholded,Right.tl(),Right.br(),Scalar(255,0,0),1);

                centerLaneLeft.push_back(Point(positionL,(win_y_high+win_y_low)/2));
                centerLaneRight.push_back(Point(positionR,(win_y_high+win_y_low)/2));
                have_magin_left= false;
                have_magin_right= false;
                magin_left= false;
                magin_right= false;
                magin_start= false;


            } else {
                i = i - 1;
                magin_start= true;
            }

        }

 imshow("eyes bird", imgThresholded);
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
        for( int i=0;i< ImageContours.rows;i++)
        {
            int x = (matrixCoefficientABCL.at<float>(0, 0) + matrixCoefficientABCL.at<float>(1, 0) * i +
                     matrixCoefficientABCL.at<float>(2, 0) * i * i);

            ImageContours.at<uchar>(Point(x,i))= 255;
        }

        for( int i=0;i< ImageContours.rows;i++)
        {
            int x = (matrixCoefficientABCR.at<float>(0, 0) + matrixCoefficientABCR.at<float>(1, 0) * i +
                     matrixCoefficientABCR.at<float>(2, 0) * i * i);

            ImageContours.at<uchar>(Point(x,i))= 255;
        }
        matrixCoefficientABCL= (matrixCoefficientABCL + matrixCoefficientABCR)/2;


        // vi tri lane center Point (x,30)
        int PointCenter = (matrixCoefficientABCL.at<float>(0, 0) + matrixCoefficientABCL.at<float>(1, 0) * 10 +
                           matrixCoefficientABCL.at<float>(2, 0) * 10 * 10);

        int PointXe = (matrixCoefficientABCL.at<float>(0, 0) + matrixCoefficientABCL.at<float>(1, 0) * 170 +
                       matrixCoefficientABCL.at<float>(2, 0) * 170 * 170);

        double theta= TinhGoc(Point(PointXe,170),Point(PointCenter,30));
        if(theta > 0)
        {
            cout<<"cua phai goc " <<theta <<  endl;

        } else
            cout<<"cua trai goc " << theta << endl;

              for( int i=0;i< ImageContours.rows;i++)
              {
                  int x = (matrixCoefficientABCL.at<float>(0, 0) + matrixCoefficientABCL.at<float>(1, 0) * i +
                           matrixCoefficientABCL.at<float>(2, 0) * i * i);

                  ImageContours.at<uchar>(Point(x,i))= 255;
              }
              imshow("tam duong", ImageContours);

        double theta= TinhGoc(Point(330,110),Point(x,30));

        _Controller->Speed(90,90);
        _Controller->Handle(theta);


                double executionTime = (getTickCount() * 1.0000 - prevTickCount * 1.0000) / (getTickFrequency() * 1.0000);
                cout << "execution Time = " << executionTime << " s" << endl;

         if(waitKey(30)==27)
        {

            break;
            }

    }
    _Controller->Speed(0,0);
 _Controller->Handle(0);
    return 0;
}

double TinhGoc(Point xe, Point center)
{
    if (xe.x == center.x)
        return 0;
    float a= xe.y-center.y;
    float b= abs(center.x-xe.x);
    double pi = acos(-1.0);
    if( center.x > xe.x)
    return  (atan(b/a)*180) / pi;
    else
        return  -(atan(b/a)*180) / pi;
}




