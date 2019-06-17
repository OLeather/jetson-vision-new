//
// Created by Owen on 4/19/2019.
//
#pragma once

#include <sstream>
#include <fstream>

#ifndef VISION_VISIONPROPERTIES_H
#define VISION_VISIONPROPERTIES_H


class VisionProperties {
private:
    int hmin;
    int hmax;
    int smin;
    int smax;
    int vmin;
    int vmax;
    int compression;
    int showfiltered;
    int drawctr;
    int sortselect;
    int groupselect;
    int directionselect;
    int show3d;
    int resizeValue;
    int cameraWidth = 640;
    int cameraHeight = 360;
    int storedAngle;
public:
    int getHMin();
    int getHMax();
    int getSMin();
    int getSMax();
    int getVMin();
    int getVMax();
    int getCompression();
    int getShowFiltered();
    int getDrawContours();
    int getSortSelect();
    int getGroupSelect();
    int getDirectionSelect();
    int getCamWidth();
    int getCamHeight();
    int getShow3D();
    int getResizeValue();
    int getStoredAngle();

    void setStoredAngle(int newVal);

    void readVisionProperties();
};


#endif //VISION_VISIONPROPERTIES_H
