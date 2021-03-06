// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
using namespace std;
using namespace cv;


int main(int argc, char **argv)
{
	if (argc != 2)
	{
		cout << "Chuong trinh mo va hien thi anh" << endl;
	}
	Mat image; // (1) Khởi tạo mảng để lưu trữ hình ảnh
	image = imread(argv[1], IMREAD_COLOR); //(2) Đọc từ file hình ảnh,
	if (!image.data)
	{
		cout << "Khong the mo anh" << endl;
		return -1;
	}
	namedWindow("Display Window", WINDOW_AUTOSIZE); //(3) Khởi tạo cửa sổ hiển thị hình ảnh
	imshow("Display Window", image); //(4) Hiển thị hình ảnh bên trong  cửa sổ  "Display Window"
	waitKey(0);

    return 0;
}

