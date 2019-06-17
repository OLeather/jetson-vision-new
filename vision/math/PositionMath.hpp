//
// Created by Owen on 4/21/2019.
//
#pragma once

#include <opencv2/opencv.hpp>
#include <math.h>

#ifndef VISION_MATH_HPP
#define VISION_MATH_HPP



class PositionMath {
private:
    double FOCAL_LENGTH = 570;
    double distance;
    double yaw;
    double offset;
    double skew;
    cv::Mat img;
    double calcDistance(double pixelDistance, double realHeight);
    double calcYaw(double pixelOffset);
    double calcOffset(double distance, double yaw);
    double calcSkew(double distanceLeft, double distanceRight, double offsetLeft, double offsetRight);


public:
    void Process3DImage(cv::Point leftPoint, cv::Point rightPoint, double pixelDistanceLeft, double realHeightLeft, double pixelDistanceRight, double realHeightRight, int camWidth, int graphScale);
    double GetDistance();
    double GetYaw();
    double GetOffset();
    double GetSkew();
    cv::Mat Get3DImage();
    void Reset3DImage(int,int);
};


#endif //VISION_MATH_HPP
