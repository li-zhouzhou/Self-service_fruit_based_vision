#pragma once
#include<opencv2/opencv.hpp>
#include <iostream>
#include<cmath>
using namespace std;
using namespace cv;
class myprocess
{
public:
    int process(Mat src,int weigth);
    int mysoble(Mat& src, Mat first, int weigth);
    int myfindcourours(Mat& src, Mat first, int weigth);
    void linefill();
    int OTSU(Mat src);
private:
    Mat src, src_gray, blur_gray, soble_x, soble_y, soble_image, canny_image;
    vector<vector<Point>>mycontours;
    vector<Vec4i>myhierancy;
    vector<int>myarea;
    int value = 0;
    int sum1=0, sum2=0, sum3=0;
};