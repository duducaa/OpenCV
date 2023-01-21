#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char ** argv) {

    string path = samples::findFile(argv[1]);

    Mat img = imread(path);
    if (img.empty()) return 1;

    imshow("Display", img);
    waitKey(0);

    return 0;

}