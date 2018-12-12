//
//  HandDetect.cpp
//  SH_sims
//
//  Created by 许若芃 on 2018/12/05.
//  Copyright © 2018 许若芃. All rights reserved.
//

#include "HandDetect.hpp"
#include <opencv2/opencv.hpp>
//#include “background_segm.hpp"

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
    
    Mat foregroundMask = Mat :: zeros(input.rows, input.cols, CV_8UC1);
    Mat foreground = Mat :: zeros(input.rows, input.cols, CV_8UC1);;
    if (!BG_calibrate){
        return input;
    }

    Mat dst;
    dst = input.clone();

    cvtColor(input, input, CV_BGR2GRAY);
    int thresBG = THRES_BG;

    for(int i = 0; i < input.rows; i++){
        for (int j = 0; j < input.cols; j++){
            uchar inputPixel = input.at<uchar>(i,j);
            uchar TemPixel = TemplateOfRemover.at<uchar>(i,j);

            if(inputPixel >= TemPixel - thresBG && inputPixel <= TemPixel + thresBG){
                foregroundMask.at<uchar>(i,j) = 0;
            }
            else
                foregroundMask.at<uchar>(i,j) = 255;

            }
        }
    dst.copyTo(foreground, foregroundMask);
    return foreground;
    
//    absdiff(input, TemplateOfRemover, foregroundMask);
//    threshold(foregroundMask, foregroundMask, 60, 255.0, CV_THRESH_BINARY);
//    return foregroundMask;
    
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
    
    inRange(
            hsvImage,
            Scalar(hLowThreshold,sLowThreshold,vLowThreshold),
            Scalar(hHighThreshold,sHighThreshold,vHighThreshold),
            SkinMask);
    Mat morphElement = getStructuringElement(MORPH_ELLIPSE, {3,3});
    morphologyEx(SkinMask, SkinMask, MORPH_OPEN, morphElement);
    
    return SkinMask;

}

void HandDetect:: RemoveFace(Mat input, Mat output){
//    String FaceClassiferFile= "../haarcascade_frontalface_alt.xml";
    CascadeClassifier FaceClassifer;
    FaceClassifer.load("/Users/bigphess/Desktop/SH_sims/SH_sims/SH_sims/haarcascade_frontalface_alt.xml");
//
//    if (!FaceClassifer.load("haarcascade_frontalface_alt.xml"))
//        throw runtime_error("can't find file" );
    
    vector<Rect> faces;
    Mat grayImage;
    cvtColor(input, grayImage, CV_BGR2GRAY);
    equalizeHist(grayImage, grayImage);

    //最后一个参数是寻找人脸的最小区域
    FaceClassifer.detectMultiScale(grayImage, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(120,120));

    for( size_t i = 0; i < faces.size(); i++){
        rectangle(
                  output,
                  Point(faces[i].x, faces[i].y + 10),
                  Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height + 10),
                  Scalar(0,0,0), -1);
    }
    
    
}


Mat HandDetect:: CountFinger(Mat input){
    int contour_num = 0;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    Mat output = Mat::zeros(input.size(), CV_8UC1);
    //转化成灰度图，转化成二值图
    Mat morphElement = getStructuringElement(MORPH_ELLIPSE, {3,3});
    morphologyEx(input, input, MORPH_OPEN, morphElement);
    morphologyEx(input, input, MORPH_CLOSE, morphElement);
//    return output;
    
    findContours(input, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    if(contours.size() <= 0){
        return output;
    }

    //find the biggest contour
    int max_contour_index = -1;
    double max_area = 0.0;
    for ( int i = 0; i < contours.size(); i++ ){
        double area = contourArea(contours[i]);
        if(area > max_area){
            max_area = area;
            max_contour_index = i;
        }
    }
    //if can't find the largest contour
    if(max_contour_index < 0)
        return output;

    
    Rect hahaha = boundingRect(contours[max_contour_index]);
    rectangle(input,hahaha,Scalar(255,255,255));
    //find the convex hull of the hand
//    vector<Point> hull_points;
//    vector<int> hull_ints;
//    convexHull(contours[max_contour_index], hull_points, false, true);
//    convexHull(contours[max_contour_index], hull_ints, false, false);
//    //find the defects
//    vector<Vec4i> defects;
//    if(hull_ints.size() > 3){
//        convexityDefects(contours[max_contour_index], hull_ints, defects);
//    }
//    else
//        return output;
//
//    Rect bound_hull = boundingRect(Mat(hull_points));
//    rectangle(input,bound_hull,Scalar(255,255,255));

    return input;

    
    
//    Point Center_bound_hull(
//                    (bound_hull.tl().x + bound_hull.br().x) / 2,
//                    (bound_hull.tl().y + bound_hull.br().y) / 2);
//
//    vector<Point> starts;
//    vector<Point> fars;
//
//    for(int i = 0; i < defects.size(); i++){
//        starts.push_back(contours[max_contour_index][defects[i].val[0]]);
////        //need something for far points
////        if(PointsDistance(contours[max_contour_index][defects[i].val[0]], <#Point b#>))
//    }
//
    

    
   
        
    return output;
  
}


double HandDetect:: PointsDistance(Point a, Point b) {
    Point difference = a - b;
    return sqrt(difference.ddot(difference));
}
