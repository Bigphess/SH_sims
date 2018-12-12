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
    
    
};

#endif /* HandDetect_hpp */
