#include <opencv2/opencv.hpp>
#include <fstream>
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
    resize(convertedSrc, scaledSrc,cv::Size(width,height),1,1,cv::INTER_AREA);

// create a region of interest in the destination image to copy the newly sized and converted source image into.
    cv::Mat ROI = dst(cv::Rect(x, y, scaledSrc.cols, scaledSrc.rows));
    scaledSrc.copyTo(ROI);
}

int main() {
    int fps = 60;
    //cv::VideoCapture cap = cv::VideoCapture("v4l2src device=/dev/video0 ! videoscale ! videorate ! video/x-raw, width=180, height=90, framerate=60/1 ! videoconvert ! appsink");
    cv::VideoCapture cap = cv::VideoCapture(0);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
   // cap.set(cv::CAP_PROP_EXPOSURE, -100);
    //cap.set(cv::CAP_PROP_FPS, 57);
    cv::Mat mat;
    int hmin = 0;
    int hmax = 180;
    int smin = 0;
    int smax = 255;
    int vmin = 0;
    int vmax = 255;
    int compressionVal = 30;
    int showFilteredBox = 0;
    int drawContoursBox = 0;
    int frameNumb = 0;
    //std::cout << cv::getBuildInformation() << std::endl;
    if (!cap.isOpened()) {
        std::cout << "ERROR! Unable to open camera\n";
        return -1;
    }
    while (cap.read(mat)) {
       // cv::resize(mat,mat,cv::Size(360,180),0,0,cv::INTER_CUBIC);
        //std::cout <<"Working" << std::endl;
        cv::Mat srcImg = mat.clone();


        std::vector<int> compression_params;
        compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
        compression_params.push_back(compressionVal);
        compression_params.push_back(cv::IMWRITE_JPEG_OPTIMIZE);
        compression_params.push_back(1);
        compression_params.push_back(cv::IMREAD_REDUCED_COLOR_8);
        compression_params.push_back(1);
        //cv::imwrite("/var/www/html/imageTestSrc.jpg", srcImg, compression_params);
        //std::rename("/var/www/html/imageTestSrc.jpg","/var/www/html/ImageUpdated/imageTestSrc.jpg");

        std::ifstream input("/var/www/html/VisionProperties.txt");
        int lineNumb = 0;
        int val;
        std::string line;

        while( std::getline(input, line, '\r')){
            std::stringstream ss;
            ss << line;

            std::string tmp;
            int found;
            while(!ss.eof()){
                ss >> tmp;
                if(std::stringstream(tmp) >> found){
                    val = found;
                }

            }
            if(lineNumb == 0){
                hmin = val;
               //std::cout << lineNumb << std::endl;
            }
            else if(lineNumb == 1){
                hmax = val;
                //std::cout << val << std::endl;
            }
            else if(lineNumb == 2){
                smin = val;
                //std::cout << "sm" << std::endl;
            }
            else if(lineNumb == 3){
                smax = val;
                //std::cout << "s" << std::endl;
            }
            else if(lineNumb == 4){
                vmin = val;
                //std::cout << "vm" << std::endl;
            }
            else if(lineNumb == 5){
                vmax = val;
               // std::cout << "v" << std::endl;
            }
            else if(lineNumb == 6){
                compressionVal = val;
                // std::cout << "v" << std::endl;
            }
            else if(lineNumb == 7){
                showFilteredBox = val;
                // std::cout << "v" << std::endl;
            }
            else if(lineNumb == 8){
                drawContoursBox = val;
                // std::cout << "v" << std::endl;
            }
            //std::cout << line << std::endl;
            lineNumb+= 1;


        }

        //cv::imshow("Window", mat);



        cv::cvtColor(mat,mat,cv::COLOR_BGR2HSV);
        cv::inRange(mat,cv::Scalar(hmin,smin,vmin), cv::Scalar(hmax,smax,vmax), mat);



        std::vector<std::vector<cv::Point>> ctr;
        std::vector<cv::Vec4i> h;
        cv::findContours(mat,ctr,h,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));
        if(drawContoursBox == 1){
            for(int i = 0; i < ctr.size(); i++){
                cv::drawContours(srcImg,ctr,i,cv::Scalar(0,0,255),2,8,h,0,cv::Point());
            }
        }
        if(showFilteredBox == 1){
            drawIntoArea(mat,srcImg,0,0,640/3,360/3);
        }
        cv::resize(srcImg,srcImg,cv::Size(160,90),0,0,cv::INTER_NEAREST);
        cv::imwrite("/var/www/html/imageTest.jpg", srcImg, compression_params);
        std::rename("/var/www/html/imageTest.jpg","/var/www/html/ImageUpdated/imageTest.jpg");
        cv::waitKey(1000/fps);
    }

    cv::destroyAllWindows();
    std::cout <<"Test"<< std::endl;

	return 1;
}

