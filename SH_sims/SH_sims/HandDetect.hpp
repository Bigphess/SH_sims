//
//  HandDetect.hpp
//  SH_sims
//
//  Created by 许若芃 on 2018/12/05.
//  Copyright © 2018 许若芃. All rights reserved.
//

#ifndef HandDetect_hpp
#define HandDetect_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class HandDetect {
public:
    HandDetect(void);
    void RemoveBGcalibrate(Mat input);
    Mat RemoveBackground(Mat input);
    void Skincalibrate(Mat input);
    Mat getSkinMask(Mat input);
    void RemoveFace(Mat input, Mat output);
    Mat CountFinger(Mat input);
private:
    bool BG_calibrate = false;
    //need to be changed
    int THRES_BG = 5;
    
    //for skin threshold
    int hLowThreshold = 0;
    int hHighThreshold = 0;
    int sLowThreshold = 0;
    int sHighThreshold = 0;
    int vLowThreshold = 0;
    int vHighThreshold = 0;
    
    Rect skinSampleRectngle1,skinSampleRectngle2;
    
    bool SK_calibrated = false;
    
    void Thres_calculate (Mat sample1, Mat sample2);
    double PointsDistance(Point a, Point b);
    
    double BOUND_FINGER_SCALE = 0.35;
    double BOUND_NEIGHBOR_SCALE = 0.05;
    int LIMIT_ANGLE_SUP = 60;
    int LIMIT_ANGLE_INF = 5;
    
    vector<Point> MedianCal (vector<Point> points, double max_neighbor);
    vector<Point> findClosestOnX(vector<Point> points, Point pivot);
    double findPointsDistanceOnX(Point a, Point b);
    bool isFinger(Point a, Point b, Point c, double limit_angle_inf, double limit_angle_sup, Point palm_center, double min_distance_from_palm);
    double findAngle(Point a, Point b, Point c);

    Scalar color_blue = Scalar(255, 0, 0);
    Scalar color_green = Scalar(0, 255, 0);
    Scalar color_red = Scalar(0, 0, 255);
    Scalar color_black = Scalar(0, 0, 0);
    Scalar color_white = Scalar(255, 255, 255);
    Scalar color_yellow = Scalar(0, 255, 255);
    Scalar color_purple = Scalar(255, 0, 255);
    
    void drawVectorPoints(Mat image, vector<Point> points, Scalar color, bool with_numbers);

    
};

#endif /* HandDetect_hpp */
