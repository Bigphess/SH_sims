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

HandDetect :: HandDetect(void){
    BG_calibrate = false;
    
    hLowThreshold = 0;
    hHighThreshold = 0;
    sLowThreshold = 0;
    sHighThreshold = 0;
    vLowThreshold = 0;
    vHighThreshold = 0;
    SK_calibrated = false;
    skinSampleRectngle1, skinSampleRectngle1;
}

void HandDetect :: RemoveBGcalibrate(Mat input){
    BG_calibrate = true;
    cvtColor(input, TemplateOfRemover, CV_BGR2GRAY);
}

void HandDetect:: Skincalibrate(Mat input){
    
    SK_calibrated = true;
    int SampleWidth = input.size().width;
    int SampleHeight = input.size().height;
    int SampleRetangleSize = 20;
    Scalar rectangleColor = Scalar(255,0,255);
    
    //choose the position of the skin samples
    skinSampleRectngle1 = Rect(SampleWidth / 5, SampleHeight / 2,SampleRetangleSize,SampleRetangleSize);
    skinSampleRectngle2 = Rect(SampleWidth / 5, SampleHeight / 3,SampleRetangleSize,SampleRetangleSize);
    
    //draw the sample area on the image
    rectangle(input,skinSampleRectngle1,rectangleColor);
    rectangle(input,skinSampleRectngle2,rectangleColor);
    
    //trans the input into HSV and take the samples
    Mat HSVimage;
    cvtColor(input, HSVimage, CV_BGR2HSV);
    
    Mat sample1 = Mat(HSVimage,skinSampleRectngle1);
    Mat sample2 = Mat(HSVimage,skinSampleRectngle2);
    
    Thres_calculate(sample1, sample2);
}

Mat HandDetect :: RemoveBackground(Mat input){
    
    Mat foreground = Mat :: zeros(input.rows, input.cols, CV_8UC1);;
    if (!BG_calibrate){
        return foreground;
    }
    
    cvtColor(input, input, CV_BGR2GRAY);
    int thresBG = THRES_BG;
    
    for(int i = 0; i < input.rows; i++){
        for (int j = 0; j < input.cols; j++){
            uchar inputPixel = input.at<uchar>(i,j);
            uchar TemPixel = TemplateOfRemover.at<uchar>(i,j);
            
            if(inputPixel >= TemPixel - thresBG && inputPixel <= TemPixel + thresBG){
                foreground.at<uchar>(i,j) = 0;
            }
            else
                foreground.at<uchar>(i,j) = 255;
            
            }
        }
    return foreground;
}



void HandDetect :: Thres_calculate(Mat sample1, Mat sample2){
    int offsetLowThreshold = 80;
    int offsetHighThreshold = 30;
    
    Scalar HSVmeanSample1 = mean(sample1);
    Scalar HSVmeanSample2 = mean(sample2);
    
    hLowThreshold = min(HSVmeanSample1[0],HSVmeanSample2[0]) - offsetLowThreshold;
    hHighThreshold = max(HSVmeanSample1[0],HSVmeanSample2[0]) + offsetHighThreshold;
    sLowThreshold = min(HSVmeanSample1[1],HSVmeanSample2[1]) - offsetLowThreshold;
    sHighThreshold = max(HSVmeanSample1[1],HSVmeanSample2[1]) + offsetHighThreshold;
    vLowThreshold = min(HSVmeanSample1[2],HSVmeanSample2[2]) - offsetLowThreshold;
    vHighThreshold = max(HSVmeanSample1[2],HSVmeanSample2[2]) + offsetHighThreshold;
}



Mat HandDetect :: getSkinMask(Mat input){
    Mat SkinMask = Mat :: zeros(input.rows, input.cols, CV_8UC1);;
    if ( !SK_calibrated ){
        return SkinMask;
    }
    
    Mat hsvImage;
    cvtColor(input, hsvImage, CV_BGR2HSV);
    
    inRange(hsvImage, Scalar(hLowThreshold,sLowThreshold,vLowThreshold), Scalar(hHighThreshold,sHighThreshold,vHighThreshold), SkinMask);
    Mat morphElement = getStructuringElement(MORPH_ELLIPSE, {3,3});
    morphologyEx(SkinMask, SkinMask, MORPH_OPEN, morphElement);
    
    return SkinMask;
}
