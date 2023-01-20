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

    Mat thresh;
    threshold(gray, thresh, 230, 255, THRESH_BINARY_INV);

    string window = "Display";
    namedWindow(window);
    createTrackbar("Shape", window, nullptr, 2);
    createTrackbar("Kernel Size", window, nullptr, 60);
    setTrackbarMin("Kernel Size", window, 1);
    createTrackbar("Iterations", window, nullptr, 100);
    

    while (true) {
        Mat clone = thresh.clone();

        int shape = getTrackbarPos("Shape", window);
        int ksize = getTrackbarPos("Kernel Size", window);
        int iterations = getTrackbarPos("Iterations", window);

        Mat rect = getStructuringElement(shape, Size(ksize, ksize));
        morphologyEx(clone, clone, MORPH_OPEN, rect, Point(-1, -1), iterations);

        imshow(window, clone);
        if ((char) waitKey(1) == 27) break;
    }

    destroyAllWindows();    

    return 0;

}