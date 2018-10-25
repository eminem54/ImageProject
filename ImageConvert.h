#pragma once
#include "Utility.h";
#include <iostream>
#include <utility>
#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

void showLaplacian(Mat Ori_Img) {
	int nHeight_in = Ori_Img.rows;
	int nWidth_in = Ori_Img.cols;

	unsigned char** ch_in_R;
	unsigned char** ch_in_G;
	unsigned char** ch_in_B;

	if (Ori_Img.channels() == 3) {
		ch_in_R = allocMem(nHeight_in, nWidth_in, 0);
		ch_in_G = allocMem(nHeight_in, nWidth_in, 0);
		ch_in_B = allocMem(nHeight_in, nWidth_in, 0);

		for (int h = 0; h < nHeight_in; h++) {
			for (int w = 0; w < nWidth_in; w++) {
				Vec3b RGB = Ori_Img.at<Vec3b>(h, w);
				ch_in_R[h][w] = (unsigned char)RGB[2];
				ch_in_G[h][w] = (unsigned char)RGB[1];
				ch_in_B[h][w] = (unsigned char)RGB[0];
			}
		}
	}

	int filter1[3][3] = { { 0,-1,0 },{ -1,4,-1 },{ 0,-1,0 } };
	int filter2[3][3] = { { -1,-1,-1 },{ -1,8,-1 },{ -1,-1,-1 } };

	unsigned char** padR = padding(ch_in_R, nHeight_in, nWidth_in, 3);
	unsigned char** padG = padding(ch_in_G, nHeight_in, nWidth_in, 3);
	unsigned char** padB = padding(ch_in_B, nHeight_in, nWidth_in, 3);

	Mat result = Mat(nHeight_in, nWidth_in, CV_8UC3);

	int sumR = 0, sumG = 0, sumB = 0;
	for (int h = 0; h < nHeight_in; h++) {
		for (int w = 0; w < nWidth_in; w++) {
			sumR = 0, sumG = 0, sumB = 0;

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					sumR += padR[h + i][w + j] * filter1[i][j];
					sumG += padG[h + i][w + j] * filter1[i][j];
					sumB += padB[h + i][w + j] * filter1[i][j];
				}
			}

			if (sumR < 0)sumR = 0;
			if (sumG < 0)sumG = 0;
			if (sumB < 0)sumB = 0;
			if (sumR > 255)sumR = 255;
			if (sumG > 255)sumG = 255;
			if (sumB > 255)sumB = 255;

			result.at<Vec3b>(h, w) = Vec3b(sumB, sumG, sumR);
		}
	}

	imshow("ori", result);
	waitKey(0);
	free(ch_in_R);
	free(ch_in_G);
	free(ch_in_B);
	free(padR);
	free(padG);
	free(padB);
}
