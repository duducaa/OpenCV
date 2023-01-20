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

        Mat blur;
        GaussianBlur(gray, blur, Size(5, 5), 0);

        int thresh = getTrackbarPos("Threshold", window);
        int maxValue = getTrackbarPos("Max Value", window);
        if (thresh > maxValue) {
            setTrackbarPos("Max Value", window, thresh);
        }

        Mat canny;
        Canny(blur, canny, thresh, maxValue);

        imshow(window, canny);
        imshow("Img", clone);
        if ((char) waitKey(1) == 27) break;
    }

    destroyAllWindows();

    return 0;

}