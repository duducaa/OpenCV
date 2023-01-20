#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char ** argv) {

    string path = samples::findFile(argv[1]);

    Mat img = imread(path);
    if (img.empty()) return 1;

    string window = "Threshold";
    namedWindow(window);
    createTrackbar("Threshold", window, nullptr, 255);
    createTrackbar("Max Value", window, nullptr, 255);

    while (true) {
        Mat clone = img.clone();
        Mat gray;
        cvtColor(clone, gray, COLOR_BGR2GRAY);

        int thresh = getTrackbarPos("Threshold", window);
        int maxValue = getTrackbarPos("Max Value", window);
        if (thresh > maxValue) {
            setTrackbarPos("Max Value", window, thresh);
        }

        Mat threshImg;
        threshold(gray, threshImg, thresh, maxValue, THRESH_BINARY_INV);

        vector<vector<Point>> contours;
        findContours(threshImg, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
        drawContours(clone, contours, -1, Scalar(0, 255, 0), 3);

        imshow(window, threshImg);
        imshow("Img", clone);
        if ((char) waitKey(1) == 27) break;
    }

    destroyAllWindows();

    return 0;

}