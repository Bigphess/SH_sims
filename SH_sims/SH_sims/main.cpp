//
//  main.cpp
//  OpencvSample
//
//  Created by 许若芃 on 2018/11/21.
//  Copyright © 2018 许若芃. All rights reserved.
//

//#include <opencv2/opencv.hpp> //Include file for every supported OpenCV function
//int main(int argc, const char * argv[] ) {
//    cv::Mat img = cv::imread(argv[1],-1);
//    if( img.empty() ) return -1;
//    cv::namedWindow( "Example1", cv::WINDOW_AUTOSIZE );
//    cv::imshow( "Example1", img );
//    cv::waitKey( 0 );
//    cv::destroyWindow( "Example1" );
//    return 0;
//}


//#include <iostream>
//#include <stdlib.h>
//#include <stdio.h>
//
//
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv/cvaux.hpp>
//#include <fstream>
//using namespace std;
//
//#define BYTE unsigned char
//
//int main(int argc, const char * argv[])
//
//{
//    // insert code here...
//#if 1
//    //get the image from the directed path
//    IplImage* img = cvLoadImage("/Users/bigphess/Downloads/IMG_6453.JPG", 1);
//    //NSLog(img);
//    //create a window to display the image
//    cvNamedWindow("picture", 1);
//    //show the image in the window
//    cvShowImage("picture", img);
//    //wait for the user to hit a key
//    cvWaitKey(0);
//    //delete the image and window
//    cvReleaseImage(&img);
//    cvDestroyWindow("picture");
//#endif
//    //return
//    return 0;
//}



/*****************************/

#include <stdlib.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "HandDetect.hpp"


int main() {
    
    cv::VideoCapture cap(0);
    HandDetect HandDetector;
    cv::Mat frame;
    Mat frameOri;
    vector<Point> fingers;

    vector<Mat> Temp_frames;
    if (!cap.isOpened()) {
        return -1;
    }
    
    cv::Mat FinalResult = cv::Mat::zeros(frame.rows, frame.cols, CV_8UC1);
    Rect ROI(0,0,640,720);
    
    cv::namedWindow("src");
    
    /********************************/
    /********************************/
    
    while (1) {
        int key = waitKey(1);
        //start remove
      
        cap >> frame;
//        frame = frameOri(ROI);
        
//        imshow("src", frame);
//        waitKey(1);
        
        //press b to remove the background
        if (key == 'b'){
            HandDetector.RemoveBGcalibrate(frame);
            printf("Template Saved and start remove background\n");
        }

        else if (key == 's'){
            HandDetector.Skincalibrate(frame);
            printf("start the skin threshold\n");

        }
        //remove the background of the picture
        Mat foreground = HandDetector.RemoveBackground(frame);
        //get the skin mask of th picture
        Mat SkinMask = HandDetector.getSkinMask(foreground);
        // remove the face from foregroundmask
        HandDetector.RemoveFace(frame, SkinMask);

        //count the number of the finger in the picture
        Mat output = Mat::zeros(frame.size(), CV_8UC3);
//        int finger_num = 0;
        fingers = HandDetector.CountFinger(SkinMask, output);

        Mat result = HandDetector.addPictures(frame, fingers);

//        Mat alpha = imread("/Users/bigphess/Desktop/SH_sims/test2.png", -1);
//        if(fingers.size()>0){
//            putText(frame, to_string(fingers.size()), Point(350,350), FONT_HERSHEY_PLAIN, 3, Scalar(255,0,255));
//            HandDetector.mapToMat(alpha, frame, fingers[0].x , fingers[0].y);
//        }
        
        
        
//        imshow("removebg", foreground);
//        waitKey(1);

//        imshow("skin", SkinMask);
//        waitKey(1);
//
        imshow("hand", output);
        waitKey(1);

        imshow("result", result);
        waitKey(1);
        
        
        //test for add picture
        Mat alpha = imread("/Users/bigphess/Desktop/SH_sims/test2.png",-1);
        HandDetector.mapToMat(alpha, frame, 640 - (alpha.rows/2), 360 - (alpha.cols / 2));
        imshow("f", frame);
        waitKey(1);


        
        
    }
    
    return 0;
}
