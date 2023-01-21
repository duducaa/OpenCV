#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

void HSVTrackbars(string window);
vector<int> GetTrackValues(string window);
vector<Mat> detection(Mat frame, vector<int> lim);

int main(int argc, char ** argv) {

    VideoCapture cap;
    string path = "http://[2804:1ef8:2c84:6b00:1ecc:d6ff:fe44:df63]:5000/video";
    if (!cap.open(path)) return 1;

    //VideoCapture cap(0);
    //if (!cap.isOpened()) return 1;

    //double capWidth = 640;
    //double capHeight = 480;

    //cap.set(CAP_PROP_FRAME_WIDTH, capWidth);
    //cap.set(CAP_PROP_FRAME_HEIGHT, capHeight);

    string window = "Trackbars";
    namedWindow(window);
    HSVTrackbars(window);
    createTrackbar("Area", window, nullptr, 1);
    createTrackbar("Window Size", window, nullptr, 10);
    setTrackbarMin("Window Size", window, 1);
    setTrackbarPos("Window Size", window, 5);

    while (true) {
        Mat frame;
        cap >> frame;
        //flip(frame, frame, 1);

        int wSize = getTrackbarPos("Window Size", window);
        resize(frame, frame, Size(frame.cols * 0.4, frame.rows * 0.4));
        setTrackbarMax("Area", window, frame.cols * frame.rows);

        vector<int> lim = GetTrackValues(window);
        vector<Mat> res = detection(frame, lim);

        Mat output(frame.rows * 2, frame.cols, frame.type());
        vconcat(res, output);

        imshow("Display", output);
        if ((char) waitKey(1) == 27) break;
    }

    destroyAllWindows();
    cap.release();

    return 0;

}

void HSVTrackbars(string window) {

    createTrackbar("Hue Min", window, nullptr, 255);
    createTrackbar("Hue Max", window, nullptr, 255);
    setTrackbarPos("Hue Max", window, 255);

    createTrackbar("Saturation Min", window, nullptr, 255);
    createTrackbar("Saturation Max", window, nullptr, 255);
    setTrackbarPos("Saturation Max", window, 255);

    createTrackbar("Value Min", window, nullptr, 255);
    createTrackbar("Value Max", window, nullptr, 255);
    setTrackbarPos("Value Max", window, 255);

}

vector<int> GetTrackValues(string window) {

    vector<int> lim;

    lim.push_back(getTrackbarPos("Hue Min", window));
    lim.push_back(getTrackbarPos("Saturation Min", window));
    lim.push_back(getTrackbarPos("Value Min", window));
    lim.push_back(getTrackbarPos("Hue Max", window));
    lim.push_back(getTrackbarPos("Saturation Max", window));
    lim.push_back(getTrackbarPos("Value Max", window));

    if (lim[0] > lim[3]) setTrackbarPos("Hue Max", window, lim[0]);
    if (lim[1] > lim[4]) setTrackbarPos("Saturation Max", window, lim[1]);
    if (lim[2] > lim[5]) setTrackbarPos("Value Max", window, lim[2]);    

    return lim;

}

vector<Mat> detection(Mat frame, vector<int> lim) {

    Mat hsv;
    cvtColor(frame, hsv, COLOR_BGR2HSV);

    Mat blur;
    GaussianBlur(hsv, blur, Size(3, 3), 0);

    vector<int> lower, upper;
    for (int i = 0; i < 3; i++) {
        lower.push_back(lim[i]);
        upper.push_back(lim[i + 3]);
    }

    Mat mask;
    inRange(hsv, lower, upper, mask);

    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    if (!contours.empty()) {
        double maxArea = contourArea(contours[0]);
        int maxId = 0;
        int i = 0;
        for (vector<Point> cnt : contours) {
            if (maxArea < contourArea(cnt)) {
                maxArea = contourArea(cnt);
                maxId = i;
            }
            i++;
        }

        drawContours(frame, contours, maxId, Scalar(0, 255, 0), 3);
    }

    merge((vector<Mat>) {mask, mask, mask}, mask);
    return {frame, mask};

}