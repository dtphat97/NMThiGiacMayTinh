// 1512390_Lab02.cpp : Defines the entry point for the console application.
//

#include"stdafx.h"
#include"opencv2\core.hpp"
#include"opencv2\highgui\highgui.hpp"
#include"opencv2\imgproc\imgproc.hpp"
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
using namespace cv;
//cau 1
Mat rgbToGray(Mat srcImg, Mat dstImg)
{
	dstImg = Mat::zeros(srcImg.size(), srcImg.type());
	if (srcImg.type() == IMREAD_GRAYSCALE)
	{
		cout << "Chi thuc hien duoc voi anh mau";
		return srcImg;
	}
	for (int y = 0; y < srcImg.rows; y++)
	{
		for (int x = 0; x < srcImg.cols; x++)
		{
			dstImg.at<Vec3b>(y, x)[0] = saturate_cast<uchar>((srcImg.at<Vec3b>)(y, x)[0] * 0.299 + (srcImg.at<Vec3b>)(y, x)[1] * 0.587 + (srcImg.at<Vec3b>)(y, x)[2] * 0.114);
			dstImg.at<Vec3b>(y, x)[1] = saturate_cast<uchar>((srcImg.at<Vec3b>)(y, x)[0] * 0.299 + (srcImg.at<Vec3b>)(y, x)[1] * 0.587 + (srcImg.at<Vec3b>)(y, x)[2] * 0.114);
			dstImg.at<Vec3b>(y, x)[2] = saturate_cast<uchar>((srcImg.at<Vec3b>)(y, x)[0] * 0.299 + (srcImg.at<Vec3b>)(y, x)[1] * 0.587 + (srcImg.at<Vec3b>)(y, x)[2] * 0.114);

		}
	}
	return dstImg;


}
//cau 2
int calcHistogram(Mat srcImg, vector<double>&histB, vector<double>&histG, vector<double>&histR)
{
	double B, G, R;
	int *arrB, *arrG, *arrR;
	arrB = new int[256];
	arrG = new int[256];
	arrR = new int[256];
	for (int i = 0; i < 256; i++)
	{
		arrB[i] = 0;
		arrG[i] = 0;
		arrR[i] = 0;

	}


	for (int y = 0; y < srcImg.rows; y++)
	{
		for (int x = 0; x < srcImg.cols; x++)
		{
			B = saturate_cast<uchar>((srcImg.at<Vec3b>)(y, x)[0]);
			G = saturate_cast<uchar>((srcImg.at<Vec3b>)(y, x)[1]);
			R = saturate_cast<uchar>((srcImg.at<Vec3b>)(y, x)[2]);
			arrB[int(B)]++;
			arrG[int(G)]++;
			arrR[int(R)]++;
		}
	}
	for (int i = 0; i < 256; i++)
	{
		histB.push_back(arrB[i]);
		histG.push_back(arrG[i]);
		histR.push_back(arrR[i]);

	}




	return 1;

}
//cau 3
double compareHist(Mat srcImg1, Mat srcImg2)
{
	vector<double> histA1, histA2, histA3;
	vector<double> histB1, histB2, histB3;
	calcHistogram(srcImg1, histA1, histA2, histA3);
	calcHistogram(srcImg2, histB1, histB2, histB3);
	double Dtu = 0, Dmau = 0, D;
	for (int i = 0; i < 256; i++)
	{
		Dtu += min(double(histA1[i] + histA2[i] + histA3[i]) / 3, (double)(histB1[i] + histB2[i] + histB3[i]) / 3);
		Dmau += double(histA1[i] + histA2[i] + histA3[i]) / 3;
	}


	return Dtu / Dmau;
}
//cau 4
double calcHistogram(Mat srcImg, Mat &dstImg, int rBin, int gBin, int bBin, vector<double>&histA, vector<double>&histB, vector<double>&histC)
{
	int B, G, R;

	dstImg = Mat::zeros(srcImg.size(), srcImg.type());
	int tmp;
	for (int y = 0; y < srcImg.rows; y++)
	{
		//int dem = 0;
		for (int x = 0; x < srcImg.cols; x++)
		{
			B = saturate_cast<uchar>((srcImg.at<Vec3b>)(y, x)[0]);
			G = saturate_cast<uchar>((srcImg.at<Vec3b>)(y, x)[1]);
			R = saturate_cast<uchar>((srcImg.at<Vec3b>)(y, x)[2]);

			if (B % bBin != 0) {
				tmp = B % bBin;
				//cout << tmp << endl;
				B += -tmp;
			}
			if (G % gBin != 0) {
				tmp = G % gBin;
				//	cout << gBin << endl;
				//cout << G << endl;
				G += -tmp;
				//	cout<< G << endl;
			}
			if (R % rBin != 0) {
				tmp = R % rBin;
				R += -tmp;
				//	cout << tmp << endl;
			}
			dstImg.at<Vec3b>(y, x)[0] = B;
			dstImg.at<Vec3b>(y, x)[1] = G;
			dstImg.at<Vec3b>(y, x)[2] = R;

			//dem++;
			//	cout << dem << endl;
		}

	}

	double Bcolor, Gcolor, Rcolor;
	int *arrB, *arrG, *arrR;
	arrB = new int[256];
	arrG = new int[256];
	arrR = new int[256];
	for (int i = 0; i < 256; i++)
	{
		arrB[i] = 0;
		arrG[i] = 0;
		arrR[i] = 0;

	}
	for (int y = 0; y < srcImg.rows; y++)
	{
		for (int x = 0; x < srcImg.cols; x++)
		{
			Bcolor = saturate_cast<uchar>((dstImg.at<Vec3b>)(y, x)[0]);
			Gcolor = saturate_cast<uchar>((dstImg.at<Vec3b>)(y, x)[1]);
			Rcolor = saturate_cast<uchar>((dstImg.at<Vec3b>)(y, x)[2]);
			arrB[int(Bcolor)]++;
			arrG[int(Gcolor)]++;
			arrR[int(Rcolor)]++;
		}
	}
	for (int i = 0; i < 256; i++)
	{
		histA.push_back(arrB[i]);
		histB.push_back(arrG[i]);
		histC.push_back(arrR[i]);

	}


	return 1;
}
//cau 5
double* calcHistogram(Mat srcImg, Mat &dstImg, int gBin)
{
	static double r[256];
	for (int i = 0; i < 256; i++)
	{
		r[i] = 0;
	}
	dstImg = Mat::zeros(srcImg.size(), srcImg.type());
	int tmp;
	int flag = 0;
	int G;
	for (int y = 0; y < srcImg.rows; y++)
	{
		for (int x = 0; x < srcImg.cols; x++)
		{
			G = saturate_cast<uchar>((srcImg.at<uchar>)(y, x));
			if (G % gBin != 0) {
				tmp = G % gBin;
				G += -tmp;

			}
			dstImg.at<uchar>(y, x) = G;

		}
	}

	for (int y = 0; y < dstImg.rows; y++)
	{
		for (int x = 0; x < dstImg.cols; x++)
		{
			G = saturate_cast<uchar>((dstImg.at<uchar>)(y, x));
			r[int(G)]++;

		}

	}
	// for (int i = 0; i < 256; i++)
	// {
	//	 cout << r[i] << "\t";

	// }
	return r;
}
//cau 6
double compareHistNol(Mat srcImg1, Mat srcImg2, int rBin, int gBin, int bBin)
{
	Mat dstImg1, dstImg2;
	vector<double> histB1, histG1, histR1, histB2, histG2, histR2;
	calcHistogram(srcImg1, dstImg1, rBin, gBin, bBin, histB1, histG1, histR1);
	calcHistogram(srcImg2, dstImg2, rBin, gBin, bBin, histB2, histG2, histR2);
	double Dtu = 0, Dmau = 0, D;

	for (int i = 0; i < 256; i++)
	{
		Dtu += min(double(histB1[i] + histG1[i] + histR1[i]) / 3, (double)(histB2[i] + histG2[i] + histR2[i]) / 3);
		Dmau += double(histB2[i] + histG2[i] + histR2[i]) / 3;
		//cout << Dtu<<"-------"<<histB1[i] << "--------" << histG2[i] << endl;

	}

	return Dtu / Dmau;
}
//cau 7
double compareHisXam(Mat srcImg1, Mat srcImg2, int gBin)
{
	double *matrixSrc1, *matrixSrc2;
	double matrixSrc1Copy[256];
	Mat dstImg1, dstImg2;
	matrixSrc1 = calcHistogram(srcImg1, dstImg1, gBin);

	for (int i = 0; i < 256; i++)
	{
		matrixSrc1Copy[i] = matrixSrc1[i];
	}
	matrixSrc2 = calcHistogram(srcImg2, dstImg2, gBin);



	double Dtu = 0, Dmau = 0, D;
	for (int i = 0; i < 256; i++)
	{
		Dtu += min((double)(matrixSrc1Copy[i]), (double)(matrixSrc2[i]));
		Dmau += double(matrixSrc1Copy[i]);
		//		cout << Dtu << " ---" << Dmau << endl;
	}


	return (double)Dtu / Dmau;


}
//cau 8
Mat xGradient(Mat img)
{
	Mat dstImg = Mat::zeros(img.size(), img.type());
	int pixel_x;
	int value;
	int sobel_x[3][3];

	sobel_x[0][0] = -1;
	sobel_x[0][1] = 0;
	sobel_x[0][2] = 1;
	sobel_x[1][0] = -2;
	sobel_x[1][1] = 0;
	sobel_x[1][2] = 2;
	sobel_x[2][0] = -1;
	sobel_x[2][1] = 0;
	sobel_x[2][2] = 1;


	int dem = 0;
	for (int y = 1; y < img.rows; y++)
	{

		for (int x = 1; x < img.cols; x++)
		{
			if (x == 0 || x == img.cols - 1 || y == 0 || y == img.rows - 1)
			{
				dstImg.at<uchar>(y, x) = 0;

			}
			else
			{
				pixel_x = (sobel_x[0][0] * img.at<uchar>(y - 1, x - 1)) + (sobel_x[0][1] * img.at<uchar>(y - 1, x)) + (sobel_x[0][2] * img.at<uchar>(y - 1, x + 1)) +
					(sobel_x[1][0] * img.at<uchar>(y, x - 1)) + (sobel_x[1][1] * img.at<uchar>(y, x)) + (sobel_x[1][2] * img.at<uchar>(y, x + 1)) +
					(sobel_x[2][0] * img.at<uchar>(y + 1, x - 1)) + (sobel_x[2][1] * img.at<uchar>(y + 1, x)) + (sobel_x[2][2] * img.at<uchar>(y + 1, x + 1));


				value = sqrt(pixel_x*pixel_x);
				//if (value > 255 && value < 0) value = 0;
				dem++;
				//	cout << value << "\t";
				//	cout << dem <<"-"<< y << endl;
				dstImg.at<uchar>(y, x) = value;
			}


		}
	}
	return dstImg;
}
//cau 9
Mat yGradient(Mat img)
{
	Mat dstImg = Mat::zeros(img.size(), img.type());
	int pixel_y;
	int value;

	int sobel_y[3][3];


	sobel_y[0][0] = -1;
	sobel_y[0][1] = -2;
	sobel_y[0][2] = -1;
	sobel_y[1][0] = 0;
	sobel_y[1][1] = 0;
	sobel_y[1][2] = 0;
	sobel_y[2][0] = 1;
	sobel_y[2][1] = 2;
	sobel_y[2][2] = 1;
	int dem = 0;
	for (int y = 1; y < img.rows; y++)
	{

		for (int x = 1; x < img.cols; x++)
		{
			if (x == 0 || x == img.cols - 1 || y == 0 || y == img.rows - 1)
			{
				dstImg.at<uchar>(y, x) = 0;

			}
			else
			{


				pixel_y = (sobel_y[0][0] * img.at<uchar>(y - 1, x - 1)) + (sobel_y[0][1] * img.at<uchar>(y - 1, x)) + (sobel_y[0][2] * img.at<uchar>(y - 1, x + 1)) +
					(sobel_y[1][0] * img.at<uchar>(y, x - 1)) + (sobel_y[1][1] * img.at<uchar>(y, x)) + (sobel_y[1][2] * img.at<uchar>(y, x + 1)) +
					(sobel_y[2][0] * img.at<uchar>(y + 1, x - 1)) + (sobel_y[2][1] * img.at<uchar>(y + 1, x)) + (sobel_y[2][2] * img.at<uchar>(y + 1, x + 1));
				value = sqrt(pixel_y *pixel_y);
				//if (value > 255 && value < 0) value = 0;
				dem++;
				//	cout << value << "\t";
				//	cout << dem <<"-"<< y << endl;
				dstImg.at<uchar>(y, x) = value;
			}


		}
	}
	return dstImg;
}
// cau 10
Mat magnitude(Mat img)
{

	Mat dstImg = Mat::zeros(img.size(), img.type());
	int pixel_x, pixel_y;
	int value;
	int sobel_x[3][3];
	int sobel_y[3][3];
	sobel_x[0][0] = -1;
	sobel_x[0][1] = 0;
	sobel_x[0][2] = 1;
	sobel_x[1][0] = -2;
	sobel_x[1][1] = 0;
	sobel_x[1][2] = 2;
	sobel_x[2][0] = -1;
	sobel_x[2][1] = 0;
	sobel_x[2][2] = 1;

	sobel_y[0][0] = -1;
	sobel_y[0][1] = -2;
	sobel_y[0][2] = -1;
	sobel_y[1][0] = 0;
	sobel_y[1][1] = 0;
	sobel_y[1][2] = 0;
	sobel_y[2][0] = 1;
	sobel_y[2][1] = 2;
	sobel_y[2][2] = 1;
	int dem = 0;
	for (int y = 1; y < img.rows; y++)
	{

		for (int x = 1; x < img.cols; x++)
		{
			if (x == 0 || x == img.cols - 1 || y == 0 || y == img.rows - 1)
			{
				dstImg.at<uchar>(y, x) = 0;

			}
			else
			{
				pixel_x = (sobel_x[0][0] * img.at<uchar>(y - 1, x - 1)) + (sobel_x[0][1] * img.at<uchar>(y - 1, x)) + (sobel_x[0][2] * img.at<uchar>(y - 1, x + 1)) +
					(sobel_x[1][0] * img.at<uchar>(y, x - 1)) + (sobel_x[1][1] * img.at<uchar>(y, x)) + (sobel_x[1][2] * img.at<uchar>(y, x + 1)) +
					(sobel_x[2][0] * img.at<uchar>(y + 1, x - 1)) + (sobel_x[2][1] * img.at<uchar>(y + 1, x)) + (sobel_x[2][2] * img.at<uchar>(y + 1, x + 1));

				pixel_y = (sobel_y[0][0] * img.at<uchar>(y - 1, x - 1)) + (sobel_y[0][1] * img.at<uchar>(y - 1, x)) + (sobel_y[0][2] * img.at<uchar>(y - 1, x + 1)) +
					(sobel_y[1][0] * img.at<uchar>(y, x - 1)) + (sobel_y[1][1] * img.at<uchar>(y, x)) + (sobel_y[1][2] * img.at<uchar>(y, x + 1)) +
					(sobel_y[2][0] * img.at<uchar>(y + 1, x - 1)) + (sobel_y[2][1] * img.at<uchar>(y + 1, x)) + (sobel_y[2][2] * img.at<uchar>(y + 1, x + 1));
				value = sqrt((pixel_x*pixel_x) + (pixel_y *pixel_y));
				//if (value > 255 && value < 0) value = 0;
				dem++;
				//	cout << value << "\t";
				//	cout << dem <<"-"<< y << endl;
				dstImg.at<uchar>(y, x) = value;
			}


		}
	}
	return dstImg;
}
int main(int argc, char* argv[])
{
	switch (atoi(argv[2]))
	{
	case 1:
	{
		Mat Src = imread(argv[1], IMREAD_COLOR);
		Mat Dst;
		namedWindow("Input", WINDOW_AUTOSIZE);
		namedWindow("Output", WINDOW_AUTOSIZE);
		imshow("Input", Src);
		imshow("Output", rgbToGray(Src, Dst));
	}
	break;
	case 2:
	{
		Mat Src = imread(argv[1], IMREAD_COLOR);
		vector<double> histB, histG, histR;
		calcHistogram(Src, histB, histG, histR);
		Mat histogram = cv::Mat(500, 256, CV_8UC3);
		Mat histogramG = cv::Mat(500, 256, CV_8UC3);
		Mat histogramR = cv::Mat(500, 256, CV_8UC3);
		histogram = Scalar(1, 2, 3);
		histogramG = Scalar(1, 2, 3);
		histogramR = Scalar(1, 2, 3);
		for (int i = 0; i < 256; i++)
		{

			cv::line(histogram, cv::Point(i, 500 - histB[i] / 100), cv::Point(i, 500), cv::Scalar(255, 0, 0), 1);
			cv::line(histogramG, cv::Point(i, 500 - histG[i] / 100), cv::Point(i, 500), cv::Scalar(0, 255, 0), 1);
			cv::line(histogramR, cv::Point(i, 500 - histR[i] / 100), cv::Point(i, 500), cv::Scalar(0, 0, 255), 1);
		}
		namedWindow("Input", WINDOW_AUTOSIZE);
		imshow("Input", Src);
		namedWindow("HistogramB", WINDOW_AUTOSIZE);
		imshow("HistogramB", histogram);
		namedWindow("HistogramG", WINDOW_AUTOSIZE);
		imshow("HistogramG", histogramG);
		namedWindow("HistogramR", WINDOW_AUTOSIZE);
		imshow("HistogramR", histogramR);
	}
	break;
	case 3:
	{
		Mat Src1 = imread(argv[1], IMREAD_COLOR);
		Mat Src2 = imread(argv[3], IMREAD_COLOR);
		cout << "Value Different:  " << compareHist(Src1, Src2);
	}
	break;
	case 4:
	{
		Mat Src = imread(argv[1], IMREAD_COLOR);
		Mat Dst;
		vector<double> histA, histB, histC;

		calcHistogram(Src, Dst, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), histA, histB, histC);
		Mat histogram = cv::Mat(500, 256, CV_8UC3);
		Mat histogramG = cv::Mat(500, 256, CV_8UC3);
		Mat histogramR = cv::Mat(500, 256, CV_8UC3);
		//Mat histogramG = cv::Mat(500, 256, CV_8UC3);
		//Mat histogramR = cv::Mat(500, 256, CV_8UC3);
		histogram = Scalar(1, 2, 3);
		histogramG = Scalar(1, 2, 3);
		histogramR = Scalar(1, 2, 3);
		for (int i = 0; i < 256; i++)
		{
			cv::line(histogram, cv::Point(i, 500 - histA[i] / 100), cv::Point(i, 500), cv::Scalar(255, 0, 0), 1);
			cv::line(histogramG, cv::Point(i, 500 - histB[i] / 100), cv::Point(i, 500), cv::Scalar(0, 255, 0), 1);
			cv::line(histogramR, cv::Point(i, 500 - histC[i] / 100), cv::Point(i, 500), cv::Scalar(0, 0, 255), 1);
		}
		namedWindow("Input", WINDOW_AUTOSIZE);
		imshow("Input", Src);
		namedWindow("Output", WINDOW_AUTOSIZE);
		imshow("Output", Dst);
		namedWindow("HistogramB", WINDOW_AUTOSIZE);
		imshow("HistogramB", histogram);
		namedWindow("HistogramG", WINDOW_AUTOSIZE);
		imshow("HistogramG", histogramG);
		namedWindow("HistogramR", WINDOW_AUTOSIZE);
		imshow("HistogramR", histogramR);
	}
	break;
	case 5:
	{
		Mat Src = imread(argv[1], IMREAD_GRAYSCALE);
		Mat Dst;
		double * arrHist;
		arrHist = calcHistogram(Src, Dst, atoi(argv[3]));
		Mat histogramGray = cv::Mat(500, 256, CV_8UC3);
		histogramGray = Scalar(1, 2, 3);
		for (int i = 0; i < 256; i++)
		{

			cv::line(histogramGray, cv::Point(i, 500 - arrHist[i] / 100), cv::Point(i, 500), cv::Scalar(255, 0, 0), 1);
		}
		namedWindow("Input", WINDOW_AUTOSIZE);
		imshow("Input", Src);
		namedWindow("Output", WINDOW_AUTOSIZE);
		imshow("Output", Dst);
		namedWindow("HistogramB", WINDOW_AUTOSIZE);
		imshow("HistogramGray", histogramGray);
	}
	break;
	case 6:
	{
		Mat Src1 = imread(argv[1], IMREAD_COLOR);
		Mat Src2 = imread(argv[3], IMREAD_COLOR);
		cout << "Value Different:  " << compareHistNol(Src1, Src2, atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
	}
	break;
	case 7:
	{
		Mat Src1, Src2;
		Src1 = imread(argv[1], IMREAD_GRAYSCALE);
		Src2 = imread(argv[3], IMREAD_GRAYSCALE);
		cout << "Value Different: " << compareHisXam(Src1, Src2, atoi(argv[4]));
	}
	break;
	case 8:
	{
		Mat Src;
		Src = imread(argv[1], IMREAD_GRAYSCALE);
		namedWindow("Input", WINDOW_AUTOSIZE);
		namedWindow("Output", WINDOW_AUTOSIZE);
		imshow("Input", Src);
		imshow("Output", xGradient(Src));
	}
	break;
	case 9:
	{
		Mat Src;
		Src = imread(argv[1], IMREAD_GRAYSCALE);
		namedWindow("Input", WINDOW_AUTOSIZE);
		namedWindow("Output", WINDOW_AUTOSIZE);
		imshow("Input", Src);
		imshow("Output", yGradient(Src));

	}break;
	case 10:
	{
		Mat Src;
		Src = imread(argv[1], IMREAD_GRAYSCALE);
		namedWindow("Input", WINDOW_AUTOSIZE);
		namedWindow("Output", WINDOW_AUTOSIZE);
		imshow("Input", Src);
		imshow("Output", magnitude(Src));
	}
	break;
	default:cout << "Nhap ma lenh sai\n";
		break;
	}

	waitKey(0);
	system("pause");
	return 0;
}
