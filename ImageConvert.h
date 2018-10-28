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

	copyColorPixel(Ori_Img, ch_in_R, ch_in_G, ch_in_B);

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

void getBitPlane(Mat &Ori_Img, Mat &result, int n)
{
	int nHeight_in = Ori_Img.rows;
	int nWidth_in = Ori_Img.cols;

	char mask = 0x01 << n;

	unsigned char** ch_in_R = allocMem(nHeight_in, nWidth_in, 0);
	unsigned char** ch_in_G = allocMem(nHeight_in, nWidth_in, 0);
	unsigned char** ch_in_B = allocMem(nHeight_in, nWidth_in, 0);

	copyColorPixel(Ori_Img, ch_in_R, ch_in_G, ch_in_B);


	for (int h = 0; h < nHeight_in; h++) {
		uchar * or = ch_in_R[h];
		uchar *og = ch_in_G[h];
		uchar *ob = ch_in_B[h];

		uchar *dr = ch_in_B[h];
		uchar *dg = ch_in_B[h];
		uchar *db = ch_in_B[h];


		for (int w = 0; w < nWidth_in; w++) {
			uchar pixelR = or [w];
			uchar pixelG = og[w];
			uchar pixelB = ob[w];

			uchar afterR = pixelR & mask;
			uchar afterG = pixelG & mask;
			uchar afterB = pixelB & mask;

			if (afterR> 0) afterR = 255;
			else afterR = 0;
			if (afterG> 0) afterG = 255;
			else afterG = 0;
			if (afterB> 0) afterB = 255;
			else afterB = 0;

			result.at<Vec3b>(h, w) = Vec3b(0, 0, afterR);
		}
	}
	free(ch_in_R);
	free(ch_in_G);
	free(ch_in_B);
}

void colorEqualization(Mat Ori_Img, Mat result) {
	int height = Ori_Img.rows;
	int width = Ori_Img.cols;

	unsigned char** ch_in_R = allocMem(height, width, 0);
	unsigned char** ch_in_G = allocMem(height, width, 0);
	unsigned char** ch_in_B = allocMem(height, width, 0);
	copyColorPixel(Ori_Img, ch_in_R, ch_in_G, ch_in_B);


	int numOfR[256] = { 0 };
	int numOfG[256] = { 0 };
	int numOfB[256] = { 0 };

	double histR[256] = { 0.0 };
	double histG[256] = { 0.0 };
	double histB[256] = { 0.0 };


	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			numOfR[ch_in_R[h][w]]++;
			numOfG[ch_in_G[h][w]]++;
			numOfB[ch_in_B[h][w]]++;
		}
	}

	histR[0] = numOfR[0];
	histG[0] = numOfG[0];
	histB[0] = numOfB[0];

	for (int i = 1; i < 256; i++) {
		histR[i] = histR[i - 1] + numOfR[i];
		histG[i] = histG[i - 1] + numOfG[i];
		histB[i] = histB[i - 1] + numOfB[i];
	}
	for (int i = 0; i < 256; i++) {
		histR[i] = histR[i] / (height*width) * 256;
		histG[i] = histG[i] / (height*width) * 256;
		histB[i] = histB[i] / (height*width) * 256;
	}



	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			result.at<Vec3b>(h, w) = Vec3b(histB[ch_in_B[h][w]], histG[ch_in_G[h][w]], histR[ch_in_R[h][w]]);
		}
	}

	//DisplayHistogram(numOfR, "Red");
	//DisplayHistogram(numOfG, "Green");
	//DisplayHistogram(numOfB, "Blue");
	//waitKey(0);

}




void grayEqualization(Mat Ori_Img, Mat result) {
	int height = Ori_Img.rows;
	int width = Ori_Img.cols;


	int num[256] = { 0 };

	double hist[256] = { 0.0 };


	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			num[Ori_Img.at<uchar>(h,w)]++;
		}
	}

	hist[0] = num[0];

	for (int i = 1; i < 256; i++) {
		hist[i] = hist[i - 1] + num[i];
	}
	for (int i = 0; i < 256; i++) {
		hist[i] = hist[i] / (height*width) * 256;
	}


	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			result.at<uchar>(h, w) = hist[Ori_Img.at<uchar>(h, w)];
		}
	}

	//DisplayHistogram(numOfR, "Red");
	//DisplayHistogram(numOfG, "Green");
	//DisplayHistogram(numOfB, "Blue");
	//waitKey(0);

}
