//
// Created by Owen on 4/19/2019.
//

#include <iostream>
#include "VisionProperties.hpp"

int VisionProperties::getHMin() {
    return this->hmin;
}

int VisionProperties::getHMax() {
    return this->hmax;
}

int VisionProperties::getSMin() {
    return this->smin;
}

int VisionProperties::getSMax() {
    return this->smax;
}

int VisionProperties::getVMin() {
    return this->vmin;
}

int VisionProperties::getVMax() {
    return this->vmax;
}

int VisionProperties::getCompression() {
    return this->compression;
}

int VisionProperties::getShowFiltered() {
    return this->showfiltered;
}

int VisionProperties::getDrawContours() {
    return this->drawctr;
}
int VisionProperties::getSortSelect() {
    return this->sortselect;
}
int VisionProperties::getGroupSelect() {
    return this->groupselect;
}
int VisionProperties::getDirectionSelect() {
    return this->directionselect;
}
int VisionProperties::getShow3D() {
    return this->show3d;
}
int VisionProperties::getResizeValue() {
    return this->resizeValue;
}
int VisionProperties::getCamWidth() {
    return this->cameraWidth;
}
int VisionProperties::getCamHeight() {
    return this->cameraHeight;
}
int VisionProperties::getStoredAngle() {
    return this->storedAngle;
}


void VisionProperties::readVisionProperties() {
   // std::ios_base::sync_with_stdio (false);
    std::ifstream input("/var/www/html/VisionProperties.txt");
    int lineNumb = 0;
    int val;
    std::string line;

    while( !input.eof()) {
        std::getline(input, line, ':');
    //    std::cout << line << std::endl;
        //val = std::stoi(line);
        //std::cout << line << std::endl;

        if (lineNumb == 1) {
            //val = std::stoi(line);
            //std::cout << val << std::endl;
            this->hmin = std::stoi(line);
        } else if (lineNumb == 3) {
           // std::cout << line << std::endl;
            this->hmax = std::stoi(line);
        } else if (lineNumb == 5) {
           // std::cout << line << std::endl;
            this->smin = std::stoi(line);
        } else if (lineNumb == 7) {
          //  std::cout << line << std::endl;
           this->smax = std::stoi(line);
        } else if (lineNumb == 9) {
          //  std::cout << line << std::endl;
            this->vmin = std::stoi(line);
        } else if (lineNumb == 11) {
           // std::cout << line << std::endl;
            this->vmax = std::stoi(line);
        } else if (lineNumb == 13) {
           // std::cout << line << std::endl;
            this->compression = std::stoi(line);
        } else if (lineNumb == 15) {
           // std::cout << line << std::endl;
           this->showfiltered = std::stoi(line);
        } else if (lineNumb == 17) {
            //std::cout << line << std::endl;
           this->drawctr = std::stoi(line);
        } else if (lineNumb == 19) {
            //std::cout << line << std::endl;
            this->sortselect = std::stoi(line);
        } else if (lineNumb == 21) {
            //std::cout << "Group: " << line << std::endl;
            this->groupselect = std::stoi(line);
        } else if (lineNumb == 23){
            //std::cout << line << std::endl;
            this->directionselect = std::stoi(line);
        }
        else if (lineNumb == 25){
            //std::cout << line << std::endl;
            this->show3d = std::stoi(line);
        }
        else if (lineNumb == 27){
            //std::cout << line << std::endl;
            this->resizeValue = std::stoi(line);
        }

        lineNumb += 1;

    }
}

void VisionProperties::setStoredAngle(int newVal) {
    this->storedAngle = newVal;
}


