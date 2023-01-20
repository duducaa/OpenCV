#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char ** argv) {

    string obj_path = samples::findFile(argv[1]);
    string train_path = samples::findFile(argv[2]);

    Mat obj = imread(obj_path);
    Mat train = imread(train_path);
    if (obj.empty() || train.empty()) return 1;

    Mat obj_gray, train_gray;
    cvtColor(obj, obj_gray, COLOR_BGR2GRAY);
    cvtColor(train, train_gray, COLOR_BGR2GRAY);

    vector<KeyPoint> obj_kp, train_kp;
    Mat obj_desc, train_desc;

    Ptr<SIFT> detector = SIFT::create();
    detector -> detectAndCompute(obj_gray, noArray(), obj_kp, obj_desc);
    detector -> detectAndCompute(train_gray, noArray(), train_kp, train_desc);

    vector<vector<DMatch>> matches;

    Ptr<BFMatcher> matcher = BFMatcher::create();
    matcher -> knnMatch(obj_desc, train_desc, matches, 2);

    vector<DMatch> good_matches;
    float ratio = 0.75f;
    for (vector<DMatch> match : matches) {
        if (match[0].distance < ratio * match[1].distance) {
            good_matches.push_back(match[0]);
        }
    }

    vector<Point2f> obj_pts, train_pts;
    for (DMatch match : good_matches) {
        obj_pts.push_back(obj_kp[match.queryIdx].pt);
        train_pts.push_back(train_kp[match.trainIdx].pt);
    }

    Mat kernel = findHomography(obj_pts, train_pts, RANSAC);

    vector<Point2f> obj_vert = {
        Point2f(0, 0), Point2f(0, obj.rows), Point2f(obj.cols, obj.rows),
        Point2f(obj.cols, 0)
    };
    vector<Point2f> train_vert;

    perspectiveTransform(obj_vert, train_vert, kernel);

    double perc = 0.2;
    resize(train, train, Size(train.cols * perc, train.rows * perc));
    for (int i = 0; i < train_vert.size(); i++) {
        line(train, train_vert[i] * perc, train_vert[(i + 1) % 4] * perc, 
        Scalar(0, 255, 0), 3);
    }
    
    imshow("Display", train);
    waitKey(0);

    return 0;

}