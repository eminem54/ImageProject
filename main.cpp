#include "Utility.h"
#include "ImageConvert.h"
#include "Scaling.h"
#include <vector>
#include <iostream>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <opencv2\opencv.hpp>
#include "Eye.h"
using namespace std;
using namespace cv;
#define SIZE 256

//cvtColor(img_ori, img_gray, CV_BGR2GRAY);
//resize(img_gray, img_resize, Size(256, 256), 0, 0, CV_INTER_LINEAR);
//GaussianBlur(iimg_ori1, iiimg_ori1, Size(3, 3), 0, 0);




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
	Mat img_ori1 = imread("face12.bmp");

	Mat img_gray;
	cvtColor(img_ori1, img_gray, CV_BGR2GRAY);

	imshow("1", img_gray);
	waitKey(0);

	Mat img_resize;
	resize(img_gray, img_resize, Size(SIZE, SIZE), 0, 0, CV_INTER_LINEAR);


	//Mat temp;
	//Mat img_smooth;

	//GaussianBlur(img_resize, temp, Size(3, 3), 0, 0);
	//GaussianBlur(temp, img_smooth, Size(3, 3), 0, 0);

	unsigned char** pixels = allocMem(SIZE, SIZE, 0);
	copyGrayPixel(img_resize, pixels);
	unsigned char** pad = padding(pixels, SIZE, SIZE, 3);

	int count = 0;

	double dx = 0, dy = 0;
	int angle = 0;
	unsigned char** gradient = allocMem(SIZE, SIZE, 0);

	for (int h = 0; h < SIZE; h++) {
		for (int w = 0; w < SIZE; w++) {
			dx = 0.0; dy = 0.0;
			if (h > 0 && h < SIZE - 1 && w>0 && w < SIZE - 1) {
				dx = pad[h][w - 1] * gMask[0] + pad[h][w + 1] * gMask[2];
				dy = pad[h + 1][w] * gMask[0] + pad[h - 1][w] * gMask[2];
				if (dx != 0) {
					angle = (atan((double)dy / dx) * 180.0 / M_PI);
					if (angle < 0) {
						gradient[h][w] = (180 + angle) / 45.0;
					}
					else
						gradient[h][w] = angle / 45.0;
				}
				else {
					gradient[h][w] = 4;
				}
			}
			else {
				gradient[h][w] = 4;
			}
		}
	}

	int cellSize = 4;
	int cellNum = SIZE / cellSize;
	unsigned char** cells = allocMem(cellNum*cellNum, 4, 0);

	for (int h = 0; h < cellNum; h++) {
		for (int w = 0; w < cellNum; w++) {
			for (int i = 0; i < cellSize; i++) {
				for (int j = 0; j < cellSize; j++) {
					if (gradient[h*cellSize + i][w*cellSize + j] != 5)
						cells[h*cellNum + w][gradient[h*cellSize + i][w*cellSize + j]]++;
				}
			}
		}
	}


	int maxIndex = 0;


	//for (int i = 0; i < cellNum; i++) {
	//	for (int j = 0; j < cellNum; j++) {
	//		cout << i << "�� " << j << "��" << endl;
	//		maxIndex = 0;
	//		for (int x = 1; x < 4; x++) {
	//			if (cells[i * cellNum + j][x] > cells[i*cellNum + j][maxIndex]) {
	//				maxIndex = x;
	//			}
	//		}
	//		cout << maxIndex <<"���� " << (int)cells[i*cellNum + j][maxIndex] << endl;
	//	}
	//}

	unsigned char** cellOrient = allocMem(cellNum, cellNum, 0);

	for (int i = 0; i < cellNum; i++) {
		for (int j = 0; j < cellNum; j++) {
			maxIndex = 0;
			for (int x = 1; x < 4; x++) {
				if (cells[i * cellNum + j][x] > cells[i*cellNum + j][maxIndex]) {
					maxIndex = x;
				}
			}
			if(!(cells[i*cellNum+j][maxIndex]<6))
				cellOrient[i][j] = maxIndex;
			else {
				cellOrient[i][j] = 4;
			}
		}
	}

	int eyesize = 5;
	int num0 = 0, num1 = 0, num2 = 0, num3 = 0;
	int secondNum0 = 0, secondNum1 = 0, secondNum2 = 0, secondNum3 = 0;

	Mat copyori = Mat(img_ori1);
	Mat output;
	resize(copyori, output, Size(SIZE, SIZE), 0, 0, CV_INTER_LINEAR);

	for (int h = 0; h < cellNum; h++) {
		for (int w = 0; w < cellNum / 2; w++) {

			num0 = 0, num1 = 0, num2 = 0, num3 = 0;
			for (int i = 0; i < eyesize; i++) {
				countCellValue(cellOrient[h][w + i], num0, num1, num2, num3);
			}
			if (isEye(num0, num1, num2, num3)) {
				for (int j = w + eyesize; j < cellNum - eyesize; j++) {
					secondNum0 = 0, secondNum1 = 0, secondNum2 = 0, secondNum3 = 0;

					for (int k = 0; k < eyesize; k++) {
						cout << (int)cellOrient[h][j + k] << endl;
						countCellValue(cellOrient[h][j + k], secondNum0, secondNum1, secondNum2, secondNum3);
					}

					if (isEye(secondNum0, secondNum1, secondNum2, secondNum3)) { //real eye
						cout << "please..." << h<<" "<<w<<endl;
						for (int b = h; b < h + 10; b++) {
							for (int a = w; a < j; a++) {
								output.at<Vec3b>(b, a) = Vec3b(0, 0, 255);
							}
						}
						imshow("h", output);
						waitKey(0);
						return 0;
					}
				}
			}
		}
	}
	return 0;
}












/*	const char* inputEye1 = "ImageProject\\aeye\\aeye01.bmp";
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
	DisplayHistogram(hist9, inputPath9);*/
