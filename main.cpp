#include "Utility.h"
#include "ImageConvert.h"
#include "Scaling.h"

#include <iostream>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;
#define SIZE 256
//cvtColor(img_ori, img_gray, CV_BGR2GRAY);
//resize(img_gray, img_resize, Size(256, 256), 0, 0, CV_INTER_LINEAR);

unsigned char** createEyeMask(int size) {
	unsigned char** mask = new unsigned char*[size * 10];
	for (int i = 0; i < size * 10; i++) {
		mask[i] = new unsigned char[size * 30];
		memset(mask[i], 0, sizeof(unsigned char)*size * 30);
	}
	return mask;
}

Mat createScaleImage(unsigned char** pixels, int height, int width) {
	double heightScaleRate = (double)SIZE / height;
	double widthScaleRate = (double)SIZE / width;

	Mat result2 = Mat(SIZE, SIZE, CV_8UC1);

	for (int h = 0; h < SIZE; h++) {
		for (int w = 0; w < SIZE; w++) {
			double h_Ori = h / heightScaleRate;
			double w_Ori = w / widthScaleRate;
			result2.at<uchar>(h, w) = BilinearInterpolation(pixels, height, width, h_Ori, w_Ori);
		}
	}
	return result2;
}
int main() {
	const char* inputPath1 = "aeye19.bmp";
	const char* inputPath2 = "aeye02.bmp";
	const char* inputPath3 = "aeye03.bmp";
	const char* inputPath4 = "aeye04.bmp";
	const char* inputPath5 = "aeye05.bmp";
	const char* inputPath6 = "aeye06.bmp";
	const char* inputPath7 = "aeye07.bmp";
	const char* inputPath8 = "aeye08.bmp";
	const char* inputPath9 = "aeye09.bmp";

	Mat img_ori1 = imread(inputPath1);
	Mat img_ori2 = imread(inputPath2);
	Mat img_ori3 = imread(inputPath3);
	Mat img_ori4 = imread(inputPath4);
	Mat img_ori5 = imread(inputPath5);
	Mat img_ori6 = imread(inputPath6);
	Mat img_ori7 = imread(inputPath7);
	Mat img_ori8 = imread(inputPath8);
	Mat img_ori9 = imread(inputPath9);


	Mat iimg_ori1;
	Mat iimg_ori2;
	Mat iimg_ori3;
	Mat iimg_ori4;
	Mat iimg_ori5;
	Mat iimg_ori6;
	Mat iimg_ori7;
	Mat iimg_ori8;
	Mat iimg_ori9;

	resize(img_ori1, iimg_ori1, Size(64,64), 0, 0, CV_INTER_LINEAR);
	resize(img_ori2, iimg_ori2, Size(64, 64), 0, 0, CV_INTER_LINEAR);
	resize(img_ori3, iimg_ori3, Size(64, 64), 0, 0, CV_INTER_LINEAR);
	resize(img_ori4, iimg_ori4, Size(64, 64), 0, 0, CV_INTER_LINEAR);
	resize(img_ori5, iimg_ori5, Size(64, 64), 0, 0, CV_INTER_LINEAR);
	resize(img_ori6, iimg_ori6, Size(64, 64), 0, 0, CV_INTER_LINEAR);
	resize(img_ori7, iimg_ori7, Size(64, 64), 0, 0, CV_INTER_LINEAR);
	resize(img_ori8, iimg_ori8, Size(64, 64), 0, 0, CV_INTER_LINEAR);
	resize(img_ori9, iimg_ori9, Size(64, 64), 0, 0, CV_INTER_LINEAR);


	Mat iiimg_ori1;
	Mat iiimg_ori2;
	Mat iiimg_ori3;
	Mat iiimg_ori4;
	Mat iiimg_ori5;
	Mat iiimg_ori6;
	Mat iiimg_ori7;
	Mat iiimg_ori8;
	Mat iiimg_ori9;


	GaussianBlur(iimg_ori1, iiimg_ori1, Size(3, 3), 0, 0);
	GaussianBlur(iimg_ori2, iiimg_ori2, Size(3, 3), 0, 0);
	GaussianBlur(iimg_ori3, iiimg_ori3, Size(3, 3), 0, 0);
	GaussianBlur(iimg_ori4, iiimg_ori4, Size(3, 3), 0, 0);
	GaussianBlur(iimg_ori5, iiimg_ori5, Size(3, 3), 0, 0);
	GaussianBlur(iimg_ori6, iiimg_ori6, Size(3, 3), 0, 0);
	GaussianBlur(iimg_ori7, iiimg_ori7, Size(3, 3), 0, 0);
	GaussianBlur(iimg_ori8, iiimg_ori8, Size(3, 3), 0, 0);
	GaussianBlur(iimg_ori9, iiimg_ori9, Size(3, 3), 0, 0);



	int hist1[256] = { 0 };
	int hist2[256] = { 0 };
	int hist3[256] = { 0 };
	int hist4[256] = { 0 };
	int hist5[256] = { 0 };
	int hist6[256] = { 0 };
	int hist7[256] = { 0 };
	int hist8[256] = { 0 };
	int hist9[256] = { 0 };

	for (int h = 0; h < 64; h++) {
		for (int w = 0; w < 64; w++) {
			hist1[iiimg_ori1.at<uchar>(h, w)]++;
			hist2[iiimg_ori2.at<uchar>(h, w)]++;
			hist3[iiimg_ori3.at<uchar>(h, w)]++;
			hist4[iiimg_ori4.at<uchar>(h, w)]++;
			hist5[iiimg_ori5.at<uchar>(h, w)]++;
			hist6[iiimg_ori6.at<uchar>(h, w)]++;
			hist7[iiimg_ori7.at<uchar>(h, w)]++;
			hist8[iiimg_ori8.at<uchar>(h, w)]++;
			hist9[iiimg_ori9.at<uchar>(h, w)]++;

		}
	}

	DisplayHistogram(hist1, inputPath1);
	DisplayHistogram(hist2, inputPath2);
	DisplayHistogram(hist3, inputPath3);
	DisplayHistogram(hist4, inputPath4);
	DisplayHistogram(hist5, inputPath5);
	DisplayHistogram(hist6, inputPath6);
	DisplayHistogram(hist7, inputPath7);
	DisplayHistogram(hist8, inputPath8);
	DisplayHistogram(hist9, inputPath9);


	waitKey(0);


	return 0;
}