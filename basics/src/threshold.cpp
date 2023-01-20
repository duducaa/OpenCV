#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char ** argv) {

    string path = samples::findFile(argv[1]);

    Mat img = imread(path);
    if (img.empty()) return 1;

    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);

    Mat blur;
    GaussianBlur(gray, blur, Size(3, 3), 0);

    string window = "Threshold";
    namedWindow(window);
    createTrackbar("Threshold", window, nullptr, 255);
    createTrackbar("Max Value", window, nullptr, 255);

    Mat result;

    while (true) {
        int thresh = getTrackbarPos("Threshold", window);
        int maxValue = getTrackbarPos("Max Value", window);
        if (thresh > maxValue) {
            setTrackbarPos("Max Value", window, thresh);
        }

        Mat threshImg;
        threshold(blur, threshImg, thresh, maxValue, THRESH_BINARY_INV);
        result = threshImg;

        imshow(window, threshImg);
        if ((char) waitKey(1) == 27) break;
    }

    destroyAllWindows();

    return 0;

}