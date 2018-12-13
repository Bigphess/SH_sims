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
    skinSampleRectngle1 = Rect(SampleWidth / 4, SampleHeight / 2,SampleRetangleSize,SampleRetangleSize);
    skinSampleRectngle2 = Rect(SampleWidth / 4, SampleHeight / 3,SampleRetangleSize,SampleRetangleSize);
    
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
    //need to be changed based on the environment
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
    
    color_blue = Scalar(255, 0, 0);
    color_green = Scalar(0, 255, 0);
    color_red = Scalar(0, 0, 255);
    color_black = Scalar(0, 0, 0);
    color_white = Scalar(255, 255, 255);
    color_yellow = Scalar(0, 255, 255);
    color_purple = Scalar(255, 0, 255);

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
    Mat morphElement = getStructuringElement(MORPH_ELLIPSE, {20,20});
    morphologyEx(SkinMask, SkinMask, MORPH_CLOSE, morphElement);
    
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
//    int contour_num = 0;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    Mat output = Mat::zeros(input.size(), CV_8UC3);
    //转化成灰度图，转化成二值图
    Mat morphElement = getStructuringElement(MORPH_ELLIPSE, {5,5});
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

//    //draw the rect on the biggest contour
//    Rect hahaha = boundingRect(contours[max_contour_index]);
//    rectangle(input,hahaha,Scalar(255,255,255));
    //find the convex hull of the hand
    vector<Point> hull_points;
    vector<int> hull_ints;
    convexHull(contours[max_contour_index], hull_points, false, true);
    convexHull(contours[max_contour_index], hull_ints, false, false);
    //find the defects。 start, end, far, distance to far
    vector<Vec4i> defects;
    if(hull_ints.size() > 3){
        convexityDefects(contours[max_contour_index], hull_ints, defects);
    }
    else
        return output;

    //fin thee conveex hull
    Rect bound_hull = boundingRect(Mat(hull_points));
    rectangle(input,bound_hull,Scalar(255,255,255));

//    return input;

    
    
    Point Center_bound_hull(
                    (bound_hull.tl().x + bound_hull.br().x) / 2,
                    (bound_hull.tl().y + bound_hull.br().y) / 2);

    //find the far points and start points
    vector<Point> starts;
    vector<Point> fars;
    for(int i = 0; i < defects.size(); i++){
        starts.push_back(contours[max_contour_index][defects[i].val[0]]);
        //need something for far points
       if(PointsDistance(contours[max_contour_index][defects[i].val[2]], Center_bound_hull) < bound_hull.height * BOUND_FINGER_SCALE)            fars.push_back(contours[max_contour_index][defects[i].val[2]]);
    }
    
    //filter the points with medians
    vector<Point> fliter_starts = MedianCal(starts,bound_hull.height * BOUND_NEIGHBOR_SCALE);
    vector<Point> fliter_fars = MedianCal(fars, bound_hull.height * BOUND_NEIGHBOR_SCALE);
    
    
    //find the fingers
    vector<Point> fliter_finger;
    vector<Point> fingers;
    
    if(fliter_fars.size() > 1){
        
        for( int i = 0; i < fliter_starts.size(); i++){
            vector<Point> cloests = findClosestOnX(fliter_fars, fliter_starts[i]);
            
            if(isFinger(cloests[0], fliter_starts[i], cloests[1], LIMIT_ANGLE_INF, LIMIT_ANGLE_SUP, Center_bound_hull, bound_hull.height * BOUND_FINGER_SCALE)){
                fingers.push_back(fliter_starts[i]);
            }
            
        }
        
        if(fingers.size() > 0){
            while (fingers.size() > 5) {
                fingers.pop_back();
            }
            
            for(int i = 0; i < fingers.size() - 1; i++){
                if(findPointsDistanceOnX(fingers[i], fingers[i+1]) > bound_hull.height * BOUND_NEIGHBOR_SCALE * 1.5)
                    fliter_finger.push_back(fingers[i]);
            }
            
            if(fingers.size() > 2){
                if(findPointsDistanceOnX(fingers[0], fingers[fingers.size() - 1]) > bound_hull.height * BOUND_NEIGHBOR_SCALE * 1.5)
                    fliter_finger.push_back(fingers[fingers.size() - 1]);
            }
            else
                fliter_finger.push_back(fingers[fingers.size() - 1]);
        }
    }

    
    //draw
    drawContours(output, contours, max_contour_index, color_green, 2, 8, hierarchy);
    polylines(output, hull_points, true, color_blue);
    rectangle(output, bound_hull.tl(), bound_hull.br(), color_red, 2, 8, 0);
    circle(output, Center_bound_hull, 5, color_purple, 2, 8);
    drawVectorPoints(output, fliter_starts, color_blue, true);
    drawVectorPoints(output, fliter_fars, color_red, true);
//    putText(output, to_string(fliter_fars.size()), Center_bound_hull, FONT_HERSHEY_PLAIN, 3, color_red);

    drawVectorPoints(output, fliter_finger, color_yellow, false);
    putText(output, to_string(fliter_finger.size()), Center_bound_hull, FONT_HERSHEY_PLAIN, 3, color_purple);



    
   
        
    return output;
  
}







double HandDetect:: PointsDistance(Point a, Point b) {
    Point difference = a - b;
    return sqrt(difference.ddot(difference));
}



vector<Point> HandDetect:: MedianCal(vector<Point> points, double max_neighbor){
    vector<Point> median_points;
    
    if (points.size() == 0)
        return median_points;
    
    if (max_neighbor <= 0)
        return median_points;
    
    // calculate the median of the vector
    Point reference = points[0];
    Point median = points[0];
    
    for (int i = 1; i < points.size(); i++) {
        if (PointsDistance(reference, points[i]) > max_neighbor) {
            
            // the point is not in range, we save the median
            median_points.push_back(median);
            
            // we swap the reference
            reference = points[i];
            median = points[i];
        }
        else
            median = (points[i] + median) / 2;
    }
    
    // last median
    median_points.push_back(median);
    
    return median_points;
}

//find tp points cloest to the far points
vector<Point> HandDetect::findClosestOnX(vector<Point> points, Point pivot){
    vector<Point> to_return(2);
    
    if (points.size() == 0)
        return to_return;
    
    double distance_x_1 = DBL_MAX;
    double distance_1 = DBL_MAX;
    double distance_x_2 = DBL_MAX;
    double distance_2 = DBL_MAX;
    int index_found = 0;
    
    for (int i = 0; i < points.size(); i++) {
        double distance_x = findPointsDistanceOnX(pivot, points[i]);
        double distance = PointsDistance(pivot, points[i]);
        
        if (distance_x < distance_x_1 && distance_x != 0 && distance <= distance_1) {
            distance_x_1 = distance_x;
            distance_1 = distance;
            index_found = i;
        }
    }
    
    to_return[0] = points[index_found];
    
    for (int i = 0; i < points.size(); i++) {
        double distance_x = findPointsDistanceOnX(pivot, points[i]);
        double distance = PointsDistance(pivot, points[i]);
        
        if (distance_x < distance_x_2 && distance_x != 0 && distance <= distance_2 && distance_x != distance_x_1) {
            distance_x_2 = distance_x;
            distance_2 = distance;
            index_found = i;
        }
    }
    
    to_return[1] = points[index_found];
    
    return to_return;

}

double HandDetect::findPointsDistanceOnX(Point a, Point b){
    double to_return = 0.0;
    
    if (a.x > b.x)
        to_return = a.x - b.x;
    else
        to_return = b.x - a.x;
    
    return to_return;
}

bool HandDetect::isFinger(Point a, Point b, Point c, double limit_angle_inf, double limit_angle_sup, Point palm_center, double min_distance_from_palm) {
    double angle = findAngle(a, b, c);
    if (angle > limit_angle_sup || angle < limit_angle_inf)
        return false;
    
    // the finger point sohould not be under the two far points
    int delta_y_1 = b.y - a.y;
    int delta_y_2 = b.y - c.y;
    if (delta_y_1 > 0 && delta_y_2 > 0)
        return false;
    
    // the two far points should not be both under the center of the hand
    int delta_y_3 = palm_center.y - a.y;
    int delta_y_4 = palm_center.y - c.y;
    if (delta_y_3 < 0 && delta_y_4 < 0)
        return false;
    
    //should not too close to the center
    double distance_from_palm = PointsDistance(b, palm_center);
    if (distance_from_palm < min_distance_from_palm)
        return false;
    
    // this should be the case when no fingers are up
    double distance_from_palm_far_1 = PointsDistance(a, palm_center);
    double distance_from_palm_far_2 = PointsDistance(c, palm_center);
    if (distance_from_palm_far_1 < min_distance_from_palm / 4 || distance_from_palm_far_2 < min_distance_from_palm / 4)
        return false;
    
    return true;
}

double HandDetect::findAngle(Point a, Point b, Point c){
    double ab = PointsDistance(a, b);
    double bc = PointsDistance(b, c);
    double ac = PointsDistance(a, c);
    return acos((ab * ab + bc * bc - ac * ac) / (2 * ab * bc)) * 180 / CV_PI;
}


void HandDetect::drawVectorPoints(Mat image, vector<Point> points, Scalar color, bool with_numbers) {
    for (int i = 0; i < points.size(); i++) {
        circle(image, points[i], 5, color, 2, 8);
        if(with_numbers)
            putText(image, to_string(i), points[i], FONT_HERSHEY_PLAIN, 3, color);
    }
}


