// 1512390_BT01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;
// Hàm chuyển đổi ảnh màu sang ảnh trắng đen
Mat CoverColor(Mat image, Mat GrayImage)
{
	cvtColor(image, GrayImage, CV_BGR2GRAY);
	return GrayImage;
}
// Hàm thay đổi độ sáng của ảnh
Mat CoverBrightness(Mat image, Mat ImageBrightness, float value)
{
	ImageBrightness = Mat::zeros(image.size(), image.type());
	for (int y = 0; y < image.rows; y++)
	{
		for (int x = 0; x < image.cols; x++)
		{
			for (int i = 0; i < 3; i++)
			{
				ImageBrightness.at<Vec3b>(y, x)[i] = saturate_cast<uchar>(value + image.at<Vec3b>(y, x)[i]);
			}
		}
	}
	return ImageBrightness;
}
// Hàm thay đổi độ tương phản của ảnh
Mat CoverContrast(Mat image, Mat ImageContrast, float value)
{
	ImageContrast = Mat::zeros(image.size(), image.type());
	for (int y = 0; y < image.rows; y++)
	{
		for (int x = 0; x < image.cols; x++)
		{
			for (int i = 0; i < 3; i++)
			{
				ImageContrast.at<Vec3b>(y, x)[i] = saturate_cast<uchar>(value * image.at<Vec3b>(y, x)[i]);
			}
		}
	}
	return ImageContrast;
}
// Hàm kiểm tra chuỗi có bằng nhau không
bool compareString(char* ch1, const char* ch2)
{
	int SumCh1 = 1;
	int SumCh2 = 1;
	for (int i = 0; i < strlen(ch1); i++)
	{
		SumCh1 += i * ch1[i];
	}
	for (int i = 0; i < strlen(ch2); i++)
	{
		SumCh2 += i * ch2[i];
	}
	if (SumCh1 == SumCh2)
	{
		return true;
	}
	return false;
}
int main(int argc, char **argv)
{
	Mat image;
	image = imread(argv[2], IMREAD_COLOR);
	if (!image.data)
	{
		cout << "Khong the mo anh" << endl;
		return -1;
	}
	// chuyen doi RGB ->Gray
	Mat GrayImage;


	//brightness
	Mat ImageBrightness;

	//contrast
	Mat ImageContrast;
	namedWindow("Display Window", WINDOW_AUTOSIZE);
	if (compareString(argv[1], "CoverImage"))
	{
		imshow("Display Window", CoverColor(image, GrayImage));
	}
	if (compareString(argv[1], "Brightness"))
	{
		imshow("Display Window", CoverBrightness(image, ImageBrightness, stoi(argv[3])));
	}
	if (compareString(argv[1], "Contrast"))
	{
		imshow("Display Window", CoverContrast(image, ImageContrast, stoi(argv[3])));
	}
	waitKey(0);
	return 0;
}

