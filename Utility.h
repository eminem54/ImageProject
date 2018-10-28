#pragma once
#include <iostream>
#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

unsigned char** allocMem(int height, int width, int val) {
	unsigned char** ret = new unsigned char*[height];
	for (int i = 0; i < height; i++) {
		ret[i] = new unsigned char[width];
		//memset(ret[i], val, sizeof(unsigned char)*width);
	}
	return ret;
}

unsigned char* ReadBMP(const char* filename)
{
	FILE* f = fopen(filename, "rb");
	if (f == NULL)
		throw "Argument Exception";

	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

											   // extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	cout << endl;
	cout << "  Name: " << filename << endl;
	cout << " Width: " << width << endl;
	cout << "Height: " << height << endl;

	int row_padded = (width * 3 + 3) & (~3);
	unsigned char* data = new unsigned char[row_padded];
	unsigned char tmp;

	for (int i = 0; i < height; i++)
	{
		fread(data, sizeof(unsigned char), row_padded, f);
		for (int j = 0; j < width * 3; j += 3)
		{
			// Convert (B, G, R) to (R, G, B)
			tmp = data[j];
			data[j] = data[j + 2];
			data[j + 2] = tmp;

			//cout << "R: " << (int)data[j] << " G: " << (int)data[j + 1] << " B: " << (int)data[j + 2] << endl;
		}
	}
	fclose(f);
	return data;
}


unsigned char** padding(unsigned char** in, int nHeight, int nWidth, int nFilterSize) {
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Pad = allocMem(nHeight + 2 * nPadSize, nWidth + 2 * nPadSize, 0);

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			Pad[h + nPadSize][w + nPadSize] = in[h][w];
		}
	}
	for (int h = 0; h < nPadSize; h++) {
		for (int w = 0; w < nWidth; w++) {
			Pad[h][w + nPadSize] = in[0][w];
			Pad[h + (nHeight - 1)][w + nPadSize] = in[nHeight - 1][w];
		}
	}
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nPadSize; w++) {
			Pad[h + nPadSize][w] = in[h][0];
			Pad[h + nPadSize][w + (nWidth - 1)] = in[h][nWidth - 1];
		}
	}
	for (int h = 0; h < nPadSize; h++) {
		for (int w = 0; w < nPadSize; w++) {
			Pad[h][w] = in[0][0];
			Pad[h + (nHeight - 1)][w] = in[nHeight - 1][0];
			Pad[h][w + (nWidth - 1)] = in[0][nWidth - 1];
			Pad[h + (nHeight - 1)][w + (nWidth - 1)] = in[nHeight - 1][nWidth - 1];
		}
	}
	return Pad;
}


void copyColorPixel(Mat img, unsigned char** ch_in_R, unsigned char** ch_in_G, unsigned char** ch_in_B) {
	int height = img.rows;
	int width = img.cols;

	if (img.channels() == 3) {


		for (int h = 0; h < height; h++) {
			for (int w = 0; w < width; w++) {
				Vec3b RGB = img.at<Vec3b>(h, w);
				ch_in_R[h][w] = (unsigned char)RGB[2];
				ch_in_G[h][w] = (unsigned char)RGB[1];
				ch_in_B[h][w] = (unsigned char)RGB[0];
			}
		}
	}
}

void DisplayHistogram(int* Hist, const char* name) {
	int nMax = 0;
	for (int n = 0; n < 256; n++) {
		if (nMax < Hist[n]) {
			nMax = Hist[n];
		}
	}
	double dNormalizeFactor = 255.0 / nMax;
	Mat HistDisp = Mat(256, 256, CV_8UC1);
	HistDisp = Scalar::all(0);

	for (int w = 0; w < 256; w++) {
		int nNormalizedValue = (int)Hist[w] * dNormalizeFactor;
		for (int h = 255; h > 255 - nNormalizedValue; h--) {
			HistDisp.at<unsigned char>(h, w) = 255;
		}
	}
	imshow(name, HistDisp);
	waitKey(0);
}

void printHistogram(int* hist) {
	for (int i = 0; i < 256; i++) {
		printf("value %d ÀÇ °³¼ö : %d\n", i, hist[i]);
	}
}


Mat convertColorToGray(Mat ori, unsigned char** ch_in_R, unsigned char** ch_in_G, unsigned char** ch_in_B) {
	int height = ori.rows;
	int width = ori.cols;
	Mat result = Mat(height, width, CV_8UC3);

	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			uchar val = (uchar)(0.2126*ch_in_R[h][w] + 0.7512*ch_in_R[h][w] + 0.0722*ch_in_B[h][w]);
			if (val < 0) val = 0;
			if (val > 255) val = 255;

			result.at<Vec3b>(h, w) = Vec3b(val, val, val);
		}
	}
	return result;
}

void copyGrayPixel(Mat img, unsigned char** des) {
	int height = img.rows;
	int width = img.cols;

	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			des[h][w] = img.at<uchar>(h, w);
		}
	}
}