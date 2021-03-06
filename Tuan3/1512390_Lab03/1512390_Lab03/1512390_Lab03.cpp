// 1512390_Lab03.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

/// Global variables
Mat src, src_gray, dst;
int thresh = 200;
int max_thresh = 255;

const char* source_window = "Source Image";
const char* corners_window = "Corner Image";


/// Function header
void cornerHarris_demo(int, void*);
void cornerHarrisMe(int, int, double);
int xGradient(Mat, int, int);
int yGradient(Mat, int, int);
void SobelMe(Mat&, Mat&, int, int);
int borderCheck(int M, int x);
void SepGaussian(Mat&, Mat&, int, int);


/** @function main */
int main(int argc, char** argv)
{
	/// Load source image and convert it to gray
	src = imread("image2.jpg", 1);
	//Mat src_gray(src.size(), CV_8UC1);
	cvtColor(src, src_gray, CV_BGR2GRAY);

	/// Create a window and a trackbar
	namedWindow(source_window, CV_WINDOW_AUTOSIZE);
	createTrackbar("Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo);
	imshow(source_window, src);

	cornerHarris_demo(0, 0);

	waitKey(0);
	return(0);
}

/** @function cornerHarris_demo */
void cornerHarris_demo(int, void*)
{
	Mat dst_norm, dst_norm_scaled;

	/// Detector parameters
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;

	/// Detecting corners
	cornerHarrisMe(blockSize, apertureSize, k);
	/// Normalizing
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, dst_norm_scaled);

	/// Drawing a circle around corners
	for (int j = 0; j < dst_norm.rows; j++)
	{
		for (int i = 0; i < dst_norm.cols; i++)
		{
			if ((int)dst_norm.at<float>(j, i) > thresh)
			{
				circle(dst_norm_scaled, Point(i, j), 5, Scalar(255), 2, 8, 0);
			}
		}
	}
	/// Showing the result
	namedWindow(corners_window, CV_WINDOW_AUTOSIZE);
	imshow(corners_window, dst_norm_scaled);
}

void cornerHarrisMe(int blockSize, int apertureSize, double k)
{
	Mat x2y2, xy, mtrace, x_der, y_der, x2_der, y2_der, xy_der, x2g_der, y2g_der, xyg_der;

	//1: calculate x and y derivative of image via Sobel
	SobelMe(src_gray, x_der, 1, 0);
	SobelMe(src_gray, y_der, 0, 1);
	//2: calculate other three images in M
	pow(x_der, blockSize, x2_der);
	pow(y_der, blockSize, y2_der);
	multiply(x_der, y_der, xy_der);
	//3: gaussain
	SepGaussian(x2_der, x2g_der, 1, 0);
	SepGaussian(y2_der, y2g_der, 0, 1);
	SepGaussian(xy_der, xyg_der, 1, 1);
	//4. calculating R with k
	multiply(x2g_der, y2g_der, x2y2);
	multiply(xyg_der, xyg_der, xy);
	pow((x2g_der + y2g_der), blockSize, mtrace);
	dst = (x2y2 - xy) - k * mtrace;
}

// gradient in the x direction
int xGradient(Mat image, int x, int y)
{
	return image.at<uchar>(y - 1, x - 1) +
		2 * image.at<uchar>(y, x - 1) +
		image.at<uchar>(y + 1, x - 1) -
		image.at<uchar>(y - 1, x + 1) -
		2 * image.at<uchar>(y, x + 1) -
		image.at<uchar>(y + 1, x + 1);
}

// gradient in the y direction
int yGradient(Mat image, int x, int y)
{
	return image.at<uchar>(y - 1, x - 1) +
		2 * image.at<uchar>(y - 1, x) +
		image.at<uchar>(y - 1, x + 1) -
		image.at<uchar>(y + 1, x - 1) -
		2 * image.at<uchar>(y + 1, x) -
		image.at<uchar>(y + 1, x + 1);
}

void SobelMe(Mat& source, Mat& destination, int xOrder, int yOrder) {
	int gradX, gradY, sum;

	destination = source.clone();

	if (xOrder == 1 && yOrder == 0) {
		for (int y = 1; y < source.rows - 1; y++) {
			for (int x = 1; x < source.cols - 1; x++) {
				gradX = xGradient(source, x, y);
				sum = abs(gradX);
				sum = sum > 255 ? 255 : sum;
				sum = sum < 0 ? 0 : sum;
				destination.at<uchar>(y, x) = sum;
			}
		}
	}
	else if (xOrder == 0 && yOrder == 1) {
		for (int y = 1; y < source.rows - 1; y++) {
			for (int x = 1; x < source.cols - 1; x++) {
				gradY = yGradient(source, x, y);
				sum = abs(gradY);
				sum = sum > 255 ? 255 : sum;
				sum = sum < 0 ? 0 : sum;
				destination.at<uchar>(y, x) = sum;
			}
		}
	}
	else if (xOrder == 1 && yOrder == 1)
		for (int y = 1; y < source.rows - 1; y++) {
			for (int x = 1; x < source.cols - 1; x++) {
				gradX = xGradient(source, x, y);
				gradY = yGradient(source, x, y);
				sum = abs(gradX) + abs(gradY);
				sum = sum > 255 ? 255 : sum;
				sum = sum < 0 ? 0 : sum;
				destination.at<uchar>(y, x) = sum;
			}
		}
}

int borderCheck(int M, int x) {
	if (x < 0)
		return -x - 1;
	if (x >= M)
		return 2 * M - x - 1;
	return x;
}

void SepGaussian(Mat& source, Mat& desination, int sigmaX, int sigmaY) {
	// coefficients of 1D gaussian kernel with sigma = 1
	double coeffs[] = { 0.0545, 0.2442, 0.4026, 0.2442, 0.0545 };
	Mat tempX, tempY;
	float sum, x1, y1;

	desination = source.clone();
	tempY = source.clone();
	tempX = source.clone();

	// along y - direction
	if (sigmaX == 0 && sigmaY == 1) {
		for (int y = 0; y < source.rows; y++) {
			for (int x = 0; x < source.cols; x++) {
				sum = 0.0;
				for (int i = -2; i <= 2; i++) {
					y1 = borderCheck(source.rows, y - i);
					sum = sum + coeffs[i + 2] * source.at<uchar>(y1, x);
				}
				desination.at<uchar>(y, x) = sum;
			}
		}
	}

	// along x - direction
	else if (sigmaX == 1 && sigmaY == 0) {
		for (int y = 0; y < source.rows; y++) {
			for (int x = 0; x < source.cols; x++) {
				sum = 0.0;
				for (int i = -2; i <= 2; i++) {
					x1 = borderCheck(source.cols, x - i);
					sum = sum + coeffs[i + 2] * source.at<uchar>(y, x1);
				}
				desination.at<uchar>(y, x) = sum;
			}
		}
	}

	// along xy - direction
	else if (sigmaX == 1 && sigmaY == 1) {
		for (int y = 0; y < source.rows; y++) {
			for (int x = 0; x < source.cols; x++) {
				sum = 0.0;
				for (int i = -2; i <= 2; i++) {
					y1 = borderCheck(source.rows, y - i);
					sum = sum + coeffs[i + 2] * source.at<uchar>(y1, x);
				}
				tempY.at<uchar>(y, x) = sum;
			}
		}
		for (int y = 0; y < source.rows; y++) {
			for (int x = 0; x < source.cols; x++) {
				sum = 0.0;
				for (int i = -2; i <= 2; i++) {
					x1 = borderCheck(source.cols, x - i);
					sum = sum + coeffs[i + 2] * tempY.at<uchar>(y, x1);
				}
				desination.at<uchar>(y, x) = sum;
			}
		}

	}
}