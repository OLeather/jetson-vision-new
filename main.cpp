#include <opencv2/opencv.hpp>
#include <fstream>
#include <chrono>
#include "vision/settings/VisionProperties.hpp"
#include "vision/pipeline/GripPipeline.hpp"
#include "vision/math/PositionMath.hpp"


void drawIntoArea(cv::Mat &src, cv::Mat &dst, int x, int y, int width, int height)
{
    cv::Mat scaledSrc;
// Destination image for the converted src image.
    cv::Mat convertedSrc(src.rows,src.cols,CV_8UC3, cv::Scalar(0,0,255));

// Convert the src image into the correct destination image type
// Could also use MixChannels here.
// Expand to support range of image source types.
    if (src.type() != dst.type())
    {
        cvtColor(src, convertedSrc, CV_GRAY2RGB);
    }else{
        src.copyTo(convertedSrc);
    }

// Resize the converted source image to the desired target width.
    resize(convertedSrc, scaledSrc,cv::Size(width,height),1,1,cv::INTER_CUBIC);

// create a region of interest in the destination image to copy the newly sized and converted source image into.
    cv::Mat ROI = dst(cv::Rect(x, y, scaledSrc.cols, scaledSrc.rows));
    scaledSrc.copyTo(ROI);
}


int resizeFactor = 2;
int main() {

    VisionProperties visionProperties = VisionProperties();
    GripPipeline pipeline = GripPipeline();
    PositionMath positionMath = PositionMath();
    int fps = 60;

    visionProperties.setStoredAngle(visionProperties.getCamWidth()/2);

    cv::VideoCapture cap = cv::VideoCapture(0);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, visionProperties.getCamWidth());
    cap.set(cv::CAP_PROP_FRAME_HEIGHT,  visionProperties.getCamHeight());
    cap.set(cv::CAP_PROP_AUTO_EXPOSURE, .75);
   // cap.set(cv::CAP_PROP_EXPOSURE, 0.1);
    cap.set(cv::CAP_PROP_AUTOFOCUS, 1);
    //cap.set(cv::CAP_PROP_FPS, 60);

    //std::cout << cv::getBuildInformation() << std::endl;
    if (!cap.isOpened()) {
        std::cout << "ERROR! Unable to open camera\n";
        return -1;
    }
    //Compression Params
    std::vector<int> compression_params;
    cv::Mat mat;
    bool trueBool = true;
    //int i = 0;
    //double avgTime = 0;
    while (cap.read(mat) ) {

        if( visionProperties.getResizeValue() == 1){
            resizeFactor = 2;
        }
        else{
            resizeFactor = 4;
        }
        int resizedWidth = visionProperties.getCamWidth()/resizeFactor;
        int resizedHeight =  visionProperties.getCamHeight()/resizeFactor;
        auto started = std::chrono::high_resolution_clock::now();
      // mat = cv::imread("../image4.jpeg");

        //Read Vision Propreties from text file
        visionProperties.readVisionProperties();

        compression_params.clear();
        compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
        compression_params.push_back(visionProperties.getCompression());
        compression_params.push_back(cv::IMWRITE_JPEG_PROGRESSIVE);
        compression_params.push_back(1);
        compression_params.push_back(cv::IMWRITE_JPEG_OPTIMIZE);
        compression_params.push_back(1);

        pipeline.Process(mat);



        std::vector<std::vector<cv::Point>> ctrs;
        ctrs = *pipeline.GetGroupedContoursOutput();
        std::vector<cv::Vec4i> h;

        //***SHOW APPROX POLY
        /*
        int a = 0;
        for(std::vector<cv::Point> contour: *pipeline.GetApproxPolyOutput()) {
            cv::drawContours(mat,*pipeline.GetApproxPolyOutput(),a, cv::Scalar(0,0,255),4);
            a++;
        }
        */

        //***SHOW CORNER POINTS
        /*
        int b = 0;
        for(std::vector<cv::Point> contour: *pipeline.GetMathPoints()){
            b = 0;
            for(cv::Point point: contour){
               // std::cout << point << std::endl;
               if(b == 0){
                   cv::circle(mat,cv::Point(point.x/4, point.y/4),1,cv::Scalar(255,0,0), -1);
               }
                if(b == 1){
                    cv::circle(mat,cv::Point(point.x/4, point.y/4),1,cv::Scalar(0,0,0), -1);
                }
                if(b == 2){
                    cv::circle(mat,cv::Point(point.x/4, point.y/4),1,cv::Scalar(0,0,255), -1);
                }
                if(b == 3){
                    cv::circle(mat,cv::Point(point.x/4, point.y/4),1,cv::Scalar(255,0,255), -1);
                }
                b++;
            }

        }*/

        //Resize image to sending size
        cv::resize(mat,mat,cv::Size(resizedWidth,resizedHeight),0,0,cv::INTER_NEAREST);


        double resizeRatioWidth = (visionProperties.getCamWidth()/resizedWidth);
        double resizeRatioHeight = (visionProperties.getCamHeight()/resizedHeight);
        //Draw contours
        if(visionProperties.getDrawContours() == 1){


            for(int i = 0; i < ctrs.size(); i++){
                //cv::drawContours(srcImg,ctrs,i,cv::Scalar(0,0,255),2,8,h,0,cv::Point());
                cv::RotatedRect rect = cv::minAreaRect(ctrs[i]);

                // We take the edges that OpenCV calculated for us
                cv::Point2f vertices2f[4];
                rect.points(vertices2f);

                cv::line(mat,cv::Point(rect.center.x/resizeRatioWidth - 5, rect.center.y/resizeRatioHeight), cv::Point(rect.center.x/resizeRatioWidth + 5, rect.center.y/resizeRatioHeight),cv::Scalar(100,100,100), 0.5);
                cv::line(mat,cv::Point(rect.center.x/resizeRatioWidth, rect.center.y/resizeRatioHeight-5), cv::Point(rect.center.x/resizeRatioWidth , rect.center.y/resizeRatioHeight + 5),cv::Scalar(100,100,100), 0.5);

                for (int i = 0; i < 4; i++)
                    cv::line(mat, cv::Point(vertices2f[i].x/resizeRatioWidth, vertices2f[i].y/resizeRatioHeight), cv::Point(vertices2f[(i+1)%4].x/resizeRatioWidth, vertices2f[(i+1)%4].y/resizeRatioHeight), cv::Scalar(50,50,50));

                cv::rectangle(mat,cv::Point(rect.boundingRect().x/resizeRatioWidth, rect.boundingRect().y/resizeRatioHeight), cv::Point(rect.boundingRect().x/resizeRatioWidth + rect.boundingRect().width/resizeRatioWidth, rect.boundingRect().y/resizeRatioHeight + rect.boundingRect().height/resizeRatioHeight), cv::Scalar(0,255,0));
            }
        }

        //Draw crosshair
        cv::line(mat,cv::Point(visionProperties.getCamWidth()/resizeRatioWidth/2 - 5,  visionProperties.getCamHeight()/resizeRatioHeight/2), cv::Point(visionProperties.getCamWidth()/resizeRatioWidth/2 + 5,  visionProperties.getCamHeight()/resizeRatioHeight/2),cv::Scalar(50,50,50));
        cv::line(mat,cv::Point(visionProperties.getCamWidth()/resizeRatioWidth/2, visionProperties.getCamHeight()/resizeRatioHeight/2 - 5), cv::Point(visionProperties.getCamWidth()/resizeRatioWidth/2,  visionProperties.getCamHeight()/resizeRatioHeight/2 + 5),cv::Scalar(50,50,50));



        if(visionProperties.getShowFiltered() == 1){
            drawIntoArea(*pipeline.GetHsvThresholdOutput(),mat,0,0,visionProperties.getCamWidth()/resizeRatioWidth/3,visionProperties.getCamHeight()/resizeRatioHeight/3);
        }



        //Show 3D map
        if(visionProperties.getShow3D() == 1) {

            positionMath.Reset3DImage(50/(resizeFactor/2), 75/(resizeFactor/2));

            for (std::vector<cv::Point> contour: *pipeline.GetMathPoints()) {

                cv::Point leftPoint = cv::Point((contour.at(0).x + contour.at(1).x) / 2,
                                                (contour.at(0).y + contour.at(1).y) / 2);
                cv::Point rightPoint = cv::Point((contour.at(2).x + contour.at(3).x) / 2,
                                                 (contour.at(2).y + contour.at(3).y) / 2);
                double leftPointDistance = sqrt(
                        (contour.at(1).x - contour.at(0).x) * (contour.at(1).x - contour.at(0).x) +
                        (contour.at(1).y - contour.at(0).y) * (contour.at(1).y - contour.at(0).y));
                double rightPointDistance = sqrt(
                        (contour.at(3).x - contour.at(2).x) * (contour.at(3).x - contour.at(2).x) +
                        (contour.at(3).y - contour.at(2).y) * (contour.at(3).y - contour.at(2).y));

                positionMath.Process3DImage(leftPoint, rightPoint, leftPointDistance, 5, rightPointDistance, 5, visionProperties.getCamWidth(), 1);

            }
            cv::Mat img = positionMath.Get3DImage();
            drawIntoArea(img,mat,mat.cols-(50/(resizeFactor/2)),0,50/(resizeFactor/2), 75/(resizeFactor/2));
        }
        auto done = std::chrono::high_resolution_clock::now();
        // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << std::endl;

        //Display latency value
        cv::putText(mat, std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count()) + "ms",cv::Point(2,10),cv::FONT_HERSHEY_PLAIN,0.7,cv::Scalar(0,255,0));

        cv::imwrite("/var/www/html/imageTest.jpg", mat, compression_params);
        std::rename("/var/www/html/imageTest.jpg","/var/www/html/ImageUpdated/imageTest.jpg");


        // += (double)std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count();
       // i++;
    }
    //std::cout <<"Avg Time: " << (avgTime/100) << std::endl;

    cv::destroyAllWindows();
    std::cout <<"Test"<< std::endl;

	return 1;
}

