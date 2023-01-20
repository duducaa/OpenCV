#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

void Trackbars(string window, Mat img);
vector<int> GetTrackValues(string window, Mat img);
Mat GetMask(Mat img, vector<int> values);

int main(int argc, char ** argv) {

    string path = samples::findFile(argv[1]);

    Mat img = imread(path);
    if (img.empty()) return 1;

    string window = "Display";
    namedWindow(window);
    Trackbars(window, img);

    while (true) {
        Mat clone = img.clone();

        int ksize = getTrackbarPos("Blur Intensity", window);
        if (ksize % 2 == 0) ksize--;

        vector<int> values = GetTrackValues(window, clone);
        Mat mask = GetMask(clone, values);

        Mat piece, out;
        bitwise_and(clone, clone, piece, mask);
        bitwise_xor(clone, piece, out);
        GaussianBlur(piece, piece, Size(ksize, ksize), 0);

        Mat output;
        add(piece, out, output);

        imshow(window, output);
        if ((char) waitKey(1) == 27) break;
    }

    destroyAllWindows();

    return 0;

}

void Trackbars(string window, Mat img) {

    createTrackbar("Blur Intensity", window, nullptr, 13);
    setTrackbarMin("Blur Intensity", window, 1);
    createTrackbar("X axis", window, nullptr, img.cols);
    createTrackbar("Y axis", window, nullptr, img.rows);

    if (img.cols < img.rows) {
        createTrackbar("Radius", window, nullptr, img.cols / 2);
    }
    else createTrackbar("Radius", window, nullptr, img.rows / 2);

}

vector<int> GetTrackValues(string window, Mat img) {

    vector<int> values;

    int axisX = getTrackbarPos("X axis", window);
    int axisY = getTrackbarPos("Y axis", window);
    int radius = getTrackbarPos("Radius", window);

    values.push_back(axisX);
    values.push_back(axisY);
    values.push_back(radius);

    if (axisX + radius > img.cols) {
        setTrackbarPos("X axis", window, img.cols - axisX);
    }

    if (axisX - radius < 0) {
        setTrackbarPos("X axis", window, radius);
    }

    if (axisY + radius > img.rows) {
        setTrackbarPos("Y axis", window, img.rows - axisY);
    }

    if (axisY - radius < 0) {
        setTrackbarPos("Y axis", window, radius);
    }

    return values;

}

Mat GetMask(Mat img, vector<int> values) {

    Mat mask = Mat::zeros(img.size(), CV_8UC1);

    circle(mask, Point(values[0], values[1]), values[2], 255, -1);

    return mask;

}