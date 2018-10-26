#include "Utility.h";
#include <iostream>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;

void getBitPlane(Mat &srcImg, Mat &dstImg, int n)
{
	uchar mask = 0x01 << n;
	Mat s = Mat(srcImg);
	Mat d = Mat(dstImg);

	for (int r = 0; r < srcImg.rows; r++)
	{
		uchar *p = srcImg.ptr(r);//첫번째행의 값을 가져온다.
		uchar *dp = dstImg.ptr(r);

		for (int c = 0; c < srcImg.cols; c++)
		{
			uchar pixelvalue = p[c];
			uchar one_zero = pixelvalue & mask;
			if (one_zero > 0)
				dp[c] = 255;
			else
				dp[c] = 0;
		}
	}
}

int main() {
	const char* inputPath = "face1.bmp";
	Mat Ori_Img = imread(inputPath);

	int nHeight_in = Ori_Img.rows;
	int nWidth_in= Ori_Img.cols;

	char mask = 0x01 << 7;
	char mask2 = 0x01 << 4;
	
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

	Mat result = Mat(nHeight_in, nWidth_in, CV_8UC3);
	for (int h = 0; h < nHeight_in; h++) {
		uchar *or = ch_in_R[h];
		uchar *og = ch_in_G[h];
		uchar *ob = ch_in_B[h];

		uchar *dr = ch_in_B[h];
		uchar *dg = ch_in_B[h];
		uchar *db = ch_in_B[h];


		for (int w = 0; w < nWidth_in; w++) {
				uchar pixelR = or[w];
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

				result.at<Vec3b>(h, w) = Vec3b(afterB, ch_in_G[h][w], afterR);
		}
	}

	//getBitPlane(Ori_Img, result, 6);

	imshow("hi", result);
	waitKey(0);

	return 0;
}