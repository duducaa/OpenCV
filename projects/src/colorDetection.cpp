#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

void Trackbars(string window);
vector<int> GetTrackValues(string window);
vector<Mat> detection(Mat frame, vector<int> lim);

int main(int argc, char ** argv) {

    //VideoCapture cap(0);
    //if (!cap.isOpened()) return 1;

    //cap.set(CAP_PROP_FRAME_WIDTH, 640);
    //cap.set(CAP_PROP_FRAME_HEIGHT, 480);
    string path = samples::findFile(argv[1]);

    Mat img = imread(path);
    if (img.empty()) return 1;

    double perc = 0.5;
    resize(img, img, Size(img.cols * perc, img.rows * perc));

    string window = "Trackbars";
    namedWindow(window);
    Trackbars(window);

    Mat mask;
    while (true) {
        //Mat frame;
        //cap >> frame;
        //flip(frame, frame, 1);
        Mat clone = img.clone();

        vector<int> lim = GetTrackValues(window);
        vector<Mat> res = detection(clone, lim);
        mask = res[1];

        Mat output(clone.rows * 2, clone.cols, clone.type());
        vconcat(res, output);

        imshow("Display", output);
        if ((char) waitKey(1) == 27) break;
    }

    Mat kernel = Mat::ones(Size(3, 3), CV_32F);
    morphologyEx(mask, mask, MORPH_OPEN, kernel, Point(-1, -1), 6);

    imshow("Display", mask);
    waitKey(0);

    destroyAllWindows();
    //cap.release();

    return 0;

}

void Trackbars(string window) {

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