//
// Created by Owen on 4/21/2019.
//

#include "PositionMath.hpp"

void PositionMath::Process3DImage(cv::Point leftPoint, cv::Point rightPoint, double pixelDistanceLeft, double realHeightLeft,
                          double pixelDistanceRight, double realHeightRight, int camWidth, int graphScale) {

    //Left of center is negative, right is positive
    // ----------|+++++++++++
    //Skew:
    //Facing right = positive
    //Facing left = negative
    // / = positive
    // \ = negative

    double lDist = calcDistance(pixelDistanceLeft, realHeightLeft);
    double rDist = calcDistance(pixelDistanceRight, realHeightRight);;
    distance = (lDist + rDist)/2;
    double lYaw = calcYaw((leftPoint.x) - (camWidth/2));
    double rYaw = calcYaw((rightPoint.x) - (camWidth/2));
    yaw = (lYaw + rYaw)/2;
    double lOffset = calcOffset(lDist, lYaw);
    double rOffset = calcOffset(rDist, rYaw);
    offset = (lOffset + rOffset)/2;
    double tSkew = calcSkew(lDist, rDist, lOffset, rOffset);
    skew = tSkew;

    int centerX = img.cols / 2;
    int centerY = img.rows - (img.rows / 5);




    cv::line(img, cv::Point(centerX + (lOffset * graphScale), centerY - (lDist * graphScale)),
             cv::Point(centerX, centerY), cv::Scalar(255, 255, 255));
    cv::line(img, cv::Point(centerX + (rOffset * graphScale), centerY - (rDist * graphScale)),
             cv::Point(centerX, centerY), cv::Scalar(255, 255, 255));
    cv::line(img, cv::Point(centerX + (rOffset * graphScale), centerY - (rDist * graphScale)),
             cv::Point(centerX + (lOffset * graphScale), centerY - (lDist * graphScale)),
             cv::Scalar(255, 255, 255));

    cv::circle(img, cv::Point(centerX + (lOffset * graphScale), centerY - (lDist * graphScale)), 0.5,
               cv::Scalar(255, 0, 0));
    cv::circle(img, cv::Point(centerX + (rOffset * graphScale), centerY - (rDist * graphScale)), 0.5,
               cv::Scalar(255, 0, 0));

    cv::circle(img, cv::Point(centerX, centerY), 0.5, cv::Scalar(0, 255, 0));

}


double PositionMath::GetDistance() {
    return distance;
}

double PositionMath::GetYaw() {
    return yaw;
}
double PositionMath::GetOffset() {
    return offset;
}
double PositionMath::GetSkew() {
    return skew;
}

cv::Mat PositionMath::Get3DImage() {
    return img;
}



double PositionMath::calcDistance(double pixelDistance, double realHeight) {
    return((realHeight * FOCAL_LENGTH)/pixelDistance);
}

double PositionMath::calcYaw(double pixelOffset) {
    double yaw = tan(abs(pixelOffset)/FOCAL_LENGTH) * (180/M_PI);
    if(pixelOffset < 0){
        yaw = -yaw;
    }
    return yaw;
}

double PositionMath::calcOffset(double distance, double yaw) {
    double offset = tan(abs(yaw) * (M_PI/180)) * distance;
    if(yaw < 0){
        offset = -offset;
    }
    return offset;
}

double PositionMath::calcSkew(double distanceLeft, double distanceRight, double offsetLeft, double offsetRight) {
    double x = abs(distanceRight-distanceLeft);
    double y = offsetLeft-offsetRight;
    double skew = tan(abs(y)/x)* (180/M_PI);
    if(y <0){
        skew = -skew;
    }
    return skew;
}

void PositionMath::Reset3DImage(int imgWidth, int imgHeight) {
    img = cv::Mat(imgHeight, imgWidth, CV_8UC3, cv::Scalar(0, 0, 0));
}


