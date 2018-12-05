//
//  HandDetect.cpp
//  SH_sims
//
//  Created by 许若芃 on 2018/12/05.
//  Copyright © 2018 许若芃. All rights reserved.
//

#include "HandDetect.hpp"
#include <opencv2/opencv.hpp>

vector <Mat> channels;
Mat frameH;
int thre_min = 7;
int thre_max = 20;


Mat TemplateOfRemover;


// save the first frame as template
void HandDetect:: SaveTemplate(Mat input){
    
    cvtColor(input, TemplateOfRemover, CV_BGR2GRAY);

}


Mat HandDetect :: RemoveBackground(Mat input){
    
    Mat foreground;
    cvtColor(input, input, CV_BGR2GRAY);
    
    
    
    
    imshow("lalala", input);
    waitKey(0);
    return foreground;
}




void HandDetect :: SkinColor(Mat input){
    
    cvtColor(input, input, CV_BGR2HSV);
    split(input, channels);
    frameH = channels[0];
    //这个方法好像不是很好实现，滤波的步骤有点问题，但是具体的思路应该差不多
    medianBlur(frameH, frameH, 11);
    Mat result = Mat::zeros(frameH.rows, frameH.cols, CV_8UC1);
    inRange(frameH, Scalar(thre_min), Scalar(thre_max), result);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(result, result, MORPH_OPEN, kernel);
    imshow("lalala", result);
            waitKey(0);

}
