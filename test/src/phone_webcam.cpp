#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char ** argv) {

    VideoCapture cap;

    string path = "https://10.0.0.165:5000/video";
    if (!cap.open(path)) return 1;

    while (true) {
        Mat frame;
        cap >> frame;
        resize(frame, frame, Size(frame.cols * 0.5, frame.rows * 0.5));

        imshow("Display", frame);
        if ((char) waitKey(1) == 27) break;
    }

    destroyAllWindows();
    cap.release();

    return 0;

}