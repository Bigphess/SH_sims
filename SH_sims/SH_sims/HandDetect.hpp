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
    void SaveTemplate(Mat input);
    Mat RemoveBackground(Mat input);
    void SkinColor(Mat input);
    
    
};

#endif /* HandDetect_hpp */
