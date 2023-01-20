#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char ** argv) {

    string path = "../../projects/samples/figures.jpg";

    Mat img = imread(path);
    if (img.empty()) return 1;

    resize(img, img, Size(img.cols * 0.4, img.rows * 0.4));

    string window = "Display";
    namedWindow(window);
    createTrackbar("Contrast", window, nullptr, 300);
    createTrackbar("Brightness", window, nullptr, 100);

    while (true) {
        Mat clone = img.clone();

        int contrast = getTrackbarPos("Contrast", window);
        int brightness = getTrackbarPos("Brightness", window);

        convertScaleAbs(clone, clone, contrast / 100.0, brightness);

        imshow(window, clone);
        if ((char) waitKey(1) == 27) break;
    }

    destroyAllWindows();

    return 0;

}