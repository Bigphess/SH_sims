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

#define THRES_BG 5
#define BOUND_FINGER_SCALE 0.35
#define BOUND_NEIGHBOR_SCALE 0.05
#define LIMIT_ANGLE_SUP 60
#define LIMIT_ANGLE_INF 5

class HandDetect {
public:
    HandDetect(void);
    void RemoveBGcalibrate(Mat input);
    Mat RemoveBackground(Mat input);
    void Skincalibrate(Mat input);
    Mat getSkinMask(Mat input);
    void RemoveFace(Mat input, Mat output);
    vector<Point> CountFinger(Mat input, Mat output);
    Mat addPictures(Mat result, vector<Point> fingers);
    void mapToMat(const cv::Mat &srcAlpha, cv::Mat &dest, int x, int y);

private:
    //flags for keyboard
    bool BG_calibrate = false;
    bool SK_calibrated = false;
    Mat TemplateOfRemover;

    //for skin threshold
    int hLowThreshold = 0;
    int hHighThreshold = 0;
    int sLowThreshold = 0;
    int sHighThreshold = 0;
    int vLowThreshold = 0;
    int vHighThreshold = 0;
    Rect skinSampleRectngle1,skinSampleRectngle2;
    
    //for drawing the fingers
    Scalar color_blue = Scalar(255, 0, 0);
    Scalar color_green = Scalar(0, 255, 0);
    Scalar color_red = Scalar(0, 0, 255);
    Scalar color_black = Scalar(0, 0, 0);
    Scalar color_white = Scalar(255, 255, 255);
    Scalar color_yellow = Scalar(0, 255, 255);
    Scalar color_purple = Scalar(255, 0, 255);
    
    //for calculate thres
    void Thres_calculate (Mat sample1, Mat sample2);
    //for finding fingers
    double PointsDistance(Point a, Point b);
    vector<Point> MedianCal (vector<Point> points, double max_neighbor);
    vector<Point> findClosestOnX(vector<Point> points, Point pivot);
    double findPointsDistanceOnX(Point a, Point b);
    bool isFinger(Point a, Point b, Point c, double limit_angle_inf, double limit_angle_sup, Point palm_center, double min_distance_from_palm);
    double findAngle(Point a, Point b, Point c);
    void drawVectorPoints(Mat image, vector<Point> points, Scalar color, bool with_numbers);
    
};

#endif /* HandDetect_hpp */
