#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <array>
#include <string>

#define WIDTH 60
#define HEIGHT 120
#define CAM_ID 1

#define MAG_COUNT 4
#define BRIGHTNESS_CUTOFF 180
#define MIN_DIR_MAG 50.0f
const char POSSIBLE_CHARACTERS[][MAG_COUNT] = { // the column signifies magnitude and the row the rotation
                                        {' ', '.', '\'', '|'},
                                        {' ', '.', ',',  '/'},
                                        {' ', '.', '-',  '_'},
                                        {' ', '.', '`',  '\\'},
                                          };
#define ROT_COUNT ((int)(sizeof(POSSIBLE_CHARACTERS) / MAG_COUNT))
#define MAG_SCALAR 0.3 //((MAG_COUNT-1)/(255*sqrt((float)2)))
#define DIR_SCALAR ((ROT_COUNT)/((float)(M_PI)))
const std::string ASCII_RANGE = "   .:-=+*%@#&$";
float mag_scale(float mag){
    mag = (((float)MAG_COUNT-1.0)*0.5)*(tanh(MAG_SCALAR*(mag - BRIGHTNESS_CUTOFF)) + tanh(MAG_SCALAR*(BRIGHTNESS_CUTOFF)));
    return mag;
}
float dir_scale(float dir){
    return DIR_SCALAR * (dir + ((dir < 0) ? M_PI : ((dir > M_PI) ? -M_PI : 0)));
}

unsigned char derivative_to_ascii(int16_t xDerivative, int16_t yDerivative, uint8_t magnitude){
    if (sqrtf(((float)xDerivative*(float)xDerivative) + ((float)yDerivative*(float)yDerivative)) < MIN_DIR_MAG) {
        int index = (int)round((float)(ASCII_RANGE.length()-1) * ((float)magnitude / 255.0f));
        return ASCII_RANGE[index];
    }
    unsigned int mag_id = (unsigned int)round(mag_scale((float)magnitude));
    float dir = atan2((float)yDerivative, (float)xDerivative);
    unsigned int dir_id = (unsigned int)(dir_scale(dir)) % (ROT_COUNT);
    return POSSIBLE_CHARACTERS[dir_id][mag_id];
}

void ImageToAsciiString(const cv::Mat& image, std::string& asciiImage) {
    cv::Mat resizedImage, gray, sobelx, sobely;
    cv::resize(image, resizedImage, cv::Size(HEIGHT, WIDTH));
    cv::cvtColor(resizedImage, gray, cv::COLOR_BGR2GRAY);
    cv::Sobel(gray, sobelx,  CV_16S, 1, 0);
    cv::Sobel(gray, sobely,  CV_16S, 0, 1);

    asciiImage.clear();

    for (int i = 0; i < gray.rows; i++) {
        for (int j = 0; j < gray.cols; j++) {
            // int index = LinearTransform(r.at<uchar>(i, j) + g.at<uchar>(i, j) + b.at<uchar>(i, j), 0, 255 * 3, 0, ASCII_RANGE.size() - 1);
            // asciiImage += ASCII_RANGE[index];
            char c = derivative_to_ascii(sobelx.at<int16_t>(i, j), sobely.at<int16_t>(i, j), gray.at<uchar>(i, j));
            asciiImage += c;
        }
        asciiImage += "\n";
    }
}

int main() {
    cv::VideoCapture cap(CAM_ID);
    if (!cap.isOpened()) {
        std::cerr << "Error opening video capture" << std::endl;
        return -1;
    }

    cv::Mat frame;
    std::string ascii;

    while (true) {
        cap >> frame;

        if (frame.empty()) {
            break;
        }

        ImageToAsciiString(frame, ascii);

        system("CLS"); 

        std::cout << ascii.c_str() << std::flush;

        if (cv::waitKey(100) == 'q') {
            break;
        }
    }

    return 0;
}
