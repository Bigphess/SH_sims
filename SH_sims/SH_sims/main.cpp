//
//  main.cpp
//  OpencvSample
//
//  Created by 许若芃 on 2018/11/21.
//  Copyright © 2018 许若芃. All rights reserved.
//


/*****************************/

#include <stdlib.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "HandDetect.hpp"


int main() {
    
    VideoCapture cap(0);
    HandDetect HandDetector;
    Mat frame_ori;
    Mat frame;
    
    
    
    vector<Mat> alpha_ori(15);
    alpha_ori[5] = imread("/Users/bigphess/Desktop/SH_sims/pics/zelda.png",-1);
    alpha_ori[0] = imread("/Users/bigphess/Desktop/SH_sims/pics/splatoon.png",-1);
    alpha_ori[6] = imread("/Users/bigphess/Desktop/SH_sims/pics/purin.png",-1);
    alpha_ori[1] = imread("/Users/bigphess/Desktop/SH_sims/pics/pikachu.png",-1);
    alpha_ori[2] = imread("/Users/bigphess/Desktop/SH_sims/pics/mario.png",-1);
    alpha_ori[3] = imread("/Users/bigphess/Desktop/SH_sims/pics/linkyonth.png",-1);
    alpha_ori[7] = imread("/Users/bigphess/Desktop/SH_sims/pics/link.png",-1);
    alpha_ori[8] = imread("/Users/bigphess/Desktop/SH_sims/pics/kingkong.png",-1);
    alpha_ori[4] = imread("/Users/bigphess/Desktop/SH_sims/pics/kabi.png",-1);
    alpha_ori[9] = imread("/Users/bigphess/Desktop/SH_sims/pics/kuba.png",-1);
    alpha_ori[10] = imread("/Users/bigphess/Desktop/SH_sims/pics/marth.png",-1);
    alpha_ori[11] = imread("/Users/bigphess/Desktop/SH_sims/pics/yoga.png",-1);
    alpha_ori[12] = imread("/Users/bigphess/Desktop/SH_sims/pics/peach.png",-1);
    alpha_ori[13] = imread("/Users/bigphess/Desktop/SH_sims/pics/ryu.png",-1);
    alpha_ori[14] = imread("/Users/bigphess/Desktop/SH_sims/pics/pacman.png",-1);

    
    
    vector<Point> fingers;

    vector<Mat> Temp_frames;
    if (!cap.isOpened()) {
        return -1;
    }
    
    /********************************/
    /********************************/
    
    while (1) {
        int key = waitKey(1);
      
//        cap >> frame_ori;
        cap >> frame;

//        resize(frame_ori, frame, Size(frame_ori.cols/2, frame_ori.rows/2));
        Mat result = frame;
        
        //press b to remove the background
        
        if (key == 'b'){
            HandDetector.RemoveBGcalibrate(frame);
            printf("Template Saved and start remove background\n");
        }
        //press to calculate the skin color
        else if (key == 's'){
            HandDetector.Skincalibrate(frame, result);
            printf("start the skin threshold\n");

        }
        //remove the background of the picture
        Mat foreground = HandDetector.RemoveBackground(frame);
//        Mat foreground = frame;
        //get the skin mask of th picture
        Mat SkinMask = HandDetector.getSkinMask(foreground);
        //remove the face from foregroundmask
        HandDetector.RemoveFace(frame, SkinMask);
        //count the number of the finger in the picture
        Mat output = Mat::zeros(frame.size(), CV_8UC3);
        fingers = HandDetector.CountFinger(SkinMask, output);
        //add pics on the fingers
        result = HandDetector.addPictures(frame, fingers, alpha_ori);
        
//        imshow("removebg", foreground);
//        waitKey(1);

//        imshow("skin", SkinMask);
//        waitKey(1);

        imshow("hand", output);
        waitKey(1);

        imshow("result", result);
        waitKey(1);

        
/**************test for add picture****************/
       
//        vector<Point> fingers_test;
//        fingers_test.push_back(Point(200,300));
//        fingers_test.push_back(Point(300,400));
//
//        result = HandDetector.addPictures(frame, fingers_test, alpha_ori);
////        HandDetector.mapToMat(alpha, frame, 640 - (alpha.rows/2), 360 - (alpha.cols / 2));
//        imshow("f", result);
//        waitKey(10);

        
        
       
        
    }
    
    return 0;
}
