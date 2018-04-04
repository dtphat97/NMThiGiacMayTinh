// demo.cpp : Defines the entry point for the console application.
//
#include "opencv2\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "stdafx.h"
#include <iostream>
using namespace std;
using namespace cv;

int main()
{
	Mat image;
	image = imread("image.jpg", IMREAD_COLOR);
	if (!image.data)
	{
		cout << "Khong the mo anh";
		return -1;
	}
	namedWindow("Display Window", WINDOW_AUTOSIZE);
	imshow("Display Window", image);
	waitKey(0);
	
    return 0; 

}

