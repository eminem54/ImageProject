#include "Utility.h"
#include "ImageConvert.h"
#include "Scaling.h"

#include <iostream>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;
#define SIZE 256


unsigned char** createEyeMask(int size) {
	unsigned char** mask = new unsigned char*[size * 10];
	for (int i = 0; i < size * 10; i++) {
		mask[i] = new unsigned char[size * 30];
		memset(mask[i], 0, sizeof(unsigned char)*size*30);
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
	const char* inputPath = "face5.bmp";
	Mat Ori_Img = imread(inputPath);
	int height = Ori_Img.rows;
	int width = Ori_Img.cols;
	Mat gray;
	cvtColor(Ori_Img, gray, CV_BGR2GRAY);
	
	return 0;
}