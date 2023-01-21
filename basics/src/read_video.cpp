#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char ** argv) {

    VideoCapture cap(0);
    if (!cap.isOpened()) return 1;

    while (true) {
        Mat frame;
        cap >> frame;

        imshow("Display", frame);
        if ((char) waitKey(1) == 27) break;
    }

    destroyAllWindows();
    cap.release();

    return 0;

}