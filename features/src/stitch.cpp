#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char ** argv) {

    vector<Mat> imgs;
    for (int i = 1; i < argc; i++) {
        string path = samples::findFile(argv[i]);
        Mat img = imread(path);
        if (img.empty()) {
            cout << "Erro!" << endl;
            return 1;
        }
        imgs.push_back(img);
    }

    Mat output;
    Ptr<Stitcher> stitcher = Stitcher::create(Stitcher::PANORAMA);
    Stitcher::Status status = stitcher -> stitch(imgs, output);


    imshow("Display", output);
    waitKey(0);

    return 0;

}