#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

void HSVTrackbars(string window);
vector<int> GetTrackValues(string window);
vector<Mat> detection(Mat frame, vector<int> lim, int area);

//255 95 101
int main(int argc, char ** argv) {

    VideoCapture cap;
    string path = "http://[2804:1ef8:2c84:6b00:1ecc:d6ff:fe44:df63]:5000/video";
    if (!cap.open(path)) return 1;

    //VideoCapture cap(0);
    //if (!cap.isOpened()) return 1;

    //int capWidth = 640;
    //int capHeight = 480;

    //cap.set(CAP_PROP_FRAME_WIDTH, capWidth);
    //cap.set(CAP_PROP_FRAME_HEIGHT, capHeight);

    string window = "Trackbars";
    namedWindow(window);
    HSVTrackbars(window);
    createTrackbar("Area", window, nullptr, 10000);

    int id_frame = 0;
    while (true) {
        Mat frame;
        cap >> frame;
        //flip(frame, frame, 1);

        int area = getTrackbarPos("Area", window);
        resize(frame, frame, Size(frame.cols * 0.4, frame.rows * 0.4));
        setTrackbarMax("Area", window, frame.cols * frame.rows);

        vector<int> lim = GetTrackValues(window);
        vector<Mat> res = detection(frame, lim, area);

        Mat output(frame.rows * 2, frame.cols, frame.type());
        vconcat(res, output);

        imshow("Display", output);
        if ((char) waitKey(1) == 32) {
            imwrite("../pictures_webcam/picture" + to_string(id_frame) + ".jpg", frame);
            id_frame++;
            cout << "Frame saved!" << endl;
        }
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

Mat GetMask(Mat frame, vector<int> lim) {

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

    return mask;

}

Point GetTextPoint(vector<Point> cnt) {

    Rect rect = boundingRect(cnt);
    return Point(rect.x + rect.width / 4, rect.y + rect.height / 2);

}

double GetRatio(vector<Point> cnt) {

    Rect rect = boundingRect(cnt);
    return rect.height / (double) rect.width;

}

string GetShape(vector<Point> cnt) {

    string shape = "-1";
    int vert = cnt.size();
    if (vert == 3) shape = "Triangle";
    if (vert == 4) {
        double ratio = GetRatio(cnt);
        if (abs(ratio - 1) <= 0.04) shape = "Square";
        else shape = "Rectangle";
    }
    if (vert == 8) {
        double ratio = GetRatio(cnt);
        if (abs(ratio - 1) <= 0.04) shape = "Circle";
        else shape = "Ellipse";
    }
    if (vert == 10 && !isContourConvex(cnt)) shape = "Star";

    if (shape == "-1") return "Some Shape";
    return shape;

}

vector<Mat> detection(Mat frame, vector<int> lim, int area) {

    Mat mask = GetMask(frame, lim);

    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    if (!contours.empty()) {
        int i = 0;
        for (vector<Point> cnt : contours) {
            if (contourArea(cnt) >= area) {
                double peri = arcLength(cnt, true);
                vector<Point> approx;
                approxPolyDP(cnt, approx, peri * 0.02, true);
                
                drawContours(frame, contours, i, Scalar(0, 255, 0), 3);
                
                string shape = GetShape(approx);
                Point pt = GetTextPoint(approx);
                putText(frame, shape, pt, FONT_HERSHEY_COMPLEX, 1, Scalar::all(0), 3);
                
                string areaText = "Area: " + to_string(contourArea(cnt));
                Point pt2 = Point(pt.x, pt.y + 50);
                putText(frame, areaText, pt2, FONT_HERSHEY_COMPLEX, 1, Scalar::all(0), 3); 
                
            }
            i++;
        }

        
    }

    merge((vector<Mat>) {mask, mask, mask}, mask);
    return {frame, mask};

}