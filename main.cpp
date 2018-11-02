#include "Utility.h"
#include "ImageConvert.h"
#include "Scaling.h"
#include <vector>
#include <iostream>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;
#define SIZE 256
//cvtColor(img_ori, img_gray, CV_BGR2GRAY);
//resize(img_gray, img_resize, Size(256, 256), 0, 0, CV_INTER_LINEAR);
//GaussianBlur(iimg_ori1, iiimg_ori1, Size(3, 3), 0, 0);

const char* inputEye1 = "ImageProject\\aeye\\aeye01.bmp";
const char* inputEye2 = "ImageProject\\aeye\\aeye02.bmp";
const char* inputEye3 = "ImageProject\\aeye\\aeye03.bmp";
const char* inputEye4 = "ImageProject\\aeye\\aeye04.bmp";
const char* inputEye5 = "ImageProject\\aeye\\aeye05.bmp";
const char* inputEye6 = "ImageProject\\aeye\\aeye06.bmp";
const char* inputEye7 = "ImageProject\\aeye\\aeye07.bmp";
const char* inputEye8 = "ImageProject\\aeye\\aeye08.bmp";
const char* inputEye9 = "ImageProject\\aeye\\aeye09.bmp";
const char* inputEye10 = "ImageProject\\aeye\\aeye10.bmp";
const char* inputEye11 = "ImageProject\\aeye\\aeye11.bmp";
const char* inputEye12 = "ImageProject\\aeye\\aeye12.bmp";
const char* inputEye13 = "ImageProject\\aeye\\aeye13.bmp";
const char* inputEye14 = "ImageProject\\aeye\\aeye14.bmp";
const char* inputEye15 = "ImageProject\\aeye\\aeye15.bmp";
const char* inputEye16 = "ImageProject\\aeye\\aeye16.bmp";
const char* inputEye17 = "ImageProject\\aeye\\aeye17.bmp";
const char* inputEye18 = "ImageProject\\aeye\\aeye18.bmp";
const char* inputEye19 = "ImageProject\\aeye\\aeye19.bmp";
const char* inputEye[19] = { inputEye1,inputEye2,inputEye3,inputEye4,inputEye5,inputEye6,inputEye7,inputEye8,inputEye9,inputEye10,
inputEye11,inputEye12,inputEye13,inputEye14,inputEye15,inputEye16,inputEye17,inputEye18,inputEye19 };





const char* inputPath1 = "face1.bmp";
const char* inputPath2 = "face2.bmp";
const char* inputPath3 = "face3.bmp";
const char* inputPath4 = "face4.bmp";
const char* inputPath5 = "face5.bmp";
const char* inputPath6 = "face6.bmp";
const char* inputPath7 = "face7.bmp";
const char* inputPath8 = "face8.bmp";
const char* inputPath9 = "face9.bmp";
const char* inputPath10 = "aeye10.bmp";
const char* inputPath11 = "aeye11.bmp";
const char* inputPath12 = "aeye12.bmp";
const char* inputPath13 = "aeye13.bmp";
const char* inputPath14 = "aeye14.bmp";
const char* inputPath15 = "aeye15.bmp";
const char* inputPath16 = "aeye16.bmp";
const char* inputPath17 = "aeye17.bmp";
const char* inputPath18 = "aeye18.bmp";
const char* inputarr[18] = { inputPath1,inputPath2, inputPath3, inputPath4, inputPath5, inputPath6, inputPath7, inputPath8, inputPath9, inputPath10,
inputPath11, inputPath12, inputPath13, inputPath14, inputPath15, inputPath16, inputPath17, inputPath18 };

unsigned char** createEyeMask(int size) {
	unsigned char** mask = new unsigned char*[size * 10];
	for (int i = 0; i < size * 10; i++) {
		mask[i] = new unsigned char[size * 30];
		memset(mask[i], 0, sizeof(unsigned char)*size * 30);
	}
	return mask;
}

int gMask[3] = { -1,0,1 };

int main() {
	Mat img_ori1 = imread("face5.bmp");

	Mat img_gray;
	cvtColor(img_ori1, img_gray, CV_BGR2GRAY);

	Mat img_resize;
	resize(img_gray, img_resize, Size(SIZE, SIZE), 0, 0, CV_INTER_LINEAR);


	Mat temp;
	Mat img_smooth;

	//GaussianBlur(img_resize, temp, Size(3, 3), 0, 0);
	//GaussianBlur(temp, img_smooth, Size(3, 3), 0, 0);

	unsigned char** pixels = allocMem(SIZE, SIZE, 0);
	copyGrayPixel(img_resize, pixels);
	unsigned char** pad = padding(pixels, SIZE, SIZE, 3);


	unsigned char** gradHist = allocMem(4096, 9, 0);

	int dx = 0, dy = 0;
	int angle = 0;
	unsigned char** xgrad = allocMem(SIZE,SIZE,0);
	for (int h = 0; h < SIZE; h++) {
		for (int w = 0; w < SIZE; w++) {
			dx = 0; dy = 0, temp = 0;
			if (h > 0 && h < SIZE - 1 && w>0 && w < SIZE - 1) {
				dx = pad[h][w - 1] * gMask[0] + pad[h][w + 1] * gMask[2];
				dy = pad[h + 1][w] * gMask[0] + pad[h - 1][w] * gMask[2];
				if (dx != 0) {
					angle = (atan(dy / dx) * 180 / M_PI);
					if (angle < 0) {
						xgrad[h][w] = (360 + angle) / 40;
					}
					else
						xgrad[h][w] = angle/40;
				}
				else
					xgrad[h][w] = 0;
			}
			else
				xgrad[h][w] = 0;
		}
	}


	for (int h = 0; h < 64; h++) {
		for (int w = 0; w < 64; w++) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					gradHist[h*64+w][xgrad[h*4+i][w*4+j]]++;
				}
			}
		}
	}


	for (int i = 22; i < 64; i++) {
		for (int j = 23; j < 64; j++) {
			cout << i << "행" << j << "번째블록" << endl;
			for (int x= 0; x < 9; x++) {
				cout << (int)gradHist[i * 64+j][x] << endl;
			}
		}
	}

	waitKey(0);


	return 0;
}











/*	Mat img_ori2 = imread(inputPath2);
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
	DisplayHistogram(hist9, inputPath9);*/