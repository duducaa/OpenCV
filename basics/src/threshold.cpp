#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char ** argv) {

    string path = samples::findFile(argv[1]);

    Mat img = imread(path);
    if (img.empty()) return 1;

    resize(img, img, Size(img.cols * 0.6, img.rows * 0.6));

    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);

    Mat blur;
    GaussianBlur(gray, blur, Size(3, 3), 0);

    string window = "Threshold";
    namedWindow(window);
    createTrackbar("Threshold", window, nullptr, 255);
    createTrackbar("Max Value", window, nullptr, 255);
    setTrackbarPos("Max Value", window, 255);

    while (true) {
        int threshValue = getTrackbarPos("Threshold", window);
        int maxValue = getTrackbarPos("Max Value", window);
        if (threshValue > maxValue) {
            setTrackbarPos("Max Value", window, threshValue);
        }

        Mat output(blur.rows * 2, blur.cols * 3, blur.type());
        Mat l1(blur.rows, blur.cols * 3, blur.type());
        Mat l2(blur.rows, blur.cols * 3, blur.type());        

        vector<Mat> imgs(6);
        imgs[0] = img;
        for (int i = 1; i < 6; i++) {
            threshold(blur, imgs[i], threshValue, maxValue, i - 1);
            merge((vector<Mat>) {imgs[i], imgs[i], imgs[i]}, imgs[i]);
        }

        hconcat((vector<Mat>) {imgs[0], imgs[1], imgs[2]}, l1);
        hconcat((vector<Mat>) {imgs[3], imgs[4], imgs[5]}, l2);
        vconcat((vector<Mat>) {l1, l2}, output);

        imshow(window, output);
        if ((char) waitKey(1) == 27) break;
    }

    destroyAllWindows();

    return 0;

}