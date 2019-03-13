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
 VideoWriter video("lane2.avi",CV_FOURCC('M','J','P','G'),15, Size(640,480));
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


    int positionL=225 ,positionR= 500;
    int positionXe=373;
//
    vector< vector<Point> >  co_ordinates;
    Point P1(250,0);
    Point P2(490,0);
    Point P3(639,244);
    Point P4(100,244);
    co_ordinates.push_back(vector<Point>());
    co_ordinates[0].push_back(P1);
    co_ordinates[0].push_back(P2);
    co_ordinates[0].push_back(P3);
    co_ordinates[0].push_back(P4);
//
//
    Point2f inputQuad[4];
    Point2f outputQuad[4];

    inputQuad[0] = Point2f( 250,0);
    inputQuad[1] = Point2f( 100,244);
    inputQuad[2] = Point2f( 639,244);
    inputQuad[3] = Point2f( 490,0);
    // The 4 points where the mapping is to be done , from top-left in clockwise order
    outputQuad[0] = Point2f(100,0 );
    outputQuad[1] = Point2f( 100,244);
    outputQuad[2] = Point2f( 639,244);
    outputQuad[3] = Point2f( 639,0);
    Mat transform= getPerspectiveTransform(inputQuad,outputQuad);
//
//
     Rect ROI1(0,170,640,245);
//

    Size size = Size(32, 32);
    Size block_size = Size(size.width / 4, size.height / 4);
    Size block_stride = Size(size.width / 8, size.height / 8);
    Size cell_size = block_stride;
    int num_bins = 9;
    HOGDescriptor hog(size, block_size, block_stride, cell_size, num_bins);


    Ptr<SVM> svm2 = Algorithm::load<SVM>("/home/ubuntu/Phuong/DoAn2/trainsvm.xml");

    for(;;)
    {

        Mat frame;
        cap >> frame; // get a new frame from camera
        long double prevTickCount = getTickCount();



        Mat ImageLeft = frame(ROI1);
        remap(ImageLeft, ImageLeft, map1, map2, INTER_LINEAR);



        Point2f pc(ImageLeft.cols/2., ImageLeft.rows/2.);
        Mat r = getRotationMatrix2D(pc, -180, 1.0);

        //  warpAffine(ImageRight, ImageRight, r, ImageRight.size()); // what size I should use?
        warpAffine(ImageLeft, ImageLeft, r, ImageLeft.size());

        Mat imgSignals= ImageLeft.clone();
        imshow("test",ImageLeft);
        Mat mask(ImageLeft.size(), CV_8UC1, Scalar(0));
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

        bitwise_and(imgThresholded,mask,imgThresholded);

        warpPerspective(imgThresholded,imgThresholded,transform,Size(600,245));

       // imshow("imgThresholded", imgThresholded);



        threshold(imgThresholded,imgThresholded,100,255,THRESH_BINARY);

        int maxL=100;
        int maxR=100;



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

        int margin= 35;

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
            int marginX_low=35;
            int marginX_high=35;
            int win_size=70;
            bool test= true;
            int minpixel= 100;
            if(win_xleft_low <0 )
                win_xleft_low = 0;

            if(win_xright_high >imgSize.width) {
                win_xright_high = imgSize.width;

            }

            if(win_xright_low+ marginX_high >imgSize.width ) {
                marginX_high = 0;

            }
            Rect Rifgt(win_xright_low,win_y_low,win_xright_high-win_xright_low,win_y_high-win_y_low);
            Rect Left(win_xleft_low,win_y_low,win_xleft_high-win_xleft_low,win_y_high-win_y_low);
            Rect Left2(win_xleft_low + 35, win_y_low, win_xleft_high - win_xleft_low, win_y_high - win_y_low);
            Rect Rifgt1(win_xright_low - 35, win_y_low, win_xright_high - win_xright_low, win_y_high - win_y_low);

            if (win_xleft_low - marginX_low < 0) {
                marginX_low = 0;
                win_xleft_low = 0;
            }
            if (win_xright_high + marginX_high > imgSize.width) {
                win_size = imgSize.width - (win_xright_low + marginX_high);
            }
            Rect Left1(win_xleft_low - marginX_low, win_y_low, win_xleft_high - win_xleft_low, win_y_high - win_y_low);
            Rect Rifgt2(win_xright_low + marginX_high, win_y_low, win_size, win_y_high - win_y_low);
            int demL=countNonZero(imgThresholded(Left));
            int demR= countNonZero(imgThresholded(Rifgt));
            if(magin_start == false)
            {
                int demL1=countNonZero(imgThresholded(Left1));
                int demR1= countNonZero(imgThresholded(Rifgt1));
                int demL2=countNonZero(imgThresholded(Left2));
                int demR2= countNonZero(imgThresholded(Rifgt2));
                if( demL1 > demL2) {
                    if (demL < demL1 && demL1 > minpixel) {
                        test = false;
                        positionL = positionL - 10;
                        magin_left= true;
                        have_magin_left= true;
                    }

                }
                else {
                    if (demL < demL2 && demL2 > minpixel) {
                        test = false;
                        positionL = positionL + 10;
                        magin_left= false;
                        have_magin_left= true;
                    }
                }
                if( demR1 > demR2) {
                    if (demR < demR1 && demR1 > minpixel) {
                        test = false;
                        positionR = positionR - 10;
                        magin_right= false;
                        have_magin_right= true;
                    }
                }
                else
                {
                    if (demR < demR2 && demR2 > minpixel) {
                        test = false;
                        positionR = positionR + 10;
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
                            positionL = positionL - 10;

                        }
                    } else { // dich sang phai
                        int demL2=countNonZero(imgThresholded(Left2));
                        if (demL < demL2 && demL2 > minpixel) {
                            test = false;
                            positionL = positionL + 10;

                        }
                    }
                }
                if(have_magin_right)
                {
                    if(!magin_right) // dich sang trai
                    {
                        int demR1= countNonZero(imgThresholded(Rifgt1));
                        if (demR < demR1 && demR1 > minpixel)
                        {
                            test = false;
                            positionR = positionR - 10;

                        }
                    } else { // dich sang phai
                        int demR2= countNonZero(imgThresholded(Rifgt2));
                        if (demR < demR2 && demR2 > minpixel) {
                            test = false;
                            positionR = positionR + 10;

                        }
                    }
                }
            }

            if(test)
            {
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



        matrixCoefficientABCL= (matrixCoefficientABCL + matrixCoefficientABCR)/2;



        // vi tri lane center Point (x,110)
        int x = (matrixCoefficientABCL.at<float>(0, 0) + matrixCoefficientABCL.at<float>(1, 0) * 110 +
                 matrixCoefficientABCL.at<float>(2, 0) * 110 * 110);


        double theta= TinhGoc(Point(373,205),Point(x,110));
        _Controller->Speed(100,100);
        _Controller->Handle(theta);
//=================================== Detect traffic signals=======================//
//=================================================================================//
//=================================================================================//
// =================================================================================//
        cvtColor(imgSignals, imgHSV, COLOR_RGB2HSV);
       // Mat imgThresholded;
        Mat imgThresholded1;
      //  inRange(imgHSV, Scalar(0, 0, 200), Scalar(179, 255, 255), imgThresholded);
        inRange(imgHSV, Scalar(93, 38, 100), Scalar(179, 255, 255), imgThresholded1); // trang vang
        inRange(imgHSV, Scalar(0, 90, 85), Scalar(10, 255, 255), imgThresholded); // xanh
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        //morphological closing (fill small holes in the foreground)
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        erode(imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        //morphological closing (fill small holes in the foreground)
        dilate( imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );


        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        vector<vector<Point> > contours1;
        vector<Vec4i> hierarchy1;
        vector<Point> contoursHull;
        vector<Point>  contoursPoly;
        findContours(imgThresholded,contours,hierarchy,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        findContours(imgThresholded1,contours1,hierarchy1,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));


        vector<Rect> boundRect;
        for (int i = 0; i < contours.size(); i++) {
            double a = arcLength(contours[i], true);
            if (hierarchy[i][3] == -1 && a > 100 ) {

                convexHull(contours[i],contoursHull);
                approxPolyDP(contoursHull,contoursPoly,a*0.02, true);

                int countPeak= contoursPoly.size();
                if ( countPeak == 4 || countPeak == 3 || countPeak == 5|| countPeak == 6|| countPeak == 7|| countPeak == 8 )
                        boundRect.push_back(boundingRect(contoursHull));
            }
        }

        for (int i = 0; i < contours1.size(); i++) {
            double a = arcLength(contours1[i], true);
            if (hierarchy1[i][3] == -1 && a > 150 ) {

                convexHull(contours1[i],contoursHull);
                // drawContours(source, vector<vector<Point>> {contoursHull}, 0, Scalar(255,0,0),5);
                approxPolyDP(contoursHull,contoursPoly,a*0.02, true);

                int countPeak= contoursPoly.size();
                if ( countPeak == 4 || countPeak == 3 || countPeak == 5|| countPeak == 6|| countPeak == 7|| countPeak == 8 )
                    boundRect.push_back(boundingRect(contoursHull));
            }
        }

        Mat detectSigns;
        for(int i=0;i< boundRect.size();i++)
        {
            detectSigns= ImageLeft(boundRect[i]);
            resize(detectSigns, detectSigns, Size(32, 32));
            cvtColor(detectSigns, detectSigns, COLOR_BGR2GRAY);
            vector< float> descriptorsValues;
            hog.compute(detectSigns, descriptorsValues, Size(8, 8), Size(0, 0));

            int result=0;
            result= svm2->predict(descriptorsValues);



                switch (result) {

                    case 1:
                        rectangle(imgSignals,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(imgSignals, "giao",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 2:
                        rectangle(imgSignals,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(imgSignals, "D sat",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 3:
                        rectangle(imgSignals,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(imgSignals, "stop",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 4:
                        rectangle(imgSignals,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(imgSignals, "bend left",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 5:
                        rectangle(imgSignals,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(imgSignals, "bend right",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 6:
                        rectangle(imgSignals,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(imgSignals, "bend",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 7:
                        rectangle(imgSignals,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(imgSignals, "ban left",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 8:
                        rectangle(imgSignals,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(imgSignals, "ban right",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 9:
                        rectangle(imgSignals,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(imgSignals, "go right",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 10:
                        rectangle(imgSignals,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(imgSignals, "go left",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;


                    case 11:
                        rectangle(imgSignals,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(imgSignals, "aroudabout",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;

                    default:
                        break;
                }



        }
          //video.write(imgSignals);

        imshow("draw",imgSignals);
        //=================================================================================//
//=================================================================================//
// =================================================================================//
//=================================================================================//
//=================================================================================//
// =================================================================================//

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
    return  (atan(b/a)*180) / pi;
}



