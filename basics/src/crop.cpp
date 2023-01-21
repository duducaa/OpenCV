#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char ** argv) {

    string path = samples::findFile(argv[1]);

    Mat img = imread(path);
    if (img.empty()) return 1;

    Mat crop = img.rowRange(0, img.rows / 2).colRange(0, img.cols / 2);

    imshow("Display", crop);
    waitKey(0);

    return 0;

}