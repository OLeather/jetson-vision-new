#include "GripPipeline.hpp"

GripPipeline::GripPipeline() {
}

/**
* Runs an iteration of the pipeline and updates outputs.
*/
int oldAngle = 320;

void GripPipeline::Process(cv::Mat &source0) {
    VisionProperties props = VisionProperties();
    props.readVisionProperties();

    //Step HSV_Threshold0:
    //input
    cv::Mat hsvThresholdInput = source0;
    int hsvThresholdHue[] = {props.getHMin(),props.getHMax()};
    int hsvThresholdSaturation[] = {props.getSMin(),props.getSMax()};
    int hsvThresholdValue[] = {props.getVMin(),props.getVMax()};
    hsvThreshold(hsvThresholdInput, hsvThresholdHue, hsvThresholdSaturation, hsvThresholdValue,
                 this->hsvThresholdOutput);







    //Step Find_Contours0:
    //input
    cv::Mat findContoursInput = hsvThresholdOutput;
    bool findContoursExternalOnly = false;  // default Boolean
    findContours(findContoursInput, findContoursExternalOnly, this->findContoursOutput);

  //Step Convex_Hulls0:
  //input
  std::vector<std::vector<cv::Point> > convexHullsContours = findContoursOutput;
  convexHulls(convexHullsContours, this->convexHullsOutput);

 //Step Filter_Contours0:
 //input

 std::vector<std::vector<cv::Point> > filterContoursContours = convexHullsOutput;
 double filterContoursMinArea = props.getCamWidth()/6;  // default Double
 double filterContoursMinPerimeter = 0;  // default Double
 double filterContoursMinWidth = props.getCamWidth()/17;  // default Double
 double filterContoursMaxWidth = 1000;  // default Double
 double filterContoursMinHeight = 50;  // default Double
 double filterContoursMaxHeight = 1000;  // default Double
 double filterContoursSolidity[] = {0, 100};
 double filterContoursMaxVertices = 30.0;  // default Double
 double filterContoursMinVertices = 0;  // default Double
 double filterContoursMinRatio = 0;  // default Double
 double filterContoursMaxRatio = 1000;  // default Double
 filterContours(filterContoursContours, filterContoursMinArea, filterContoursMinPerimeter, filterContoursMinWidth,
                filterContoursMaxWidth, filterContoursMinHeight, filterContoursMaxHeight, filterContoursSolidity,
                filterContoursMaxVertices, filterContoursMinVertices, filterContoursMinRatio, filterContoursMaxRatio,
                this->filterContoursOutput);
 std::vector<std::vector<cv::Point> > groupedContoursContours = filterContoursOutput;
 groupContours(groupedContoursContours, (props.getGroupSelect() + 1), props.getDirectionSelect(), this->groupContoursOutput);
 convexHullsContours = groupContoursOutput;
 convexHulls(convexHullsContours, this->convexHullsOutput);
 std::vector<std::vector<cv::Point> > approxPolyContours = convexHullsOutput;
 approxPoly(approxPolyContours, this->approxPolyOutput);
 std::vector<std::vector<cv::Point> > mathPointsContours = approxPolyOutput;
 mathPoints(mathPointsContours, this->mathPointsOutput);

}

/**
 * This method is a generated getter for the output of a HSV_Threshold.
 * @return Mat output from HSV_Threshold.
 */
cv::Mat *GripPipeline::GetHsvThresholdOutput() {
    return &(this->hsvThresholdOutput);
}

/**
 * This method is a generated getter for the output of a Find_Contours.
 * @return ContoursReport output from Find_Contours.
 */
std::vector<std::vector<cv::Point> > *GripPipeline::GetFindContoursOutput() {
    return &(this->findContoursOutput);
}

/**
 * This method is a generated getter for the output of a Convex_Hulls.
 * @return ContoursReport output from Convex_Hulls.
 */
std::vector<std::vector<cv::Point> > *GripPipeline::GetConvexHullsOutput() {
    return &(this->convexHullsOutput);
}

/**
 * This method is a generated getter for the output of a Filter_Contours.
 * @return ContoursReport output from Filter_Contours.
 */
std::vector<std::vector<cv::Point> > *GripPipeline::GetFilterContoursOutput() {
    return &(this->filterContoursOutput);
}

std::vector<std::vector<cv::Point> > *GripPipeline::GetGroupedContoursOutput() {
    return &(this->groupContoursOutput);
}

std::vector<std::vector<cv::Point> > *GripPipeline::GetApproxPolyOutput() {
    return  &(this->approxPolyOutput);
}
std::vector<std::vector<cv::Point> > *GripPipeline::GetMathPoints() {
    return &(this->mathPointsOutput);
}

/**
 * Segment an image based on hue, saturation, and value ranges.
 *
 * @param input The image on which to perform the HSL threshold.
 * @param hue The min and max hue.
 * @param sat The min and max saturation.
 * @param val The min and max value.
 * @param output The image in which to store the output.
 */
void GripPipeline::hsvThreshold(cv::Mat &input, int hue[], int sat[], int val[], cv::Mat &out) {
    cv::cvtColor(input, out, cv::COLOR_BGR2HSV);
    cv::inRange(out, cv::Scalar(hue[0], sat[0], val[0]), cv::Scalar(hue[1], sat[1], val[1]), out);
}

/**
 * Finds contours in an image.
 *
 * @param input The image to find contours in.
 * @param externalOnly if only external contours are to be found.
 * @param contours vector of contours to put contours in.
 */
void GripPipeline::findContours(cv::Mat &input, bool externalOnly, std::vector<std::vector<cv::Point> > &contours) {
    std::vector<cv::Vec4i> hierarchy;
    contours.clear();
    int mode = externalOnly ? cv::RETR_EXTERNAL : cv::RETR_LIST;
    int method = cv::CHAIN_APPROX_SIMPLE;
    cv::findContours(input, contours, hierarchy, mode, method);
}

/**
 * Compute the convex hulls of contours.
 *
 * @param inputContours The contours on which to perform the operation.
 * @param outputContours The contours where the output will be stored.
 */
void GripPipeline::convexHulls(std::vector<std::vector<cv::Point> > &inputContours,
                               std::vector<std::vector<cv::Point> > &outputContours) {
    std::vector<std::vector<cv::Point> > hull(inputContours.size());
    outputContours.clear();
    for (size_t i = 0; i < inputContours.size(); i++) {
        cv::convexHull(cv::Mat((inputContours)[i]), hull[i], false);
    }
    outputContours = hull;
}


/**
 * Filters through contours.
 * @param inputContours is the input vector of contours.
 * @param minArea is the minimum area of a contour that will be kept.
 * @param minPerimeter is the minimum perimeter of a contour that will be kept.
 * @param minWidth minimum width of a contour.
 * @param maxWidth maximum width.
 * @param minHeight minimum height.
 * @param maxHeight  maximimum height.
 * @param solidity the minimum and maximum solidity of a contour.
 * @param minVertexCount minimum vertex Count of the contours.
 * @param maxVertexCount maximum vertex Count.
 * @param minRatio minimum ratio of width to height.
 * @param maxRatio maximum ratio of width to height.
 * @param output vector of filtered contours.
 */
void
GripPipeline::filterContours(std::vector<std::vector<cv::Point> > &inputContours, double minArea, double minPerimeter,
                             double minWidth, double maxWidth, double minHeight, double maxHeight, double solidity[],
                             double maxVertexCount, double minVertexCount, double minRatio, double maxRatio,
                             std::vector<std::vector<cv::Point> > &output) {
    std::vector<cv::Point> hull;
    output.clear();
    for (std::vector<cv::Point> contour: inputContours) {
        cv::Rect bb = boundingRect(contour);
        if (bb.width < minWidth || bb.width > maxWidth) continue;
        if (bb.height < minHeight || bb.height > maxHeight) continue;
        double area = cv::contourArea(contour);
        if (area < minArea) continue;
        if (arcLength(contour, true) < minPerimeter) continue;
        cv::convexHull(cv::Mat(contour, true), hull);
        double solid = 100 * area / cv::contourArea(hull);
        if (solid < solidity[0] || solid > solidity[1]) continue;
        if (contour.size() < minVertexCount || contour.size() > maxVertexCount) continue;
        double ratio = (double) bb.width / (double) bb.height;
        if (ratio < minRatio || ratio > maxRatio) continue;
        output.push_back(contour);
    }
}

void GripPipeline::groupContours(std::vector<std::vector<cv::Point> > &inputContours, int grouping, int dirFilter, std::vector<std::vector<cv::Point> > &output) {
    output.clear();
    int ctrAmmount = 0;
    std::vector<std::vector<cv::Point> > curOutput;
    std::vector<cv::Point> groupedTarget;
    //std::vector<std::vector<cv::Point> > leftTargets;
    //std::vector<std::vector<cv::Point> > rightTargets;
    for (std::vector<cv::Point> contour: inputContours) {
        if(grouping == 2){
            for (std::vector<cv::Point> contour1: inputContours) {
                cv::RotatedRect rect = cv::minAreaRect(contour);
                cv::RotatedRect rect1 = cv::minAreaRect(contour1);
                int xOffset = rect.center.x - rect1.center.x;
                int yOffset = rect.center.y - rect1.center.y;
                int avgHeight = (rect.boundingRect().height+rect1.boundingRect().height)/2;
                if(abs(xOffset) > 10 && abs(xOffset) < (avgHeight*3) && abs(yOffset) < 100){
                    groupedTarget.clear();
                    if(dirFilter == 0){
                        groupedTarget.insert(groupedTarget.begin(), contour.begin(), contour.end());
                        groupedTarget.insert(groupedTarget.begin(), contour1.begin(), contour1.end());
                        curOutput.push_back(groupedTarget);
                    }
                    else if(dirFilter == 1) {
                        if((rect.angle < -80 || rect.angle > -10) && (rect1.angle < -80 || rect1.angle > -10)){
                            if(xOffset > 0){
                                groupedTarget.insert(groupedTarget.begin(), contour.begin(), contour.end());
                                groupedTarget.insert(groupedTarget.begin(), contour1.begin(), contour1.end());
                                curOutput.push_back(groupedTarget);
                            }
                        }

                    }
                    else if(dirFilter == 2){
                        if(rect.angle < -45 && rect1.angle > -45){
                            if(xOffset > 0){
                                groupedTarget.insert(groupedTarget.begin(), contour.begin(), contour.end());
                                groupedTarget.insert(groupedTarget.begin(), contour1.begin(), contour1.end());
                                curOutput.push_back(groupedTarget);
                            }
                        }
                    }
                    else if(dirFilter == 3){
                        if(rect.angle < -45 && rect1.angle > -45){
                            if(xOffset < 0){
                                groupedTarget.insert(groupedTarget.begin(), contour.begin(), contour.end());
                                groupedTarget.insert(groupedTarget.begin(), contour1.begin(), contour1.end());
                                curOutput.push_back(groupedTarget);
                            }
                        }
                    }

                }
            }



            /*
            if(cv::minAreaRect(contour).angle < -45 ){
                leftTargets.push_back(contour);
            }
            else{
                rightTargets.push_back(contour);
            }
             */
            /*
            if(ctrAmmount < 2){
                groupedTarget.insert(groupedTarget.begin(), contour.begin(), contour.end());
                ctrAmmount ++;
                if(ctrAmmount == 2){
                    curOutput.push_back(groupedTarget);
                    ctrAmmount = 0;
                }
            }*/
        }
        else{
            curOutput.push_back(contour);

        }
    }

    VisionProperties props = VisionProperties();
    props.readVisionProperties();
    std::vector<cv::Point> curCtr;
    int curClosest = 9999;
    if(props.getSortSelect() == 0 && curOutput.size() > 0){
        for (std::vector<cv::Point> contour: curOutput) {
            cv::Rect box = cv::boundingRect(contour);
            std::cout << oldAngle << std::endl;
            if(abs((box.x + (box.width/2)) -  oldAngle) + abs((box.y + (box.height/2)) -  props.getCamHeight()/2) < curClosest){

                curClosest = abs((box.x + (box.width/2)) - oldAngle) + abs((box.y + (box.height/2)) -  props.getCamHeight()/2);
                curCtr = contour;

            }


        }
        cv::Rect box = cv::boundingRect(curCtr);
        oldAngle = ((box.x + (box.width/2)));
        output.push_back(curCtr);
    }
    else{
        for (std::vector<cv::Point> contour: curOutput) {
            output.push_back(contour);
        }
    }

}

void GripPipeline::approxPoly(std::vector<std::vector<cv::Point> > &inputContours,
                              std::vector<std::vector<cv::Point> > &outputContours) {
    std::vector<std::vector<cv::Point> > approxPoly(inputContours.size());
    outputContours.clear();
    for (size_t i = 0; i < inputContours.size(); i++) {
        cv::approxPolyDP(inputContours[i], approxPoly[i], 0.04 * cv::arcLength(inputContours[i], true), true);
    }
    outputContours = approxPoly;


}

void GripPipeline::mathPoints(std::vector<std::vector<cv::Point> > &inputContours,  std::vector<std::vector<cv::Point> > &outputPoints) {
    std::vector<std::vector<cv::Point> > mathPoints(inputContours.size());

    //Point Order:
    // 1-----3
    // |     |
    // 2-----4
    int j = 0;
    for (std::vector<cv::Point> contour: inputContours) {
        int currentLeft = 9999;
        int leftIndex1 = 0;
        int leftIndex2 = 0;
        int currentRight = 0;
        int rightIndex1 = 0;
        int rightIndex2 = 0;
        int i = 0;
        for (cv::Point point: contour) {
            if(point.x < currentLeft){
                leftIndex1 = i;
                currentLeft = point.x;
            }
            i++;
        }
        i = 0;
        currentLeft = 9999;
        for (cv::Point point: contour) {
            if(i != leftIndex1){
                if(point.x < currentLeft){
                    leftIndex2 = i;
                    currentLeft = point.x;
                }
            }
            i++;
        }
        i = 0;
        for (cv::Point point: contour) {
            if(point.x > currentRight){
                rightIndex1 = i;
                currentRight = point.x;
            }
            i++;
        }
        i = 0;
        currentRight = 0;
        for (cv::Point point: contour) {
            if(i != rightIndex1){
                if(point.x > currentRight){
                    rightIndex2 = i;
                    currentRight = point.x;
                }
            }
            i++;
        }



        if(contour[leftIndex1].y < contour[leftIndex2].y){
            mathPoints.at(j).push_back(contour.at(leftIndex1));
            mathPoints.at(j).push_back(contour.at(leftIndex2));
        }
        else{
            mathPoints.at(j).push_back(contour.at(leftIndex2));
            mathPoints.at(j).push_back(contour.at(leftIndex1));

        }
        if(contour[rightIndex1].y < contour[rightIndex2].y){
            mathPoints.at(j).push_back(contour.at(rightIndex1));
            mathPoints.at(j).push_back(contour.at(rightIndex2));
        }
        else{
            mathPoints.at(j).push_back(contour.at(rightIndex2));
            mathPoints.at(j).push_back(contour.at(rightIndex1));
        }


        j++;
    }
    outputPoints = mathPoints;
}








