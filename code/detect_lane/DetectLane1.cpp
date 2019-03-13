#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
Mat SobelXY(Mat segImage);
int main() {


        Mat source= imread("/home/hoaiphuong/Desktop/photos/10.png",CV_LOAD_IMAGE_COLOR);;
         // get a new frame from camera

        // Mat source = imread("/home/hoaiphuong/Desktop/photos/10.png",CV_LOAD_IMAGE_COLOR);
        imshow("source", source);
        Mat imgHSV;

        cvtColor(source, imgHSV, COLOR_BGR2HLS); //Convert the captured frame from BGR to HSV


        Mat imgThresholded;

        inRange(imgHSV, Scalar(0, 238, 0), Scalar(178, 255, 255), imgThresholded); //Threshold the image

        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)));
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)));

        //morphological closing (fill small holes in the foreground)
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)));
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)));

        imshow("imgThresholded", imgThresholded);
        Mat cannyImage;
        Canny(imgThresholded, cannyImage, 50, 100);
        imshow("cannyImage", cannyImage);

        vector<Vec4i> hierarchy;
        vector<vector<Point>> contours;

        findContours(cannyImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        vector<Point> contours_poly;
        vector<vector<Point>> contours_list;
        vector<vector<Point>> contours_final;

        for (int i = 0; i < contours.size(); i++) {
            double a = arcLength(contours[i], false);
            if (hierarchy[i][3] == -1 && a > 600) {
                // convexHull(contours[i],hull[i]);
                contours_list.push_back(contours[i]);
            }


        }
    Mat ImageContours(cannyImage.size(),CV_8UC1,Scalar(0));
    Mat ImageContours2(cannyImage.size(),CV_8UC1,Scalar(0));
      //  ketqua.setTo(Scalar(0));
//  imshow("ketqua1", ketqua);
  //  drawContours(ketqua, contours_list,-1, 255, CV_FILLED);
       // Mat contoursImage = source.clone();

    for(size_t i = 0; i < contours_list.size(); i++){
        // approxPolyDP(contours[i], approxShape, arcLength(Mat(contours[i]), true)*0.04, true);
        drawContours(ImageContours, contours_list, i, Scalar(255));   // fill BLUE
    }


 //   drawContours(ketqua, contours_list,-1, 255, CV_FILLED);
    imshow("ImageContours", ImageContours);


    bool kiemtra =true;

    // Step of each window

     vector<Point> locations;   // output, locations of non-zero pixels
    vector<Point> centerLaneLeft;
    vector<Point> centerLaneRight;
    vector<Point> test;
    int dem=0;

  //  cout << ImageContours.cols<<endl;
   // cout << ImageContours.rows<<endl;

    for (int row = 0; row < ImageContours.rows; row++) {

        for (int col = 0; col < ImageContours.cols / 2; col++) {

            //  ketqua.at<uchar>(row,col)=255;

            ImageContours2.at<uchar>(row,col) = 255;
            //dem = col;

        }

    }

  //  imshow("ImageContours2", ImageContours2);

    for (int row = 0; row < ImageContours.rows; row++)
    {

        for (int col = 0; col <= ImageContours.cols/2; col ++)
        {

          //  ketqua.at<uchar>(row,col)=255;


           if( ImageContours.at<uchar>(Point(col,row)) == 255)
           {
              //test.push_back(Point(col,row));

                if(kiemtra==true)
                {
                    locations.push_back(Point(col,row));
                    dem =dem+1;
                    kiemtra= false;
                    if(dem ==2)
                    {
                        break;
                    }
                }


           }
           else
           {
               kiemtra= true;
           }
        }
        if(dem ==2)
        {
            int x=(locations[0].x+locations[1].x) / 2;
            int y=locations[0].y;
            centerLaneLeft.push_back(Point(x,y));
            row = row+20;
        }
        dem =0;
        locations.clear();
        kiemtra= true;
    }

    for (int i=0 ; i < test.size() ; i++)
    {
        line(source,centerLaneLeft[i],centerLaneLeft[i+1],Scalar(0,255,0),3);
     //   circle(source,test[i],10,Scalar(255,0,0));

    }
    imshow("ketquacuoi", source);



    for (int row = 0; row < ImageContours.rows; row++)
    {

        for (int col = ImageContours.cols/2; col <= ImageContours.cols; col ++)
        {

            //  ketqua.at<uchar>(row,col)=255;
            if(ImageContours.at<uchar>(Point(col,row))==255)
            {
                //test.push_back(Point(col,row));

                if(kiemtra==true)
                {
                    locations.push_back(Point(col,row));
                    dem =dem+1;
                    kiemtra= false;
                    if(dem ==2)
                    {
                        break;
                    }
                }


            }
            else
            {
                kiemtra= true;
            }
        }
        if(dem ==2)
        {
            int x=(locations[0].x+locations[1].x) / 2;
            int y=locations[0].y;
            centerLaneRight.push_back(Point(x,y));
            row = row+20;
        }
        dem =0;
        locations.clear();
        kiemtra= true;
    }


    //   imshow("Step 2 draw Rectangle", ketqua);

    for (int i=0 ; i < centerLaneLeft.size()-1 ; i++)
    {
        line(source,centerLaneLeft[i],centerLaneLeft[i+1],Scalar(0,255,0),4);
      // circle(source,test[i],10,Scalar(0,255,0));

    }
    for (int i=0 ; i < centerLaneRight.size()-1 ; i++)
    {
        //line(source,centerLane[i],centerLane[i+1],Scalar(0,255,0));
        line(source,centerLaneRight[i],centerLaneRight[i+1],Scalar(0,255,0),4);

    }

    // ROI=SobleImage(Rect(0,SobleImage.rows/2,SobleImage.cols,SobleImage.rows/2));
   // imshow("ketqua", ketqua);
    imshow("ve vong", source);

    centerLaneLeft.clear();
    centerLaneLeft.push_back(Point(0,0));
    centerLaneLeft.push_back(Point(1,1));
    centerLaneLeft.push_back(Point(1,2));
    centerLaneLeft.push_back(Point(2,4));
    centerLaneLeft.push_back(Point(2,6));

    float x1=0,x2=0,x3=0,x4=0,y1=0,xy=0,xy2=0;
    Mat matrixA = Mat(3, 3, CV_32FC1);
    Mat matrixB = Mat(3, 1, CV_32FC1);
    Mat matrixCoefficientABC = cv::Mat(3, 1, CV_32FC1);
    for(int i=0; i<centerLaneLeft.size();i++)
    {
        int x= centerLaneLeft[i].x;
        int y = centerLaneLeft[i].y;
        x1= x1+ x;
        x2= x2+ x*x;
        x3= x3 + x*x*x;
        x4= x4 + x*x*x*x;
       y1= y1+  y;
        xy= xy + x*y;
        xy2= xy2+ x*x*y*y;
    }

    matrixA.at<float>(0,0)= centerLaneLeft.size();

    matrixA.at<float>(0,1)= x1;
    matrixA.at<float>(1,0)= x1;

    matrixA.at<float>(0,2)= x2;
    matrixA.at<float>(1,1)= x2;
    matrixA.at<float>(2,0)= x2;

    matrixA.at<float>(1,2)= x3;
    matrixA.at<float>(2,1)= x3;

    matrixA.at<float>(2,2)= x4;

    matrixB.at<float>(0,0)= y1;
    matrixB.at<float>(1,0)= xy;
    matrixB.at<float>(2,0)= xy2;




    cout << matrixA << endl;

    cout << matrixB << endl;
    matrixA=matrixA.inv();
    cout << matrixA << endl;
    matrixCoefficientABC= matrixA* matrixB;

    cout << matrixCoefficientABC << endl;
    cout << matrixCoefficientABC.at<float>(0,0)<< endl;
    float col=0;
    float y= (matrixCoefficientABC.at<float>(0,0) + matrixCoefficientABC.at<float>(1,0)*col+ matrixCoefficientABC.at<float>(2,0)*col*col);
    cout << y << endl;
/*
        for (int col = 0; col <= ImageContours.cols/2; col ++)
        {
            int y= (matrixCoefficientABC.at<int>(0,0) + matrixCoefficientABC.at<int>(1,0)*col+ matrixCoefficientABC.at<int>(2,0)*col*col);

                ImageContours.at<uchar>(Point(col,y))= 255;

        }

    imshow("ve tam duong", ImageContours);
*/
    waitKey(0);

    return 0;
}
