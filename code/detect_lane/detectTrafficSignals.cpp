#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using namespace cv::ml;

int main( int argc, char** argv ) {

    VideoCapture cap("/home/hoaiphuong/Desktop/image/lane2.avi"); // open the default camera
    if (!cap.isOpened())  // check if we succeeded
        return -1;
    VideoWriter video("outcpp.avi", CV_FOURCC('M', 'J', 'P', 'G'), 15, Size(640, 480));

    for (;;) {

        Mat source;
        cap >> source; // get a new frame from camera
        imshow("source", source);
        Mat imgBlur;
        GaussianBlur(source,imgBlur,Size(5,5),5);
        Mat imgHSV;
        cvtColor(imgBlur, imgHSV, COLOR_RGB2HSV);
        // cvtColor(source, imBRG, COLOR_RGB2BGR);
        // threshold( imGray, imBinary, 194, 255,0 );
        Mat imgThresholded;
        Mat imgThresholded1;

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

        imshow("imgThresholded", imgThresholded);
        imshow("imgThresholded1", imgThresholded1);
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
                // drawContours(source, vector<vector<Point>> {contoursHull}, 0, Scalar(255,0,0),5);
                approxPolyDP(contoursHull,contoursPoly,a*0.02, true);

                int countPeak= contoursPoly.size();
               // cout<<countPeak<< endl;
                switch (countPeak) {
                    case 4:

                        boundRect.push_back(boundingRect(contoursHull));

                      //   drawContours(source, vector<vector<Point>> {contoursHull}, 0, Scalar(0,255,255),5);
                       // cout<< "chu nhat" << endl;

                        break;
                    case 3:
                        boundRect.push_back(boundingRect(contoursHull));
                      //    drawContours(source, vector<vector<Point>> {contoursHull}, 0, Scalar(0,255,0),5);
                      //  cout<< "tam giac" << endl;
                        break;
                    case 5:
                    case 6:
                        boundRect.push_back(boundingRect(contoursHull));
                      //     drawContours(source, vector<vector<Point>> {contoursHull}, 0, Scalar(0,0,255),5);
                     //   cout<< "ngu giac" << endl;
                        break;
                    case 8:
                    case 7:
                        boundRect.push_back(boundingRect(contoursHull));
                     //     drawContours(source, vector<vector<Point>> {contoursHull}, 0, Scalar(255,0,0),5);
                     //   cout<< "tron" << endl;
                        break;

                    default:
                        //   cout<<"loi"<< endl;
                        break;
                }

            }


        }

        for (int i = 0; i < contours1.size(); i++) {
            double a = arcLength(contours1[i], true);
            if (hierarchy1[i][3] == -1 && a > 150 ) {

                convexHull(contours1[i],contoursHull);
                // drawContours(source, vector<vector<Point>> {contoursHull}, 0, Scalar(255,0,0),5);
                approxPolyDP(contoursHull,contoursPoly,a*0.02, true);

                int countPeak= contoursPoly.size();
                //  cout<<countPeak<< endl;
                switch (countPeak) {
                    case 4:

                        boundRect.push_back(boundingRect(contoursHull));

                     //   drawContours(source, vector<vector<Point>> {contoursHull}, 0, Scalar(0,255,255),5);
                        // cout<< "chu nhat" << endl;

                        break;
                    case 3:
                        boundRect.push_back(boundingRect(contoursHull));
                      //  drawContours(source, vector<vector<Point>> {contoursHull}, 0, Scalar(0,255,0),5);
                        //  cout<< "tam giac" << endl;
                        break;
                    case 5:
                    case 6:
                        boundRect.push_back(boundingRect(contoursHull));
                       // drawContours(source, vector<vector<Point>> {contoursHull}, 0, Scalar(0,0,255),5);
                        //   cout<< "ngu giac" << endl;
                        break;
                    case 8:
                    case 7:
                        boundRect.push_back(boundingRect(contoursHull));
                     //   drawContours(source, vector<vector<Point>> {contoursHull}, 0, Scalar(255,0,0),5);
                        //   cout<< "tron" << endl;
                        break;

                    default:
                        //   cout<<"loi"<< endl;
                        break;
                }

            }


        }



        Mat detectSigns;
        for(int i=0;i< boundRect.size();i++)
        {
            detectSigns= source(boundRect[i]);

            //   imshow("detectSigns",detectSigns);

            resize(detectSigns, detectSigns, Size(64, 64));
//gray
            cvtColor(detectSigns, detectSigns, COLOR_BGR2GRAY);
            //    imshow("detectSigns", detectSigns);
//Extract HogFeature
            Size size = Size(64, 64);
            Size block_size = Size(size.width / 4, size.height / 4);
            Size block_stride = Size(size.width / 8, size.height / 8);
            Size cell_size = block_stride;
            int num_bins = 9;
            HOGDescriptor hog(size, block_size, block_stride, cell_size, num_bins);
            vector< float> descriptorsValues;


            hog.compute(detectSigns, descriptorsValues, Size(8, 8), Size(0, 0));
            // Mat fm = Mat(descriptorsValues);
            Ptr<SVM> svm2 = Algorithm::load<SVM>("/home/hoaiphuong/CLionProjects/FindHOG/trainsvm.xml");

            //   Mat image1d(1, fm.rows, CV_32FC1);
            //  imshow("gradient 1", image1d);

            int result=0;
            result= svm2->predict(descriptorsValues);
          //  cout<< "result " << result << endl;
          //  if(result==34 || result==33)

            {


                switch (result) {

                    case 1:
                        rectangle(source,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(source, "giao",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 2:
                        rectangle(source,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(source, "D sat",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 3:
                        rectangle(source,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(source, "stop",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 4:
                        rectangle(source,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(source, "bend left",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 5:
                        rectangle(source,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(source, "bend right",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 6:
                        rectangle(source,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(source, "bend",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 7:
                        rectangle(source,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(source, "ban left",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 8:
                        rectangle(source,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(source, "ban right",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 9:
                        rectangle(source,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(source, "go right",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;
                    case 10:
                        rectangle(source,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(source, "go left",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;


                    case 11:
                        rectangle(source,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),3);
                        putText(source, "aroudabout",boundRect[i].tl(),0,0.8,Scalar(0,0,255),2);
                        break;

                    default:
                        //   cerr<<"error"<< endl;
                        break;
                }
            }
            // waitKey(0);

        }
      //  video.write(source);

        imshow("draw",source);

        if(waitKey(30)==' ') break;
    }
}