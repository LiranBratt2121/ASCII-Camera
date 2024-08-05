#include <iostream>
#include <opencv2/opencv.hpp>
#include <array>
#include <string>

const std::string ASCII_RANGE = " .:-=+*%@#&$^!";

std::array<cv::Mat, 3> GetRGB(const cv::Mat& image) {
    cv::Mat rgbImg;
    cv::cvtColor(image, rgbImg, cv::COLOR_BGR2RGB);

    std::array<cv::Mat, 3> result;
    cv::split(rgbImg, result);
    return result;    
}

int LinearTransform(int value, int min, int max, int newMin, int newMax) {
    return (value - min) * (newMax - newMin) / (max - min) + newMin;
}

void ImageToAsciiString(const cv::Mat& image, std::string& asciiImage) {
    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(80, 60));

    auto rgbArray = GetRGB(resizedImage);
    auto& r = rgbArray[0];
    auto& g = rgbArray[1];
    auto& b = rgbArray[2];

    asciiImage.clear();

    for (int i = 0; i < r.rows; i++) {
        for (int j = 0; j < r.cols; j++) {
            int index = LinearTransform(r.at<uchar>(i, j) + g.at<uchar>(i, j) + b.at<uchar>(i, j), 0, 255 * 3, 0, ASCII_RANGE.size() - 1);
            asciiImage += ASCII_RANGE[index];
        }
        asciiImage += "\n";
    }
}

int main() {
    cv::VideoCapture cap(0);
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

        std::cout << ascii << std::flush;

        if (cv::waitKey(100) == 'q') {
            break;
        }
    }

    return 0;
}
