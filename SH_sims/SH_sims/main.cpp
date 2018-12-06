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
    vector<Mat> Temp_frames;
    if (!cap.isOpened()) {
        return -1;
    }
    
    cv::Mat FinalResult = cv::Mat::zeros(frame.rows, frame.cols, CV_8UC1);
    
    cv::namedWindow("src");
    
    /********************************/
    /********************************/
    
    while (1) {
        int key = waitKey(1);
        //start remove
      
        cap >> frame;
        imshow("src", frame);
        waitKey(1);
        
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
        Mat SkinMask = HandDetector.getSkinMask(frame);
//        imshow("src", frame);
//        waitKey(1);
        imshow("yahjahah", foreground);
        waitKey(1);
    
//        cv::imshow("hahaha", );
//        cv::imshow("binarized", binarizedImage);
    }
    
    return 0;
}
