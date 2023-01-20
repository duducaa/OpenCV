#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

Point GetTextPoint(vector<Point> cnt);
string GetShape(vector<Point> cnt);

int main(int argc, char ** argv) {

    string path = samples::findFile(argv[1]);

    Mat img = imread(path);
    if (img.empty()) return 1;

    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);

    Mat blur;
    GaussianBlur(gray, blur, Size(3, 3), 0);

    Mat canny;
    Canny(blur, canny, 226, 255);

    vector<vector<Point>> contours;
    findContours(canny, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    if (!contours.empty()) {
        for (vector<Point> cnt : contours) {
            double peri = arcLength(cnt, true);
            vector<Point> approx;
            approxPolyDP(cnt, approx, peri * 0.02, true);

            string shape = GetShape(approx);
            Point pt = GetTextPoint(approx);

            putText(img, shape, pt, FONT_HERSHEY_COMPLEX, 1, Scalar::all(0), 3);
            
        }
    }

    imshow("Display", img);
    waitKey(0);
    
    return 0;

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